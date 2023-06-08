/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <string.h>
#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void eventCallback(uintptr_t context) {
    APP_DATA* intAppData = (APP_DATA*)context;

    intAppData->sampleReady = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void APP_Initialize (const SYS_MODULE_INDEX drvIndex, int intpin)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;  

    appData.driverData.drvIndex = drvIndex;
    appData.interruptPin = intpin;
    appData.sampleReady = true;  // Allows system to request the first sample after configuration
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{

    switch(appData.state) {
        case APP_STATE_INIT:
            if(DRV_TSL2591_Initialize(&appData.driverData, appData.interruptPin) != RET_TSL2591_SUCCESS) {
                printf("App.c: Error Initializing TSL Driver\r\n");
            }
            DRV_TSL2591_RegisterCallback(&appData.driverData, &eventCallback, (void*)&appData);
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        case APP_STATE_SERVICE_TASKS:
            if(appData.sampleReady) {
                appData.sampleReady = false;
                DRV_TSL2591_GetRawValue(&appData.driverData);
                printf("app.c RawData: 0x%02x%02x%02x%02x\r\n", appData.driverData.rxBuffer[0], appData.driverData.rxBuffer[1], appData.driverData.rxBuffer[2], appData.driverData.rxBuffer[3]);
                printf("app.c Lux:%d\r\n", appData.driverData.lux);
            }
            break;
        case APP_STATE_ERROR:
        default:
            break;
    }
            
    return;
}


/*******************************************************************************
 End of File
 */
