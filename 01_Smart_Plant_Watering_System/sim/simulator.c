/* sim/simulator.c
 * Standalone PC simulator - Single file to run the entire SPWS system
 * No need to compile the whole project - just: gcc sim/simulator.c -o spws && ./spws
 * Perfect for quick demo, presentation, or sharing with friends/teachers
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>

/* ==================== CONFIG & TYPES ==================== */
typedef enum { MODE_AUTO, MODE_MANUAL } SystemMode_t;
typedef enum { PUMP_OFF, PUMP_ON } PumpState_t;
typedef enum {
    LED_NORMAL, LED_WATERING, LED_MOISTURE_ALERT, LED_ERROR
} LedState_t;

typedef struct {
    float moisturePercent;
    float airTemperatureCelsius;
} SensorData_t;

typedef struct {
    float    minMoistureThreshold;
    float    maxMoistureThreshold;
    uint32_t maxWateringDuration_s;
    uint32_t manualWateringDuration_s;
    uint32_t sensorReadInterval_s;
} SystemSettings_t;

typedef struct {
    SystemMode_t currentMode;
    PumpState_t  pumpState;
    LedState_t   ledState;
    uint32_t     wateringTimeCounter;
    uint32_t     sensorCheckCounter;
} SystemState_t;

/* Default values */
#define DEFAULT_MIN_MOISTURE           30.0f
#define DEFAULT_MAX_MOISTURE           70.0f
#define DEFAULT_MAX_WATERING_TIME      30u
#define DEFAULT_MANUAL_WATERING_TIME   10u
#define DEFAULT_SENSOR_INTERVAL         5u

/* ==================== GLOBAL VARIABLES ==================== */
static SensorData_t     sensor = { .moisturePercent = 24.0f };
static SystemSettings_t cfg;
static SystemState_t    state;
static uint32_t         startMs = 0;

/* ==================== SIMULATED HAL ==================== */
static uint32_t HAL_GetTick(void) {
    return (uint32_t)(time(NULL) * 1000ULL) - startMs;
}

static void HAL_DelayMs(uint32_t ms) { usleep(ms * 1000); }

static void HAL_TurnPumpOn(void)  { printf("*** PUMP ON  (Watering started) ***\n"); }
static void HAL_TurnPumpOff(void) { printf("*** PUMP OFF (Watering stopped) ***\n"); }

static void HAL_SetLedState(LedState_t st) {
    switch(st) {
        case LED_NORMAL:         printf("LED → GREEN   (Normal)\n"); break;
        case LED_WATERING:       printf("LED → YELLOW  (Watering)\n"); break;
        case LED_MOISTURE_ALERT: printf("LED → RED BLINK (Too dry!)\n"); break;
        case LED_ERROR:          printf("LED → RED SOLID (Error)\n"); break;
    }
}

static float HAL_ReadSoilMoisture(void) {
    /* Simulate moisture slowly increasing when pump is ON */
    if (state.pumpState == PUMP_ON) {
        sensor.moisturePercent += 1.8f + (rand() % 10) * 0.1f;
    } else {
        sensor.moisturePercent -= 0.05f + (rand() % 10) * 0.01f;
    }
    if (sensor.moisturePercent > 100.0f) sensor.moisturePercent = 100.0f;
    if (sensor.moisturePercent < 0.0f)   sensor.moisturePercent = 0.0f;
    return sensor.moisturePercent;
}

static float HAL_ReadAirTemperature(void) {
    return 25.0f + (rand() % 80) * 0.1f;
}

static int kbhit(void) {
    struct timeval tv = {0, 0};
    fd_set rdfs;
    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);
    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}

