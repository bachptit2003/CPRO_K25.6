/**
 * @file logger.c
 * @brief Implementation of the logging system
 */

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LOG_MESSAGE 4096
#define MAX_TIMESTAMP 32
#define MAX_LEVEL_NAME 16

/**
 * @brief Logger configuration structure (static - module encapsulation)
 */
static struct {
    FILE *log_file;           // File handle for log file
    LogLevel min_level;       // Minimum level to log
    bool initialized;         // Initialization status
    char log_file_path[256];  // Path to log file
} logger_config = {
    .log_file = NULL,
    .min_level = LOG_INFO,
    .initialized = false,
    .log_file_path = {0}
};

/**
 * @brief Level name mapping
 */
static const char *level_names[] = {
    "EMERGENCY",
    "ALERT",
    "CRITICAL",
    "ERROR",
    "WARNING",
    "NOTICE",
    "INFO",
    "DEBUG"
};

/**
 * @brief Get current timestamp in format: YYYY-MM-DD HH:MM:SS
 * 
 * @param buffer Buffer to store timestamp
 * @param size Size of buffer
 */
static void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    if (tm_info == NULL) {
        snprintf(buffer, size, "0000-00-00 00:00:00");
        return;
    }
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

/**
 * @brief Get level name string
 * 
 * @param level Log level
 * @return Level name as string
 */
static const char* get_level_name(LogLevel level) {
    if (level < LOG_EMERGENCY || level > LOG_DEBUG) {
        return "UNKNOWN";
    }
    return level_names[level];
}

/**
 * @brief Extract filename from full path
 * 
 * @param path Full file path
 * @return Filename only
 */
static const char* extract_filename(const char *path) {
    const char *filename = strrchr(path, '/');
    if (filename == NULL) {
        filename = strrchr(path, '\\');
    }
    return (filename != NULL) ? filename + 1 : path;
}

bool logger_init(const char *log_file, LogLevel min_level) {
    if (logger_config.initialized) {
        fprintf(stderr, "Logger already initialized\n");
        return false;
    }
    
    logger_config.min_level = min_level;
    logger_config.log_file = NULL;
    
    /* Open log file if path provided*/
    if (log_file != NULL) {
        logger_config.log_file = fopen(log_file, "a");
        if (logger_config.log_file == NULL) {
            fprintf(stderr, "Failed to open log file: %s\n", log_file);
            return false;
        }
        strncpy(logger_config.log_file_path, log_file, 
                sizeof(logger_config.log_file_path) - 1);
        
        /* Set line buffering for immediate output*/
        setvbuf(logger_config.log_file, NULL, _IOLBF, 0);
    }
    
    logger_config.initialized = true;
    
    // Log initialization message
    log_info("Logger initialized (min_level=%s, file=%s)", 
             get_level_name(min_level),
             log_file ? log_file : "console-only");
    
    return true;
}

void logger_set_level(LogLevel level) {
    if (!logger_config.initialized) {
        fprintf(stderr, "Logger not initialized\n");
        return;
    }
    
    if (level < LOG_EMERGENCY || level > LOG_DEBUG) {
        fprintf(stderr, "Invalid log level: %d\n", level);
        return;
    }
    
    LogLevel old_level = logger_config.min_level;
    logger_config.min_level = level;
    
    log_info("Log level changed from %s to %s", 
             get_level_name(old_level),
             get_level_name(level));
}

LogLevel logger_get_level(void) {
    return logger_config.min_level;
}

void logger_log(LogLevel level, const char *file, int line, 
                const char *format, ...) {
    // Check initialization
    if (!logger_config.initialized) {
        static bool warning_shown = false;
        if (!warning_shown) {
            fprintf(stderr, "Warning: Logger not initialized. "
                           "Call logger_init() first.\n");
            warning_shown = true;
        }
        return;
    }
    
    // Level filtering
    if (level > logger_config.min_level) {
        return;
    }
    
    // Prepare timestamp
    char timestamp[MAX_TIMESTAMP];
    get_timestamp(timestamp, sizeof(timestamp));
    
    // Prepare level name
    const char *level_name = get_level_name(level);
    
    // Extract filename
    const char *filename = extract_filename(file);
    
    // Format user message
    char message[MAX_LOG_MESSAGE];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    // Build complete log line
    char log_line[MAX_LOG_MESSAGE + 256];
    snprintf(log_line, sizeof(log_line), 
             "[%s] [%s] [%s:%d] - %s\n",
             timestamp, level_name, filename, line, message);
    
    // Output to console (stderr for errors, stdout for others)
    FILE *console = (level <= LOG_ERROR) ? stderr : stdout;
    fprintf(console, "%s", log_line);
    
    // Output to file if available
    if (logger_config.log_file != NULL) {
        fprintf(logger_config.log_file, "%s", log_line);
    }
}

void logger_flush(void) {
    if (logger_config.initialized && logger_config.log_file != NULL) {
        fflush(logger_config.log_file);
    }
    fflush(stdout);
    fflush(stderr);
}

void logger_cleanup(void) {
    if (!logger_config.initialized) {
        return;
    }
    
    log_info("Logger shutting down");
    logger_flush();
    
    if (logger_config.log_file != NULL) {
        fclose(logger_config.log_file);
        logger_config.log_file = NULL;
    }
    
    logger_config.initialized = false;
}