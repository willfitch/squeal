#ifndef HAVE_SQUEAL_RESULT_H
#define HAVE_SQUEAL_RESULT_H

#include <stdlib.h>
#include <squeal_types.h>

struct _squeal_column {
    SquealString *catalog;
    SquealString *db;
    SquealString *table;
    SquealString *orig_table;
    SquealString *name;
    SquealString *orig_name;
    SquealString *charset;
    uint16_t size;
    uint16_t type;
};

struct _squeal_row {

};

struct _squeal_result {
    uint16_t column_count;
    uint32_t row_count;

    ResultRow *row[1]; /* struct hack. must be last field!! */
};

#endif