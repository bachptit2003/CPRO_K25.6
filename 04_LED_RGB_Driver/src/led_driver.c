/**
 * @file led_driver.c
 * @brief Implementation of the LED RGB driver
 */

#include "led_driver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief LED driver state structure (private)
 */
static struct {
    uint32_t *buffer;      // Buffer storing color data for each pixel
    size_t num_pixels;     // Number of pixels in the strip
    bool initialized;      // Initialization status
} led_state = {
    .buffer = NULL,
    .num_pixels = 0,
    .initialized = false
};

/**
 * @brief Pack RGB components into a 32-bit color value
 * 
 * Format: 0x00GGRRBB (G-R-B order for WS2812B)
 * - Bits 23-16: Green
 * - Bits 15-8:  Red
 * - Bits 7-0:   Blue
 * 
 * @param r Red component (0-255)
 * @param g Green component (0-255)
 * @param b Blue component (0-255)
 * @return 32-bit color value
 */
static inline uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

/**
 * @brief Check if a pixel index is valid
 * 
 * @param index Pixel index to check
 * @return true if valid, false otherwise
 */
static inline bool is_valid_index(size_t index) {
    return led_state.initialized && (index < led_state.num_pixels);
}

int led_init(size_t num_pixels) {
    // Validate input
    if (num_pixels == 0) {
        fprintf(stderr, "Error: num_pixels must be greater than 0\n");
        return -1;
    }
    
    // Check if already initialized
    if (led_state.initialized) {
        fprintf(stderr, "Warning: LED driver already initialized. "
                       "Call led_shutdown() first.\n");
        return -1;
    }
    
    // Allocate memory for the buffer
    led_state.buffer = (uint32_t*)calloc(num_pixels, sizeof(uint32_t));
    if (led_state.buffer == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for %zu pixels\n", 
                num_pixels);
        return -1;
    }
    
    // Initialize state
    led_state.num_pixels = num_pixels;
    led_state.initialized = true;
    
    printf("LED driver initialized: %zu pixels\n", num_pixels);
    return 0;
}

void led_shutdown(void) {
    if (!led_state.initialized) {
        return;
    }
    
    // Free allocated memory
    if (led_state.buffer != NULL) {
        free(led_state.buffer);
        led_state.buffer = NULL;
    }
    
    // Reset state
    led_state.num_pixels = 0;
    led_state.initialized = false;
    
    printf("LED driver shut down\n");
}

void led_set_pixel_color(size_t index, uint8_t r, uint8_t g, uint8_t b) {
    // Bounds checking
    if (!is_valid_index(index)) {
        return;  // Silently ignore out-of-bounds access
    }
    
    // Pack color and store in buffer
    led_state.buffer[index] = pack_color(r, g, b);
}

void led_fill(uint8_t r, uint8_t g, uint8_t b) {
    if (!led_state.initialized) {
        return;
    }
    
    // Pack color once
    uint32_t color = pack_color(r, g, b);
    
    // Fill entire buffer with this color
    for (size_t i = 0; i < led_state.num_pixels; i++) {
        led_state.buffer[i] = color;
    }
}

void led_clear(void) {
    // Clear is just fill with black (0, 0, 0)
    led_fill(0, 0, 0);
}

const uint32_t* led_get_buffer(void) {
    if (!led_state.initialized) {
        return NULL;
    }
    return led_state.buffer;
}

size_t led_get_pixel_count(void) {
    return led_state.initialized ? led_state.num_pixels : 0;
}

uint32_t led_get_pixel(size_t index) {
    if (!is_valid_index(index)) {
        return 0;
    }
    return led_state.buffer[index];
}

void led_print_buffer(void) {
    if (!led_state.initialized) {
        printf("LED buffer not initialized\n");
        return;
    }
    
    printf("\n=== LED Buffer (%zu pixels) ===\n", led_state.num_pixels);
    for (size_t i = 0; i < led_state.num_pixels; i++) {
        uint32_t color = led_state.buffer[i];
        
        // Extract RGB components
        uint8_t g = (color >> 16) & 0xFF;
        uint8_t r = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        
        printf("Pixel[%2zu]: 0x%08X  (R:%3d, G:%3d, B:%3d)\n", 
               i, color, r, g, b);
    }
    printf("================================\n\n");
}
