/**
 * @file activity_log.h
 * @brief Header file cho Activity Log - Nhật ký sử dụng Doubly Linked List
 * 
 * Danh sách liên kết đôi là BẮT BUỘC cho nhật ký vì:
 * - Cần di chuyển cả hai chiều (next và prev)
 * - Người dùng có thể duyệt tới lui trong lịch sử
 * - Thao tác di chuyển O(1) theo cả hai hướng
 */

#ifndef ACTIVITY_LOG_H
#define ACTIVITY_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Kích thước tối đa cho nội dung log */
#define LOG_ENTRY_SIZE 50

/**
 * @brief Cấu trúc Node cho Activity Log (Doubly Linked List)
 * 
 * Mỗi node chứa:
 * - log_entry: Nội dung nhật ký
 * - next: Con trỏ tới node mới hơn
 * - prev: Con trỏ tới node cũ hơn
 */
typedef struct HistoryNode {
    char log_entry[LOG_ENTRY_SIZE];   /* Nội dung nhật ký */
    struct HistoryNode* next;          /* Con trỏ tới node kế tiếp (mới hơn) */
    struct HistoryNode* prev;          /* Con trỏ tới node trước đó (cũ hơn) */
} HistoryNode_t;

/* ======================== FUNCTION PROTOTYPES ======================== */

/**
 * @brief Ghi một hoạt động mới vào đầu nhật ký
 * @param entry Nội dung nhật ký cần ghi
 * 
 * Hoạt động mới nhất sẽ nằm ở đầu danh sách
 */
void history_log_activity(const char* entry);

/**
 * @brief Chế độ tương tác duyệt nhật ký
 * 
 * Cho phép người dùng:
 * - n: Di chuyển tới entry mới hơn (next)
 * - p: Di chuyển tới entry cũ hơn (prev)  
 * - q: Thoát chế độ duyệt
 */
void history_navigate(void);

/**
 * @brief In toàn bộ nhật ký
 */
void history_print_all(void);

/**
 * @brief Kiểm tra nhật ký có rỗng không
 * @return 1 nếu rỗng, 0 nếu có dữ liệu
 */
int history_is_empty(void);

/**
 * @brief Giải phóng toàn bộ bộ nhớ nhật ký
 */
void history_destroy(void);

#endif /* ACTIVITY_LOG_H */
