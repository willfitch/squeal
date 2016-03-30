#ifndef HAVE_SQUEAL_TYPES_H
#define HAVE_SQUEAL_TYPES_H

#define SQUEAL_IPV4 (1 << 1)
#define SQUEAL_IPV6 (1 << 2)

#define SVAL_TYPE_STR (1 << 1)
#define SVAL_TYPE_DOUBLE (1 << 2)
#define SVAL_TYPE_INT (1 << 3)

#define ORDER_BY_ASC (1 << 1)
#define ORDER_BY_DESC (1 << 2)

#if defined(__GNUC__)
#   if __GNUC__ >= 3
#       define squeal_always_inline inline __attribute__((always_inline))
#   else
#       define squeal_always_inline inline
#   endif
#else
#   define squeal_always_inline inline
#endif


typedef struct _squeal_string SquealString;
typedef struct _squeal_hashtable HashTable;
typedef struct _squeal_val sval;
typedef struct _where Where;
typedef struct _parameter Parameter;
typedef struct _sql_function SqlFunction;
typedef struct _order_by OrderBy;
typedef struct _column Column;
typedef struct _table Table;
typedef struct _sql_hint SqlHint;
typedef struct _group_by GroupBy;

typedef enum {
    TYPE_DOUBLE,
    TYPE_INT,
    TYPE_STRING,
    TYPE_FIELD
} SqlType;

typedef enum {
    OP_WHERE_AND,
    OP_WHERE_OR,
    OP_WHERE_COMPARE_EQ,
    OP_WHERE_COMPARE_NEQ,
    OP_WHERE_COMPARE_LT,
    OP_WHERE_COMPARE_LTE,
    OP_WHERE_COMPARE_GT,
    OP_WHERE_COMPARE_GTE,
    OP_WHERE_NULL,
    OP_WHERE_NOT_NULL,
    OP_WHERE_TRUE,
    OP_WHERE_FALSE,
    OP_WHERE_IN,
    OP_WHERE_NOT_IN
} WhereOp;

typedef enum {
    STMT_TYPE_SELECT
} StatementType;


typedef struct {
    unsigned short port; /* max 65,535 */
    SquealString *address;
} ServerInfo;

struct _squeal_val {
    union {
        struct {
            SquealString *strval;
            double dval;
            int ival;
        } v;

        short type;
    } val;
};

struct _column {
    union {
        SqlFunction *function;
        sval *sval;
    } v;

    SquealString *alias;
    SqlType val_type;
    SquealString *name;
};

struct _where {
    sval *left; /* left operand */
    SqlType left_type; /* left operand type */

    sval *right; /* right operand */
    SqlType right_type; /* right operand type */

    Where *next; /* next operation */
    Where *prev; /* previous operation */
    Where *child; /* child nodes */

    WhereOp currop; /* current operation (e.g. IN/BOOL) */
    WhereOp nextop; /* next operation (e.g. AND/OR) */

    HashTable *values; /* HashTable of svals for in clauses */
};

struct _order_by {
    short direction;
    Column *columns[1]; /* struct hack. must be last! */
};

struct _group_by {
    Column *columns[1];
};

struct _table {
    SquealString *name;
    SquealString *alias;
};

typedef struct {
    Where *where;

    Table *tables;
    OrderBy *order;
    GroupBy *group;
    uint32_t limit;
    uint32_t offset;
    Column *columns[1]; /* struct hack. must be last! */
} Select;

typedef struct {
    SquealString *raw_query; /* raw sql string */

    Select *select_stmt; /* SELECT statement */
    StatementType type; /* identification of the statement type */
} SqlStatement;

/*
 * Represents a client connection
 */
typedef struct {
    uint64_t connection_id;
} Client;


#endif
