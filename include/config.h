#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <tomlc17.h>
#include <linux/limits.h>

#define HOST_INDEX 0
#define ROOT_INDEX 1

#define SITES_MAX 8
#define IPV4_MAX_LEN 16
#define MAX_CONTENT 16
#define BUF_SIZE 560

typedef struct Site
{
  char host[PATH_MAX];
  char root[PATH_MAX];
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

#endif // CONFIG_H
