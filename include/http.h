#ifndef HTTP_H
#define HTTP_H

enum HTTP_METHODS
{
  GET,
  POST,
  PUT,
  DELETE
};

enum status_code
{
  OK,
  NOT_FOUND
};

enum HTTP_METHODS parse_http_method(char *method);

#endif // HTTP_H
