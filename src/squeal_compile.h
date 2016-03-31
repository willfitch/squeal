#ifndef HAVE_SQUEAL_COMPILE_H
#define HAVE_SQUEAL_COMPILE_H

#include <squeal.h>


Select *squeal_compile_select(
    SqlStatement *statement,
    Column *columns,
    Table *tables,
    Where *where,
    GroupBy *group_by,
    OrderBy *order_by,
    uint32_t limit,
    uint32_t offset
);

#endif