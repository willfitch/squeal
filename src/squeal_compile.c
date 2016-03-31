#include "squeal_compile.h"

Select *squeal_compile_select(
        SqlStatement *statement,
        Column *columns,
        Table *tables,
        Where *where,
        GroupBy *group_by,
        OrderBy *order_by,
        uint32_t limit,
        uint32_t offset
)
{
    Select *select = sql_select_init();

    if (!select) {
        return NULL;
    }

    select->columns = columns;
    select->tables = tables;
    select->where = where;
    select->group = group_by;
    select->limit = limit;
    select->offset = offset;
    select->order = order_by;

    return select;
}