static int getch(void) {
    int ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/* ==================== MAIN LOGIC (same as spws_controller.c) ==================== */
static void System_Init(void) {
    startMs = (uint32_t)(time(NULL) * 1000ULL);
    cfg = (SystemSettings_t){
        .minMoistureThreshold = DEFAULT_MIN_MOISTURE,
        .maxMoistureThreshold = DEFAULT_MAX_MOISTURE,
        .maxWateringDuration_s = DEFAULT_MAX_WATERING_TIME,
        .manualWateringDuration_s = DEFAULT_MANUAL_WATERING_TIME,
        .sensorReadInterval_s = DEFAULT_SENSOR_INTERVAL
    };
    state = (SystemState_t){
        .currentMode = MODE_AUTO,
        .pumpState = PUMP_OFF,
        .ledState = LED_NORMAL
    };
    printf("Smart Plant Watering System - Standalone Simulator\n");
    printf("Press 'm' = toggle mode | 't' = manual water | 'q' = quit\n\n");
}

static void PrintStatus(const char* msg) {
    printf("[SPWS] %s\n", msg);
}

int main(void) {
    System_Init();
    uint32_t lastTick = 0;

    while (1) {
        uint32_t now = HAL_GetTick();

        /* Handle keyboard */
        if (kbhit()) {
            int c = getch();
            if (c == 'm' || c == 'M') {
                state.currentMode = (state.currentMode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
                printf(">>> Mode → %s <<<\n", state.currentMode == MODE_AUTO ? "AUTO" : "MANUAL");
                if (state.currentMode == MODE_MANUAL && state.pumpState == PUMP_ON) {
                    HAL_TurnPumpOff();
                    state.pumpState = PUMP_OFF;
                }
            }
            else if ((c == 't' || c == 'T') && state.currentMode == MODE_MANUAL) {
                if (state.pumpState == PUMP_OFF) {
                    HAL_TurnPumpOn();
                    state.pumpState = PUMP_ON;
                    state.ledState = LED_WATERING;
                    state.wateringTimeCounter = 0;
                    PrintStatus("MANUAL: Pump started by user");
                }
            }
            else if (c == 'q' || c == 'Q') {
                printf("\nGoodbye!\n");
                return 0;
            }
        }

        /* 1-second tick */
        if (now - lastTick >= 1000) {
            lastTick = now;

            /* Read sensors every N seconds */
            state.sensorCheckCounter++;
            if (state.sensorCheckCounter >= cfg.sensorReadInterval_s) {
                state.sensorCheckCounter = 0;
                sensor.moisturePercent = HAL_ReadSoilMoisture();
                char buf[100];
                snprintf(buf, sizeof(buf), "Sensors → Moisture: %.1f%% | Temp: %.1f°C",
                         sensor.moisturePercent, HAL_ReadAirTemperature());
                PrintStatus(buf);
            }

            /* AUTO mode logic */
            if (state.currentMode == MODE_AUTO) {
                if (sensor.moisturePercent < cfg.minMoistureThreshold && state.pumpState == PUMP_OFF) {
                    HAL_TurnPumpOn();
                    state.pumpState = PUMP_ON;
                    state.ledState = LED_WATERING;
                    state.wateringTimeCounter = 0;
                    PrintStatus("AUTO: Soil too dry → Pump ON");
                }
                if (state.pumpState == PUMP_ON) {
                    state.wateringTimeCounter++;
                    if (sensor.moisturePercent > cfg.maxMoistureThreshold ||
                        state.wateringTimeCounter >= cfg.maxWateringDuration_s) {
                        HAL_TurnPumpOff();
                        state.pumpState = PUMP_OFF;
                        state.ledState = (sensor.moisturePercent < cfg.minMoistureThreshold) ?
                                         LED_MOISTURE_ALERT : LED_NORMAL;
                        PrintStatus("AUTO: Target reached or timeout → Pump OFF");
                    }
                }
            }

            /* MANUAL mode timer */
            if (state.currentMode == MODE_MANUAL && state.pumpState == PUMP_ON) {
                state.wateringTimeCounter++;
                if (state.wateringTimeCounter >= cfg.manualWateringDuration_s) {
                    HAL_TurnPumpOff();
                    state.pumpState = PUMP_OFF;
                    state.ledState = LED_NORMAL;
                    PrintStatus("MANUAL: Duration finished → Pump OFF");
                }
            }

            HAL_SetLedState(state.ledState);
        }

        HAL_DelayMs(10);
    }
    return 0;
}