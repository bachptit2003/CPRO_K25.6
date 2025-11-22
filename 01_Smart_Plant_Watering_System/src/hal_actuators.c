/* hal_actuators.c
 * Simulated pump and RGB/LED control for PC testing
 */

#include "hal_actuators.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static uint32_t systemStartMs = 0;

/* -------------------------------------------------------------------------- */
void HAL_Init(void)
{
    systemStartMs = (uint32_t)(time(NULL) * 1000ULL);
    printf("HAL: Actuators initialized (simulated)\n");
}

/* -------------------------------------------------------------------------- */
void HAL_TurnPumpOn(void)
{
    printf("*** PUMP ON  (Watering started) ***\n");
}

/* -------------------------------------------------------------------------- */
void HAL_TurnPumpOff(void)
{
    printf("*** PUMP OFF (Watering stopped) ***\n");
}

/* -------------------------------------------------------------------------- */
void HAL_SetLedState(LedState_t state)
{
    switch (state) {
        case LED_NORMAL:
            printf("LED → GREEN   (Normal - Sufficient moisture)\n");
            break;
        case LED_WATERING:
            printf("LED → YELLOW  (Watering in progress)\n");
            break;
        case LED_MOISTURE_ALERT:
            printf("LED → RED BLINK (ALERT: Soil too dry!)\n");
            break;
        case LED_ERROR:
            printf("LED → RED SOLID (System ERROR)\n");
            break;
        default:
            break;
    }
}

/* -------------------------------------------------------------------------- */
void HAL_DelayMs(uint32_t ms)
{
    usleep(ms * 1000);
}

/* -------------------------------------------------------------------------- */
uint32_t HAL_GetTick(void)
{
    return ((uint32_t)(time(NULL) * 1000ULL)) - systemStartMs;
}