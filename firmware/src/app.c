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

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize (const SYS_MODULE_INDEX drvIndex)
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;  

    appData.driverData.drvIndex = drvIndex;
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
            if(DRV_TSL2591_Initialize(&appData.driverData) != RET_TSL2591_SUCCESS) {
                
            }
            appData.state = APP_STATE_SERVICE_TASKS;
            break;
        case APP_STATE_SERVICE_TASKS:
            DRV_TSL2591_GetRawValue(&appData.driverData);
            printf("app.c RawData: 0x%02x%02x%02x%02x\r\n", appData.driverData.rxBuffer[0], appData.driverData.rxBuffer[1], appData.driverData.rxBuffer[2], appData.driverData.rxBuffer[3]);
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
