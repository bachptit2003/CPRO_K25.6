/* spws_controller.c
 * Core logic of the Smart Plant Watering System
 * Implements state machine, sensor handling, and user interaction
 */

#include "spws_controller.h"
#include "hal_sensors.h"
#include "hal_actuators.h"
#include "hal_buttons.h"
#include <stdio.h>

/* Private variables (module scope) */
static SensorData_t     sensorData;
static SystemSettings_t settings;
static SystemState_t    state;

/* Private function prototypes */
static void UpdateSensors(void);
static void RunAutoMode(void);
static void RunManualMode(void);
static void CheckButtons(void);
static void UpdateLed(void);
static void SendStatus(const char *msg);

/* -------------------------------------------------------------------------- */
/* Public Functions                                                           */
/* -------------------------------------------------------------------------- */

void SPWS_Init(void)
{
    /* Initialize default settings */
    settings.minMoistureThreshold       = DEFAULT_MIN_MOISTURE;
    settings.maxMoistureThreshold       = DEFAULT_MAX_MOISTURE;
    settings.maxWateringDuration_s      = DEFAULT_MAX_WATERING_TIME;
    settings.manualWateringDuration_s   = DEFAULT_MANUAL_WATERING_TIME;
    settings.sensorReadInterval_s       = DEFAULT_SENSOR_INTERVAL;

    /* Initialize system state */
    state.currentMode          = MODE_AUTO;
    state.pumpState            = PUMP_OFF;
    state.ledState             = LED_NORMAL;
    state.wateringTimeCounter  = 0;
    state.sensorCheckCounter   = 0;

    /* Hardware abstraction layer initialization */
    HAL_Init();

    SendStatus("System initialized - AUTO mode");
}

void SPWS_Run(void)
{
    static uint32_t lastTickMs = 0;
    uint32_t currentTickMs = HAL_GetTick();

    /* 1-second periodic tasks */
    if (currentTickMs - lastTickMs >= 1000u) {
        lastTickMs = currentTickMs;

        CheckButtons();
        state.sensorCheckCounter++;

        /* Periodic sensor reading */
        if (state.sensorCheckCounter >= settings.sensorReadInterval_s) {
            state.sensorCheckCounter = 0;
            UpdateSensors();
        }

        /* Execute mode-specific logic */
        if (state.currentMode == MODE_AUTO) {
            RunAutoMode();
        } else {
            RunManualMode();
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Private Functions                                                          */
/* -------------------------------------------------------------------------- */

static void UpdateSensors(void)
{
    sensorData.moisturePercent       = HAL_ReadSoilMoisture();
    sensorData.airTemperatureCelsius = HAL_ReadAirTemperature();

    char buffer[100];
    snprintf(buffer, sizeof(buffer),
             "Sensors - Moisture: %.1f%% | Temp: %.1f°C",
             sensorData.moisturePercent, sensorData.airTemperatureCelsius);
    SendStatus(buffer);
}

static void RunAutoMode(void)
{
    /* Start watering if soil is too dry */
    if (sensorData.moisturePercent < settings.minMoistureThreshold && state.pumpState == PUMP_OFF) {
        HAL_TurnPumpOn();
        state.pumpState = PUMP_ON;
        state.ledState  = LED_WATERING;
        state.wateringTimeCounter = 0;
        SendStatus("AUTO: Starting pump - soil too dry");
    }

    /* Stop conditions when pump is running */
    if (state.pumpState == PUMP_ON) {
        state.wateringTimeCounter++;

        if (sensorData.moisturePercent > settings.maxMoistureThreshold) {
            HAL_TurnPumpOff();
            state.pumpState = PUMP_OFF;
            state.ledState  = LED_NORMAL;
            SendStatus("AUTO: Target moisture reached - pump OFF");
        }
        else if (state.wateringTimeCounter >= settings.maxWateringDuration_s) {
            HAL_TurnPumpOff();
            state.pumpState = PUMP_OFF;
            state.ledState  = LED_MOISTURE_ALERT;
            SendStatus("AUTO: Max watering time exceeded - pump OFF");
        }
    }

    /* Update alert LED when not watering */
    if (state.pumpState == PUMP_OFF) {
        state.ledState = (sensorData.moisturePercent < settings.minMoistureThreshold) ?
                         LED_MOISTURE_ALERT : LED_NORMAL;
    }

    UpdateLed();
}

static void RunManualMode(void)
{
    /* Manual watering trigger */
    if (HAL_IsManualButtonPressed() && state.pumpState == PUMP_OFF) {
        HAL_TurnPumpOn();
        state.pumpState = PUMP_ON;
        state.ledState  = LED_WATERING;
        state.wateringTimeCounter = 0;
        SendStatus("MANUAL: Pump activated by user");
    }

    /* Auto-stop after duration */
    if (state.pumpState == PUMP_ON) {
        state.wateringTimeCounter++;
        if (state.wateringTimeCounter >= settings.manualWateringDuration_s) {
            HAL_TurnPumpOff();
            state.pumpState = PUMP_OFF;
            state.ledState  = LED_NORMAL;
            SendStatus("MANUAL: Watering duration complete - pump OFF");
        }
    }

    UpdateLed();
}

static void CheckButtons(void)
{
    if (HAL_IsModeButtonPressed()) {
        if (state.currentMode == MODE_AUTO) {
            state.currentMode = MODE_MANUAL;
            SendStatus("Mode changed: MANUAL");
        } else {
            state.currentMode = MODE_AUTO;
            SendStatus("Mode changed: AUTO");
        }

        /* Force pump off when entering manual mode */
        if (state.currentMode == MODE_MANUAL && state.pumpState == PUMP_ON) {
            HAL_TurnPumpOff();
            state.pumpState = PUMP_OFF;
            state.ledState  = LED_NORMAL;
            SendStatus("MANUAL mode: Pump forcibly turned OFF");
        }

        HAL_DelayMs(300);  /* Simple debounce */
    }
}

static void UpdateLed(void)
{
    HAL_SetLedState(state.ledState);
}

static void SendStatus(const char *msg)
{
    printf("[SPWS] %s\n", msg);
}