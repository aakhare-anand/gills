#ifndef __GILLSGEN_H__
#define __GILLSGEN_H__

#include "gillsfileop.h"

#define GILLSGEN_FREAD_BUF_SIZE     4096
#define GILLSGEN_FREAD_BUF_MIN_SIZE 1024

#define TYPE_DEF_STR    "union-directive"
#define TYPE_UNION_STR  "union"

#define UNION_DIRECTIVE       "union-directive"
#define UNION                 "union"
#define SYMBOL_DEF_DATATYPE   "int"
#define NULLFUNCNAME          "NULL"

union gillsgentoken {
    char *str;
};

typedef union gillsgentoken GILLSGENYYSTYPE;

#define GILLSGENYYSTYPE YYSTYPE

enum symbol_assoc_ {
    ASSOC_NONE = 0,
    ASSOC_LEFT,
    ASSOC_RIGHT
} symbol_assoc_e;

typedef struct symbol_node_ {
    char *symname;
    int id;
    int assoc;
    int prec;
    int rules_num;
    char *datatypefield;
    int datatypeid;
    int union_field_id;
    list_t *rules_list;
//    int *rules_prec;
} symbol_node_t;

typedef struct gills_union_field_ {
    char *fieldtype;
    char *fieldname;
    int id;
} gills_union_field_t;

typedef struct gills_union_ {
//    char *uniontag;
    list_t *fieldlist;
    gills_union_field_t *fields;
    int fields_num;
    int total_len;
}   gills_union_t;

#define VALUETYPE_STR_DEF_SIZE    1024
#define YYLVAL_DEF                "yylval"
#define YYSTYPE_DEF               "YYSTYPE"
#define ACT_FUNC_NAME_DEF         "gillsfunction"

typedef struct gillsgen_ {
    gills_context_t *gills;
    gills_fread_t gfread;
    gills_fwrite_t gfwrite;
    list_t *sym_list;
    int syms_num;
    list_t *symtype_list;
    int symtypes_num;
    char *symprefix;
    int leftassoc_prec_max;
    int rightassoc_prec_max;
    int prec_max;
 //   list_t *symassocleft;
 //   list_t *symassocright;
    list_t *symprec;
 //   char *symtypestr;
    gills_union_t *union;
    char *uniontag;
    char *valuetype;
    char *apiprefix;
    char *yylvalname;
    char *yystypename;
    char *yystypestr;
    char *yylvalstr;
    symbol_node_t *cur_symnode;
    symrule_t *cur_symrule;
    char *act_funcname;
    int act_funcnum;
} gillsgen_t;

typedef struct symrule_ {
    list_t *symbols;
    int syms_num;
    int assoc;
    int prec;
    char *symrulename;
    char *act_funcname;
} symrule_t;

/*
typedef struct gillsgen_symbol_ {
    char *symname;
    char *typestr;
    int id;
    int rules_num;
    list_t *symrules_list;
    int prec;
} gillsgen_symbol_t;
*/
#endif  /* __GILLSGEN_H__ */
