#include <squeal.h>
#include <squeal_result.h>

ResultColumn *sql_result_column_init()
{
    ResultColumn *column = (ResultColumn *) malloc(sizeof(ResultColumn));

    if (column == NULL) {
        fprintf(stderr, "sql_result_column_init: unable to allocate");
        return NULL;
    }

    column->catalog = NULL;
    column->charset = NULL;
    column->db = NULL;
    column->name = NULL;
    column->next = NULL;
    column->orig_name = NULL;
    column->orig_table = NULL;
    column->table = NULL;

    return column;
}

void sql_result_column_free(ResultColumn *column)
{
    if (column->catalog) {
        squeal_string_free(column->catalog);
    }

    if (column->charset) {
        squeal_string_free(column->charset);
    }

    if (column->db) {
        squeal_string_free(column->db);
    }

    if (column->name) {
        squeal_string_free(column->name);
    }

    if (column->next) {
        sql_result_column_free(column->next);
    }

    if (column->orig_name) {
        squeal_string_free(column->orig_name);
    }

    if (column->orig_table) {
        squeal_string_free(column->orig_table);
    }

    if (column->table) {
        squeal_string_free(column->table);
    }

    free(column);
}

ResultRow *sql_result_row_init()
{
    ResultRow *row = (ResultRow *) malloc(sizeof(ResultRow));

    if (row == NULL) {
        fprintf(stderr, "sql_result_row_init: unable to allocate");
        return NULL;
    }

    row->next = NULL;

    return row;
}

void sql_result_row_free(ResultRow *row)
{
    if (row) {
        if (row->next) {
            sql_result_row_free(row->next);
        }

        free(row);
    }
}

Result *sql_result_init()
{
    Result *result = (Result *) malloc(sizeof(Result));

    if (result == NULL) {
        fprintf(stderr, "sql_result_init: unable to allocate");
        return NULL;
    }

    result->columns = NULL;
    result->rows = NULL;
    result->row_count = 0;
    result->column_count = 0;
    return result;
}

void sql_result_free(Result *result)
{
    if (result) {
        if (result->rows) {
            sql_result_row_free(result->rows);
        }

        if (result->columns) {
            sql_result_column_free(result->columns);
        }

        free(result);
    }
}