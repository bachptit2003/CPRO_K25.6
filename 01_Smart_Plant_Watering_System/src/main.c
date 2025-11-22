/* main.c
 * Entry point for Smart Plant Watering System
 * Initializes the system and runs the main control loop
 */

#include "spws_controller.h"
#include <stdio.h>
#include "hal_actuators.h"

int main(void)
{
    printf("======================================\n");
    printf("  Smart Plant Watering System (SPWS)\n");
    printf("======================================\n");
    printf("Controls:\n");
    printf("  [m]  → Toggle AUTO / MANUAL mode\n");
    printf("  [t]  → Manual watering (only in MANUAL mode)\n");
    printf("--------------------------------------\n\n");

    SPWS_Init();

    while (1) {
        SPWS_Run();
        HAL_DelayMs(50);   /* Small delay to reduce CPU usage */
    }

    return 0;
}