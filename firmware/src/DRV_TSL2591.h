/* ************************************************************************** */
/** DRV_TSL2591.h

  @Company
    Microchip, Inc

  @File Name
    DRV_TSL2591.h

  @Summary
  Header File and API for DRV_TSL2591 driver, for use in a Microchip Harmony 3
 * project using I2C Driver

 */
/* ************************************************************************** */

#ifndef DRV_TSL2591_H    /* Guard against multiple inclusion */
#define DRV_TSL2591_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

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
#define TSL2591_RXBUFFER_SIZE             10

/**
 * @brief TSL2591 config register setting.
 * @details Specified settings for config register of TSL2591 driver.
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

/** 
 * @Function
 *  RET_TSL2591 DRV_TSL2591_Initialize ( DATA_TSL2591* instance, int intpin ) 
 * 
 * @Summary
 *  Initialize the TSL2591 Driver with an empty DATA_TSL2591 object and an
 *   interrupt pin to configure 
 * 
 * @param instance - Blank DATA_TSL2591 object to use
 * @param intpin - Harmony 3 EIC Pin to use (i.e. EIC_PIN_07) 
 * 
 */
RET_TSL2591 DRV_TSL2591_Initialize(DATA_TSL2591* instance, int intpin);

/** 
 * @Function
 *  RET_TSL2591 DRV_TSL2591_GetRawValue ( DATA_TSL2591* instance ) 
 * 
 * @Summary
 *  Get a value from the TSL2591 pin and return it in the 
 *  provided DATA_TSL2591 object
 * 
 * @param instance - DATA_TSL2591 object to use
 * 
 */
RET_TSL2591 DRV_TSL2591_SetConfig(DATA_TSL2591* instance);

/** 
 * @Function
 *  RET_TSL2591 DRV_TSL2591_GetRawValue ( DATA_TSL2591* instance, uint8_t again, uint8_t atime )
 * 
 * @Summary
 *  Configure and use new time and gain values for the given instance
 * 
 * @param instance - DATA_TSL2591 object to use
 * @param again - Gain setting to use (TSL2591_CONFIG_AGAIN_LOW/MID/HIGH/MAX)
 * @param atime - Time to use for analog conversion (TSL2591_CONFIG_ATIME_100MS...600MS)
 * 
 */
RET_TSL2591 DRV_TSL2591_SetConfig(DATA_TSL2591* instance, uint8_t again, uint8_t atime);

/** 
 * @Function
 *  RET_TSL2591 DRV_TSL2591_RegisterCallback ( DATA_TSL2591* instance, TSL2591_Event_CallBack cb, void* context ) 
 * 
 * @Summary
 *  Register an application callback to trigger when interrupt pin is asserted
 * 
 * @param instance - DATA_TSL2591 object to use
 * @param cb - Callback to Trigger
 * @param context - User Data to be delivered back through the callback
 * 
 */
RET_TSL2591 DRV_TSL2591_RegisterCallback(DATA_TSL2591* instance, TSL2591_Event_CallBack cb, void* context);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* DRV_TSL2591_H */

/* *****************************************************************************
 End of File
 */
