/**
 * @file ds3231.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source Code File for DS3231 Driver
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stm32f051_i2c.h>
#include "ds3231.h"

// Exported Function ----------------------------------------------------------
uint32_t DS3231_GetDate(DS3231_t * const pDS)
{
  if(pDS == NULL)
  {
    return 1;
  }

  uint8_t buf[4] = {[0] = 3, 0};

  if(I2C1_Transmit(pDS->Adress, buf, 1, 100) != 0)
  {
    return 1;
  }
  if(I2C1_Receive(pDS->Adress, buf, 4, 100) != 0)
  {
    return 1;
  }

  const DS3231_Mem_Date_t * pDate = (DS3231_Mem_Date_t *)buf;

  pDS->Date.Date = pDate->Date.Date.Date10 * 10 + pDate->Date.Date.Date;
  pDS->Date.Month = pDate->Date.Month.Month10 * 10 + pDate->Date.Month.Month;
  pDS->Date.Year = pDate->Date.Year.Year10 * 10 + pDate->Date.Year.Year;
  pDS->Date.Day = pDate->Day;

  return 0;
}

uint32_t DS3231_GetTime(DS3231_t * const pDS)
{
  if(pDS == NULL)
  {
    return 1;
  }

  uint8_t buf[3] = {[0] = 0, 0};

  if(I2C1_Transmit(pDS->Adress, buf, 1, 100) != 0)
  {
    return 1;
  }
  if(I2C1_Receive(pDS->Adress, buf, 3, 100) != 0)
  {
    return 1;
  }

  const DS3231_Mem_Time_t * pTime = (DS3231_Mem_Time_t *)buf;

  pDS->Time.Hours = pTime->Hours.Hour10 * 10 + pTime->Hours.Hour;
  pDS->Time.Minutes = pTime->Minutes.Min10 * 10 + pTime->Minutes.Min;
  pDS->Time.Seconds = pTime->Seconds.Sec10 * 10 + pTime->Seconds.Sec;

  return 0;
}

uint32_t DS3231_SetAddress(DS3231_t * const pDS, const uint8_t addr) 
{
  if(pDS == NULL)
  {
    return 1;
  }

  pDS->Adress = addr;
  
  I2C1_Init();

  return 0;
}

uint32_t DS3231_GetTemp(DS3231_t * const pDS)
{
  if(pDS == NULL)
  {
    return 1;
  }

  uint8_t buf[2] = {[0] = 0x11, 0};

  if(I2C1_Transmit(pDS->Adress, buf, 1, 100) != 0)
  {
    return 1;
  }
  if(I2C1_Receive(pDS->Adress, buf, 2, 100) != 0)
  {
    return 1;
  }

  const DS3231_Temp_t * pTemp = (DS3231_Temp_t *)buf;

  pDS->Temp = pTemp->Integer + pTemp->Fraction * 0.25f;

  return 0;
}

uint32_t DS3231_SetDate(const DS3231_t * const pDS, const DS3231_Date_t * const pDSDate)
{
  if(pDS == NULL || pDSDate == NULL)
  {
    return 1;
  }

  uint8_t buf[5] = {[0] = 3, 0};

  DS3231_Mem_Date_t * pDate = (DS3231_Mem_Date_t *)&buf[1];
  pDate->Date.Date.Date10   = (pDSDate->Date & 0x3F) / 10;
  pDate->Date.Date.Date     = (pDSDate->Date & 0x3F) % 10;
  pDate->Date.Month.Month10 = (pDSDate->Month & 0x1F) / 10;
  pDate->Date.Month.Month   = (pDSDate->Month & 0x1F) % 10;
  pDate->Date.Year.Year10   = (pDSDate->Year & 0xFF) / 10;
  pDate->Date.Year.Year     = (pDSDate->Year & 0xFF) % 10;
  pDate->Day = pDSDate->Day & 0x07;

  return I2C1_Transmit(pDS->Adress, buf, 5, 100);
}

uint32_t DS3231_SetTime(const DS3231_t * const pDS, const DS3231_Time_t * const pDSTime)
{
  if(pDS == NULL || pDSTime == NULL)
  {
    return 1;
  }

  uint8_t buf[4] = {[0] = 0, 0};

  DS3231_Mem_Time_t * pTime = (DS3231_Mem_Time_t *)&buf[1];
  pTime->Seconds.Sec10  = (pDSTime->Seconds & 0x7F) / 10;
  pTime->Seconds.Sec    = (pDSTime->Seconds & 0x7F) % 10;
  pTime->Minutes.Min10  = (pDSTime->Minutes & 0x7F) / 10;
  pTime->Minutes.Min    = (pDSTime->Minutes & 0x7F) % 10;
  pTime->Hours.Hour10   = (pDSTime->Hours & 0x1F) / 10;
  pTime->Hours.Hour     = (pDSTime->Hours & 0x1F) % 10;

  return I2C1_Transmit(pDS->Adress, buf, 4, 100);
}
