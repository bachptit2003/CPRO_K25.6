/**
 * @file main.c
 * @brief Test program for LED RGB Driver
 */

#include "led_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Test result structure
 */
typedef struct {
    int total;
    int passed;
    int failed;
} TestResult;

static TestResult test_result = {0, 0, 0};

/**
 * @brief Print test header
 */
void print_test_header(const char *test_name) {
    printf("\n");
    printf("========================================\n");
    printf("TEST: %s\n", test_name);
    printf("========================================\n");
}

/**
 * @brief Assert function for testing
 */
bool assert_equal_uint32(const char *name, uint32_t expected, uint32_t actual) {
    test_result.total++;
    
    if (expected == actual) {
        printf("  ✓ %s: PASS (0x%08X)\n", name, actual);
        test_result.passed++;
        return true;
    } else {
        printf("  ✗ %s: FAIL (expected: 0x%08X, got: 0x%08X)\n", 
               name, expected, actual);
        test_result.failed++;
        return false;
    }
}

/**
 * @brief Assert that all pixels have the same value
 */
bool assert_all_pixels_equal(const char *name, uint32_t expected, size_t count) {
    const uint32_t *buffer = led_get_buffer();
    if (buffer == NULL) {
        printf("  ✗ %s: FAIL (buffer is NULL)\n", name);
        test_result.total++;
        test_result.failed++;
        return false;
    }
    
    bool all_match = true;
    for (size_t i = 0; i < count; i++) {
        if (buffer[i] != expected) {
            all_match = false;
            break;
        }
    }
    
    test_result.total++;
    if (all_match) {
        printf("  ✓ %s: PASS (all %zu pixels = 0x%08X)\n", 
               name, count, expected);
        test_result.passed++;
        return true;
    } else {
        printf("  ✗ %s: FAIL (not all pixels match 0x%08X)\n", 
               name, expected);
        test_result.failed++;
        return false;
    }
}

/**
 * @brief Test 1: Initialization and basic operations
 */
void test_initialization(void) {
    print_test_header("Initialization & Basic Operations");
    
    // Test initialization with 10 pixels
    int result = led_init(10);
    if (result == 0) {
        printf("  ✓ Initialization: PASS\n");
        test_result.passed++;
    } else {
        printf("  ✗ Initialization: FAIL\n");
        test_result.failed++;
    }
    test_result.total++;
    
    // Check buffer is initialized to 0 (black)
    assert_all_pixels_equal("Buffer initialized to black", 0x00000000, 10);
    
    // Check pixel count
    size_t count = led_get_pixel_count();
    assert_equal_uint32("Pixel count", 10, (uint32_t)count);
}

/**
 * @brief Test 2: Color setting and bit manipulation
 */
void test_color_setting(void) {
    print_test_header("Color Setting & Bit Manipulation");
    
    // Set pixel 0 to RED (255, 0, 0)
    // Expected: 0x0000FF00 (G=0, R=255, B=0)
    led_set_pixel_color(0, 255, 0, 0);
    uint32_t pixel0 = led_get_pixel(0);
    assert_equal_uint32("Pixel[0] = RED", 0x0000FF00, pixel0);
    
    // Set pixel 9 to BLUE (0, 0, 255)
    // Expected: 0x000000FF (G=0, R=0, B=255)
    led_set_pixel_color(9, 0, 0, 255);
    uint32_t pixel9 = led_get_pixel(9);
    assert_equal_uint32("Pixel[9] = BLUE", 0x000000FF, pixel9);
    
    // Set pixel 4 to WHITE (255, 255, 255)
    // Expected: 0x00FFFFFF (G=255, R=255, B=255)
    led_set_pixel_color(4, 255, 255, 255);
    uint32_t pixel4 = led_get_pixel(4);
    assert_equal_uint32("Pixel[4] = WHITE", 0x00FFFFFF, pixel4);
    
    // Test GREEN (0, 255, 0)
    // Expected: 0x00FF0000 (G=255, R=0, B=0)
    led_set_pixel_color(5, 0, 255, 0);
    uint32_t pixel5 = led_get_pixel(5);
    assert_equal_uint32("Pixel[5] = GREEN", 0x00FF0000, pixel5);
    
    // Print buffer for visual verification
    led_print_buffer();
}

/**
 * @brief Test 3: Fill operation
 */
void test_fill_operation(void) {
    print_test_header("Fill Operation");
    
    // Fill entire strip with GREEN (0, 255, 0)
    // Expected: all pixels = 0x00FF0000
    led_fill(0, 255, 0);
    assert_all_pixels_equal("Fill with GREEN", 0x00FF0000, 10);
    
    printf("\nBuffer after fill:\n");
    led_print_buffer();
}

