Gills is GLR parser generator.
Core component is gills.[ch] files which is to be linked with application as library.
It's by default multi-threading enabled.
To try it out, download all code in a directory and compile with following command:
cc -o gills gills.c simplex.c gillsfileop.c testtokens.c test.c testdata.c
cc is C compiler.
Create a file named "data" and populate it with arithmetric expression for calculator.
eg is 1+2-3*4/5*6
Don't leave any whitespace characters.
Then just run ./gills 
It will read data file and parse it and exit.
gills.[ch], gillsfileop.[ch] are used for parsing and testdata and testtokens and test
can be replaced with application files.
gills_app.h file has to be populated with data type of yylval of tokens given by scanners.
Parsing synax is populated through writing data structure token_read_t(Refer testdata.c file)
gills_tool is in development to automatically generate this data structure from yacc/bison compatible
syntax (.y) files.


Code flow for using parser is given below (Same can be understood by referring test.c file) : 

read_parse() call which will read the parsing data in the form of array of token_read_t and generate and soak in parsing grammar.
int read_parse (gills_context_t *gills,
                token_read_t *tkread, int tokens_num,
                char **tkleft_assoc, int tkleftassoc_num,
                char **tkright_assoc, int tkrtassoc_num,
                char **tkprec, int tkprec_num)


It will initialise parsing instance on gills_context_t which is to be passed further down the parsing calls. Every parsing tree will have its own context.

Next call parse() will start parsing.

int parse (gills_context_t *gills, token_node_t *starttoken)

parse() will in turn call get_token() which is input call for input tokens to be read one by one.
get_token() has to be implemented by application.
token_read_t is array based data for input parsing grammar.
gillsgen tool is getting developed to automate generation of token_read_t data from grammar file.
Grammar file is typically same as how yacc/bison grammar files are. Below is sample provided for this.

input: token1

token1 : token2 token3
         | token4 token 5

token2 : token6
          token2 token6

......

Instead of implementing get_token(), a scanning function or multiple functions can be implemented and the same functions has to be initialised in gills context. Below code explains this.

void init_gills_scan_func (gills_context_t *gills,
                     scan_func_t scan_func, void *scan_val,
                     int val_size)
{
    gills->scan_funcs_max_idx += 1;
    gills->scan_funcs[gills->scan_funcs_max_idx] = *scan_func;
    gills->scan_funcs_val[gills->scan_funcs_max_idx] = scan_val;
    gills->scan_val_size[gills->scan_funcs_max_idx] = val_size;
}


void init_composer_gills(gills_context_t *gills)
{
 //   init_gills_param(
    init_gills_scan_func(gills, &prepyylex, &prepval, sizeof(prepval));
 //   init_gills_scan_func(gills, &makeyylex, &makeval, sizeof(makeval));
}

get_token() can call scan_funcs() depending on which scan_idx is passed to it.

int get_token (gills_context_t *gills, int scan_idx)
{
    return gills->scan_funcs[scan_idx](gills->scan_funcs_val[scan_idx], gills->yyscan);
}


Maximum number of scanning functions and lot many parameters can be initialised in init_gills_params()

void init_gills_params (gills_context_t *gills,
                       int init_pstack_max_num,
                       int init_pstackidx_max_num,
                       int init_tkstack_max_num,
                       int init_listnodearr_max_num,
                       int init_scan_funcs_max_num,
                       int init_pnode_max_num,
                       int init_action_params_max_num,
                       int init_last_tk_num)

Internal parse tree node stack size is controlled by pstack_max_num.
Internal parse tree token stack size is controlled by tkstack_max_num.
Internal parse tree nodes memory is controlled by pnode_max_num.
Action function will be executed when a nonterminal is resolved and at that point number of parameters passed will depend on number of tokens in rule. action_params_max_num will provide this.
Last token number is always zero unless it is changed here by last_tk_num.


Scanning files are scanner.c and scanner.h which is simple scanner implemented (simplex).

Sample calculator and some combinations of parsing logic are provided for token_read_t as tkr arrays in parser.c file.
One can be kept and rest can be commented out to validate logic.
Depending on which tkr is active, respective input data file has to be created as text file with name "data". 
