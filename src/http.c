#include "http.h"
#include <string.h>

#define GET 1
#define PUT 2
#define HEAD 3
#define POST 4
#define DELETE 5 

const int string_to_method(char *string)
{
  if(strcmp(string, "GET") == 0)
  {
    return GET;
  }
  else if(strcmp(string, "PUT") == 0)
  {
    return PUT;
  }
  else if(strcmp(string, "HEAD") == 0)
  {
    return HEAD;
  }
  else if(strcmp(string, "POST") == 0)
  {
    return POST;
  }
  else if(strcmp(string, "DELETE") == 0)
  {
    return DELETE;
  }

}

enum HTTP_METHODS parse_http_method(char *method_string)
{
  int method = string_to_method(method_string);
  switch(method)
  {
    case GET: 
      return GET;
    case PUT: 
      return PUT;
    case HEAD:
      return HEAD;
    case POST:
      return POST;
    case DELETE:
      return DELETE;
  }
}
