#include "config.h"



struct Config parse_config(char *file_name)
{

  toml_result_t config_file = toml_parse_file_ex(file_name);
  toml_datum_t listen = toml_seek(config_file.toptab, "server.listen");
  toml_datum_t port = toml_seek(config_file.toptab, "server.port");
  toml_datum_t sites = toml_seek(config_file.toptab, "sites");
  
  struct Config config;

  strcpy(config.server.ipv4, listen.u.s);
  config.server.port = port.u.int64;

  toml_datum_t site[sites.u.arr.size]; 
  config.sites.sites_size = 0;

  for(int i = 0; i < sites.u.arr.size; i++)
  {
    site[i] = sites.u.arr.elem[i];
    config.sites.sites_size++;
  }

  for(int i = 0; i < sites.u.arr.size; i++)
  { 
    strcpy(config.sites.site[i].host, site[i].u.tab.value[HOST_INDEX].u.s);
    strcpy(config.sites.site[i].root, site[i].u.tab.value[ROOT_INDEX].u.s);
  }

  
  toml_free(config_file);
  return config;

}
