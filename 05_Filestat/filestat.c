/**
 * @file filestat.c
 * @brief Chương trình chính filestat - Trình kiểm tra siêu dữ liệu file
 * 
 * Chương trình này sử dụng system call lstat() để đọc và hiển thị
 * các thông tin metadata của file/thư mục trên hệ thống Linux
 * 
 * Cách sử dụng: ./filestat <file_path>
 * 
 * @author Student
 * @date 2026
 */

#include "filestat.h"

/* ======================== HELPER FUNCTIONS ======================== */

/**
 * @brief In hướng dẫn sử dụng chương trình
 * @param program_name Tên chương trình (argv[0])
 */
static void print_usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s <file_path>\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Description:\n");
    fprintf(stderr, "  Display metadata information of a file or directory.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "  file_path    Path to the file or directory to inspect\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Example:\n");
    fprintf(stderr, "  %s /home/user/document.txt\n", program_name);
    fprintf(stderr, "  %s /etc\n", program_name);
}

/* ======================== MAIN FUNCTION ======================== */

/**
 * @brief Điểm vào chính của chương trình
 * 
 * Workflow:
 * 1. Kiểm tra số lượng tham số dòng lệnh
 * 2. Gọi lstat() để lấy metadata của file
 * 3. In thông tin metadata ra console
 * 
 * @param argc Số lượng tham số dòng lệnh
 * @param argv Mảng các tham số dòng lệnh
 * @return EXIT_SUCCESS nếu thành công, EXIT_FAILURE nếu có lỗi
 */
int main(int argc, char *argv[])
{
    const char *filepath;   /* Đường dẫn file cần kiểm tra */
    struct stat file_stat;  /* Struct để lưu thông tin metadata */
    int result;             /* Kết quả trả về từ lstat() */
    
    /* 
     * Bước 1: Kiểm tra số lượng tham số
     * Chương trình cần chính xác 2 tham số: tên chương trình và đường dẫn file
     */
    if (argc != 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Lấy đường dẫn file từ tham số dòng lệnh */
    filepath = argv[1];
    
    /* 
     * Bước 2: Gọi lstat() để lấy thông tin file
     * 
     * lstat() vs stat():
     * - stat(): Nếu file là symbolic link, sẽ theo dõi link và 
     *           trả về thông tin của file đích
     * - lstat(): Trả về thông tin của chính symbolic link
     * 
     * Sử dụng lstat() để có thể nhận diện được symbolic links
     */
    result = lstat(filepath, &file_stat);
    
    /* Kiểm tra kết quả của lstat() */
    if (result == -1) {
        /* 
         * Nếu lstat() trả về -1, có lỗi xảy ra
         * perror() sẽ in ra thông báo lỗi tương ứng với errno
         */
        perror("Error");
        fprintf(stderr, "Cannot get information for: %s\n", filepath);
        return EXIT_FAILURE;
    }
    
    /* 
     * Bước 3: In thông tin metadata
     * Gọi hàm utility để định dạng và in thông tin
     */
    print_file_info(filepath, &file_stat);
    
    return EXIT_SUCCESS;
}
