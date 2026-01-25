/**
 * @file task_queue.h
 * @brief Header file cho Task Queue - Hàng đợi FIFO sử dụng Singly Linked List
 * 
 * Danh sách liên kết đơn là lựa chọn hoàn hảo cho hàng đợi FIFO vì:
 * - Chỉ cần duyệt theo một chiều (head -> tail)
 * - Thêm vào cuối (enqueue) và lấy từ đầu (dequeue) đều O(1)
 * - Tiết kiệm bộ nhớ hơn danh sách liên kết đôi
 */

#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Kích thước tối đa cho mô tả tác vụ */
#define TASK_DESC_SIZE 50

/**
 * @brief Cấu trúc Node cho Task Queue (Singly Linked List)
 * 
 * Mỗi node chứa:
 * - task_description: Mô tả công việc cần thực hiện
 * - next: Con trỏ tới node tiếp theo trong hàng đợi
 */
typedef struct TaskNode {
    char task_description[TASK_DESC_SIZE];  /* Mô tả tác vụ */
    struct TaskNode* next;                   /* Con trỏ tới node kế tiếp */
} TaskNode_t;

/* ======================== FUNCTION PROTOTYPES ======================== */

/**
 * @brief Thêm một tác vụ mới vào cuối hàng đợi (enqueue)
 * @param description Mô tả của tác vụ cần thêm
 */
void queue_add_task(const char* description);

/**
 * @brief Lấy tác vụ tiếp theo từ đầu hàng đợi (dequeue)
 * @return Con trỏ tới node tác vụ, hoặc NULL nếu hàng đợi rỗng
 * @note Caller có trách nhiệm free() node sau khi sử dụng
 */
TaskNode_t* queue_get_next_task(void);

/**
 * @brief In tất cả các tác vụ đang chờ trong hàng đợi
 */
void print_task_queue(void);

/**
 * @brief Kiểm tra xem hàng đợi có rỗng không
 * @return 1 nếu rỗng, 0 nếu có tác vụ
 */
int queue_is_empty(void);

/**
 * @brief Giải phóng toàn bộ bộ nhớ của hàng đợi
 */
void queue_destroy(void);

#endif /* TASK_QUEUE_H */
