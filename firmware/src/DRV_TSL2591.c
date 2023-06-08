/* ************************************************************************** */
/** 

  @Company
 Microchip, Inc

  @File Name
    DRV_TSL2591.c

  @Summary
  Driver layer for the TSL2591 Ambient Light Sensor to be used in a 
   Microchip Harmony3 project with I2C Driver

 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "definitions.h"
#include <stdio.h>
#include <string.h>
#include "DRV_TSL2591.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

#define TSL2591_I2C_ADDRESS             0x29
#define TSL2591_VAL_CHIPID              0x50

/**
 * @brief TSL2591 command register setting.
 */
#define TSL2591_COMMAND_NORMAL_OP       0xA0
#define TSL2591_COMMAND_SPEC_FUNC       0xE0
#define TSL2591_SF_INT_SET              0x04
#define TSL2591_SF_CLEAR_ALS_INT        0x06
#define TSL2591_SF_CLEAR_ALS_NOPERS_INT 0x07
#define TSL2591_SF_CLEAR_NOPERS_INT     0x0A

/**
 * @brief TSL2591 enable register setting.
 */
#define TSL2591_ENABLE_NPIEN            0x80
#define TSL2591_ENABLE_SAI              0x40
#define TSL2591_ENABLE_AIEN             0x10
#define TSL2591_ENABLE_AEN              0x02
#define TSL2591_ENABLE_PON              0x01

/**
 * @brief TSL2591 registers list.
 */
#define TSL2591_REG_ENABLE              0x00
#define TSL2591_REG_CONFIG              0x01
#define TSL2591_REG_AILTL               0x04
#define TSL2591_REG_AILTH               0x05
#define TSL2591_REG_AIHTL               0x06
#define TSL2591_REG_AIHTH               0x07
#define TSL2591_REG_NPAILTL             0x08
#define TSL2591_REG_NPAILTH             0x09
#define TSL2591_REG_NPAIHTL             0x0A
#define TSL2591_REG_NPAIHTH             0x0B
#define TSL2591_REG_PERSIST             0x0C
#define TSL2591_REG_PID                 0x11
#define TSL2591_REG_CHIPID              0x12
#define TSL2591_REG_STATUS              0x13
#define TSL2591_REG_C0DATAL             0x14
#define TSL2591_REG_C0DATAH             0x15
#define TSL2591_REG_C1DATAL             0x16
#define TSL2591_REG_C1DATAH             0x17

/**
 * @brief TSL2591 lux calculation values.
 */
#define AMBIENT21_GAIN_0                    1.0f
#define AMBIENT21_GAIN_1                    25.0f
#define AMBIENT21_GAIN_2                    428.0f
#define AMBIENT21_GAIN_3                    9876.0f
#define AMBIENT21_TIME_RES                  100.0f
#define AMBIENT21_LUX_GDF                   900.0f
#define AMBIENT21_LUX_COEF                  1.0f

/**
 * @brief Values to be used within this Driver
 */
#define TSL2591_DEFAULT_CONFIG          (TSL2591_CONFIG_AGAIN_MID | TSL2591_CONFIG_ATIME_200MS)
#define TSL2591_ENABLE_READING          (TSL2591_ENABLE_PON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN)
#define TSL2591_CLEAR_INTERRUPTS        (TSL2591_COMMAND_SPEC_FUNC | TSL2591_SF_CLEAR_ALS_NOPERS_INT) 
#define TSL2591_REG_RAWDATA             0x14

DATA_TSL2591 driverData;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */


/**
 * @brief writeCommand - Deliver the specified command via I2C
 * @param driver - Driver Object to use for I2C Communications
 * @param command - 8 Bit Command to send
 * @param len - length of command (in bytes)
 * @param normalop - boolean, true to OR with NORMAL OPERATION flag
 * @return - return value from RET_TSL2591 typedef enum
 */
RET_TSL2591 writeCommand(DATA_TSL2591* driver, char command, char len, bool normalop) {
    if(normalop) {
        command |= TSL2591_COMMAND_NORMAL_OP;
    }
    
    if(!DRV_I2C_WriteTransfer(driver->drvI2CHandle, TSL2591_I2C_ADDRESS, (void *)&command, len)) {
        return RET_TSL2591_I2C_DRIVER_ERROR;
    }
    
    return RET_TSL2591_SUCCESS;
}

/**
 * @brief writeReadCommand - Deliver the specified command via I2C, then read back values
 * @param driver - Driver Object to use for I2C Communications
 * @param command - 8 Bit Command to send
 * @param len - length of values to be read (in bytes)
 * @return - return value from RET_TSL2591 typedef enum
 */
