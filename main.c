#include "server.h"
#include <getopt.h>
#include <stdio.h>

enum 
{
  OPT_CONFIG = 1,
  OPT_HELP
};

int main(int argc, char *argv[])
{
  if (argc == 1) {
    fprintf(stderr, "Error: no arguments provided\n");
    fprintf(stderr, "Usage: %s --config=<file.toml>\n", argv[0]);
    return EXIT_FAILURE;
  }

  struct option long_options[] =
  {
    {"config=", required_argument, 0, OPT_CONFIG},
    {"help"   , no_argument      , 0, OPT_HELP}
  };

  int option;
  
  while((option = getopt_long(argc, argv, "", long_options, NULL)) != -1)
  {
    switch(option)
    {
      case OPT_CONFIG:
        int is_success = start_server(optarg);
        return is_success;
        break;
      case OPT_HELP:
        printf("Usage: %s --config=<file.toml>\n", argv[0]);
        break;
      case '?':
        fprintf(stderr, "Unkown or invalid option\n");
        printf("Usage: %s --config=<file.toml>\n", argv[0]);
        return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}




