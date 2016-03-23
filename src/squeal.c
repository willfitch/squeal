#include <squeal_global.h>
#include <netinet/in.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <assert.h>

squeal_server_info_t *squeal_server_info_init(char *addr, uint16_t port)
{
    squeal_server_info_t *info;

    assert(addr != NULL);

    info = (squeal_server_info_t *) malloc(sizeof(squeal_server_info_t));
    info->address = squeal_string_init(addr, sizeof(addr));
    info->port = port;
    return info;
}

void squeal_server_info_free(squeal_server_info_t *server_info)
{
    if (server_info) {
        squeal_string_free(server_info->address);
        free(server_info);
    }
}




