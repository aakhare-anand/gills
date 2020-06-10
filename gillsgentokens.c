
token_t preptokens[8] = {
{ "[0-9]", 2 },

 { "{%", 1 },    /* LEFTBRACEPCENT  */
 { "%}", 2 },    /* PCENTRIGHTBRACE */
 { "{", 3 },     /* LEFTBRACE       */
 { "}", 4 },     /* RIGHTBRACE      */
 { "\n",5 },     /* NEWLINE         */
 { "[a-zA-Z0-9_]+", 6 },    /* NAMESTRING    */
 { "[ \t\r\n#<>\.a-zA-Z_+*\\\/\{\[\]\:\;=\-\$\"\`\'\(\)\^]+", 7 },
                 /*    CWORD   */
 { "[ \t]+", 7 },  /* WSPACE          */
 { "\$", 8 },    /*    DOLLAR  */
 { "[0-9]+", 9 },           /* NUM           */
 { "\|", 10 },   /* PIPE       */
 { "\%\%", 11 }, /* DOUBLEPCENT     */
 { "\%", 12 },   /* PCENT           */
 { "\;", 13 },   /* SEMICOLON       */
 { "\:", 14 },   /* COLON           */
 { "\/\/", 15 }, /* SLASHCOMMENTSTART */
 { "{\%", 16},   /* RIGHTBRACEPCENT */
 { "\'", 17 },   /* QUOTE           */
 { "\"", 18 },   /* DOUBLEQUOTE     */
 { "\/\*", 19 }, /* COMMENTSTART    */
 { \*\/", 20 },  /* COMMENTEND      */
 { "union", 21 },/* UNION           */
 { "\<", 22 },   /* LEFTANGLEBRACKT */
 { "\>", 23 },   /* RIGHTANGLEBRACKT */
};

declarations
-------------
%require "version"

%token tokename
%token <type> tokenname in case of union
%token NUM 300
%token XNUM 0x300

%left SYMBOLS
%right SYMBOLS
%precedence SYMBOLS

%type <datatype> nonterm

%initial-action { CODE }
$$ and @$
%parse-param 

%destructor { CODE } symbols and or types or default <*>
%destructor { CODE } <type>
%destructor { CODE } <*>
%destructor { CODE } <>    for token without type

%expect N   expected shift reduce conflicts without warnings
%expect-rr N expected reduce reduce conflicts without warnings

%start symbol

%define api.pure full

%define api.push-pull push or pull or both   default is pull

%define api.namespace { VALUE }

%define api.prefix { prefix }

%define api.pure true, false, full

%define api.token.constructor true, false

%define api.token.prefix { PREFIX }

%define api.value.type SUPPORT or { TYPE }
    union-directive
    union
    variant
    { TYPE }   where type can be nested and any field can be type value for symbol
default union-directive

%define api.value.union.name Name

%define api.location.type

%define lr.default-reduction when
    most, consistent, accepting    default is accepting

%define lr.keep-unreachable-state   true or false, default is false

%define lr.type  lalr, ielr, canonical-lr    default is lalr
   if glr is set?

%define parse.assert

%define parse.error verbosity   simple or verbose  default is simple

%define parse.lac when  none or full   default is none
    only for deterministic parsers

%define parse.trace true or false   default is false

%code qualifier { CODE}
qualifiers are requires, provide, top and imports

%lex-param { params }

%parse-param { params }

%param { params }      params are comma separated



