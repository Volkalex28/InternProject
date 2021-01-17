#ifndef _DS3231_H_
#define _DS3231_H_

#include "stm32f051_i2c.h"
#include <stddef.h>

typedef struct 
{
  struct
  {
    uint8_t Sec : 4;
    uint8_t Sec10 : 4;
  } Seconds;
  struct
  {
    uint8_t Min : 4;
    uint8_t Min10 : 4;
  } Minutes;
  struct 
  {
    uint8_t Hour : 4;
    uint8_t Hour10 : 2;
    uint8_t is12 : 2;
  } Hours;
} DS3231_Mem_Time_t;

typedef struct 
{
  uint8_t Day;
  struct 
  {
    struct
    {
      uint8_t Date : 4;
      uint8_t Date10 : 4;
    } Date;
    struct
    {
      uint8_t Month : 4;
      uint8_t Month10 : 3;
      uint8_t Century : 1;
    } Month;
    struct 
    {
      uint8_t Year : 4;
      uint8_t Year10 : 4;
    } Year;
  } Date;
} DS3231_Mem_Date_t;

typedef struct 
{
  uint8_t Seconds;
  uint8_t Minutes;
  uint8_t Hours;
} DS3231_Time_t;

typedef struct 
{
  uint8_t Day;
  uint8_t Date;
  uint8_t Month;
  uint8_t Year;
} DS3231_Date_t;

typedef struct 
{
  uint8_t Integer;
  uint8_t Reserved : 6;
  uint8_t Fraction : 2;
} DS3231_Temp_t;

typedef struct 
{
  DS3231_Mem_Time_t Time;
  DS3231_Mem_Date_t Date;
  
  struct
  {
    struct Alarm1_t
    {
      uint8_t Seconds : 4;
      uint8_t Seconds10 : 3;
      uint8_t M1 : 1;
    } Seconds;
    struct 
    {
      uint8_t Minutes : 4;
      uint8_t Minutes10 : 3;
      uint8_t M2 : 1;
    } Minutes;
    struct 
    {
      uint8_t Hour : 4;
      uint8_t Hour10 : 2;
      uint8_t is12 : 1;
      uint8_t M3 : 1;
    } Hours;
    struct 
    {
      uint8_t Date : 4;
      uint8_t Date10 : 2;
      uint8_t isDY : 1;
      uint8_t M4 : 1;
    } Date;
  } Alarm1;
  struct Alarm2_t
  {
    struct 
    {
      uint8_t Minutes : 4;
      uint8_t Minutes10 : 3;
      uint8_t M2 : 1;
    } Minutes;
    struct 
    {
      uint8_t Hour : 4;
      uint8_t Hour10 : 2;
      uint8_t is12 : 1;
      uint8_t M3 : 1;
    } Hours;
    struct 
    {
      uint8_t Date : 4;
      uint8_t Date10 : 2;
      uint8_t isDY : 1;
      uint8_t M4 : 1;
    } Date;
  } Alarm2;

  struct Contol_t
  {
    uint8_t A1IE  : 1;
    uint8_t A2IE  : 1;
    uint8_t INTCN : 1;
    uint8_t RS1   : 1;
    uint8_t RS2   : 1;
    uint8_t CONV  : 1;
    uint8_t BBSQW : 1;
    uint8_t EOSC  : 1;
  } Control;
  struct Status_t
  {
    uint8_t A1F     : 1;
    uint8_t A2F     : 1;
    uint8_t BSY     : 1;
    uint8_t EN32kHz : 1;
    uint8_t Reserv  : 3;
    uint8_t OSF     : 1;
  } Status;

  uint8_t Offset;

  DS3231_Temp_t Temp;

} DS3231_Mem_t;

typedef struct 
{
  uint8_t Adress;

  DS3231_Time_t Time;
  DS3231_Date_t Date;

  struct 
  {
    uint8_t A1IE  : 1;
    uint8_t A2IE  : 1;
    uint8_t INTCN : 1;
    uint8_t RS1   : 1;
    uint8_t RS2   : 1;
    uint8_t CONV  : 1;
    uint8_t BBSQW : 1;
    uint8_t EOSC  : 1;
  } Control;

  struct 
  {
    uint8_t A1F     : 1;
    uint8_t A2F     : 1;
    uint8_t BSY     : 1;
    uint8_t EN32kHz : 1;
    uint8_t Reserv  : 3;
    uint8_t OSF     : 1;
  } Status;

  float Temp;
  
} DS3231_t;

uint32_t DS3231_GetDate(DS3231_t* pDS);
uint32_t DS3231_GetTime(DS3231_t* pDS);
uint32_t DS3231_GetTemp(DS3231_t* pDS);
uint32_t DS3231_SetAddress(DS3231_t* pDS, const uint8_t addr);

uint32_t DS3231_SetDate(const DS3231_t* pDS, const DS3231_Date_t* pDSDate);
uint32_t DS3231_SetTime(const DS3231_t* pDS, const DS3231_Time_t* pDSTime);

#endif // _DS3231_H_
