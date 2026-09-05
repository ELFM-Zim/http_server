#ifndef UTILS_H
#define UTILS_H

#include "http.h"
#include "config.h"
#include <linux/limits.h>

char *get_status_code(enum status_code status_code);
char *get_content_type(char *path);

char** get_directory_files(char *directory_path );

#endif // UTILS_H
