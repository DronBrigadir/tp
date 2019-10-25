#ifndef TASK_UTILS_H
#define TASK_UTILS_H

#define BUF_SIZE_BYTE 100 * 1024 * 1024

#define DEFAULT           0
#define ERR_MEM_ALLOC    -1
#define CANT_OPEN_FILE   -2
#define ERR_CONTENT_FILE -3

int read_from_file(int **buf, int *size, char *file_path);

#endif //TASK_UTILS_H
