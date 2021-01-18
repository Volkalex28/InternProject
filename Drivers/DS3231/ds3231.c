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

/**
 * @defgroup DS3231_Driver Driver for DS3231
 * 
 * DS3231 module (RTC, ZS-042) - This is a low cost board with an extremely 
 * accurate real time clock (RTC), with a temperature compensated crystal 
 * oscillator and crystal. The module includes a lithium battery which maintains 
 * uninterrupted operation even when the power supply is disconnected. 
 * An integrated generator improves the accuracy of the device and reduces 
 * the number of components
 */

#include <stm32f051_i2c.h>

#include "ds3231.h"

// Exported Function ----------------------------------------------------------
/** @defgroup DS3231_Exported_Function UDS3231 Exported Function
 * @ingroup DS3231_Driver
 * 
 * DS3231 module function available from other files
 * @{
 */

/**
 * @brief Function for getting date from DS3231 module
 * 
 * The function reads the date from the module via the I2C1 interface 
 * and returns it by the pointer to the @p pDS module object
 * 
 * @param[in, out] pDS Pointer to DS3231 module object
 * 
 * @return Data receiving status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t DS3231_GetDate(DS3231_t* pDS)
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

  const DS3231_Mem_Date_t* pDate = (DS3231_Mem_Date_t*)buf;

  pDS->Date.Date = pDate->Date.Date.Date10 * 10 + pDate->Date.Date.Date;
  pDS->Date.Month = pDate->Date.Month.Month10 * 10 + pDate->Date.Month.Month;
  pDS->Date.Year = pDate->Date.Year.Year10 * 10 + pDate->Date.Year.Year;
  pDS->Date.Day = pDate->Day;

  return 0;
}

/**
 * @brief Function for getting time from DS3231 module
 * 
 * The function reads the time from the module via the I2C1 interface 
 * and returns it by the pointer to the @p pDS module object
 * 
 * @param[in, out] pDS Pointer to DS3231 module object
 * 
 * @return Data receiving status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t DS3231_GetTime(DS3231_t* pDS)
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

  const DS3231_Mem_Time_t* pTime = (DS3231_Mem_Time_t*)buf;

  pDS->Time.Hours = pTime->Hours.Hour10 * 10 + pTime->Hours.Hour;
  pDS->Time.Minutes = pTime->Minutes.Min10 * 10 + pTime->Minutes.Min;
  pDS->Time.Seconds = pTime->Seconds.Sec10 * 10 + pTime->Seconds.Sec;

  return 0;
}

/**
 * @brief DS3231 module address setting function
 * 
 * @param[in, out] pDS Pointer to DS3231 module object
 * @param[in] addr DS3231 module address
 * 
 * @return Address setting status
 * @retval 0 If no errors occurred
 * @retval 1 If the pointer to the object of the DS3231 module refers to NULL
 */
uint32_t DS3231_SetAddress(DS3231_t* pDS, const uint8_t addr) 
{
  if(pDS == NULL)
  {
    return 1;
  }

  pDS->Adress = addr;

  return 0;
}

/**
 * @brief Function for getting temperature from DS3231 module
 * 
 * The function reads the temperature from the module via the I2C1 interface 
 * and returns it by the pointer to the @p pDS module object
 * 
 * @param[in, out] pDS Pointer to DS3231 module object
 * 
 * @return Data receiving status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t DS3231_GetTemp(DS3231_t* pDS)
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

  const DS3231_Temp_t* pTemp = (DS3231_Temp_t*)buf;

  pDS->Temp = pTemp->Integer + pTemp->Fraction * 0.25f;

  return 0;
}

/**
 * @brief Date setting function in DS3231 module
 * 
 * The function sets the date passed through the @p pDSDate pointer   
 * to the module (pointed to by @p pDS) via the I2C1 interface
 * 
 * @param[in] pDS Pointer to DS3231 module object
 * @param[in] pDSDate Pointer to DS3231 module date data structure
 * 
 * @return Data sending status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t DS3231_SetDate(const DS3231_t* pDS, const DS3231_Date_t* pDSDate)
{
  if(pDS == NULL || pDSDate == NULL)
  {
    return 1;
  }

  uint8_t buf[5] = {[0] = 3, 0};

  DS3231_Mem_Date_t* pDate = (DS3231_Mem_Date_t*)&buf[1];
  pDate->Date.Date.Date10   = (pDSDate->Date & 0x3F) / 10;
  pDate->Date.Date.Date     = (pDSDate->Date & 0x3F) % 10;
  pDate->Date.Month.Month10 = (pDSDate->Month & 0x1F) / 10;
  pDate->Date.Month.Month   = (pDSDate->Month & 0x1F) % 10;
  pDate->Date.Year.Year10   = (pDSDate->Year & 0xFF) / 10;
  pDate->Date.Year.Year     = (pDSDate->Year & 0xFF) % 10;
  pDate->Day = pDSDate->Day & 0x07;

  return I2C1_Transmit(pDS->Adress, buf, 5, 100);
}

/**
 * @brief Time setting function in DS3231 module
 * 
 * The function sets the time passed through the @p pDSTime pointer   
 * to the module (pointed to by @p pDS) via the I2C1 interface
 * 
 * @param[in] pDS Pointer to DS3231 module object
 * @param[in] pDSTime Pointer to DS3231 module time data structure
 * 
 * @return Data sending status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t DS3231_SetTime(const DS3231_t* pDS, const DS3231_Time_t* pDSTime)
{
  if(pDS == NULL || pDSTime == NULL)
  {
    return 1;
  }

  uint8_t buf[4] = {[0] = 0, 0};

  DS3231_Mem_Time_t* pTime = (DS3231_Mem_Time_t*)&buf[1];
  pTime->Seconds.Sec10  = (pDSTime->Seconds & 0x7F) / 10;
  pTime->Seconds.Sec    = (pDSTime->Seconds & 0x7F) % 10;
  pTime->Minutes.Min10  = (pDSTime->Minutes & 0x7F) / 10;
  pTime->Minutes.Min    = (pDSTime->Minutes & 0x7F) % 10;
  pTime->Hours.Hour10   = (pDSTime->Hours & 0x1F) / 10;
  pTime->Hours.Hour     = (pDSTime->Hours & 0x1F) % 10;

  return I2C1_Transmit(pDS->Adress, buf, 4, 100);
}
///@}
