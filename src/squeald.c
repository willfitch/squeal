/*#include "squeald.h"
#include "libdrizzle/drizzle_server.h"

int main()
{
    drizzle_return_t ret;
    printf("Good to go\n");
}*/
/*
 * Drizzle Client & Protocol Library
 *
 * Copyright (C) 2008 Eric Day (eday@oddments.org)
 * All rights reserved.
 *
 * Use and distribution licensed under the BSD license.  See
 * the COPYING file in this directory for full text.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <libdrizzle/drizzle_server.h>
#include <squeal.h>
#include <squeal_threads.h>

#define MAX_LINE 16384
#define DEFAULT_MAX_CONNECTIONS 200
#define DEFAULT_THREAD_COUNT 4

#define INI_FILE "/Users/wfitch/projects/csqueal/scripts/squeal.ini"

typedef struct {
    ThreadPool *pool;
    ServerInfo server_info; /* Server information */
    SquealConfig *config;
    SquealStats stats;
    Client *client[1]; /* Client connections */
} SquealServer;


SquealServer *server;


void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);
static void initialize_server(SquealConfig *config);
static squeal_always_inline int inc_and_check_connections();
void sig_callback(int signal);

void readcb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input, *output;
    char *line;
    size_t n;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);

    while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
        evbuffer_add(output, line, n);
        evbuffer_add(output, "\n", 1);
        free(line);
    }

    if (evbuffer_get_length(input) >= MAX_LINE) {
        /* Too long; just process what there is and go on so that the buffer
         * doesn't grow infinitely long. */
        char buf[1024];

        while (evbuffer_get_length(input)) {
            n = evbuffer_remove(input, buf, sizeof(buf));
            evbuffer_add(output, buf, n);
        }

        evbuffer_add(output, "\n", 1);
    }

    evbuffer_add(output, "whats up\n", 9);
}

void errorcb(struct bufferevent *bev, short error, void *ctx)
{
    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(bev);
}

void on_accept(evutil_socket_t listener, short event, void *arg)
{

    /* if we've hit max connections, refuse the connection */
    if (!inc_and_check_connections()) {
        /* @todo: refuse the connection */
        close(listener);
        return;
    }

    struct event_base *base = arg;
    struct sockaddr_storage ss;
    /*drizzle_con_st *connection = NULL;*/

    socklen_t slen = sizeof(ss);
    printf("accepting\n");
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd);
    } else {
        struct bufferevent *bev;
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
        bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

static squeal_always_inline int inc_and_check_connections()
{
    int is_safe = 1;
    pthread_mutex_lock(&server->stats.connection.lock);
    server->stats.connection.current++;

    if (server->stats.connection.current > server->server_info.max_connections) {
        is_safe = 0;
        server->stats.connection.current--;
    }

    if (is_safe) {
        server->stats.connection.total_served++;
    }

    pthread_mutex_unlock(&server->stats.connection.lock);

    return is_safe;
}

static void initialize_server(SquealConfig *config)
{
    unsigned int port = 0;
    unsigned int thread_count = 0;
    uint32_t max_connections = 0;

    S_CONFIG_INT(config, "squeald", "port", &port);

    if (port < 1) {
        port = 3306;
    }

    S_CONFIG_INT(config, "squeald", "max_connections", &max_connections);

    if (max_connections < 1) {
        max_connections = DEFAULT_MAX_CONNECTIONS;
    }

    S_CONFIG_INT(config, "squeald", "io_thread_count", &thread_count);

    if (thread_count < 1) {
        thread_count = DEFAULT_THREAD_COUNT;
    }

    server = malloc(sizeof(SquealServer) + sizeof(Client) * max_connections);

    if (server == NULL) {
        fprintf(stderr, "main: unable to allocate server");
        exit(EXIT_FAILURE);
    }

    server->server_info.max_connections = max_connections;
    server->server_info.port = port;
    server->server_info.io_thread_count = thread_count;
    server->pool = squeal_tp_init(thread_count);

    if (server->pool == NULL) {
        exit(EXIT_FAILURE);
    }

    server->config = config;
    server->stats.threads_alive = &server->pool->total_threads_alive;
    server->stats.threads_working = &server->pool->total_threads_working;
    server->stats.start_time = (unsigned) time(NULL);


    sigset_t sigset;
    sigemptyset(&sigset);
    struct sigaction siginfo = {
            .sa_handler = sig_callback,
            .sa_mask = sigset,
            .sa_flags = SA_RESTART,
    };

    sigaction(SIGINT, &siginfo, NULL);
    sigaction(SIGTERM, &siginfo, NULL);
}

static void kill_squeal()
{
    LOG_TRACE("Releasing networking");

    /*if (event_base_loopexit(evbase_accept, NULL)) {
        perror("Error shutting down server");
    }*/

    LOG_TRACE("Shutting down IO threads");
    squeal_tp_destroy(server->pool);

    LOG_TRACE("Freeing all resources");
    squeal_config_free(server->config);

    LOG_CRIT("Shutting down Squeal");
    log_release();
}

void sig_callback(int signal)
{
    fprintf(stdout, "Shutting down SQueaL due to signal %d: %s\n", signal, strsignal(signal));
    kill_squeal();
    exit(signal);
}

int main(int argc, char *argv[])
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event *listener_event;
    log_init(L_TRACEV, "./poopymonster.txt");
    SquealConfig *config = squeal_config_init(INI_FILE);

    LOG_WARN("Unable to poop");
    initialize_server(config);

    base = event_base_new();

    if (!base)
        return EXIT_FAILURE;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(server->server_info.port);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(listener, server->server_info.max_connections ) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    listener_event = event_new(base, listener, EV_READ | EV_PERSIST, on_accept, (void *) base);
    event_add(listener_event, NULL);

    event_base_dispatch(base);
}