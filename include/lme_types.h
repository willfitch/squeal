#ifndef HAVE_LME_TYPES_H
#define HAVE_LME_TYPES_H

typedef struct _lme_string lme_string;
typedef struct _lme_server_info lme_server_info_t;

struct _lme_string {
    size_t len;
    char val[1];
};

struct _lme_server_info {
    uint16_t port;
    lme_string *address;
};

#endif
