#ifndef HAVE_LME_GLOBAL_H
#define HAVE_LME_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include "lme_types.h"
#include "lme_string.h"

lme_server_info_t *lme_server_info_init(char *addr, uint16_t port);
void lme_server_info_free(lme_server_info_t *server_info);

#endif
