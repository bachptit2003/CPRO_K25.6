/* hal_buttons.h
 * Hardware Abstraction Layer - Button Input
 */

#ifndef HAL_BUTTONS_H
#define HAL_BUTTONS_H

#include <stdint.h>

/* Returns 1 if mode toggle button is pressed (with debounce) */
uint8_t HAL_IsModeButtonPressed(void);

/* Returns 1 only once per press - for manual watering trigger */
uint8_t HAL_IsManualButtonPressed(void);

#endif /* HAL_BUTTONS_H */