/**
 * @brief Test 4: Clear operation
 */
void test_clear_operation(void) {
    print_test_header("Clear Operation");
    
    // Set some colors first
    led_set_pixel_color(0, 255, 0, 0);
    led_set_pixel_color(5, 0, 255, 0);
    
    // Clear entire strip
    led_clear();
    assert_all_pixels_equal("Clear (all black)", 0x00000000, 10);
}

/**
 * @brief Test 5: Bounds checking
 */
void test_bounds_checking(void) {
    print_test_header("Bounds Checking");
    
    // Set pixel 0 to RED first
    led_set_pixel_color(0, 255, 0, 0);
    
    // Try to set out-of-bounds pixel (should be ignored)
    led_set_pixel_color(100, 0, 255, 0);  // Out of bounds
    
    // Check that pixel 0 is still RED (unchanged)
    uint32_t pixel0 = led_get_pixel(0);
    assert_equal_uint32("Out-of-bounds ignored", 0x0000FF00, pixel0);
    
    printf("  Note: Out-of-bounds access was silently ignored (correct)\n");
}

/**
 * @brief Test 6: Color constants
 */
void test_color_constants(void) {
    print_test_header("Color Constants");
    
    printf("  Color Constants:\n");
    printf("    BLACK:   0x%08X\n", LED_COLOR_BLACK);
    printf("    WHITE:   0x%08X\n", LED_COLOR_WHITE);
    printf("    RED:     0x%08X\n", LED_COLOR_RED);
    printf("    GREEN:   0x%08X\n", LED_COLOR_GREEN);
    printf("    BLUE:    0x%08X\n", LED_COLOR_BLUE);
    printf("    YELLOW:  0x%08X\n", LED_COLOR_YELLOW);
    printf("    CYAN:    0x%08X\n", LED_COLOR_CYAN);
    printf("    MAGENTA: 0x%08X\n", LED_COLOR_MAGENTA);
    printf("    ORANGE:  0x%08X\n", LED_COLOR_ORANGE);
    printf("    PURPLE:  0x%08X\n", LED_COLOR_PURPLE);
}

/**
 * @brief Test 7: Rainbow pattern demo
 */
void test_rainbow_pattern(void) {
    print_test_header("Rainbow Pattern Demo");
    
    // Create a simple rainbow pattern
    led_set_pixel_color(0, 255, 0, 0);     // Red
    led_set_pixel_color(1, 255, 127, 0);   // Orange
    led_set_pixel_color(2, 255, 255, 0);   // Yellow
    led_set_pixel_color(3, 0, 255, 0);     // Green
    led_set_pixel_color(4, 0, 255, 127);   // Cyan-ish
    led_set_pixel_color(5, 0, 255, 255);   // Cyan
    led_set_pixel_color(6, 0, 127, 255);   // Sky blue
    led_set_pixel_color(7, 0, 0, 255);     // Blue
    led_set_pixel_color(8, 127, 0, 255);   // Purple
    led_set_pixel_color(9, 255, 0, 255);   // Magenta
    
    printf("\nRainbow pattern created:\n");
    led_print_buffer();
}

/**
 * @brief Print test summary
 */
void print_test_summary(void) {
    printf("\n");
    printf("========================================\n");
    printf("TEST SUMMARY\n");
    printf("========================================\n");
    printf("Total tests:  %d\n", test_result.total);
    printf("Passed:       %d\n", test_result.passed);
    printf("Failed:       %d\n", test_result.failed);
    printf("Success rate: %.1f%%\n", 
           test_result.total > 0 ? 
           (100.0 * test_result.passed / test_result.total) : 0.0);
    printf("========================================\n");
    
    if (test_result.failed == 0) {
        printf("✓ ALL TESTS PASSED!\n");
    } else {
        printf("✗ Some tests failed\n");
    }
    printf("\n");
}

int main(void) {
    printf("========================================\n");
    printf("  LED RGB Driver Test Suite\n");
    printf("  WS2812B/NeoPixel Compatible\n");
    printf("========================================\n");
    
    // Run all tests
    test_initialization();
    test_color_setting();
    test_fill_operation();
    test_clear_operation();
    test_bounds_checking();
    test_color_constants();
    test_rainbow_pattern();
    
    // Print summary
    print_test_summary();
    
    // Cleanup
    led_shutdown();
    
    return (test_result.failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
