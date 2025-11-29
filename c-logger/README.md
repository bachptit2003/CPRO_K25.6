# Professional Logger Module for C

A complete, production-ready logging system for C applications with multi-target output, level filtering, and automatic metadata generation.

## Features

✅ **8 Log Levels** - Following syslog RFC 5424 standard  
✅ **Multi-target Output** - Console (stdout/stderr) and file simultaneously  
✅ **Level Filtering** - Runtime-configurable minimum log level  
✅ **Automatic Metadata** - Timestamp, filename, and line number in every log  
✅ **Safe & Clean** - Uses `vsnprintf`, proper buffer management, no memory leaks  
✅ **Easy to Use** - Convenient macros for all log levels  
✅ **Modular Design** - Clean separation, reusable in any C project

## Project Structure

```
c-logger/
├── src/
│   ├── logger.c          # Logger implementation
│   └── main.c            # Demo application
├── include/
│   └── logger.h          # Public API header
├── logs/                 # Log files directory
├── build/                # Build artifacts
├── Makefile              # Build configuration
├── .gitignore           # Git ignore rules
└── README.md            # This file
```

## Log Levels

| Level | Value | Usage |
|-------|-------|-------|
| `LOG_EMERGENCY` | 0 | System is unusable |
| `LOG_ALERT` | 1 | Action must be taken immediately |
| `LOG_CRITICAL` | 2 | Critical conditions |
| `LOG_ERROR` | 3 | Error conditions |
| `LOG_WARNING` | 4 | Warning conditions |
| `LOG_NOTICE` | 5 | Normal but significant |
| `LOG_INFO` | 6 | Informational messages |
| `LOG_DEBUG` | 7 | Debug-level messages |

## Installation & Build

### Prerequisites
- GCC compiler (or any C11-compatible compiler)
- Make utility
- Unix-like system (Linux, macOS) or Windows with MinGW

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/c-logger.git
cd c-logger

# Build the project
make

# Run the demo
make run

# Clean build files
make clean

# Clean everything including logs
make distclean
```

## Usage

### 1. Initialize the Logger

```c
#include "logger.h"

int main(void) {
    // Initialize with file output and INFO level
    if (!logger_init("logs/app.log", LOG_INFO)) {
        fprintf(stderr, "Failed to initialize logger\n");
        return 1;
    }
    
    // Your application code here
    
    // Cleanup before exit
    logger_cleanup();
    return 0;
}
```

### 2. Log Messages

```c
// Using convenience macros (recommended)
log_info("Application started");
log_error("Connection failed: %s", error_message);
log_debug("Processing item %d of %d", current, total);

// Using generic macro with level
log_message(LOG_WARNING, "Memory usage: %d%%", usage);

// All level-specific macros available
log_emergency("System emergency!");
log_alert("Security alert!");
log_critical("Critical failure!");
log_error("Error occurred");
log_warning("Warning message");
log_notice("Notable event");
log_info("Information");
log_debug("Debug details");
```

### 3. Runtime Configuration

```c
// Change minimum log level at runtime
logger_set_level(LOG_DEBUG);  // Show all messages
logger_set_level(LOG_ERROR);  // Show only errors and above

// Get current level
LogLevel current = logger_get_level();

// Flush buffers immediately
logger_flush();
```

## Output Format

Each log entry follows this format:
```
[YYYY-MM-DD HH:MM:SS] [LEVEL] [FILENAME:LINE] - Message
```

Example:
```
[2024-07-20 11:45:10] [ERROR] [main.c:25] - Failed to connect to database
[2024-07-20 11:45:11] [INFO] [app.c:102] - Processing 150 records
[2024-07-20 11:45:12] [DEBUG] [parser.c:45] - Token parsed: "identifier"
```

## Key Implementation Details

### Thread Safety
⚠️ This implementation is **not thread-safe**. For multi-threaded applications, add mutex locks around logger operations.

### Performance
- Buffered I/O for file operations
- Efficient string formatting with `vsnprintf`
- Level filtering before formatting (no overhead for filtered messages)

### Error Handling
- Graceful degradation if file can't be opened (falls back to console only)
- Safe buffer management prevents overflows
- Validates input parameters

## API Reference

### Initialization Functions
```c
bool logger_init(const char *log_file, LogLevel min_level);
void logger_cleanup(void);
```

### Configuration Functions
```c
void logger_set_level(LogLevel level);
LogLevel logger_get_level(void);
void logger_flush(void);
```

### Logging Functions
```c
void logger_log(LogLevel level, const char *file, int line, 
                const char *format, ...);
```

### Convenience Macros
```c
log_message(level, format, ...)
log_emergency(format, ...)
log_alert(format, ...)
log_critical(format, ...)
log_error(format, ...)
log_warning(format, ...)
log_notice(format, ...)
log_info(format, ...)
log_debug(format, ...)
```

## Integration into Your Project

### Option 1: Copy Files
1. Copy `src/logger.c` and `include/logger.h` to your project
2. Add to your build system
3. Include `logger.h` where needed

### Option 2: Static Library
```bash
# Compile as library
gcc -c src/logger.c -o logger.o -Iinclude
ar rcs liblogger.a logger.o

# Link in your project
gcc your_app.c -L. -llogger -Iinclude -o your_app
```

## Examples

See `src/main.c` for comprehensive usage examples including:
- All log level demonstrations
- Level filtering
- Formatted messages with variables
- Real-world simulation scenarios

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

This project is released under the MIT License. See LICENSE file for details.

## Author

Your Name - [your.email@example.com](mailto:your.email@example.com)

## Acknowledgments

- Log levels based on RFC 5424 (syslog protocol)
- Inspired by professional logging libraries in C

---

**Learning Objectives Achieved:**
- ✅ Variable argument lists (`stdarg.h`)
- ✅ Safe string formatting (`snprintf`, `vsnprintf`)
- ✅ File I/O operations
- ✅ Static variables for module encapsulation
- ✅ Preprocessor macros (`__FILE__`, `__LINE__`)
- ✅ Modular C programming
