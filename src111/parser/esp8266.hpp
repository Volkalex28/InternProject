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

class esp8266 : public Parser<5>
{
public:
  AT_Command<> AT;
  AT_Command<int, string_t, int, string_t> CWLAP;
  AT_Command<int> Echo;
  AT_Command<> Test;
  AT_Command<> Test2;

  esp8266(printf_t write);
};

