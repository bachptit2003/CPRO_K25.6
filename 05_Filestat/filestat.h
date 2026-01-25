/**
 * @file filestat.h
 * @brief Header file cho chương trình filestat
 * 
 * Chứa các khai báo cần thiết cho việc đọc và hiển thị
 * siêu dữ liệu (metadata) của file sử dụng lstat()
 */

#ifndef FILESTAT_H
#define FILESTAT_H

/* Feature test macro - bật các hàm POSIX như lstat(), S_ISSOCK() */
#define _XOPEN_SOURCE 700

/* ======================== SYSTEM HEADERS ======================== */
#include <stdio.h>      /* printf, fprintf, perror */
#include <stdlib.h>     /* exit, EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/types.h>  /* Các kiểu dữ liệu hệ thống */
#include <sys/stat.h>   /* struct stat, lstat(), S_ISREG, S_ISDIR, S_ISLNK */
#include <unistd.h>     /* Hằng số POSIX */
#include <time.h>       /* ctime(), strftime(), localtime() */
#include <string.h>     /* strlen() */

/* ======================== CONSTANTS ======================== */
#define TIME_BUFFER_SIZE 64   /* Kích thước buffer cho chuỗi thời gian */

/* ======================== FUNCTION PROTOTYPES ======================== */

/**
 * @brief Xác định loại file từ st_mode
 * @param mode Giá trị st_mode từ struct stat
 * @return Chuỗi mô tả loại file ("Regular File", "Directory", etc.)
 */
const char* get_file_type(mode_t mode);

/**
 * @brief Định dạng timestamp thành chuỗi readable
 * @param mtime Giá trị time_t cần chuyển đổi
 * @param buffer Buffer để lưu chuỗi kết quả
 * @param buffer_size Kích thước của buffer
 */
void format_time(time_t mtime, char *buffer, size_t buffer_size);

/**
 * @brief In thông tin metadata của file
 * @param filepath Đường dẫn file
 * @param file_stat Con trỏ đến struct stat chứa thông tin file
 */
void print_file_info(const char *filepath, const struct stat *file_stat);

#endif /* FILESTAT_H */
