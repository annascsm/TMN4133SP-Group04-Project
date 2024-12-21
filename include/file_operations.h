#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

void create_or_open_file(const char *filename);
void change_file_permissions(const char *filename, const char *permissions);
void read_file(const char *filename);
void write_file(const char *filename);
void delete_file(const char *filename);

#endif

