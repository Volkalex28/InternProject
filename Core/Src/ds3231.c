#include "ds3231.h"

void DS3231_GetDate(DS3231_t* pDS)
{
  uint8_t buf[4] = {3};

  I2C_Transmit(pDS->Adress, buf, 1, 100);
  I2C_Receive(pDS->Adress, buf, 4, 100);

  DS3231_Mem_Date_t* pDate = (DS3231_Mem_Date_t*)buf;

  pDS->Date.Date = pDate->Date.Date.Date10 * 10 + pDate->Date.Date.Date;
  pDS->Date.Month = pDate->Date.Month.Month10 * 10 + pDate->Date.Month.Month;
  pDS->Date.Year = pDate->Date.Year.Year10 * 10 + pDate->Date.Year.Year;
  pDS->Date.Day = pDate->Day;
}

void DS3231_GetTime(DS3231_t* pDS)
{
  uint8_t buf[3] = {0};

  I2C_Transmit(pDS->Adress, buf, 1, 100);
  I2C_Receive(pDS->Adress, buf, 3, 100);

  DS3231_Mem_Time_t* pTime = (DS3231_Mem_Time_t*)buf;

  pDS->Time.Hours = pTime->Hours.Hour10 * 10 + pTime->Hours.Hour;
  pDS->Time.Minutes = pTime->Minutes.Min10 * 10 + pTime->Minutes.Min;
  pDS->Time.Seconds = pTime->Seconds.Sec10 * 10 + pTime->Seconds.Sec;
}

void DS3231_SetAdress(DS3231_t* pDS, uint8_t Adr) 
{
  pDS->Adress = Adr;
}

void DS3231_GetTemp(DS3231_t* pDS)
{
  uint8_t buf[2] = {0x11};

  I2C_Transmit(pDS->Adress, buf, 1, 100);
  I2C_Receive(pDS->Adress, buf, 2, 100);

  DS3231_Temp_t* pTemp = (DS3231_Temp_t*)buf;

  pDS->Temp = pTemp->Integer + pTemp->Fraction * 0.25f;
}



void DS3231_SetDate(DS3231_t* pDS, DS3231_Date_t* pDSDate)
{
  uint8_t buf[5] = {3};

  DS3231_Mem_Date_t* pDate = (DS3231_Mem_Date_t*)&buf[1];
  pDate->Date.Date.Date10   = (pDSDate->Date & 0x3F) / 10;
  pDate->Date.Date.Date     = (pDSDate->Date & 0x3F) % 10;
  pDate->Date.Month.Month10 = (pDSDate->Month & 0x1F) / 10;
  pDate->Date.Month.Month   = (pDSDate->Month & 0x1F) % 10;
  pDate->Date.Year.Year10   = (pDSDate->Year & 0xFF) / 10;
  pDate->Date.Year.Year     = (pDSDate->Year & 0xFF) % 10;
  pDate->Day = pDSDate->Day & 0x07;

  I2C_Transmit(pDS->Adress, buf, 5, 100);
}

void DS3231_SetTime(DS3231_t* pDS, DS3231_Time_t* pDSTime)
{
  uint8_t buf[4] = {0};

  DS3231_Mem_Time_t* pTime = (DS3231_Mem_Time_t*)&buf[1];
  pTime->Seconds.Sec10  = (pDSTime->Seconds & 0x7F) / 10;
  pTime->Seconds.Sec    = (pDSTime->Seconds & 0x7F) % 10;
  pTime->Minutes.Min10  = (pDSTime->Minutes & 0x7F) / 10;
  pTime->Minutes.Min    = (pDSTime->Minutes & 0x7F) % 10;
  pTime->Hours.Hour10   = (pDSTime->Hours & 0x1F) / 10;
  pTime->Hours.Hour     = (pDSTime->Hours & 0x1F) % 10;

  I2C_Transmit(pDS->Adress, buf, 4, 100);
}