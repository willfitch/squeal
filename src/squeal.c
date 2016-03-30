#include <squeal.h>
#include <squeal_hashtable.h>
#include <squeal_function.h>

/* SQL statement */
SqlStatement *sql_statement_init()
{
    SqlStatement *statement = (SqlStatement *) malloc(sizeof(SqlStatement));

    if (statement == NULL) {
        fprintf(stderr, "sql_statement_init: unable to allocate sqlstatement");
        return NULL;
    }

    statement->raw_query = NULL;
    statement->select_stmt = NULL;

    return statement;
}

void sql_statement_free(SqlStatement *statement)
{
    if (statement) {
        if (statement->raw_query) {
            squeal_string_free(statement->raw_query);
        }

        if (statement->select_stmt) {
            sql_select_free(statement->select_stmt);
        }

        free(statement);
    }
}

/* SELECT statement */
Select *sql_select_init()
{
    Select *select = (Select *) malloc(sizeof(Select));

    if (select == NULL) {
        fprintf(stderr, "sql_select_init: unable to allocate select");
        return NULL;
    }

    select->columns = NULL;
    select->group = NULL;
    select->order = NULL;
    select->where = NULL;
    select->tables = NULL;

    return select;
}

void sql_select_free(Select *select)
{
    if (select) {
        if (select->columns) {
            sql_column_free(select->columns);
        }

        if (select->group) {
            sql_group_by_free(select->group);
        }

        if (select->order) {
            sql_order_by_free(select->order);
        }

        if (select->tables) {
            sql_table_free(select->tables);
        }

        if (select->where) {
            sql_where_free(select->where);
        }

        free(select);
    }
}

Table *sql_table_init()
{
    Table *table = (Table *) malloc(sizeof(Table));

    if (table == NULL) {
        fprintf(stderr, "sql_table_init: unable to allocate");
        return NULL;
    }

    table->alias = NULL;
    table->name = NULL;
    table->next = NULL;
    table->prev = NULL;

    return table;
}

void sql_table_free(Table *table)
{
    if (table) {
        if (table->alias) {
            squeal_string_free(table->alias);
        }

        if (table->name) {
            squeal_string_free(table->name);
        }

        if (table->next) {
            sql_table_free(table->next);
        }

        free(table);
    }
}

/* ORDER BY */
OrderBy *sql_order_by_init()
{
    OrderBy *order = (OrderBy *) malloc(sizeof(OrderBy));

    if (order == NULL) {
        fprintf(stderr, "sql_order_by_init: unable to allocate");
        return NULL;
    }

    order->columns = NULL;

    return order;
}

void sql_order_by_free(OrderBy *order)
{
    if (order) {
        if (order->columns) {
            sql_column_free(order->columns);
        }

        free(order);
    }
}

/* GROUP BY */
GroupBy *sql_group_by_init()
{
    GroupBy *group = (GroupBy *) malloc(sizeof(GroupBy));

    if (group == NULL) {
        fprintf(stderr, "sql_group_by_init: unable to allocate");
        return NULL;
    }

    group->columns = NULL;

    return group;
}

void sql_group_by_free(GroupBy *group)
{
    if (group) {
        if (group->columns) {
            sql_column_free(group->columns);
        }

        free(group);
    }
}

/* columns */
Column *sql_column_init()
{
    Column *column = (Column *) malloc(sizeof(Column));

    if (column == NULL) {
        fprintf(stderr, "sql_column_init: unable to allocate column");
        return NULL;
    }

    column->name = NULL;
    column->alias = NULL;
    column->next = NULL;
    column->prev = NULL;
    column->v.function = NULL;
    column->v.sval = NULL;

    return column;
}

void sql_column_free(Column *column)
{
    if (column) {
        if (column->name) {
            squeal_string_free(column->name);
        }

        if (column->alias) {
            squeal_string_free(column->alias);
        }

        if (column->next) {
            sql_column_free(column->next);
        }

        if (column->v.sval) {
            squeal_sval_free(column->v.sval);
        }

        if (column->v.function) {
            sql_function_free(column->v.function);
        }

        free(column);
    }
}

/* WHERE clause */
Where *sql_where_init()
{
    Where *where = (Where *) malloc(sizeof(Where));

    if (where == NULL) {
        fprintf(stderr, "sql_where_init: unable to allocate where");
        return NULL;
    }

    where->child = NULL;
    where->left = NULL;
    where->right = NULL;
    where->prev = NULL;
    where->next = NULL;
    where->values = NULL;

    return where;
}

void sql_where_free(Where *where)
{
    if (where) {
        sql_where_free(where->child);

        if (where->left) {
            squeal_sval_free(where->left);
        }

        if (where->right) {
            squeal_sval_free(where->right);
        }

        if (where->next) {
            sql_where_free(where->next);
        }

        if (where->values) {
            squeal_ht_free(where->values);
        }

        free(where);
    }
}

ServerInfo *squeal_server_info_init(char *addr, uint16_t port)
{
    ServerInfo *info;

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
