#include "lme_global.h"
#include <assert.h>

lme_server_info_t *lme_server_info_init(char *addr, uint16_t port)
{
    lme_server_info_t *info;

    assert(addr != NULL);

    info = (lme_server_info_t *) malloc(sizeof(lme_server_info_t));
    info->address = lme_string_init(addr, sizeof(addr));
    return info;
}

void lme_server_info_free(lme_server_info_t *server_info)
{
    if (server_info) {
        lme_string_free(server_info->address);

        free(server_info);
    }
}