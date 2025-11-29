/**
 * @file logger.h
 * @brief Professional logging system for C applications
 * @author Your Name
 * @date 2024
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Log levels following syslog standard (RFC 5424)
 * Lower numeric values indicate higher severity
 */
typedef enum {
    LOG_EMERGENCY = 0,  // System is unusable
    LOG_ALERT     = 1,  // Action must be taken immediately
    LOG_CRITICAL  = 2,  // Critical conditions
    LOG_ERROR     = 3,  // Error conditions
    LOG_WARNING   = 4,  // Warning conditions
    LOG_NOTICE    = 5,  // Normal but significant condition
    LOG_INFO      = 6,  // Informational messages
    LOG_DEBUG     = 7   // Debug-level messages
} LogLevel;

/**
 * @brief Initialize the logger system
 * 
 * @param log_file Path to log file (NULL for console only)
 * @param min_level Minimum log level to display
 * @return true if initialization successful, false otherwise
 */
bool logger_init(const char *log_file, LogLevel min_level);

/**
 * @brief Set the minimum log level for filtering
 * 
 * @param level New minimum log level
 */
void logger_set_level(LogLevel level);

/**
 * @brief Get the current minimum log level
 * 
 * @return Current minimum log level
 */
LogLevel logger_get_level(void);

/**
 * @brief Internal logging function (do not call directly)
 * 
 * @param level Log level
 * @param file Source file name
 * @param line Line number
 * @param format Format string (printf-style)
 * @param ... Variable arguments
 */
void logger_log(LogLevel level, const char *file, int line, 
                const char *format, ...);

/**
 * @brief Flush all log buffers
 */
void logger_flush(void);

/**
 * @brief Clean up and close the logger
 */
void logger_cleanup(void);

/**
 * @brief Convenience macro for logging with automatic file and line info
 * 
 * Usage: log_message(LOG_ERROR, "Connection failed: %s", error_msg);
 */
#define log_message(level, ...) \
    logger_log(level, __FILE__, __LINE__, __VA_ARGS__)

// Convenience macros for each log level
#define log_emergency(...) log_message(LOG_EMERGENCY, __VA_ARGS__)
#define log_alert(...)     log_message(LOG_ALERT, __VA_ARGS__)
#define log_critical(...)  log_message(LOG_CRITICAL, __VA_ARGS__)
#define log_error(...)     log_message(LOG_ERROR, __VA_ARGS__)
#define log_warning(...)   log_message(LOG_WARNING, __VA_ARGS__)
#define log_notice(...)    log_message(LOG_NOTICE, __VA_ARGS__)
#define log_info(...)      log_message(LOG_INFO, __VA_ARGS__)
#define log_debug(...)     log_message(LOG_DEBUG, __VA_ARGS__)

#endif // LOGGER_H
