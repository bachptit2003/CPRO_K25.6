# Filestat - Trình Kiểm tra Siêu dữ liệu File

Công cụ dòng lệnh hiển thị thông tin metadata của file/thư mục trên Linux sử dụng system call `lstat()`.

## 📋 Mục lục

- [Tính năng](#-tính-năng)
- [Cấu trúc dự án](#-cấu-trúc-dự-án)
- [Yêu cầu hệ thống](#-yêu-cầu-hệ-thống)
- [Cài đặt và Build](#-cài-đặt-và-build)
- [Sử dụng](#-sử-dụng)
- [Giải thích kỹ thuật](#-giải-thích-kỹ-thuật)
- [Đẩy lên GitHub](#-đẩy-lên-github)

## ✨ Tính năng

- Hiển thị **đường dẫn file**
- Xác định **loại file** (Regular File, Directory, Symbolic Link, Character Device, Block Device, FIFO, Socket)
- Hiển thị **kích thước** file (bytes)
- Hiển thị **thời gian sửa đổi cuối cùng** (Last Modified)

## 📁 Cấu trúc dự án

```
filestat/
├── filestat.h          # Header declarations
├── filestat.c          # Chương trình chính (main)
├── filestat_utils.c    # Các hàm tiện ích
├── Makefile            # Build automation
└── README.md           # Tài liệu hướng dẫn
```

### Mô tả các file

| File | Mô tả |
|------|-------|
| `filestat.h` | Khai báo các header, hằng số và prototype hàm |
| `filestat.c` | Hàm `main()`, xử lý tham số và gọi `lstat()` |
| `filestat_utils.c` | Các hàm: `get_file_type()`, `format_time()`, `print_file_info()` |

## 💻 Yêu cầu hệ thống

- **OS**: Linux (đã test trên Ubuntu/Debian)
- **Compiler**: GCC
- **Make**: GNU Make

## 🔧 Cài đặt và Build

### Clone repository (nếu đã push lên GitHub)

```bash
git clone https://github.com/<username>/filestat.git
cd filestat
```

### Build chương trình

```bash
make
```

### Các lệnh Make khác

```bash
make clean    # Xóa các file object và executable
make rebuild  # Build lại từ đầu
make test     # Chạy test tự động
```

## 🚀 Sử dụng

### Cú pháp

```bash
./filestat <file_path>
```

### Ví dụ

```bash
# Kiểm tra một file thông thường
./filestat /etc/passwd

# Kiểm tra một thư mục
./filestat /home

# Kiểm tra symbolic link
./filestat /usr/bin/python

# Kiểm tra chính file source code
./filestat filestat.c
```

### Output mẫu

```
========================================
       FILE METADATA INFORMATION        
========================================
File Path:     /etc/passwd
File Type:     Regular File
Size:          2847 bytes
Last Modified: 2026-01-20 15:30:45
========================================
```

## 📚 Giải thích kỹ thuật

### System call `lstat()`

```c
int lstat(const char *pathname, struct stat *statbuf);
```

- **Khác biệt với `stat()`**: `lstat()` không theo dõi symbolic link, trả về thông tin của chính link đó
- **Return value**: 0 nếu thành công, -1 nếu lỗi (errno được set)

### Struct `stat`

```c
struct stat {
    mode_t    st_mode;    /* Loại file và quyền truy cập */
    off_t     st_size;    /* Kích thước file (bytes) */
    time_t    st_mtime;   /* Thời gian sửa đổi cuối */
    /* ... và nhiều trường khác */
};
```

### Các macro kiểm tra loại file

| Macro | Kiểm tra |
|-------|----------|
| `S_ISREG(mode)` | Regular file |
| `S_ISDIR(mode)` | Directory |
| `S_ISLNK(mode)` | Symbolic link |
| `S_ISCHR(mode)` | Character device |
| `S_ISBLK(mode)` | Block device |
| `S_ISFIFO(mode)` | FIFO/Pipe |
| `S_ISSOCK(mode)` | Socket |

## 📤 Đẩy lên GitHub

### Bước 1: Khởi tạo Git repository

```bash
cd filestat
git init
```

### Bước 2: Tạo file .gitignore

```bash
echo -e "# Compiled files\n*.o\nfilestat\n\n# Editor files\n*.swp\n*~" > .gitignore
```

### Bước 3: Add và commit

```bash
git add .
git commit -m "Initial commit: filestat - File metadata checker"
```

### Bước 4: Tạo repository trên GitHub

1. Truy cập https://github.com
2. Click **"New repository"**
3. Đặt tên: `filestat`
4. Chọn **Public** hoặc **Private**
5. **Không** tích chọn "Initialize with README" (vì đã có sẵn)
6. Click **"Create repository"**

### Bước 5: Kết nối và push

```bash
# Thay <username> bằng username GitHub của bạn
git remote add origin https://github.com/<username>/filestat.git
git branch -M main
git push -u origin main
```

### Các lần push tiếp theo

```bash
git add .
git commit -m "Mô tả thay đổi"
git push
```

## 📝 License

MIT License - Xem file [LICENSE](LICENSE) để biết thêm chi tiết.

## 👤 Tác giả

- **Tên**: [Họ và tên của bạn]
- **Email**: [email@example.com]
