/* hal_sensors.c
 * Simulated sensor readings for development and testing
 * Replace with real ADC driver when deploying on microcontroller
 */

#include "hal_sensors.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Simulated dynamic soil moisture (starts dry, slowly increases when watered) */
static float simulatedMoisture = 22.0f;   /* Start very dry */

/* -------------------------------------------------------------------------- */
float HAL_ReadSoilMoisture(void)
{
    /* Simulate gradual moisture change */
    if (simulatedMoisture < 25.0f) {
        simulatedMoisture += (rand() % 3) * 0.1f;     /* Slowly recover */
    } else if (simulatedMoisture > 80.0f) {
        simulatedMoisture -= (rand() % 5) * 0.2f;     /* Slowly dry out */
    } else {
        simulatedMoisture += ((rand() % 20) - 10) * 0.05f; /* Natural fluctuation */
    }

    if (simulatedMoisture < 0.0f)   simulatedMoisture = 0.0f;
    if (simulatedMoisture > 100.0f) simulatedMoisture = 100.0f;

    return simulatedMoisture;
}

/* -------------------------------------------------------------------------- */
float HAL_ReadAirTemperature(void)
{
    /* Realistic temperature with small random variation */
    static float baseTemp = 26.0f;
    baseTemp += ((rand() % 20) - 10) * 0.01f;
    if (baseTemp < 15.0f) baseTemp = 15.0f;
    if (baseTemp > 40.0f) baseTemp = 40.0f;
    return baseTemp;
}