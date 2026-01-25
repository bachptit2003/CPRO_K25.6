/**
 * @file activity_log.c
 * @brief Triển khai Activity Log - Nhật ký sử dụng Doubly Linked List
 * 
 * Danh sách liên kết đôi cho phép:
 * - Di chuyển tới (next) - xem hoạt động mới hơn
 * - Di chuyển lui (prev) - xem hoạt động cũ hơn
 * - Thao tác O(1) theo cả hai hướng
 */

#include "activity_log.h"

/* ======================== GLOBAL VARIABLES ======================== */

/* Con trỏ tới đầu danh sách (entry mới nhất) */
static HistoryNode_t* history_head = NULL;

/* Con trỏ tới cuối danh sách (entry cũ nhất) */
static HistoryNode_t* history_tail = NULL;

/* Con trỏ vị trí hiện tại khi duyệt */
static HistoryNode_t* current_node = NULL;

/* ======================== FUNCTION IMPLEMENTATIONS ======================== */

/**
 * @brief Ghi một hoạt động mới vào đầu nhật ký
 * 
 * Thuật toán:
 * 1. Cấp phát node mới
 * 2. Copy nội dung log
 * 3. Nếu danh sách rỗng: head = tail = node mới
 * 4. Nếu không: chèn vào đầu danh sách
 *    - new_node->next = head
 *    - head->prev = new_node
 *    - head = new_node
 * 
 * Độ phức tạp: O(1)
 * 
 * @param entry Nội dung nhật ký
 */
void history_log_activity(const char* entry)
{
    HistoryNode_t* new_node;
    
    /* Kiểm tra tham số */
    if (entry == NULL) {
        fprintf(stderr, "Error: Log entry cannot be NULL\n");
        return;
    }
    
    /* Cấp phát bộ nhớ */
    new_node = (HistoryNode_t*)malloc(sizeof(HistoryNode_t));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    
    /* Khởi tạo dữ liệu */
    strncpy(new_node->log_entry, entry, LOG_ENTRY_SIZE - 1);
    new_node->log_entry[LOG_ENTRY_SIZE - 1] = '\0';
    new_node->next = NULL;
    new_node->prev = NULL;
    
    /* Chèn vào đầu danh sách */
    if (history_head == NULL) {
        /* Danh sách rỗng */
        history_head = new_node;
        history_tail = new_node;
    } else {
        /* 
         * Chèn vào đầu:
         * new_node <-> old_head <-> ... <-> tail
         */
        new_node->next = history_head;
        history_head->prev = new_node;
        history_head = new_node;
    }
    
    printf("[Log] Recorded: \"%s\"\n", new_node->log_entry);
}

/**
 * @brief In thông tin entry hiện tại
 * @param node Node cần in
 * @param position Vị trí hiện tại (để hiển thị)
 */
static void print_current_entry(HistoryNode_t* node)
{
    printf("\n--------------------------------------------\n");
    printf("Current log entry: \"%s\"\n", node->log_entry);
    printf("--------------------------------------------\n");
    
    /* Hiển thị các tùy chọn di chuyển có thể */
    printf("Navigation: ");
    if (node->prev != NULL) {
        printf("[n] Next (newer) ");
    }
    if (node->next != NULL) {
        printf("[p] Previous (older) ");
    }
    printf("[q] Quit\n");
}

/**
 * @brief Chế độ tương tác duyệt nhật ký
 * 
 * Cho phép người dùng di chuyển trong danh sách:
 * - n: Di chuyển tới entry mới hơn (prev trong doubly linked list)
 * - p: Di chuyển tới entry cũ hơn (next trong doubly linked list)
 * - q: Thoát
 * 
 * Lưu ý: Vì entry mới nhất ở đầu (head), nên:
 * - "next" (n) thực tế là di chuyển về phía head (prev pointer)
 * - "previous" (p) thực tế là di chuyển về phía tail (next pointer)
 */
void history_navigate(void)
{
    char command;
    
    /* Kiểm tra danh sách có rỗng không */
    if (history_head == NULL) {
        printf("\n[Log] Activity log is empty. Nothing to navigate.\n");
        return;
    }
    
    /* Bắt đầu từ entry mới nhất (head) */
    current_node = history_head;
    
    printf("\n========== ACTIVITY LOG NAVIGATION ==========\n");
    printf("Starting from the most recent entry.\n");
    printf("Commands: [n] Newer, [p] Older, [q] Quit\n");
    printf("=============================================\n");
    
    /* Hiển thị entry đầu tiên */
    print_current_entry(current_node);
    
    /* Vòng lặp điều hướng */
    while (1) {
        printf("\nEnter command (n/p/q): ");
        
        /* Đọc lệnh từ người dùng */
        if (scanf(" %c", &command) != 1) {
            /* Xử lý lỗi đọc */
            while (getchar() != '\n');  /* Clear input buffer */
            printf("Invalid input. Please enter n, p, or q.\n");
            continue;
        }
        
        switch (command) {
            case 'n':
            case 'N':
                /* Di chuyển tới entry mới hơn (về phía head) */
                if (current_node->prev != NULL) {
                    current_node = current_node->prev;
                    print_current_entry(current_node);
                } else {
                    printf("\n[!] Already at the newest entry.\n");
                }
                break;
                
            case 'p':
            case 'P':
                /* Di chuyển tới entry cũ hơn (về phía tail) */
                if (current_node->next != NULL) {
                    current_node = current_node->next;
                    print_current_entry(current_node);
                } else {
                    printf("\n[!] Already at the oldest entry.\n");
                }
                break;
                
            case 'q':
            case 'Q':
                printf("\n[Log] Exiting navigation mode.\n");
                return;
                
            default:
                printf("Unknown command. Use n (newer), p (older), or q (quit).\n");
                break;
        }
    }
}

/**
 * @brief In toàn bộ nhật ký từ mới nhất đến cũ nhất
 */
void history_print_all(void)
{
    HistoryNode_t* current;
    int index = 1;
    
    printf("\n========== ACTIVITY LOG ==========\n");
    
    if (history_head == NULL) {
        printf("(Log is empty)\n");
    } else {
        current = history_head;
        while (current != NULL) {
            printf("  %d. %s\n", index++, current->log_entry);
            current = current->next;
        }
    }
    
    printf("===================================\n\n");
}

/**
 * @brief Kiểm tra nhật ký có rỗng không
 * @return 1 nếu rỗng, 0 nếu có dữ liệu
 */
int history_is_empty(void)
{
    return (history_head == NULL);
}

/**
 * @brief Giải phóng toàn bộ bộ nhớ nhật ký
 */
void history_destroy(void)
{
    HistoryNode_t* current;
    HistoryNode_t* next_node;
    
    current = history_head;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    
    history_head = NULL;
    history_tail = NULL;
    current_node = NULL;
    
    printf("[Log] Activity log cleared.\n");
}
