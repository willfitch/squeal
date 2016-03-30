#ifndef HAVE_SQUEAL_H
#define HAVE_SQUEAL_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <squeal_types.h>
#include <squeal_sval.h>
#include <squeal_string.h>

/* SELECT statement */
Select *sql_select_init();

/* SQL statement */
SqlStatement *sql_statement_init();

/* ORDER BY */
OrderBy *order_by_init();

/* GROUP BY */
GroupBy *group_by_init();

/* columns */
Column *column_init();

/* WHERE clause */
Where *where_init();


#endif
