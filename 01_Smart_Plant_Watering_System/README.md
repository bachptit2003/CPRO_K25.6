# 01 - Smart Plant Watering System (SPWS)  
**Hệ thống tưới cây tự động thông minh dựa trên cảm biến độ ẩm đất**



## Giới thiệu ngắn gọn
Hệ thống mô phỏng hoàn chỉnh một máy tưới cây tự động với 2 chế độ:
- **AUTO**: tự động tưới khi đất khô (< 30%), tự dừng khi đủ ẩm hoặc quá thời gian
- **MANUAL**: người dùng nhấn nút để tưới thủ công

## Tính năng chính
- Cảm biến độ ẩm đất + nhiệt độ môi trường (mô phỏng)
- Bơm nước mini (ON/OFF)
- LED báo trạng thái 4 màu: Xanh (bình thường), Vàng (đang tưới), Đỏ nhấp nháy (đất quá khô), Đỏ sáng (lỗi)
- Nút bấm chuyển chế độ & tưới tay
- Kiến trúc Clean Code + HAL →  port sang STM32, ESP32, Arduino để chạy thật.

## Cấu trúc thư mục
```textcd
cd

01_Smart_Plant_Watering_System/
├── inc/                  → tất cả file header (.h)
├── src/                  → tất cả file nguồn (.c)
├── sim/                  → (tùy chọn) file simulator 1-click
├── .gitignore
└── README.md             → bạn đang đọc đây
```
## Cách biên dịch & chạy trên PC (Linux / macOS / WSL)
 ### Yêu cầu
 - Đã cài `gcc` và `make` (nếu dùng WSL thì chạy `sudo apt install build-essential`)
```bash
cd "01_Smart_Plant_Watering_System"
gcc -Iinc src/*.c -o spws_sim -lm
./spws_sim
```

## Hướng dẫn sử dụng khi chương trình đang chạy.


| Phím     | Chức năng                          | Ghi chú                                   |
|----------|-------------------------------------|--------------------------------------------|
| `m`      | Chuyển đổi giữa **AUTO** và **MANUAL** | Có thể nhấn bất kỳ lúc nào                 |
| `t`      | Tưới nước thủ công                 | Chỉ hoạt động khi đang ở chế độ **MANUAL** |
| `q`      | Thoát chương trình                 | (nếu bạn dùng file simulator.c)            |
| `Ctrl + C` | Thoát khẩn cấp                     | Luôn luôn hoạt động                        |








### Trạng thái LED hiển thị trên màn hình

- Xanh   → Đất đủ ẩm, hệ thống ổn định
- Vàng   → Đang tưới nước (bơm đang chạy)
- Đỏ nhấp nháy → Đất quá khô (cần tưới ngay)
- Đỏ sáng → Lỗi hệ thống (hiện tại chưa dùng)

### Ví dụ thực tế khi chạy
```text
======================================
  Smart Plant Watering System (SPWS)
======================================
Controls:
  [m]  → Toggle AUTO / MANUAL mode
  [t]  → Manual watering (only in MANUAL mode)
--------------------------------------

[SPWS] System initialized - AUTO mode
[SPWS] Sensors - Moisture: 23.1% | Temp: 26.4°C
*** PUMP ON  (Watering started) ***
LED → YELLOW  (Watering in progress)
```

































