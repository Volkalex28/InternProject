/**
 * @file FreeRTOSInit.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief
 *
 */

#ifndef __FREERTOSINIT_H__
#define __FREERTOSINIT_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern xSemaphoreHandle endReadUART;
extern xSemaphoreHandle endCommandUART;

void FreeRTOS_Init(void);

#endif // __FREERTOSINIT_H__