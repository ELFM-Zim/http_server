#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>
#include <stdio.h>
#include <tomlc17.h>
#include <string.h>

#define HOST_INDEX 0
#define ROOT_INDEX 1


#define SITES_PATH_MAX_LEN 256
#define SITES_MAX 8
#define IPV4_MAX_LEN 16

typedef struct Site 
{
  char host[SITES_PATH_MAX_LEN];
  char root[SITES_PATH_MAX_LEN];
} Site;


struct Config 
{
  struct 
  {
    char ipv4[IPV4_MAX_LEN];
    int64_t port;
  } server;
  struct
  {
      int sites_size;
      Site site[SITES_MAX];
  } sites;
};


struct Config parse_config(char *file_name);

#endif //CONFIGURATION_H
