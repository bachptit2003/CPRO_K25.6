# LED RGB Driver (WS2812B/NeoPixel)

Software driver for controlling addressable RGB LED strips like WS2812B (NeoPixel). This high-level driver manages a color buffer and provides convenient functions for LED control.

## Features

✅ **Dynamic Memory Management** - Efficiently allocates buffer for any strip size  
✅ **Bit Manipulation** - Correct G-R-B color packing for WS2812B protocol  
✅ **Bounds Checking** - Safe operations with automatic boundary validation  
✅ **Utility Functions** - Fill, clear, and individual pixel control  
✅ **Color Constants** - Pre-defined colors for convenience  
✅ **Comprehensive Testing** - Full test suite with visual verification  
✅ **Memory Safe** - No memory leaks, validated with Valgrind

## Hardware Background

### WS2812B LED Protocol

WS2812B (NeoPixel) LEDs use a single-wire serial protocol where:
- Each LED chip receives 24 bits of color data (G-R-B format)
- The LED takes its color data and passes remaining data to the next LED
- Data format: 8 bits Green, 8 bits Red, 8 bits Blue

### Color Data Format

Each pixel uses a 32-bit value with this layout:

```
Bits:     31-24      23-16       15-8        7-0
         [unused]    [Green]     [Red]      [Blue]
Value:    0x00    |   0-255   |  0-255   |  0-255
Hex:      0x00    |    GG     |    RR    |    BB
```

**Example Colors:**
- Red:   `0x0000FF00` (G=0,   R=255, B=0)
- Green: `0x00FF0000` (G=255, R=0,   B=0)
- Blue:  `0x000000FF` (G=0,   R=0,   B=255)
- White: `0x00FFFFFF` (G=255, R=255, B=255)

## Project Structure

```
04_LED_RGB_Driver/
├── src/
│   ├── led_driver.c      # Driver implementation
│   └── main.c            # Test suite
├── include/
│   └── led_driver.h      # Public API
├── build/                # Build artifacts
├── Makefile              # Build configuration
├── .gitignore           # Git ignore rules
└── README.md            # This file
```

## API Reference

### Initialization & Cleanup

```c
int led_init(size_t num_pixels);
```
Initialize LED strip buffer. Returns 0 on success, -1 on failure.

```c
void led_shutdown(void);
```
Free allocated memory and cleanup resources.

### Color Control

```c
void led_set_pixel_color(size_t index, uint8_t r, uint8_t g, uint8_t b);
```
Set color of a specific pixel. Silently ignores out-of-bounds indices.

```c
void led_fill(uint8_t r, uint8_t g, uint8_t b);
```
Fill entire strip with one color.

```c
void led_clear(void);
```
Turn off all LEDs (set to black).

### Data Access

```c
const uint32_t* led_get_buffer(void);
```
Get read-only pointer to color buffer (for sending to hardware).

```c
size_t led_get_pixel_count(void);
```
Get number of pixels in the strip.

```c
uint32_t led_get_pixel(size_t index);
```
Get 32-bit color value of a specific pixel.

### Utility

```c
void led_print_buffer(void);
```
Print entire buffer in hex format with RGB breakdown (debugging).

## Color Constants

Pre-defined color values for convenience:

```c
LED_COLOR_BLACK      // 0x00000000
LED_COLOR_WHITE      // 0x00FFFFFF
LED_COLOR_RED        // 0x0000FF00
LED_COLOR_GREEN      // 0x00FF0000
LED_COLOR_BLUE       // 0x000000FF
LED_COLOR_YELLOW     // 0x00FFFF00
LED_COLOR_CYAN       // 0x00FF00FF
LED_COLOR_MAGENTA    // 0x0000FFFF
LED_COLOR_ORANGE     // 0x00FF8000
LED_COLOR_PURPLE     // 0x008000FF
```

## Building & Running

### Prerequisites
- GCC compiler (C11 support)
- Make utility
- Valgrind (optional, for memory testing)

### Build Commands

```bash
# Build the project
make

# Build and run tests
make run

# Check for memory leaks
make valgrind

# Clean build files
make clean
```

