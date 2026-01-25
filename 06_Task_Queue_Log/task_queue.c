/**
 * @file task_queue.c
 * @brief Triển khai Task Queue - Hàng đợi FIFO sử dụng Singly Linked List
 * 
 * Hàng đợi sử dụng hai con trỏ:
 * - head: Trỏ tới đầu hàng đợi (nơi lấy tác vụ ra)
 * - tail: Trỏ tới cuối hàng đợi (nơi thêm tác vụ mới)
 * 
 * Điều này đảm bảo cả enqueue và dequeue đều O(1)
 */

#include "task_queue.h"

/* ======================== GLOBAL VARIABLES ======================== */

/* Con trỏ tới đầu hàng đợi - nơi lấy tác vụ ra */
static TaskNode_t* queue_head = NULL;

/* Con trỏ tới cuối hàng đợi - nơi thêm tác vụ mới */
static TaskNode_t* queue_tail = NULL;

/* ======================== FUNCTION IMPLEMENTATIONS ======================== */

/**
 * @brief Thêm một tác vụ mới vào cuối hàng đợi (enqueue)
 * 
 * Thuật toán:
 * 1. Cấp phát bộ nhớ cho node mới
 * 2. Copy mô tả vào node
 * 3. Nếu hàng đợi rỗng: head = tail = node mới
 * 4. Nếu không: thêm vào sau tail, cập nhật tail
 * 
 * Độ phức tạp: O(1)
 * 
 * @param description Mô tả của tác vụ cần thêm
 */
void queue_add_task(const char* description)
{
    TaskNode_t* new_node;
    
    /* Kiểm tra tham số đầu vào */
    if (description == NULL) {
        fprintf(stderr, "Error: Task description cannot be NULL\n");
        return;
    }
    
    /* Bước 1: Cấp phát bộ nhớ cho node mới */
    new_node = (TaskNode_t*)malloc(sizeof(TaskNode_t));
    if (new_node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    
    /* Bước 2: Khởi tạo dữ liệu cho node */
    strncpy(new_node->task_description, description, TASK_DESC_SIZE - 1);
    new_node->task_description[TASK_DESC_SIZE - 1] = '\0';  /* Đảm bảo null-terminated */
    new_node->next = NULL;
    
    /* Bước 3 & 4: Thêm node vào hàng đợi */
    if (queue_head == NULL) {
        /* Hàng đợi rỗng - node mới là cả head và tail */
        queue_head = new_node;
        queue_tail = new_node;
    } else {
        /* Thêm vào cuối hàng đợi */
        queue_tail->next = new_node;
        queue_tail = new_node;
    }
    
    printf("[Queue] Added task: \"%s\"\n", new_node->task_description);
}

/**
 * @brief Lấy tác vụ tiếp theo từ đầu hàng đợi (dequeue)
 * 
 * Thuật toán:
 * 1. Nếu hàng đợi rỗng, trả về NULL
 * 2. Lưu con trỏ tới node đầu
 * 3. Di chuyển head tới node tiếp theo
 * 4. Nếu head == NULL, cập nhật tail = NULL
 * 5. Trả về node đã lấy (caller sẽ free)
 * 
 * Độ phức tạp: O(1)
 * 
 * @return Con trỏ tới node tác vụ, hoặc NULL nếu rỗng
 */
TaskNode_t* queue_get_next_task(void)
{
    TaskNode_t* task_node;
    
    /* Kiểm tra hàng đợi có rỗng không */
    if (queue_head == NULL) {
        printf("[Queue] Queue is empty. No task to execute.\n");
        return NULL;
    }
    
    /* Lưu node đầu tiên */
    task_node = queue_head;
    
    /* Di chuyển head tới node tiếp theo */
    queue_head = queue_head->next;
    
    /* Nếu hàng đợi trở nên rỗng, cập nhật tail */
    if (queue_head == NULL) {
        queue_tail = NULL;
    }
    
    /* Ngắt kết nối node khỏi hàng đợi */
    task_node->next = NULL;
    
    return task_node;
}

/**
 * @brief In tất cả các tác vụ đang chờ trong hàng đợi
 * 
 * Duyệt từ head đến tail và in ra từng tác vụ
 * 
 * Độ phức tạp: O(n)
 */
void print_task_queue(void)
{
    TaskNode_t* current;
    int index = 1;
    
    printf("\n========== TASK QUEUE ==========\n");
    
    if (queue_head == NULL) {
        printf("(Queue is empty)\n");
    } else {
        current = queue_head;
        while (current != NULL) {
            printf("  %d. %s\n", index++, current->task_description);
            current = current->next;
        }
    }
    
    printf("=================================\n\n");
}

/**
 * @brief Kiểm tra xem hàng đợi có rỗng không
 * @return 1 nếu rỗng, 0 nếu có tác vụ
 */
int queue_is_empty(void)
{
    return (queue_head == NULL);
}

/**
 * @brief Giải phóng toàn bộ bộ nhớ của hàng đợi
 * 
 * Duyệt qua từng node và free bộ nhớ
 */
void queue_destroy(void)
{
    TaskNode_t* current;
    TaskNode_t* next_node;
    
    current = queue_head;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    
    queue_head = NULL;
    queue_tail = NULL;
    
    printf("[Queue] All tasks cleared.\n");
}
