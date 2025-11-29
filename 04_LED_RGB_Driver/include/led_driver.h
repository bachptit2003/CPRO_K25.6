/**
 * @file led_driver.h
 * @brief Software driver for addressable RGB LED strips (WS2812B/NeoPixel)
 * @author Bach Pham
 * @date 2024
 * 
 * This driver manages a buffer of 32-bit color values for controlling
 * RGB LED strips. Each pixel uses 24 bits in G-R-B format:
 * - Bits 23-16: Green (0-255)
 * - Bits 15-8:  Red (0-255)
 * - Bits 7-0:   Blue (0-255)
 * - Bits 31-24: Unused (always 0x00)
 */

#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initialize the LED strip buffer
 * 
 * Allocates memory for the specified number of pixels and initializes
 * all pixels to black (0x00000000).
 * 
 * @param num_pixels Number of LEDs in the strip
 * @return 0 on success, -1 on failure (e.g., memory allocation failed)
 * 
 * @note Must call led_shutdown() to free allocated memory
 */
int led_init(size_t num_pixels);

/**
 * @brief Shutdown the LED driver and free resources
 * 
 * Frees the memory allocated for the LED strip buffer.
 * After calling this function, led_init() must be called again
 * before using any other functions.
 */
void led_shutdown(void);

/**
 * @brief Set the color of a specific pixel
 * 
 * Sets the color of the pixel at the given index using separate
 * 8-bit values for Red, Green, and Blue components.
 * 
 * @param index Pixel index (0 to num_pixels-1)
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * 
 * @note If index is out of bounds, the function silently ignores the request
 */
void led_set_pixel_color(size_t index, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Fill the entire strip with a single color
 * 
 * Sets all pixels in the strip to the same color.
 * 
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 */
void led_fill(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Clear the entire strip (turn off all pixels)
 * 
 * Sets all pixels to black (0, 0, 0).
 */
void led_clear(void);

/**
 * @brief Get a read-only pointer to the LED buffer
 * 
 * Returns a constant pointer to the internal buffer containing
 * the color data for all pixels. This is useful for testing
 * or sending data to actual hardware.
 * 
 * @return Pointer to the color buffer, or NULL if not initialized
 * 
 * @note The returned buffer should not be modified directly
 */
const uint32_t* led_get_buffer(void);

/**
 * @brief Get the number of pixels in the strip
 * 
 * @return Number of pixels, or 0 if not initialized
 */
size_t led_get_pixel_count(void);

/**
 * @brief Get the color value of a specific pixel
 * 
 * @param index Pixel index (0 to num_pixels-1)
 * @return 32-bit color value in G-R-B format, or 0 if index is invalid
 */
uint32_t led_get_pixel(size_t index);

/**
 * @brief Print the entire LED buffer in hexadecimal format
 * 
 * Utility function for debugging and visualization.
 */
void led_print_buffer(void);

/* Color Constants - Common colors in 32-bit format (0x00GGRRBB) */
#define LED_COLOR_BLACK     0x00000000
#define LED_COLOR_WHITE     0x00FFFFFF
#define LED_COLOR_RED       0x0000FF00
#define LED_COLOR_GREEN     0x00FF0000
#define LED_COLOR_BLUE      0x000000FF
#define LED_COLOR_YELLOW    0x00FFFF00
#define LED_COLOR_CYAN      0x00FF00FF
#define LED_COLOR_MAGENTA   0x0000FFFF
#define LED_COLOR_ORANGE    0x00FF8000
#define LED_COLOR_PURPLE    0x008000FF

#endif // LED_DRIVER_H
