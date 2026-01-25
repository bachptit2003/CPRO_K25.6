/**
 * @file filestat_utils.c
 * @brief Module chứa các hàm tiện ích cho filestat
 * 
 * Các hàm trong module này xử lý việc phân tích và định dạng
 * thông tin metadata từ struct stat
 */

#include "filestat.h"

/* ======================== FILE TYPE DETECTION ======================== */

/**
 * @brief Xác định loại file dựa trên st_mode
 * 
 * Sử dụng các macro S_ISREG, S_ISDIR, S_ISLNK từ <sys/stat.h>
 * để kiểm tra bit mode và xác định loại file
 * 
 * @param mode Giá trị st_mode từ struct stat
 * @return Chuỗi mô tả loại file
 */
const char* get_file_type(mode_t mode)
{
    /* Kiểm tra xem có phải file thông thường không */
    if (S_ISREG(mode)) {
        return "Regular File";
    }
    
    /* Kiểm tra xem có phải thư mục không */
    if (S_ISDIR(mode)) {
        return "Directory";
    }
    
    /* Kiểm tra xem có phải symbolic link không */
    if (S_ISLNK(mode)) {
        return "Symbolic Link";
    }
    
    /* Kiểm tra các loại file đặc biệt khác */
    if (S_ISCHR(mode)) {
        return "Character Device";
    }
    
    if (S_ISBLK(mode)) {
        return "Block Device";
    }
    
    if (S_ISFIFO(mode)) {
        return "FIFO/Pipe";
    }
    
    if (S_ISSOCK(mode)) {
        return "Socket";
    }
    
    return "Unknown";
}

/* ======================== TIME FORMATTING ======================== */

/**
 * @brief Chuyển đổi timestamp thành chuỗi ngày-tháng-năm giờ-phút-giây
 * 
 * Sử dụng strftime() để định dạng thời gian theo format:
 * "YYYY-MM-DD HH:MM:SS"
 * 
 * @param mtime Giá trị time_t (Unix timestamp)
 * @param buffer Buffer để lưu chuỗi kết quả
 * @param buffer_size Kích thước của buffer
 */
void format_time(time_t mtime, char *buffer, size_t buffer_size)
{
    struct tm *time_info;
    
    /* Chuyển đổi time_t sang struct tm (local time) */
    time_info = localtime(&mtime);
    
    if (time_info == NULL) {
        /* Nếu chuyển đổi thất bại, dùng chuỗi mặc định */
        snprintf(buffer, buffer_size, "Unknown");
        return;
    }
    
    /* 
     * Định dạng thời gian theo format: "YYYY-MM-DD HH:MM:SS"
     * %Y - Năm đầy đủ (2024)
     * %m - Tháng (01-12)
     * %d - Ngày (01-31)
     * %H - Giờ (00-23)
     * %M - Phút (00-59)
     * %S - Giây (00-59)
     */
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", time_info);
}

/* ======================== OUTPUT PRINTING ======================== */

/**
 * @brief In thông tin metadata của file ra console
 * 
 * Hiển thị các thông tin:
 * - File Path: Đường dẫn file
 * - File Type: Loại file (Regular, Directory, Symbolic Link, etc.)
 * - Size: Kích thước file (bytes)
 * - Last Modified: Thời gian sửa đổi cuối cùng
 * 
 * @param filepath Đường dẫn file người dùng nhập
 * @param file_stat Con trỏ đến struct stat chứa metadata
 */
void print_file_info(const char *filepath, const struct stat *file_stat)
{
    char time_buffer[TIME_BUFFER_SIZE];
    
    /* In đường dẫn file */
    printf("========================================\n");
    printf("       FILE METADATA INFORMATION        \n");
    printf("========================================\n");
    
    printf("File Path:     %s\n", filepath);
    
    /* In loại file - sử dụng hàm get_file_type() */
    printf("File Type:     %s\n", get_file_type(file_stat->st_mode));
    
    /* 
     * In kích thước file
     * st_size có kiểu off_t, dùng %ld để in
     */
    printf("Size:          %ld bytes\n", (long)file_stat->st_size);
    
    /* 
     * In thời gian sửa đổi cuối cùng
     * st_mtime là modification time của file
     */
    format_time(file_stat->st_mtime, time_buffer, sizeof(time_buffer));
    printf("Last Modified: %s\n", time_buffer);
    
    printf("========================================\n");
}
