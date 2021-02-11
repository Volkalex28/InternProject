/**
 * @file ds3231.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source Code File for DS3231 Driver
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @addtogroup Drivers
 * @defgroup DS3231_Driver DS3231
 * @ingroup Drivers
 * 
 * DS3231 module (RTC, ZS-042) - This is a low cost board with an extremely 
 * accurate real time clock (RTC), with a temperature compensated crystal 
 * oscillator and crystal. The module includes a lithium battery which maintains 
 * uninterrupted operation even when the power supply is disconnected. 
 * An integrated generator improves the accuracy of the device and reduces 
 * the number of components
 */

#ifndef _DS3231_H_
#define _DS3231_H_

#include <stddef.h>

// Exported Types -------------------------------------------------------------
/** @defgroup DS3231_Exported_Types Exported Types
 * @ingroup DS3231_Driver
 * 
 * DS3231 module types available from other files
 * @{
 */

/**
 * @brief Data type describing the data structure of the current time 
 * in the DS3231 module
 */
typedef struct
{
  struct
  {
    uint8_t Sec : 4;      /**< Least decimal place of seconds */
    uint8_t Sec10 : 4;    /**< Most significant decimal place of seconds */
  } Seconds;              ///< Seconds register

  struct
  {
    uint8_t Min : 4;      /**< Least decimal place of minutes */
    uint8_t Min10 : 4;    /**< Most significant decimal place of minutes */
  } Minutes;              ///< Minutes register

  struct
  {
    uint8_t Hour : 4;     /**< Least decimal place of hours */
    uint8_t Hour10 : 2;   /**< Most significant decimal place of hours */
    uint8_t is12 : 2;     /**< Is the 12 hour mode set? */
  } Hours;                ///< Hours register

} DS3231_Mem_Time_t;

/**
 * @brief Data type describing the data structure of the current date 
 * in the DS3231 module
 */
typedef struct
{
  uint8_t Day;  ///< Current day of weak

  struct
  {
    struct
    {
      uint8_t Date : 4;     /**< Least decimal place of date */
      uint8_t Date10 : 4;   /**< Most significant decimal place of date */
    } Date;                 ///< Date register

    struct
    {
      uint8_t Month : 4;    /**< Least decimal place of month */
      uint8_t Month10 : 3;  /**< Most significant decimal place of month */
      uint8_t Century : 1;  /**< Has the century changed */
    } Month;                ///< Month register

    struct
    {
      uint8_t Year : 4;     /**< Least decimal place of year */
      uint8_t Year10 : 4;   /**< Most significant decimal place of year */
    } Year;                 ///< Year register

  } Date;       ///< Current date

} DS3231_Mem_Date_t;

/**
 * @brief Data type declaration for time
 */
typedef struct
{
  uint8_t Seconds;  ///< Seconds
  uint8_t Minutes;  ///< Minutes
  uint8_t Hours;    ///< Hours
} DS3231_Time_t;

/**
 * @brief Data type declaration for date
 */
typedef struct
{
  uint8_t Day;    ///< Day of week
  uint8_t Date;   ///< Day of month
  uint8_t Month;  ///< Month
  uint8_t Year;   ///< Year
} DS3231_Date_t;

/**
 * @brief Data type declaration for temperature
 */
typedef struct
{
  uint8_t Integer;      /**< Whole part */
  uint8_t Reserved : 6; /**< Reserved */
  uint8_t Fraction : 2; /**< Fractional part with a resolution of 0.25 */
} DS3231_Temp_t;

/**
 * @brief Data type declaration for control registesr
 */
typedef struct 
{
  /** Enables/disables alarm interrupts 1 */
  uint8_t A1IE  : 1;    
  /** Enables/disables alarm interrupts 2 */    
  uint8_t A2IE  : 1;        
  /** Enables/disables interrupts on the INT/SQW pin of the microcircuit */
  uint8_t INTCN : 1;    
  /** Set the frequency of the square wave displayed on the INT/SQW pin of the microcircuit */
  uint8_t RS    : 2;    
  /** It causes the chip to start temperature conversion */    
  uint8_t CONV  : 1;
  /** If you write 1 to this bit, then when operating on battery 
   * power and there is no power on the INT / SQW pin, pulses will 
   * continue to be output
   */
  uint8_t BBSQW : 1;      
  /**
   * Generator control discharge. If set to logic 0, the generator is on
   */
  uint8_t EOSC  : 1;
} DS3231_Control_t;

/**
 * @brief Data type declaration for status registesr
 */
typedef struct 
{
  uint8_t A1F     : 1;  /**< Interrupt flag when alarm 1 and chronometer value match */
  uint8_t A2F     : 1;  /**< Interrupt flag when alarm 2 and chronometer value match */
  uint8_t BSY     : 1;  /**< Busy flag. Cocked on temperature conversion */
  uint8_t EN32kHz : 1;  /**< 32 kHz square wave output resolution bit at 32 kHz output of the microcircuit */
  uint8_t Reserv  : 3;  /**< Reserved */
  uint8_t OSF     : 1;  /**< Generator operation control flag. Logic 1 indicates the generator is stopped */
} DS3231_Status_t;

/**
 * @brief Data type declaration for the memory structure of the DS3231 module
 */
typedef struct
{
  DS3231_Mem_Time_t Time;     ///< Current time in DS3231 module
  DS3231_Mem_Date_t Date;     ///< Current date in DS3231 module
  
  uint8_t Alarms[7];          ///< Alarms registers

  DS3231_Control_t Control;   ///< Control register
  DS3231_Status_t Status;     ///< Status register

  uint8_t Offset;             ///< The register stores data for editing the frequency of the crystal

  DS3231_Temp_t Temp;         ///< Crystal temperature register

} DS3231_Mem_t;

/**
 * @brief DS3231 Module Object Declaration
 */
typedef struct
{
  uint8_t Adress;              ///< DS3231 module address

  DS3231_Time_t Time;          ///< Time received from the module DS3231
  DS3231_Date_t Date;          ///< Date received from the module DS3231

  DS3231_Control_t Control;    ///< DS3231 module settings
  DS3231_Status_t Status;      ///< DS3231 module status

  float Temp;                  ///< DS3231 module temperature
  
} DS3231_t;

///@}

// Exported Function ----------------------------------------------------------
/** @defgroup DS3231_Exported_Function Exported Function
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
uint32_t DS3231_GetDate(DS3231_t * const pDS);

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
uint32_t DS3231_GetTime(DS3231_t * const pDS);

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
uint32_t DS3231_GetTemp(DS3231_t * const pDS);

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
uint32_t DS3231_SetAddress(DS3231_t * const pDS, const uint8_t addr);

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
uint32_t DS3231_SetDate(const DS3231_t * const pDS, const DS3231_Date_t * const pDSDate);

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
uint32_t DS3231_SetTime(const DS3231_t * const pDS, const DS3231_Time_t * const pDSTime);
///@}

#endif // _DS3231_H_
