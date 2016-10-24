#include <fcntl.h>
#include <squeal.h>
#include <sys/time.h>

static log_config logger;

const char *log_labels[9] = {
        "[fatal]",
        "[critical]",
        "[error]",
        "[warn]",
        "[notice]",
        "[info]",
        "[debug]",
        "[trace]",
        "[tracev]"
};

int log_init(int level, char *file)
{
    if (file == NULL || strncmp(file, "stderr", sizeof("stderr")) == 0) {
        logger.descriptor = STDERR_FILENO;
        logger.filename = NULL;
    } else if (strncmp(file, "stdout", sizeof("stderr")) == 0) {
        logger.descriptor = STDOUT_FILENO;
        logger.filename = NULL;
    } else {
        logger.descriptor = open(file, O_WRONLY | O_APPEND | O_CREAT, 0600);

        if (logger.descriptor < 0) {
            fprintf(stderr, "Unable to open or write to log file %s\n", file);
            return FAILURE;
        }
    }

    logger.level = MAX(L_FATAL, MIN(level, L_TRACEV));

    return SUCCESS;
}

void log_release()
{
    if (logger.descriptor < 0 || logger.descriptor == STDOUT_FILENO
        || logger.descriptor == STDERR_FILENO) {
        return;
    }

    close(logger.descriptor);
}

int is_loggable(int level)
{
    return (level > logger.level) ? FAILURE : SUCCESS;
}

void squeal_log(int level, const char *file, int line, const char *format, ...)
{
    va_list args;
    int len = 0;
    struct timeval tv;
    char buffer[MAX_LOG_LEN];
    int size = MAX_LOG_LEN;

    if (is_loggable(level) != SUCCESS) {
        return;
    }

    gettimeofday(&tv, NULL);
    buffer[len++] = '[';
    len += strftime(buffer + len, (size_t) (size - len), "%Y-%m-%d %H:%M:%S.", localtime(&tv.tv_sec));
    len += snprintf(buffer + len, (size - len), "%03ld]", (long) tv.tv_usec/1000);
    len += snprintf(buffer + len, (size - len), "%s", log_labels[level]);
    len += snprintf(buffer + len, (size - len), "[%s:%d] - ", file, line);

    va_start(args, format);
    len += vsnprintf(buffer + len, size - len, format, args);
    va_end(args);

    buffer[len++] = '\n';

    write(logger.descriptor, buffer, (size_t) len);
}
