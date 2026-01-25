/**
 * @file main.c
 * @brief Chương trình chính - Hệ thống Quản lý Tác vụ và Nhật ký
 * 
 * Tích hợp:
 * - Task Queue (Singly Linked List) cho hàng đợi FIFO
 * - Activity Log (Doubly Linked List) cho nhật ký với navigation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "task_queue.h"
#include "activity_log.h"

/* Kích thước buffer cho input */
#define INPUT_BUFFER_SIZE 256

/* ======================== HELPER FUNCTIONS ======================== */

/**
 * @brief In menu hướng dẫn sử dụng
 */
static void print_menu(void)
{
    printf("\n============ TASK MANAGER MENU ============\n");
    printf("Commands:\n");
    printf("  add <description>  - Add a new task to queue\n");
    printf("  run                - Execute next task (FIFO)\n");
    printf("  list               - Show all pending tasks\n");
    printf("  history            - Navigate activity log\n");
    printf("  log                - Show all log entries\n");
    printf("  help               - Show this menu\n");
    printf("  quit               - Exit program\n");
    printf("=============================================\n\n");
}

/**
 * @brief Xử lý lệnh add
 * @param args Phần còn lại của dòng lệnh (mô tả tác vụ)
 */
static void handle_add_command(const char* args)
{
    /* Bỏ qua khoảng trắng đầu */
    while (*args && isspace((unsigned char)*args)) {
        args++;
    }
    
    if (*args == '\0') {
        printf("Usage: add <task description>\n");
        printf("Example: add Read temperature sensor\n");
        return;
    }
    
    queue_add_task(args);
}

/**
 * @brief Xử lý lệnh run - thực thi tác vụ và ghi log
 */
static void handle_run_command(void)
{
    TaskNode_t* task;
    char log_message[LOG_ENTRY_SIZE];
    
    /* Lấy tác vụ từ đầu hàng đợi */
    task = queue_get_next_task();
    
    if (task == NULL) {
        /* Hàng đợi rỗng, đã in thông báo trong queue_get_next_task() */
        return;
    }
    
    /* In thông báo đang thực thi */
    printf("\n>>> EXECUTING TASK: \"%s\"\n", task->task_description);
    printf(">>> Task completed successfully!\n");
    
    /* Tạo log entry */
    snprintf(log_message, LOG_ENTRY_SIZE, "Executed: %s", task->task_description);
    
    /* Ghi vào nhật ký */
    history_log_activity(log_message);
    
    /* Giải phóng bộ nhớ của node tác vụ */
    free(task);
    task = NULL;  /* Tránh dangling pointer */
}

/**
 * @brief Đọc một dòng input từ stdin
 * @param buffer Buffer để lưu input
 * @param size Kích thước buffer
 * @return 1 nếu thành công, 0 nếu EOF hoặc lỗi
 */
static int read_input_line(char* buffer, size_t size)
{
    printf("> ");
    fflush(stdout);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    /* Xóa newline cuối */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

/* ======================== MAIN FUNCTION ======================== */

int main(void)
{
    char input[INPUT_BUFFER_SIZE];
    char command[32];
    char* args;
    int running = 1;
    
    printf("\n");
    printf("==============================================\n");
    printf("   TASK QUEUE & ACTIVITY LOG MANAGER\n");
    printf("==============================================\n");
    printf("  Task Queue: Singly Linked List (FIFO)\n");
    printf("  Activity Log: Doubly Linked List (Navigation)\n");
    printf("==============================================\n");
    
    print_menu();
    
    /* Vòng lặp chính - đọc và xử lý lệnh */
    while (running) {
        /* Đọc input từ người dùng */
        if (!read_input_line(input, sizeof(input))) {
            printf("\nEnd of input. Exiting...\n");
            break;
        }
        
        /* Bỏ qua dòng trống */
        if (input[0] == '\0') {
            continue;
        }
        
        /* Tách command và arguments */
        args = strchr(input, ' ');
        if (args != NULL) {
            /* Copy command (phần trước space) */
            size_t cmd_len = args - input;
            if (cmd_len >= sizeof(command)) {
                cmd_len = sizeof(command) - 1;
            }
            strncpy(command, input, cmd_len);
            command[cmd_len] = '\0';
            args++;  /* Bỏ qua space, chỉ tới arguments */
        } else {
            /* Không có arguments */
            strncpy(command, input, sizeof(command) - 1);
            command[sizeof(command) - 1] = '\0';
            args = input + strlen(input);  /* Empty string */
        }
        
        /* Xử lý commands */
        if (strcmp(command, "add") == 0) {
            handle_add_command(args);
        }
        else if (strcmp(command, "run") == 0) {
            handle_run_command();
        }
        else if (strcmp(command, "list") == 0) {
            print_task_queue();
        }
        else if (strcmp(command, "history") == 0) {
            history_navigate();
        }
        else if (strcmp(command, "log") == 0) {
            history_print_all();
        }
        else if (strcmp(command, "help") == 0) {
            print_menu();
        }
        else if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
            printf("\nExiting...\n");
            running = 0;
        }
        else {
            printf("Unknown command: '%s'. Type 'help' for usage.\n", command);
        }
    }
    
    /* Dọn dẹp bộ nhớ trước khi thoát */
    printf("\nCleaning up...\n");
    queue_destroy();
    history_destroy();
    
    printf("Goodbye!\n\n");
    
    return 0;
}
