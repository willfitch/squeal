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
    ResultColumn *next;
};

struct _squeal_row {

    ResultRow *next;
};

struct _squeal_result {
    uint16_t column_count;
    uint32_t row_count;
    ResultColumn *columns;

    ResultRow *rows;
};

ResultColumn *sql_result_column_init();
void sql_result_column_free(ResultColumn *column);

ResultRow *sql_result_row_init();
void sql_result_row_free(ResultRow *row);

Result *sql_result_init();
void sql_result_free(Result *result);

#endif