### Expected Output

```
========================================
  LED RGB Driver Test Suite
  WS2812B/NeoPixel Compatible
========================================

========================================
TEST: Initialization & Basic Operations
========================================
LED driver initialized: 10 pixels
  ✓ Initialization: PASS
  ✓ Buffer initialized to black: PASS (all 10 pixels = 0x00000000)
  ✓ Pixel count: PASS (0x0000000A)

========================================
TEST: Color Setting & Bit Manipulation
========================================
  ✓ Pixel[0] = RED: PASS (0x0000FF00)
  ✓ Pixel[9] = BLUE: PASS (0x000000FF)
  ✓ Pixel[4] = WHITE: PASS (0x00FFFFFF)
  ✓ Pixel[5] = GREEN: PASS (0x00FF0000)

=== LED Buffer (10 pixels) ===
Pixel[ 0]: 0x0000FF00  (R:255, G:  0, B:  0)
Pixel[ 1]: 0x00000000  (R:  0, G:  0, B:  0)
...
```

## Usage Example

```c
#include "led_driver.h"

int main(void) {
    // Initialize strip with 60 LEDs
    if (led_init(60) != 0) {
        return -1;
    }
    
    // Set first LED to red
    led_set_pixel_color(0, 255, 0, 0);
    
    // Fill strip with blue
    led_fill(0, 0, 255);
    
    // Get buffer to send to hardware
    const uint32_t *buffer = led_get_buffer();
    // send_to_hardware(buffer, led_get_pixel_count());
    
    // Cleanup
    led_shutdown();
    return 0;
}
```

## Bit Manipulation Explained

### Packing RGB into 32-bit Value

```c
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}
```

**Step-by-step for RGB(255, 128, 64):**

1. Green shifted left 16 bits: `128 << 16` = `0x00800000`
2. Red shifted left 8 bits:    `255 << 8`  = `0x0000FF00`
3. Blue stays in place:        `64`        = `0x00000040`
4. OR them together:           `0x00`|`80`|`FF`|`40` = `0x0080FF40`

### Extracting RGB from 32-bit Value

```c
uint8_t g = (color >> 16) & 0xFF;  // Extract bits 23-16
uint8_t r = (color >> 8) & 0xFF;   // Extract bits 15-8
uint8_t b = color & 0xFF;          // Extract bits 7-0
```

## Testing

The test suite validates:
- ✅ Memory allocation and initialization
- ✅ Correct bit manipulation for G-R-B format
- ✅ Individual pixel color setting
- ✅ Fill and clear operations
- ✅ Bounds checking (out-of-range access)
- ✅ Color constants accuracy
- ✅ No memory leaks (Valgrind)

## Memory Management

- Uses `calloc()` for zero-initialized allocation
- Proper cleanup with `free()` in `led_shutdown()`
- Static state prevents multiple initializations
- Validated with Valgrind for leak detection

## Integration with Hardware

To use this driver with real WS2812B strips:

1. Get the buffer: `const uint32_t *buffer = led_get_buffer();`
2. Implement timing-critical serial transmission
3. Send each bit according to WS2812B protocol:
   - 0 bit: ~0.4µs high, ~0.85µs low
   - 1 bit: ~0.8µs high, ~0.45µs low

Hardware interfacing typically requires:
- GPIO bit-banging (software)
- SPI peripheral (hardware-assisted)
- Dedicated LED controller chip

## Learning Objectives

This project demonstrates:
- ✅ Dynamic memory allocation (`malloc`, `calloc`, `free`)
- ✅ Bit manipulation (`<<`, `>>`, `|`, `&`)
- ✅ Static variables for module state
- ✅ Proper bounds checking
- ✅ Comprehensive testing methodology
- ✅ Clean API design

## License

MIT License - Free for educational and commercial use

## Author

**Bach Nguyen**  
Student ID: K25.6  
Course: Computer Programming (CPRO)

---

⭐ Star this repository if you find it helpful!

## References

- [WS2812B Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
- [NeoPixel Guide](https://learn.adafruit.com/adafruit-neopixel-uberguide)
