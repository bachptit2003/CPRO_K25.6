/**
 * @file main.c
 * @brief Demonstration program for the logger module
 */

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

/**
 * @brief Simulate database connection
 */
static void simulate_database_connection(void) {
    log_info("Attempting to connect to database...");
    
    // Simulate connection attempt
    int success = rand() % 2;
    
    if (success) {
        log_notice("Database connection established successfully");
    } else {
        log_error("Failed to connect to database: Connection timeout");
        log_debug("Connection parameters: host=localhost, port=5432");
    }
}

/**
 * @brief Simulate file processing
 */
static void simulate_file_processing(void) {
    const char *filename = "data.csv";
    
    log_info("Starting file processing: %s", filename);
    
    // Simulate warnings
    log_warning("File size is larger than expected (10MB > 5MB limit)");
    
    // Simulate processing
    for (int i = 1; i <= 3; i++) {
        log_debug("Processing batch %d/3", i);
    }
    
    log_notice("File processing completed: %d records processed", 1500);
}

/**
 * @brief Simulate critical system event
 */
static void simulate_critical_event(void) {
    log_critical("Memory usage exceeded 90%% threshold!");
    log_alert("System is running out of disk space");
}

/**
 * @brief Demonstrate all log levels
 */
static void demonstrate_all_levels(void) {
    printf("\n=== Demonstrating all log levels ===\n");
    
    log_emergency("System emergency: Kernel panic detected!");
    log_alert("Security alert: Unauthorized access attempt detected");
    log_critical("Critical: Primary database server is down");
    log_error("Error: Failed to read configuration file");
    log_warning("Warning: API rate limit approaching (90%%)");
    log_notice("Notice: User 'admin' logged in from new IP address");
    log_info("Info: Application started successfully");
    log_debug("Debug: Configuration loaded from /etc/app/config.ini");
}

/**
 * @brief Demonstrate level filtering
 */
static void demonstrate_level_filtering(void) {
    printf("\n=== Demonstrating level filtering ===\n");
    
    printf("\n--- Set level to WARNING (only WARNING and above) ---\n");
    logger_set_level(LOG_WARNING);
    
    log_debug("This debug message will NOT appear");
    log_info("This info message will NOT appear");
    log_warning("This warning WILL appear");
    log_error("This error WILL appear");
    
    printf("\n--- Set level to DEBUG (all messages) ---\n");
    logger_set_level(LOG_DEBUG);
    
    log_debug("Now debug messages appear");
    log_info("And info messages appear too");
}

/**
 * @brief Demonstrate formatted messages
 */
static void demonstrate_formatted_messages(void) {
    printf("\n=== Demonstrating formatted messages ===\n");
    
    int user_id = 12345;
    const char *username = "john_doe";
    double response_time = 125.67;
    
    log_info("User login: id=%d, username=%s", user_id, username);
    log_debug("API response time: %.2fms", response_time);
    log_warning("Failed login attempts: %d/%d", 3, 5);
    log_error("Transaction failed: amount=$%.2f, reason=%s", 
              99.99, "Insufficient funds");
}

int main(void) {
    printf("===========================================\n");
    printf("  Professional Logger Module Demo\n");
    printf("===========================================\n\n");
    
    // Initialize logger with file output and INFO level
    if (!logger_init("logs/application.log", LOG_INFO)) {
        fprintf(stderr, "Failed to initialize logger\n");
        return EXIT_FAILURE;
    }
    
    printf("Logger initialized successfully!\n");
    printf("Logs are being written to: logs/application.log\n");
    printf("Check both console and file for output.\n\n");
    
    // Run demonstrations
    demonstrate_all_levels();
    
    printf("\nWaiting 2 seconds...\n");
    sleep(2);
    
    demonstrate_level_filtering();
    
    printf("\nWaiting 2 seconds...\n");
    sleep(2);
    
    demonstrate_formatted_messages();
    
    printf("\n=== Simulating real application scenarios ===\n");
    simulate_database_connection();
    simulate_file_processing();
    simulate_critical_event();
    
    // Clean up
    printf("\n===========================================\n");
    printf("Demo completed. Cleaning up...\n");
    logger_cleanup();
    
    printf("Check 'logs/application.log' for the complete log file.\n");
    printf("===========================================\n");
    
    return EXIT_SUCCESS;
}