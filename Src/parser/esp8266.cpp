/**
 * @file esp8266.cpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief
 * @version 0.1
 * @date 2021-02-21
 *
 * @copyright Copyright (c) 2021
 *
 */
extern "C"
{
#include <main.h>
#include <stm32f051_rcc.h>
#include <stm32f051_uart.h>
#include <user_assert.h>
#include <uart_ring.h>

extern UART_Ring_t uart1;
}

#include "esp8266.h"
#include "esp8266.hpp"

esp8266::esp8266(printf_t write)
  : Parser("AT", write), AT(this, "", "OK"), CWLAP(this, "+CWLAP", "#:(%,\"%\",%,\"%\""),
    CWJAP(this, "+CWJAP_DEF", "#="), WIFI_GotIP(this, "", "WIFI GOT IP"),
    WIFI_Disconnected(this, "", "WIFI DISCONNECT"), ServerConnect(this, "+CIPSTART", "CONNECT"),
    ServerClose(this, "+CIPCLOSE", "CLOSED"), ServerSend(this, "+CIPSEND", ">"), ServerSended(this, "", "SEND OK")
{
  this->AT >> [this](void) {
    if (this->getInit() == false)
    {
      this->setInit(true);
    }
  };
  this->WIFI_GotIP >> [this](void) { this->setConnected(true); };
  this->WIFI_Disconnected >> [this](void) { this->setConnected(false); };
  this->CWLAP >> [this](int Channel, Parser::string_t SSID, int Level, Parser::string_t MAC) {
    this->Nets.push_back(std::move(SSID));
  };
  this->ServerConnect >> [this](void) { this->setConnectedServer(true); };
  this->ServerClose >> [this](void) { this->setConnectedServer(false); };
  this->ServerSend >> [this](void) { this->_sendServerReady = true; };
  this->ServerSended >> [this](void) { this->_sendServerReady = false; };
}

void esp8266::init(void)
{
  this->AT();
}

const bool esp8266::getInit(void) const
{
  return this->_init;
}

const bool esp8266::getConnected(void) const
{
  return this->_connected;
}

void esp8266::setInit(const bool state)
{
  this->_init = state;
}

void esp8266::setConnected(const bool state)
{
  this->_connected = state;
}

const bool esp8266::getConnectedServer(void) const
{
  return this->_connectServer;
}

void esp8266::setConnectedServer(const bool state)
{
  this->_connectServer = state;
}

esp8266 esp([](const char * str) { UART_Transmit(&uart1.uart, (const uint8_t *)str, strlen(str), 1000); });

// =========================================== C implementation of the algorithm ======================================

const uint32_t ESP_Init(const uint32_t timeout)
{
  if (esp.getInit() == true)
  {
    return 0;
  }

  esp.init();
  const uint32_t startTick = GetTick();
  while (startTick + timeout >= GetTick())
  {
    if (esp.getInit() == true)
    {
      return 0;
    }
  }
  return 1;
}

static etl::string<80> str = "";

const uint32_t ESP_Parse(const char sym)
{
  if (sym == '\r')
  {
    if (str.back() == '\n')
    {
      str.pop_back();
    }
    if (str.length() > 0)
    {
      esp.Parse(str.c_str());
      str.clear();
      return 0;
    }
  }
  else
  {
    str += sym;
  }
  return 1;
}

const uint32_t ESP_Scan(void)
{
  if (ESP_Init(1000) != 0)
  {
    return 1;
  }
  esp.Nets.clear();
  esp.CWLAP();
  return 0;
}

const uint32_t ESP_ConnectWIFI(const char * SSID, const char * Password, const uint32_t timeout)
{
  const uint32_t startTick = GetTick();

  if (ESP_Init(1000) != 0)
  {
    return 1;
  }

  esp.setConnected(false);
  esp.CWJAP("=", SSID, ",", Password);
  while (startTick + timeout >= GetTick())
  {
    if (esp.getConnected() == true)
    {
      return 0;
    }
  }
  return 2;
}

const uint32_t ESP_PrintSSIDs(STM32F051_UART_t * pUART, const uint32_t timeout)
{
  const char * end = "\n";
  if (esp.Nets.size() == 0)
  {
    return 1;
  }
  for (size_t i = 0; i < esp.Nets.size(); i++)
  {
    if (UART_Transmit(pUART, (const uint8_t *)esp.Nets.at(i).c_str(), esp.Nets.at(i).length(), timeout) != 0)
    {
      return 2;
    }
    UART_Transmit(pUART, (const uint8_t *)end, 2, timeout);
  }
  return 0;
}

const uint32_t ESP_ConnectServer(const char * ip, const uint16_t port, const uint32_t timeout)
{
  uint32_t startTick = GetTick();

  esp.ServerClose();
  while (startTick + timeout >= GetTick())
  {
    if (esp.getConnectedServer() == false)
    {
      break;
    }
  }
  if (esp.getConnectedServer() == true)
  {
    return 1;
  }
  esp.ServerConnect("=\"TCP\"", ip, port);

  while (startTick + timeout >= GetTick())
  {
    if (esp.getConnectedServer() == true)
    {
      break;
    }
  }
  if (esp.getConnectedServer() == false)
  {
    return 2;
  }

  return 0;
}

const uint32_t ESP_SendToServer(const char * str, const uint32_t timeout)
{
  const uint32_t startTick = GetTick();

  if(esp.getConnectedServer() == false)
  {
    return 3;
  }

  esp.ServerSend("=", strlen(str));

  while (startTick + timeout >= GetTick())
  {
    if (esp._sendServerReady == true)
    {
      break;
    }
  }
  if (esp._sendServerReady == false)
  {
    return 1;
  }

  esp.print(str);

  while (startTick + timeout >= GetTick())
  {
    if (esp._sendServerReady == false)
    {
      break;
    }
  }
  if (esp._sendServerReady == true)
  {
    return 2;
  }

  return 0;
}
