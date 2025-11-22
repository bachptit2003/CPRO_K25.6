/* config.h
 * Configuration definitions and data structures for Smart Plant Watering System
 * Author: PTIT_Bach
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* System operating modes */
typedef enum {
    MODE_AUTO,      /* Automatic watering based on soil moisture */
    MODE_MANUAL     /* Manual control via button */
} SystemMode_t;

/* Pump states */
typedef enum {
    PUMP_OFF,
    PUMP_ON
} PumpState_t;

/* LED status indicators */
typedef enum {
    LED_NORMAL,           /* Green  - Normal operation, sufficient moisture */
    LED_WATERING,         /* Yellow - Pump is running */
    LED_MOISTURE_ALERT,   /* Red blinking - Soil too dry */
    LED_ERROR             /* Red solid - System error */
} LedState_t;

/* Sensor readings structure */
typedef struct {
    float moisturePercent;        /* Soil moisture level (0.0 - 100.0 %) */
    float airTemperatureCelsius;  /* Ambient air temperature in Celsius */
} SensorData_t;

/* User-configurable system settings */
typedef struct {
    float    minMoistureThreshold;        /* Start watering if below this (%) */
    float    maxMoistureThreshold;        /* Stop watering if above this (%) */
    uint32_t maxWateringDuration_s;       /* Maximum watering time per cycle */
    uint32_t manualWateringDuration_s;    /* Duration of manual watering */
    uint32_t sensorReadInterval_s;        /* How often to read sensors */
} SystemSettings_t;

/* Runtime system state */
typedef struct {
    SystemMode_t currentMode;
    PumpState_t  pumpState;
    LedState_t   ledState;
    uint32_t     wateringTimeCounter;     /* Seconds pump has been ON */
    uint32_t     sensorCheckCounter;      /* Countdown for next sensor read */
} SystemState_t;

/* Default configuration values */
#define DEFAULT_MIN_MOISTURE           30.0f
#define DEFAULT_MAX_MOISTURE           70.0f
#define DEFAULT_MAX_WATERING_TIME      30u      /* seconds */
#define DEFAULT_MANUAL_WATERING_TIME   10u      /* seconds */
#define DEFAULT_SENSOR_INTERVAL         5u      /* seconds */

#endif /* CONFIG_H */