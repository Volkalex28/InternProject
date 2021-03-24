/**
 * @file esp8266.hpp
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief 
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "AT/src/Parser.hpp"
#include "AT/src/etl/include/etl/vector.h"

class esp8266 : public Parser<8>
{
  bool _init = false;
  bool _connected = false;
  bool _connectServer = false;

public:
  AT_Command<> AT;
  AT_Command<int, string_t, int, string_t> CWLAP;
  AT_Command<> CWJAP;
  AT_Command<> WIFI_GotIP;
  AT_Command<> WIFI_Disconnected;
  AT_Command<> ServerConnect;
  AT_Command<> ServerClose;
  AT_Command<> ServerSend;
  AT_Command<> ServerSended;

  esp8266(printf_t write);

  void init(void);
  const bool getInit(void) const;
  const bool getConnected(void) const;
  void setInit(const bool state);
  void setConnected(const bool state);
  const bool getConnectedServer(void) const;
  void setConnectedServer(const bool state);

  etl::vector<Parser::string_t, 10> Nets;
  bool _sendServerReady = false;
};

