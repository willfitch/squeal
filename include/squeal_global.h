#ifndef HAVE_SQUEAL_GLOBAL_H
#define HAVE_SQUEAL_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <squeal_types.h>
#include <squeal_string.h>

squeal_server_info_t *squeal_server_info_init(char *addr, uint16_t port);
void squeal_server_info_free(squeal_server_info_t *server_info);



#endif
