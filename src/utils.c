#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

char* get_status_code(enum status_code status_code)
{
  if(status_code      == OK) return "200 OK";
  else if(status_code == NOT_FOUND) return "404 Not Found";
}

char* get_content_type(char* path)
{
  
  struct content_type
  {
    char* extension;
    char* MIME;
  };
  printf("PATH: %s\n", path);
  struct content_type ct_list[] = {
    {".html" , "text/html"},
    {".htm"  , "text/html"},
    {".txt"  , "text/plain"},
    {".css"  , "text/css"},
    {".js"   , "text/javascript"},
    {".json" , "application/json"},
    {".xml"  , "application/xml"}
  };

  int ct_len = sizeof(ct_list) / sizeof(ct_list[0]);

  char* extension = strrchr(path + 1, '.');
  printf("EXTENSION: %s\n", extension);
  for(int i = 0; i < ct_len; i++)
  {
      printf("Extension: %s comparada com: %s\n", extension, ct_list[i].extension);
      if(strcmp(extension, ct_list[i].extension) == 0)
      { 
        return ct_list[i].MIME;
      }
  }
  
  return "application/octet-stream";

}

void get_directory_files(char current_request_content[MAX_CONTENT][SITES_PATH_MAX_LEN], char* directory_path)
{
  DIR *open_directory = opendir(directory_path);
  errno = 0;
  struct dirent *directory_content;

  int i = 0;
  while((directory_content = readdir(open_directory)) != NULL)
  {
    if(strcmp(directory_content->d_name, ".") == 0 || strcmp(directory_content->d_name, "..") == 0) 
    {
      continue;
    }
    else
    {
      printf("Copying %s", directory_content->d_name);
      strcpy(current_request_content[i], directory_content->d_name);
      printf(" to %s\n", current_request_content[i]);
      i++;
    }
  }
  if(errno != 0)
  {
    perror("Error");
    exit(1);
  }
  closedir(open_directory);
}