RET_TSL2591 writeReadCommand(DATA_TSL2591* driver, char command, char len) {
    char* rxbuffer = (char*)&driver->rxBuffer;
    command = command | TSL2591_COMMAND_NORMAL_OP;
    
    if(!DRV_I2C_WriteReadTransfer(driver->drvI2CHandle, TSL2591_I2C_ADDRESS, (void*)&command, 1, (void *)rxbuffer, len)) {
        return RET_TSL2591_I2C_DRIVER_ERROR;
    }
    
    return RET_TSL2591_SUCCESS;
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */


RET_TSL2591 DRV_TSL2591_Initialize(DATA_TSL2591* instance, int intpin) {
    instance->drvI2CHandle = DRV_I2C_Open(instance->drvIndex, DRV_IO_INTENT_READWRITE);
    instance->interruptPin = intpin;
    
    if(instance->drvI2CHandle == DRV_HANDLE_INVALID) {
        printf("TSL2591 Invalid I2C Driver Handle\r\n");
        return RET_TSL2591_INVALID_I2C;
    }
    else {
        printf("TSL2591 Driver Init OK\r\n");
    }
    
    writeReadCommand(instance, TSL2591_REG_CHIPID, 1);

    if(instance->rxBuffer[0] == TSL2591_VAL_CHIPID) {
        printf("TSL2591 ChipID Found: 0x%x\r\n", instance->rxBuffer[0]);
    }
    else {
        printf("TSL2591 ChipID Invalid: 0x%x\r\n", instance->rxBuffer[0]);
        return RET_TSL2591_INVALID_CHIPID;
    }

    DRV_TSL2591_SetConfig(instance, TSL2591_CONFIG_AGAIN_MID, TSL2591_CONFIG_ATIME_200MS);
    
    if(writeCommand(instance, TSL2591_REG_ENABLE, 1, true) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    
    if(writeCommand(instance, TSL2591_ENABLE_READING, 1, false) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    
    if(writeReadCommand(instance, TSL2591_CLEAR_INTERRUPTS, 1) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    return RET_TSL2591_SUCCESS;
}

RET_TSL2591 DRV_TSL2591_GetRawValue(DATA_TSL2591* instance) {
    if(instance->drvI2CHandle == DRV_HANDLE_INVALID) {
        printf("TSL2591 Invalid I2C Driver Handle\r\n");
        return RET_TSL2591_INVALID_I2C;
    }
    
    if(writeReadCommand(instance, TSL2591_CLEAR_INTERRUPTS, 1) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    
    if(writeReadCommand(instance, TSL2591_REG_RAWDATA, 4) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    
    // Perform CHO/CH1 -> Lux calculations
    uint16_t ch0, ch1;
    
    ch0 = (instance->rxBuffer[1]<<8) | instance->rxBuffer[0];
    ch1 = (instance->rxBuffer[3]<<8) | instance->rxBuffer[2];
    
    uint16_t lux = (uint16_t) ((((float) ch0-ch1) * (1.0 - (float) ch1/ch0)) / instance->cpl);
    printf("%d, %d, Lux:%d\r\n", ch0, ch1, lux);
    
    return RET_TSL2591_SUCCESS;
}

RET_TSL2591 DRV_TSL2591_SetConfig(DATA_TSL2591* instance, uint8_t again, uint8_t atime) {
        
    if(writeCommand(instance, TSL2591_REG_CONFIG, 1, true) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }
    if(writeCommand(instance, (again | atime) , 1, false) != RET_TSL2591_SUCCESS) {
        return RET_TSL2591_ERROR_UNKNOWN;
    }   

    switch (again) {
        case TSL2591_CONFIG_AGAIN_LOW:
            instance->again = AMBIENT21_GAIN_0;
            break;
        case TSL2591_CONFIG_AGAIN_MID:
            instance->again = AMBIENT21_GAIN_1;
            break;
        case TSL2591_CONFIG_AGAIN_HIGH:
            instance->again = AMBIENT21_GAIN_2;
            break;
        case TSL2591_CONFIG_AGAIN_MAX:
            instance->again = AMBIENT21_GAIN_3;
            break;
        default:
            instance->again = AMBIENT21_GAIN_0;
            break;
    }
    instance->atime_ms = AMBIENT21_TIME_RES + AMBIENT21_TIME_RES * atime;
    instance->cpl = (instance->atime_ms * instance->again) / AMBIENT21_LUX_GDF;
    
    return RET_TSL2591_SUCCESS;
}

RET_TSL2591 DRV_TSL2591_RegisterCallback(DATA_TSL2591* instance, TSL2591_Event_CallBack cb, void* context) {
    if(cb == NULL) {
        return RET_TSL2591_NULL_CALLBACK; 
    }
    instance->callBack = cb;
    
    EIC_CallbackRegister(instance->interruptPin, (EIC_CALLBACK)instance->callBack, (uintptr_t)context);
    
    return RET_TSL2591_SUCCESS;
}


/* *****************************************************************************
 End of File
 */
