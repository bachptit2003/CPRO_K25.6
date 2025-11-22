/* hal_actuators.h
 * Hardware Abstraction Layer - Actuators (Pump & LED)
 */

#ifndef HAL_ACTUATORS_H
#define HAL_ACTUATORS_H

#include "config.h"
#include <stdint.h>

/* ------------------------------------------------------------------------ */
/* Public Functions                                                         */
/* ------------------------------------------------------------------------ */

void HAL_Init(void);
void HAL_TurnPumpOn(void);
void HAL_TurnPumpOff(void);
void HAL_SetLedState(LedState_t state);
void HAL_DelayMs(uint32_t ms);
uint32_t HAL_GetTick(void);   /* Milliseconds since system start */

#endif /* HAL_ACTUATORS_H */