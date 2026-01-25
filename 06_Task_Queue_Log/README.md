# Task Queue & Activity Log System

Hệ thống quản lý hàng đợi tác vụ (FIFO) và nhật ký hoạt động cho bộ điều khiển nhúng.

## 📋 Mô tả

| Module | Cấu trúc dữ liệu | Mục đích |
|--------|------------------|----------|
| Task Queue | Singly Linked List | Hàng đợi FIFO - vào trước ra trước |
| Activity Log | Doubly Linked List | Nhật ký với navigation tới/lui |

## 📁 Cấu trúc Project

```
task_queue_log/
├── task_queue.h      # Header Task Queue
├── task_queue.c      # Implement Singly Linked List
├── activity_log.h    # Header Activity Log  
├── activity_log.c    # Implement Doubly Linked List
├── main.c            # Chương trình chính
├── Makefile
└── README.md
```

## 🔧 Build & Run

```bash
make        # Build
./task_manager   # Run
make clean  # Clean
```

## 🚀 Sử dụng

### Commands

| Lệnh | Mô tả |
|------|-------|
| `add <mô tả>` | Thêm tác vụ vào hàng đợi |
| `run` | Thực thi tác vụ tiếp theo (FIFO) |
| `list` | Hiển thị tất cả tác vụ đang chờ |
| `history` | Duyệt nhật ký (n/p/q) |
| `log` | Hiển thị toàn bộ nhật ký |
| `quit` | Thoát |

### Ví dụ

```
> add Read temperature sensor
[Queue] Added task: "Read temperature sensor"

> add Control motor speed
[Queue] Added task: "Control motor speed"

> list
========== TASK QUEUE ==========
  1. Read temperature sensor
  2. Control motor speed
=================================

> run
>>> EXECUTING TASK: "Read temperature sensor"
>>> Task completed successfully!
[Log] Recorded: "Executed: Read temperature sensor"

> history
Commands: [n] Newer, [p] Older, [q] Quit
Current log entry: "Executed: Read temperature sensor"
```

## 📚 Phân tích Câu hỏi

### 1. Tại sao Singly Linked List đủ cho Task Queue FIFO?

- **Chỉ cần duyệt một chiều**: Head → Tail
- **Thao tác O(1)**: Thêm vào tail, lấy từ head đều O(1) với tail pointer
- **Tiết kiệm bộ nhớ**: Không cần lưu con trỏ `prev`

### 2. Nếu dùng Singly Linked List cho history_navigate_backward()?

- **Phức tạp O(n)**: Phải duyệt từ head để tìm node trước đó
- **Tốn hiệu năng**: Mỗi lần di chuyển lui cần duyệt lại toàn bộ danh sách
- **Code phức tạp**: Cần theo dõi node hiện tại và tìm node trước

### 3. Đánh đổi bộ nhớ giữa DLL và SLL?

| Tiêu chí | Singly Linked List | Doubly Linked List |
|----------|-------------------|-------------------|
| Bộ nhớ/node | `data + 1 pointer` | `data + 2 pointers` |
| Overhead | Thấp hơn ~50% | Cao hơn (thêm prev) |
| Di chuyển | Chỉ một chiều | Cả hai chiều |
| Xóa node | O(n) cần tìm prev | O(1) có sẵn prev |

**Kết luận**: DLL tốn thêm bộ nhớ cho mỗi node nhưng cho phép thao tác hai chiều hiệu quả O(1).

## 📤 Push lên GitHub

```bash
cd /home/bach/Linux/CPRO_K25.6
cp -r /home/bach/Linux/filesystem/task_queue_log 06_Task_Queue_Log
git add 06_Task_Queue_Log
git commit -m "Add 06_Task_Queue_Log: SLL and DLL implementation"
git push origin main
```

## 👤 Tác giả

- **Sinh viên**: [Họ tên]
- **Môn học**: Lập trình C nâng cao
