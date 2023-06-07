/* ************************************************************************** */
/** DRV_TSL2591.h

  @Company
    Microchip

  @File Name
    DRV_TSL2591.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef DRV_TSL2591_H    /* Guard against multiple inclusion */
#define DRV_TSL2591_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
    
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define TSL2591_RXBUFFER_SIZE       10

/**
 * @brief TSL2591 config register setting.
 * @details Specified setting for config register of TSL2591 driver.
 */
#define TSL2591_CONFIG_SRESET             0x80
#define TSL2591_CONFIG_AGAIN_LOW          0x00
#define TSL2591_CONFIG_AGAIN_MID          0x10
#define TSL2591_CONFIG_AGAIN_HIGH         0x20
#define TSL2591_CONFIG_AGAIN_MAX          0x30
#define TSL2591_CONFIG_AGAIN_MASK         0x30
#define TSL2591_CONFIG_ATIME_100MS        0x00
#define TSL2591_CONFIG_ATIME_200MS        0x01
#define TSL2591_CONFIG_ATIME_300MS        0x02
#define TSL2591_CONFIG_ATIME_400MS        0x03
#define TSL2591_CONFIG_ATIME_500MS        0x04
#define TSL2591_CONFIG_ATIME_600MS        0x05
#define TSL2591_CONFIG_ATIME_MASK         0x07

    
// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
typedef void (*TSL2591_Event_CallBack)(uintptr_t context);

typedef struct {
   /* The application's current state */
   DRV_HANDLE drvI2CHandle;
   TSL2591_Event_CallBack callBack;
   SYS_MODULE_INDEX drvIndex;
   int interruptPin;
   float atime_ms;
   float again;
   float cpl;
   char rxBuffer[TSL2591_RXBUFFER_SIZE];
} DATA_TSL2591;

typedef enum {
    RET_TSL2591_SUCCESS = 0,
    RET_TSL2591_INVALID_I2C,
    RET_TSL2591_INVALID_CHIPID,
    RET_TSL2591_I2C_DRIVER_ERROR,
    RET_TSL2591_NULL_CALLBACK,
    RET_TSL2591_ERROR_UNKNOWN
}RET_TSL2591;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************
RET_TSL2591 DRV_TSL2591_Initialize(DATA_TSL2591* instance, int intpin);
RET_TSL2591 DRV_TSL2591_GetRawValue(DATA_TSL2591* instance);
RET_TSL2591 DRV_TSL2591_SetConfig(DATA_TSL2591* instance, uint8_t again, uint8_t atime);
RET_TSL2591 DRV_TSL2591_RegisterCallback(DATA_TSL2591* instance, TSL2591_Event_CallBack cb, void* context);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* DRV_TSL2591_H */

/* *****************************************************************************
 End of File
 */
