/**
 * @file esp8266.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief
 *
 */

#ifndef _ESP8266_H_
#define _ESP8266_H_

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @defgroup ESP8266
   * @{
   */

  /**
   * @brief Function to initialize and check for the presence of ESP.
   *
   * Sends an "AT" command and waits for an "OK" response
   *
   * @param[in] timeout Maximum time to wait for a response in milliseconds
   * @return ESP init status
   * @retval 0 If the device is present
   * @retval 1 If there is no response from the device
   */
  const uint32_t ESP_Init(const uint32_t timeout);

  /**
   * @brief Function for parsing data from ESP.
   *
   * Accepts one character at a time. Considers the command completed after the arrival of "\\r"
   *
   * @param[in] sym Symbol from ESP
   * @return ESP parse status
   * @retval 0 If a trailing character is found
   * @retval 1 If a character is added to the string or the string is empty
   */
  const uint32_t ESP_Parse(const char sym);

  /**
   * @brief Function to start scanning for available Wi-Fi networks.
   *
   * Calls the "CWLAP" command. The scanned networks will be placed in the esp8266::Nets array
   *
   * @attention After executing this command, you need to wait 2-5 seconds for the scan to complete
   *
   * @return ESP scan status
   * @retval 0 If the scan has started successfully
   * @retval 1 ESP not initialized
   */
  const uint32_t ESP_Scan(void);

  /**
   * @brief WIFI connection function
   *
   * @param[in] SSID Network name to connect
   * @param[in] Password Network password to connect
   * @param[in] timeout Maximum time to wait for a response in milliseconds
   * @return ESP connected status
   * @retval 0 If the connection was successful
   * @retval 1 ESP not initialized
   * @retval 2 Connection timeout
   */
  const uint32_t ESP_ConnectWIFI(const char * SSID, const char * Password, const uint32_t timeout);

  /**
   * @brief Function to display available networks for connection
   *
   * Outputs via @p pUART networks that were detected after executing the ESP_Scan() function
   *
   * @param pUART Pointer to UART
   * @param timeout Maximum time to wait for sending data in milliseconds
   * @return ESP status for printing SSIDs
   * @retval 0 If the sending data was successful
   * @retval 1 Available networks not found
   * @retval 2 Connection timeout
   */
  const uint32_t ESP_PrintSSIDs(STM32F051_UART_t * pUART, const uint32_t timeout);

  /**
   * @brief Function for connect to TCP Server
   *
   * @param ip IP address TCP server in the format "%.%.%.%"
   * @param port Port on which TCP Server is open
   * @param timeout Maximum server connection timeout in milliseconds
   * @return ESP status for Server connection
   * @retval 0 If connecting was successful
   * @retval 1 If diconnection from last connection failed
   * @retval 2 If conection to the new connection failed
   */
  const uint32_t ESP_ConnectServer(const char * ip, const uint16_t port, const uint32_t timeout);

  /**
   * @brief Sending function to the TCP Server
   *
   * @param str Data for sending in string format
   * @param timeout Maximum time to wait for sending successfully data to TCP Server
   * @return ESP status for data sending
   * @retval 0 If data sended was successful
   * @retval 1 If request to sending data failed
   * @retval 2 If sending data failed
   * @retval 3 If connected to Server not available
   */
  const uint32_t ESP_SendToServer(const char * str, const uint32_t timeout);

  ///@}

#ifdef __cplusplus
}
#endif

#endif // _ESP8266_H_