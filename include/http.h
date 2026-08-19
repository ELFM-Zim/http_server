#ifndef UTILS_H
#define UTILS_H

#include <string.h>

enum HTTP_METHODS
{
  Get,
  Put,
  Head,
  Post,
  Delete,
};

const int string_to_method(char *string);

enum HTTP_METHODS parse_http_method(char *method_string);


#endif //UITLS_H
