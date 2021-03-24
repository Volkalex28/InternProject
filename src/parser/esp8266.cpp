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

#include "esp8266.h"
#include "esp8266.hpp"

esp8266::esp8266(printf_t write) 
  : Parser("AT", write),
    AT(this, "", ""),
    CWLAP(this, "+CWLAP", "#:(%,\"%\",%,\"%\""),
    Echo(this, "E", "AT#%"),
    Test(this, "", "WIFI_CONNECTED"),
    Test2(this, "", "WIFI")
{
  
}

esp8266 esp([](const char *){ }); 

void Parse(const char sym)
{
  static etl::string<80> str = "";

  str += sym;

  if(sym == '\r')
  { 
    esp.Parse(str.c_str());
  } 
}
