#include <squeal.h>
#include <assert.h>

ServerInfo *squeal_server_info_init(char *addr, uint16_t port)
{
    ServerInfo *info;

    assert(addr != NULL);

    info = (ServerInfo *) malloc(sizeof(ServerInfo));
    info->address = squeal_string_init(addr, sizeof(addr));
    info->port = port;
    return info;
}

void squeal_server_info_free(ServerInfo *server_info)
{
    if (server_info) {
        squeal_string_free(server_info->address);
        free(server_info);
    }
}
