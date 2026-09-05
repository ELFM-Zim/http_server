#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>

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

char** get_directory_files(char* directory_path)
{
  char **files_path; 
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

    char new_path[PATH_MAX];
    snprintf(new_path, sizeof(new_path), "%s/%s", directory_path, directory_content->d_name);
    
    struct stat stat_buf;
    stat(new_path, &stat_buf);
    if(S_ISDIR(stat_buf.st_mode)) 
    {
      char ** recursive_return = get_directory_files(new_path); 
      
    }
    if(S_ISREG(stat_buf.st_mode))
    {
      memcpy(*files_path, new_path, strlen(new_path); 
    }
  }
  if(errno != 0)
  {
    perror("Error");
    exit(1);
  }
  return files_path;
  closedir(open_directory);
}
