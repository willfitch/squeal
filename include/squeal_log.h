#ifndef HAVE_SQUEAL_LOG_H
#define HAVE_SQUEAL_LOG_H

#define L_FATAL 0
#define L_CRIT 1
#define L_ERROR 2
#define L_WARN 3
#define L_NOTICE 4
#define L_INFO 5
#define L_DEBUG 6
#define L_TRACE 7
#define L_TRACEV 8

#define MAX_LOG_LEN 4096
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef ENABLE_DEBUG

#define LOG_V(_level, ...) do {                                   \
    squeal_log(_level, __FILENAME__, __LINE__, __VA_ARGS__);      \
} while (0)

#define LOG_NOTICE(...) do {                                      \
    squeal_log(L_NOTICE, __FILENAME__, __LINE__, __VA_ARGS__);    \
} while (0)

#define LOG_INFO(...) do {                                        \
    squeal_log(L_INFO, __FILENAME__, __LINE__, __VA_ARGS__);      \
} while (0)

#define LOG_DEBUG(...) do {                                       \
    squeal_log(L_DEBUG, __FILENAME__, __LINE__, __VA_ARGS__);     \
} while (0)

#define LOG_TRACE(...) do {                                       \
    squeal_log(L_TRACE, __FILENAME__, __LINE__, __VA_ARGS__);     \
} while (0)

#define LOG_TRACEV(...) do {                                      \
    squeal_log(L_TRACEV, __FILENAME__, __LINE__, __VA_ARGS__);    \
} while (0)

#else

#define LOG_V(_level, ...)
#define LOG_NOTICE(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_TRACE(...)
#define LOG_TRACEV(...)

#endif

#define LOG_FATAL(...) do {                                 \
    squeal_log(L_FATAL, __FILENAME__, __LINE__, __VA_ARGS__); \
} while (0)

#define LOG_CRIT(...) do {                                  \
    squeal_log(L_CRIT, __FILENAME__, __LINE__, __VA_ARGS__);  \
} while (0)

#define LOG_ERROR(...) do {                                 \
    squeal_log(L_ERROR, __FILENAME__, __LINE__, __VA_ARGS__); \
} while (0)

#define LOG_WARN(...) do {                                  \
    squeal_log(L_WARN, __FILENAME__, __LINE__, __VA_ARGS__);  \
} while (0)


typedef struct {
    const char *filename;
    int level;
    int descriptor;
} log_config;

int log_init(int level, char *file);
void log_release();
int is_loggable(int level);
void squeal_log(int level, const char *file, int line, const char *format, ...);

#endif