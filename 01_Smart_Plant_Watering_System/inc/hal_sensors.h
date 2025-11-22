/* hal_sensors.h
 * Hardware Abstraction Layer - Sensor Interface
 * Provides unified access to soil moisture and temperature sensors
 */

#ifndef HAL_SENSORS_H
#define HAL_SENSORS_H

#include <stdint.h>

/* ------------------------------------------------------------------------ */
/* Public Functions                                                         */
/* ------------------------------------------------------------------------ */

/* Read soil moisture level (0.0 - 100.0 %) */
float HAL_ReadSoilMoisture(void);

/* Read ambient air temperature in Celsius */
float HAL_ReadAirTemperature(void);

#endif /* HAL_SENSORS_H */