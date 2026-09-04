#ifndef UTILS_H
#define UTILS_H

#include "http.h"
#include "config.h"

char *get_status_code(enum status_code status_code);
char *get_content_type(char *path);

void get_directory_files(
char current_request_content[MAX_CONTENT][SITES_PATH_MAX_LEN],
char *directory_path
);

#endif // UTILS_H
