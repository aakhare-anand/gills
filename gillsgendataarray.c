
token_read_t tkr[7] = {
    { "input", 0, 0, 0, { 1, { 1 }, { 0 }, { 0 }, { f0 },
       { "expr", "\0" } } },

char *input_rule1[] = { "firstblock",  "dirblock",  "ruleblock",  "lastblock" };

char *leftbracepcent_rule1[] = { "wspace",  "LEFTBRACEPCENT",  "wspace" };
char *leftbracepcent_rule2[] = { "LEFTBRACEPCENT",  "wspace" };
char *leftbracepcent_rule3[] = { "LEFTBRACEPCENT" };
               ;

char *pcentrightbrace_rule1[] = { "wspace",  "PCENTRIGHTBRACE",  "wspace" };
char *pcentrightbrace_rule2[] = { "PCENTRIGHTBRACE",  "wspace" };
char *pcentrightbrace_rule3[] = { "PCENTRIGHTBRACE" };
               ;

char *firstblock_rule1[] = { "leftbracepcent",  "codeblock",  "pcentrightbrace" };

char *codeblock_rule1[] = { "codewords" };
char *codeblock_rule2[] = { "commentline" };
char *codeblock_rule3[] = { "qcwlines" };
char *codeblock_rule4[] = { "slashcommentline" };
char *codeblock_rule5[] = { "codeblock",  "codewords" };
char *codeblock_rule6[] = { "codeblock",  "commentline" };
char *codeblock_rule7[] = { "codeblock",  "slashcommentline" };
char *codeblock_rule8[] = { "codeblock",  "qcwlines" };
char *codeblock_rule9[] = { "LEFTBRACE",  "codeblock",  "RIGHTBRACE" };
               ;

char *codewords_rule1[] = { "codeword" };
f1 
char *codewords_rule2[] = { "codewords",  "NEWLINE",  "codeword" };
f2 
               ;

char *codeword_rule1[] = { "CWORD" };
char *codeword_rule2[] = { "NAMESTRING" };
char *codeword_rule3[] = { "DOLLAR" };
char *codeword_rule4[] = { "NUM" };
char *codeword_rule5[] = { "PIPE" };
char *codeword_rule6[] = { "PCENT" };
char *codeword_rule7[] = { "WSPACEWONLINE" };
char *codeword_rule8[] = { "SEMICOLON" };
char *codeword_rule9[] = { "COLON" };
char *codeword_rule10[] = { "PCENTTOKEN" };
char *codeword_rule11[] = { "PCENTTYPE" };
char *codeword_rule12[] = { "LEFTANGLEBRACKT" };
char *codeword_rule13[] = { "RIGHTANGLEBRACKT" };
char *codeword_rule14[] = { "SYMTYPENAME" };
char *codeword_rule15[] = { "PCENTLEFT" };
char *codeword_rule16[] = { "PCENTRIGHT" };
char *codeword_rule17[] = { "PCENTPREC" };
char *codeword_rule18[] = { "PCENTUNION" };
char *codeword_rule19[] = { "API" };
char *codeword_rule20[] = { "DOT" };
char *codeword_rule21[] = { "VALUE" };
char *codeword_rule22[] = { "TYPE" };
char *codeword_rule23[] = { "UNION" };
char *codeword_rule24[] = { "NAME" };
char *codeword_rule25[] = { "dollarnum" };
f2_1 
               ;

char *slinecommentline_rule1[] = { "slinecommentstart",  "slinecomment",  "NEWLINE" };
f3
char *slinecommentstart_rule1[] = { "SLINECOMMENTSTART" };
f4 
char *slinecommentstart_rule2[] = { "WSPACE",  "SLINECOMMENTSTART" };
f5 
               ;

char *slinecomment_rule1[] = { "slinecommentword" };
f6 
char *slinecomment_rule2[] = { "slinecomment",  "slinecommentword" };
f6_1
              ;

char *slinecommentword_rule1[] = { "CWORD" };
char *slinecommentword_rule2[] = { "NAMESTRING" };
char *slinecommentword_rule3[] = { "DOLLAR" };
char *slinecommentword_rule4[] = { "NUM" };
char *slinecommentword_rule5[] = { "PIPE" };
char *slinecommentword_rule6[] = { "LEFTBRACE" };
char *slinecommentword_rule7[] = { "RIGHTBRACE" };
char *slinecommentword_rule8[] = { "PCENT" };
char *slinecommentword_rule9[] = { "WSPACEWONLINE" };
char *slinecommentword_rule10[] = { "SEMICOLON" };
char *slinecommentword_rule11[] = { "COLON" };
char *slinecommentword_rule12[] = { "LEFTBRACEPCENT" };
char *slinecommentword_rule13[] = { "PCENTRIGHTBRACE" };
char *slinecommentword_rule14[] = { "DOUBLEPCENT" };
char *slinecommentword_rule15[] = { "QUOTE" };
char *slinecommentword_rule16[] = { "DOUBLEQUOTE" };
char *slinecommentword_rule17[] = { "ESCQUOTE" };
char *slinecommentword_rule18[] = { "ESCDOUBLEQUOTE" };
char *slinecommentword_rule19[] = { "PCENTTOKEN" };
char *slinecommentword_rule20[] = { "PCENTTYPE" };
char *slinecommentword_rule21[] = { "LEFTANGLEBRACKT" };
char *slinecommentword_rule22[] = { "RIGHTANGLEBRACKT" };
char *slinecommentword_rule23[] = { "SYMTYPENAME" };
char *slinecommentword_rule24[] = { "PCENTLEFT" };
char *slinecommentword_rule25[] = { "PCENTRIGHT" };
char *slinecommentword_rule26[] = { "PCENTPREC" };
char *slinecommentword_rule27[] = { "PCENTUNION" };
char *slinecommentword_rule28[] = { "API" };
char *slinecommentword_rule29[] = { "DOT" };
char *slinecommentword_rule30[] = { "VALUE" };
char *slinecommentword_rule31[] = { "TYPE" };
char *slinecommentword_rule32[] = { "UNION" };
char *slinecommentword_rule33[] = { "NAME" };
char *slinecommentword_rule34[] = { "dollarnum" };
               ; 

char *commentline_rule1[] = { "commentstart",  "comment",  "commentend" };
               ;

char *commentstart_rule1[] = { "COMMENTSTART" };
f5 
char *commentstart_rule2[] = { "WSPACE",  "COMMENTSTART" };
f5_1 
               ;

char *commentend_rule1[] = { "COMMENTEND" };
f7 
char *commentend_rule1[] = { "COMMENTEND",  "wspace" };
f8 
               ;

char *comment_rule1[] = { "commentword" };
f9 
char *comment_rule2[] = { "comment",  "NEWLINE",  "commentword" };
f10 

char *commentword_rule1[] = { "CWORD" };
char *commentword_rule2[] = { "NAMESTRING" };
char *commentword_rule3[] = { "DOLLAR" };
char *commentword_rule4[] = { "NUM" };
char *commentword_rule5[] = { "PIPE" };
char *commentword_rule6[] = { "LEFTBRACE" };
char *commentword_rule7[] = { "RIGHTBRACE" };
char *commentword_rule8[] = { "PCENT" };
char *commentword_rule9[] = { "WSPACEWONLINE" };
char *commentword_rule10[] = { "SEMICOLON" };
char *commentword_rule11[] = { "COLON" };
char *commentword_rule12[] = { "LEFTBRACEPCENT" };
char *commentword_rule13[] = { "PCENTRIGHTBRACE" };
char *commentword_rule14[] = { "DOUBLEPCENT" };
char *commentword_rule15[] = { "QUOTE" };
char *commentword_rule16[] = { "DOUBLEQUOTE" };
char *commentword_rule17[] = { "ESCQUOTE" };
char *commentword_rule18[] = { "ESCDOUBLEQUOTE" };
char *commentword_rule19[] = { "PCENTTOKEN" };
char *commentword_rule20[] = { "PCENTTYPE" };
char *commentword_rule21[] = { "LEFTANGLEBRACKT" };
char *commentword_rule22[] = { "RIGHTANGLEBRACKT" };
char *commentword_rule23[] = { "SYMTYPENAME" };
char *commentword_rule24[] = { "PCENTLEFT" };
char *commentword_rule25[] = { "PCENTRIGHT" };
char *commentword_rule26[] = { "PCENTPREC" };
char *commentword_rule27[] = { "PCENTUNION" };
char *commentword_rule28[] = { "API" };
char *commentword_rule29[] = { "DOT" };
char *commentword_rule30[] = { "VALUE" };
char *commentword_rule31[] = { "TYPE" };
char *commentword_rule32[] = { "UNION" };
char *commentword_rule33[] = { "NAME" };
char *commentword_rule34[] = { "dollarnum" };
                ;

char *qcwlines_rule1[] = { "qcwline" };
char *qcwlines_rule2[] = { "qcwlines",  "qcwline" };

char *qcwline_rule1[] = { "qcwords" };
char *qcwline_rule2[] = { "quotestart",  "qcwline",  "quoteend" };
char *qcwline_rule3[] = { "doublequotestart",  "qcwline",  "doublequoteend" };
               ;

char *qcwords_rule1[] = { "qcword" };
f11 
char *qcwords_rule2[] = { "qcwords",  "NEWLINE",  "qcword" };
f12 
               ;

char *qcword_rule1[] = { "CWORD" };
char *qcword_rule2[] = { "NAMESTRING" };
char *qcword_rule3[] = { "DOLLAR" };
char *qcword_rule4[] = { "NUM" };
char *qcword_rule5[] = { "PIPE" };
char *qcword_rule6[] = { "LEFTBRACE" };
char *qcword_rule7[] = { "RIGHTBRACE" };
char *qcword_rule8[] = { "PCENT" };
char *qcword_rule9[] = { "WSPACE" };
char *qcword_rule10[] = { "SEMICOLON" };
char *qcword_rule11[] = { "COLON" };
char *qcword_rule12[] = { "LEFTBRACEPCENT" };
char *qcword_rule13[] = { "PCENTRIGHTBRACE" };
char *qcword_rule14[] = { "DOUBLEPCENT" };
char *qcword_rule15[] = { "COMMENTSTART" };
char *qcword_rule16[] = { "COMMENTEND" };
char *qcword_rule17[] = { "ESCQUOTE" };
char *qcword_rule18[] = { "ESCDOUBLEQUOTE" };
char *qcword_rule19[] = { "PCENTTOKEN" };
char *qcword_rule20[] = { "PCENTTYPE" };
char *qcword_rule21[] = { "LEFTANGLEBRACKT" };
char *qcword_rule22[] = { "RIGHTANGLEBRACKT" };
char *qcword_rule23[] = { "SYMTYPENAME" };
char *qcword_rule24[] = { "PCENTLEFT" };
char *qcword_rule25[] = { "PCENTRIGHT" };
char *qcword_rule26[] = { "PCENTPREC" };
char *qcword_rule27[] = { "PCENTUNION" };
char *qcword_rule28[] = { "API" };
char *qcword_rule29[] = { "DOT" };
char *qcword_rule30[] = { "VALUE" };
char *qcword_rule31[] = { "TYPE" };
char *qcword_rule32[] = { "UNION" };
char *qcword_rule33[] = { "NAME" };
char *qcword_rule34[] = { "dollarnum" };
                ;

char *quotestart_rule1[] = { "QUOTE" };
f13 
char *quotestart_rule2[] = { "WSPACE",  "QUOTE" };
f14 
                ; 

char *quoteend_rule1[] = { "QUOTE" };
f15 
char *quoteend_rule2[] = { "QUOTE",  "wspace" };
f16 
                ;

char *doublequotestart_rule1[] = { "DOUBLEQUOTE" };
f17 
char *doublequotestart_rule2[] = { "WSPACE",  "DOUBLEQUOTE" };
f18 
                ;

char *doublequoteend_rule1[] = { "DOUBLEQUOTE" };
f19 
char *doublequoteend_rule2[] = { "DOUBLEQUOTE",  "wspace" };
f20 
                ;

char *dirblock_rule1[] = { "dirlist",  "DOUBLEPCENT" };
dirblock_endprocessing
                ;

char *dirlist_rule1[] = { "directives" };
char *dirlist_rule2[] = { "dirlist",  "symtype" };
char *dirlist_rule3[] = { "dirlist",  "tokentype" };
char *dirlist_rule4[] = { "dirlist",  "leftassoc" };
char *dirlist_rule5[] = { "dirlist",  "rightassoc" };
char *dirlist_rule6[] = { "dirlist",  "precedence" };
char *dirlist_rule7[] = { "dirlist",  "uniondirective" };
char *dirlist_rule8[] = { "dirlist",  "tokenprefix" };
char *dirlist_rule9[] = { "dirlist",  "apivaluetype" };
char *dirlist_rule10[] = { "dirlist",  "apiprefix" };
                ;

char *directives_rule1[] = { "symtype" };
char *directives_rule2[] = { "tokentype" };
char *directives_rule3[] = { "leftassoc" };
char *directives_rule4[] = { "rightassoc" };
char *directives_rule5[] = { "precedence" };
char *directives_rule6[] = { "uniondirective" };
char *directives_rule7[] = { "tokenprefix" };
char *directives_rule8[] = { "apivaluetype" };
char *directives_rule9[] = { "apiprefix" };
               ;

char *doublepcent_rule1[] = { "DOUBLEPCENT" };
char *doublepcent_rule2[] = { "wspace",  "DOUBLEPCENT" };
char *doublepcent_rule3[] = { "wspace",  "DOUBLEPCENT",  "wspace" };
               ;

char *ruleblock_rule1[] = { "symrules",  "doublepcent" };
ruleblock_processing
               ;

char *symrules_rule1[] = { "symrule" };
char *symrules_rule2[] = { "symrules",  "symrule" };
               ;

char *symrule_rule1[] = { "symrulestart",  "rules",  "symruleend" };

char *symrulestart_rule1[] = { "symname" };
               ;

char *symname_rule1[] = { "NAMESTRING",  "symrulecolon" };
f44 

char *symname_rule2[] = { "wspace",  "NAMESTRING",  "symrulecolon" };
f45 
              ;

char *symrulecolon_rule1[] = { "COLON" };
char *symrulecolon_rule2[] = { "wspace",  "COLON",  "wspace" };
char *symrulecolon_rule3[] = { "wspace",  "COLON" };
char *symrulecolon_rule4[] = { "COLON",  "wspace" };
              ;

char *rules_rule1[] = { "ruleact" };
char *rules_rule2[] = { "rules",  "wspace",  "PIPE",  "wspace",  "ruleact" };
char *rules_rule3[] = { "rules",  "PIPE",  "wspace",  "ruleact" };
char *rules_rule4[] = { "rules",  "wspace",  "PIPE",  "ruleact" };
char *rules_rule5[] = { "rules",  "rulepcentprec",  "NAMESTRING", "PIPE",  "wspace",  "ruleact" };
char *rules_rule6[] = { "rules",  "rulepcentprec", "NAMESTRING",  "PIPE",  "ruleact" };
               ;

char *pcentprec_rule1[] = { "wspace",  "PCENTPREC",  "wspace" };
char *pcentprec_rule2[] = { "wspace",  "PCENTPREC" };
char *pcentprec_rule3[] = { "PCENTPREC",  "wspace" };
               ;

char *ruleact_rule1[] = { "rule" };
f49 
char *ruleact_rule2[] = { "rule",  "ruleactioncodeblock" };
f46
               ;
char *rule_rule1[] = { "NAMESTRING" };
f47 
char *rule_rule2[] = { "commentline" };
char *rule_rule3[] = { "rule",  "wspace",  "NAMESTRING" };
f48 
char *rule_rule4[] = { "rule",  "wspace",  "commentline" };
             ;

char *ruleactioncodeblock_rule1[] = { "rulecodeblockstart",  "codeblock",  "rulecodeblockend" };
               ;

char *rulecodeblockstart_rule1[] = { "LEFTBRACE" };
f50 
char *rulecodeblockstart_rule2[] = { "wspace",  "LEFTBRACE" };
f51 
    
               ;

char *rulecodeblockend_rule1[] = { "RIGHTBRACE" };
f52 
char *rulecodeblockend_rule2[] = { "wspace",  "RIGHTBRACE" };
f51 
               ;

char *symruleend_rule1[] = { "SEMICOLON" };
char *symruleend_rule2[] = { "wspace",  "SEMICOLON" };
char *symruleend_rule3[] = { "SEMICOLON",  "wspace" };
               ;

char *lastblock_rule1[] = { "codeblock" };

char *lblockwords_rule1[] = { "lblockword" };
char *lblockwords_rule2[] = { "lblockwords",  "NEWLINE",  "lblockword" };
               ;

char *lblockword_rule1[] = { "CWORD" };
char *lblockword_rule2[] = { "LEFTBRACE" };
char *lblockword_rule3[] = { "RIGHTBRACE" };
char *lblockword_rule4[] = { "QUOTE" };
char *lblockword_rule5[] = { "DOUBLEQUOTE" };
char *lblockword_rule6[] = { "ESCQUOTE" };
char *lblockword_rule7[] = { "ESCDOUBLEQUOTE" };
char *lblockword_rule8[] = { "NAMESTRING" };
char *lblockword_rule9[] = { "DOLLAR" };
char *lblockword_rule10[] = { "NUM" };
char *lblockword_rule11[] = { "PIPE" };
char *lblockword_rule12[] = { "PCENT" };
char *lblockword_rule13[] = { "WSPACEWONLINE" };
char *lblockword_rule14[] = { "SEMICOLON" };
char *lblockword_rule15[] = { "COLON" };
char *lblockword_rule16[] = { "PCENTTOKEN" };
char *lblockword_rule17[] = { "PCENTTYPE" };
char *lblockword_rule18[] = { "LEFTANGLEBRACKT" };
char *lblockword_rule19[] = { "RIGHTANGLEBRACKT" };
char *lblockword_rule20[] = { "SYMTYPENAME" };
char *lblockword_rule21[] = { "PCENTLEFT" };
char *lblockword_rule22[] = { "PCENTRIGHT" };
char *lblockword_rule23[] = { "PCENTPREC" };
char *lblockword_rule24[] = { "PCENTUNION" };
char *lblockword_rule25[] = { "API" };
char *lblockword_rule26[] = { "DOT" };
char *lblockword_rule27[] = { "VALUE" };
char *lblockword_rule28[] = { "TYPE" };
char *lblockword_rule29[] = { "UNION" };
char *lblockword_rule30[] = { "NAME" };
char *lblockword_rule31[] = { "dollarnum" };
             ;

char *dollarnum_rule1[] = { "DOLLAR",  "NUM" };
f54 
             ;

char *wspace_rule1[] = { "WSPACE" };
char *wspace_rule2[] = { "NEWLINE" };
char *wspace_rule3[] = { "wspace",  "WSPACE" };
char *wspace_rule4[] = { "wspace",  "NEWLINE" };
               ; 

char *tokentype_rule1[] = { "pcenttoken",  "typetokenname" };
f21 
char *tokentype_rule2[] = { "pcenttoken",  "LEFTANGLEBRACKT",  "symtypename",  "RIGHTANGLEBRACKT",  "typetokenname" };
f22 
              ;

char *symtype_rule1[] = { "pcenttype",  "typesymname" };
f23 
char *symtype_rule2[] = { "pcenttype",  "LEFTANGLEBRACKT",  "symtypename",  "RIGHTANGLEBRACKT",  "typesymname" };
f24 
               ;

char *pcenttoken_rule1[] = { "PCENTTOKEN" };
char *pcenttoken_rule2[] = { "wspace",  "PCENTTOKEN" };
char *pcenttoken_rule3[] = { "wspace",  "PCENTTOKEN",  "wspace" };
char *pcenttoken_rule4[] = { "PCENTTOKEN",  "wspace" };
               ;

char *typetokenname_rule1[] = { "NAMESTRING" };
               ;

char *typesymname_rule1[] = { "NAMESTRING" };
char *typesymname_rule2[] = { "wspace",  "NAMESTRING" };
f24_1 
               ;

char *pcenttype_rule1[] = { "PCENTTYPE" };
char *pcenttype_rule2[] = { "wspace",  "PCENTTYPE" };
char *pcenttype_rule3[] = { "wspace",  "PCENTTYPE",  "wspace" };
char *pcenttype_rule4[] = { "PCENTTYPE",  "wspace" };
              ;

char *symtypename_rule1[] = { "NAMESTRING" };
char *symtypename_rule2[] = { "NAMESTRING",  "wspace" };
char *symtypename_rule3[] = { "wspace",  "NAMESTRING",  "wspace" };
char *symtypename_rule4[] = { "wspace",  "NAMESTRING" };
              ;

char *leftassoc_rule1[] = { "pcentleft",  "wspace",  "assocsymnames" };
              ;

char *rightassoc_rule1[] = { "pcentright",  "wspace",  "assocsymnames" };
              ;

char *pcentleft_rule1[] = { "PCENTLEFT" };
f25  
char *pcentleft_rule2[] = { "wspace",  "PCENTLEFT" };
f26 
              ;

char *pcentright_rule1[] = { "PCENTRIGHT" };
f27 
char *pcentright_rule2[] = { "wspace",  "PCENTRIGHT" };
f28 
              ;

char *precedence_rule1[] = { "pcentprec",  "assocsymnames" };
              ;

char *pcentprec_rule1[] = { "PCENTPREC" };
f29 
char *pcentprec_rule2[] = { "wspace",  "PCENTPREC" };
f30 
              ;

char *assocsymnames_rule1[] = { "NAMESTRING" };
f31 
    
char *assocsymnames_rule2[] = { "assocsymnames",  "wspace",  "NAMESTRING" };
f32 
             ;

char *uniondirective_rule1[] = { "pcentunion",  "unionleftbrace",  "unionfields",  "unionrightbrace" };
char *uniondirective_rule2[] = { "pcentunion",  "uniontag",  "unionleftbrace",  "unionfields",  "unionrightbrace" };
              ;

char *pcentunion_rule1[] = { "PCENTUNION" };
f38 
char *pcentunion_rule2[] = { "wspace",  "PCENTUNION" };
f39 
              ; 

char *uniontag_rule1[] = { "wspace",  "NAMESTRING" };
f40 
              ;

char *unionleftbrace_rule1[] = { "LEFTBRACE" };
char *unionleftbrace_rule2[] = { "wspace",  "LEFTBRACE" };
char *unionleftbrace_rule3[] = { "wspace",  "LEFTBRACE",  "wspace" };
              ;

char *unionfields_rule1[] = { "unionfield" };
char *unionfields_rule2[] = { "unionfields",  "unionfield" };
              ;

char *unionfield_rule1[] = { "NAMESTRING",  "wspace",  "NAMESTRING",  "unionfieldsemicolon" };
f33 

char *unionfield_rule2[] = { "NAMESTRING",  "wspace",  "NAMESTRING",  "wspace",  "NAMESTRING",  "unionfieldsemicolon" };
f34 
              ;

char *unionfieldsemicolon_rule1[] = { "wspace",  "SEMICOLON" };
char *unionfieldsemicolon_rule2[] = { "SEMICOLON" };
char *unionfieldsemicolon_rule3[] = { "wspace",  "SEMICOLON",  "wspace" };
              ;

char *unionrightbrace_rule1[] = { "RIGHTBRACE" };
char *unionrightbrace_rule2[] = { "wspace",  "RIGHTBRACE" };
char *unionrightbrace_rule3[] = { "wspace",  "RIGHTBRACE",  "wspace" };
              ;

// %"token",  "NUM",  300
// %"token",  "XNUM",  0x300

char *apiprefix_rule1[] = { "pcentdefine",  "wspace",  "API",  "DOT",  "PREFIX",  "apiprefixstart",  "NAMESTRING",  "apiprefixend" };
f41 
             ;

char *apiprefixstart_rule1[] = { "LEFTBRACE" };
char *apiprefixstart_rule2[] = { "wspace",  "LEFTBRACE",  "wspace" };
char *apiprefixstart_rule3[] = { "LEFTBRACE",  "wspace" };
              ;

char *apiprefixend_rule1[] = { "RIGHTBRACE" };
char *apiprefixend_rule2[] = { "wspace",  "RIGHTBRACE",  "wspace" };
char *apiprefixend_rule3[] = { "wspace",  "RIGHTBRACE" };
              ;

char *tokenprefix_rule1[] = { "pcentdefine",  "wspace",  "API",  "DOT",  "TOKEN",  "DOT",  "PREFIX",  "wspace",  "tkprefixstart",  "NAMESTRING",  "tkprefixend" };
f35 
             ;

char *pcentdefine_rule1[] = { "PCENTDEFINE" };
char *pcentdefine_rule2[] = { "wspace",  "PCENTDEFINE" };
              ;

char *tkprefixstart_rule1[] = { "LEFTBRACE" };
char *tkprefixstart_rule2[] = { "wspace",  "LEFTBRACE",  "wspace" };
char *tkprefixstart_rule3[] = { "LEFTBRACE",  "wspace" };
              ;

char *tkprefixend_rule1[] = { "RIGHTBRACE" };
char *tkprefixend_rule2[] = { "wspace",  "RIGHTBRACE" };
              ;

char *apivaluetype_rule1[] = { "pcentdefine",  "wspace",  "API",  "DOT",  "VALUE",  "DOT",  "TYPE",  "wspace",  "LEFTBRACE",  "RIGHTBRACE" };
char *apivaluetype_rule2[] = { "pcentdefine",  "wspace",  "API",  "DOT",  "VALUE",  "DOT",  "TYPE",  "wspace",  "valuetypestart",  "valuetype",  "valuetypeend" };
              ;

char *valuetypestart_rule1[] = { "LEFTBRACE" };
char *valuetypestart_rule2[] = { "LEFTBRACE",  "wspace" };
              ;

char *valuetype_rule1[] = { "NAMESTRING" };
f36 
char *valuetype_rule2[] = { "valuetype",  "wspace",  "NAMESTRING" };
f37 

char *valuetypeend_rule1[] = { "RIGHTBRACE" };
char *valuetypeend_rule2[] = { "wspace",  "RIGHTBRACE" };
              ;

char *unionnamevalue_rule1[] = { "pcentdefine",  "wspace",  "API",  "DOT",  "VALUE",  "DOT",  "UNION",  "DOT",  "NAME",  "wspace",  "unionname" };
              ;

char *unionname_rule1[] = { "NAMESTRING" };
f42 
char *unionname_rule2[] = { "NAMESTRING",  "wspace" };
f43 
              ;

#############

input: firstblock dirblock ruleblock lastblock 

leftbracepcent:   wspace LEFTBRACEPCENT wspace
                | LEFTBRACEPCENT wspace
                | LEFTBRACEPCENT
               ;

pcentrightbrace:   wspace PCENTRIGHTBRACE wspace
                 | PCENTRIGHTBRACE wspace
                 | PCENTRIGHTBRACE
               ;

firstblock: leftbracepcent codeblock pcentrightbrace

codeblock:   codewords
           | commentline
           | qcwlines
           | slashcommentline
           | codeblock codewords
           | codeblock commentline
           | codeblock slashcommentline
           | codeblock qcwlines
           | LEFTBRACE codeblock RIGHTBRACE
               ;

codewords:   codeword
f1 
           | codewords NEWLINE codeword
f2 
               ;

codeword:   CWORD
        | NAMESTRING
        | DOLLAR
        | NUM
        | PIPE
        | PCENT
        | WSPACEWONLINE
        | SEMICOLON
        | COLON
        | PCENTTOKEN
        | PCENTTYPE
        | LEFTANGLEBRACKT
        | RIGHTANGLEBRACKT
        | SYMTYPENAME
        | PCENTLEFT
        | PCENTRIGHT
        | PCENTPREC
        | PCENTUNION
        | API
        | DOT
        | VALUE
        | TYPE
        | UNION
        | NAME
        | dollarnum
f2_1 
f2_1 ()
{
    char *str = ((YYSTYPE *)(gills->act_val[0]))->str;
    int param_num, sym_num;
    symbol_node_t *sym_node;
    symrule_t *rule = gillsgen->cur_rule;
    list_t *symlist = rule->symbols;

    param_num = atoi(str);

    while (1) {
        symlist = symlist->next;
        if (symlist == rule->symbols)
            break;
        sym_num += 1;
        if (sym_num == param_num) {
            sym_node = (symbol_node_t *)(symlist->data);
            fprintf(gillsgen->gfwrite->outfileptr, "%s%s->act_val[%s]", sym_node->datatype,
                    gillsgen->gills_context_name, param_num);
            break;
        }
    }
    if (symlist == rule->symbols) {
        printf("invalid symbol symantic value $%s ...exiting\n", param_num);
        exit(-1);
    }
}
               ;

slinecommentline:   slinecommentstart slinecomment NEWLINE
f3
slinecommentstart:   SLINECOMMENTSTART
f4 
                   | WSPACE SLINECOMMENTSTART
f5 
               ;

slinecomment:   slinecommentword
f6 
              | slinecomment slinecommentword
f6_1
              ;

slinecommentword:   CWORD
        | NAMESTRING
        | DOLLAR
        | NUM
        | PIPE
        | LEFTBRACE
        | RIGHTBRACE
        | PCENT
        | WSPACEWONLINE
        | SEMICOLON
        | COLON
        | LEFTBRACEPCENT
        | PCENTRIGHTBRACE
        | DOUBLEPCENT
        | QUOTE
        | DOUBLEQUOTE
        | ESCQUOTE
        | ESCDOUBLEQUOTE
        | PCENTTOKEN
        | PCENTTYPE
        | LEFTANGLEBRACKT
        | RIGHTANGLEBRACKT
        | SYMTYPENAME
        | PCENTLEFT
        | PCENTRIGHT
        | PCENTPREC
        | PCENTUNION
        | API
        | DOT
        | VALUE
        | TYPE
        | UNION
        | NAME
        | dollarnum
               ;

commentline:   commentstart comment commentend
               ;

commentstart:   COMMENTSTART
f5 
              | WSPACE COMMENTSTART
f5_1 
               ;

commentend:   COMMENTEND
f7 
            | COMMENTEND wspace
f8 
               ;

comment:    commentword
f9 
         | comment NEWLINE commentword
f10 

commentword:   CWORD
        | NAMESTRING
        | DOLLAR
        | NUM
        | PIPE
        | LEFTBRACE
        | RIGHTBRACE
        | PCENT
        | WSPACEWONLINE
        | SEMICOLON
        | COLON
        | LEFTBRACEPCENT
        | PCENTRIGHTBRACE
        | DOUBLEPCENT
        | QUOTE
        | DOUBLEQUOTE
        | ESCQUOTE
        | ESCDOUBLEQUOTE
        | PCENTTOKEN
        | PCENTTYPE
        | LEFTANGLEBRACKT
        | RIGHTANGLEBRACKT
        | SYMTYPENAME
        | PCENTLEFT
        | PCENTRIGHT
        | PCENTPREC
        | PCENTUNION
        | API
        | DOT
        | VALUE
        | TYPE
        | UNION
        | NAME
        | dollarnum
                ;

qcwlines:   qcwline
          | qcwlines qcwline

qcwline:   qcwords
         | quotestart qcwline quoteend
         | doublequotestart qcwline doublequoteend
               ;

qcwords:   qcword
f11 
         | qcwords NEWLINE qcword
f12 
               ;

qcword:   CWORD
        | NAMESTRING
        | DOLLAR
        | NUM
        | PIPE
        | LEFTBRACE
        | RIGHTBRACE
        | PCENT
        | WSPACE
        | SEMICOLON
        | COLON
        | LEFTBRACEPCENT
        | PCENTRIGHTBRACE
        | DOUBLEPCENT
        | COMMENTSTART
        | COMMENTEND
        | ESCQUOTE
        | ESCDOUBLEQUOTE
        | PCENTTOKEN
        | PCENTTYPE
        | LEFTANGLEBRACKT
        | RIGHTANGLEBRACKT
        | SYMTYPENAME
        | PCENTLEFT
        | PCENTRIGHT
        | PCENTPREC
        | PCENTUNION
        | API
        | DOT
        | VALUE
        | TYPE
        | UNION
        | NAME
        | dollarnum
                ;

quotestart:   QUOTE
f13 
            | WSPACE QUOTE
f14 
                ;

quoteend:   QUOTE
f15 
          | QUOTE wspace
f16 
                ;

doublequotestart:   DOUBLEQUOTE
f17 
                  | WSPACE DOUBLEQUOTE
f18 
                ;

doublequoteend:   DOUBLEQUOTE
f19 
                | DOUBLEQUOTE wspace
f20 
                ;

dirblock:   dirlist DOUBLEPCENT
dirblock_endprocessing
                ;

dirlist:   directives
         | dirlist symtype
         | dirlist tokentype
         | dirlist leftassoc
         | dirlist rightassoc
         | dirlist precedence
         | dirlist uniondirective
         | dirlist tokenprefix
         | dirlist apivaluetype
         | dirlist apiprefix
                ;

directives:   symtype
            | tokentype
            | leftassoc
            | rightassoc
            | precedence
            | uniondirective
            | tokenprefix
            | apivaluetype
            | apiprefix
               ;

doublepcent:   DOUBLEPCENT
             | wspace DOUBLEPCENT
             | wspace DOUBLEPCENT wspace
               ;

ruleblock: symrules doublepcent
ruleblock_processing
               ;

symrules:   symrule
          | symrules symrule
               ;

symrule: symrulestart rules symruleend

symrulestart: symname 
               ;

symname:   NAMESTRING symrulecolon
f44 

         | wspace NAMESTRING symrulecolon
f45 
              ;

symrulecolon:   COLON
              | wspace COLON wspace
              | wspace COLON
              | COLON wspace
              ;

rules:  ruleact
      | rules wspace PIPE wspace ruleact
      | rules PIPE wspace ruleact
      | rules wspace PIPE ruleact
      | rules rulepcentprec PIPE wspace ruleact
      | rules rulepcentprec PIPE ruleact
               ;

rulepcentprec:   wspace PCENTPREC wspace
           | wspace PCENTPREC
           | PCENTPREC wspace
               ;

ruleact:   rule
f49 
         | rule ruleactioncodeblock
f46
               ;
rule:   NAMESTRING
f47 
      | commentline
      | rule wspace NAMESTRING
f48 
             ;

ruleactioncodeblock: rulecodeblockstart codeblock rulecodeblockend
               ;

rulecodeblockstart:   LEFTBRACE
f50 
                    | wspace LEFTBRACE
f51 
    
               ;

rulecodeblockend:   RIGHTBRACE
f52 
                    | wspace RIGHTBRACE
f51 
               ;

symruleend:   SEMICOLON
            | wspace SEMICOLON
            | SEMICOLON wspace
               ;

lastblock: codeblock

lblockwords:   lblockword
             | lblockwords NEWLINE lblockword
               ;

lblockword:   CWORD
        | LEFTBRACE
        | RIGHTBRACE
        | QUOTE
        | DOUBLEQUOTE
        | ESCQUOTE
        | ESCDOUBLEQUOTE
        | NAMESTRING
        | DOLLAR
        | NUM
        | PIPE
        | PCENT
        | WSPACEWONLINE
        | SEMICOLON
        | COLON
        | PCENTTOKEN
        | PCENTTYPE
        | LEFTANGLEBRACKT
        | RIGHTANGLEBRACKT
        | SYMTYPENAME
        | PCENTLEFT
        | PCENTRIGHT
        | PCENTPREC
        | PCENTUNION
        | API
        | DOT
        | VALUE
        | TYPE
        | UNION
        | NAME
        | dollarnum               ;
             ;

dollarnum:   DOLLAR NUM
f54 
             ;

wspace:    WSPACE
         | NEWLINE
         | wspace WSPACE
         | wspace NEWLINE
               ;

tokentype:   pcenttoken typetokenname
f21 
          | pcenttoken LEFTANGLEBRACKT symtypename RIGHTANGLEBRACKT typetokenname 
f22 
              ;

symtype:   pcenttype typesymname
f23 
         | pcenttype LEFTANGLEBRACKT symtypename RIGHTANGLEBRACKT typesymname
f24 
               ;

pcenttoken:   PCENTTOKEN
       | wspace PCENTTOKEN
       | wspace PCENTTOKEN wspace
       | PCENTTOKEN wspace
               ;

typetokenname:   NAMESTRING
               ;

typesymname:   NAMESTRING
             | wspace NAMESTRING
f24_1 
               ;

pcenttype:   PCENTTYPE
      | wspace PCENTTYPE
      | wspace PCENTTYPE wspace
      | PCENTTYPE wspace
              ;

symtypename:   NAMESTRING
             | NAMESTRING wspace
             | wspace NAMESTRING wspace
             | wspace NAMESTRING
              ;

leftassoc:   pcentleft wspace assocsymnames
              ;

rightassoc: pcentright wspace assocsymnames
              ;

pcentleft:   PCENTLEFT
f25 
           | wspace PCENTLEFT
f26 
              ;

pcentright:   PCENTRIGHT
f27 
           | wspace PCENTRIGHT
f28 
              ;

precedence:   pcentprec assocsymnames
              ;

pcentprec:   PCENTPREC
f29 
           | wspace PCENTPREC
f30 
              ;

assocsymnames:   NAMESTRING
f31 
    
               | assocsymnames wspace NAMESTRING
f32 
             ;

uniondirective:   pcentunion unionleftbrace unionfields unionrightbrace
                | pcentunion uniontag unionleftbrace unionfields unionrightbrace
              ;

pcentunion:   PCENTUNION
f38 
            | wspace PCENTUNION
f39 
              ;

uniontag:   wspace NAMESTRING
f40 
              ;

unionleftbrace:   LEFTBRACE
                | wspace LEFTBRACE
                | wspace LEFTBRACE wspace
              ;

unionfields:   unionfield
             | unionfields unionfield
              ;

unionfield:   NAMESTRING wspace NAMESTRING unionfieldsemicolon
f33 

            | NAMESTRING wspace NAMESTRING wspace NAMESTRING unionfieldsemicolon
f34 
              ;

unionfieldsemicolon:   wspace SEMICOLON
                     | SEMICOLON
                     | wspace SEMICOLON wspace
              ;

unionrightbrace:   RIGHTBRACE
                 | wspace RIGHTBRACE
                 | wspace RIGHTBRACE wspace
              ;

// %token NUM 300
// %token XNUM 0x300

apiprefix:    pcentdefine wspace API DOT PREFIX apiprefixstart NAMESTRING apiprefixend
f41 
             ;

apiprefixstart:   LEFTBRACE
                | wspace LEFTBRACE wspace
                | LEFTBRACE wspace
              ;

apiprefixend:   RIGHTBRACE
              | wspace RIGHTBRACE wspace
              | wspace RIGHTBRACE
              ;

tokenprefix:   pcentdefine wspace API DOT TOKEN DOT PREFIX wspace tkprefixstart NAMESTRING tkprefixend
f35 
             ;

pcentdefine:   PCENTDEFINE
             | wspace PCENTDEFINE
              ;

tkprefixstart:   LEFTBRACE
               | wspace LEFTBRACE wspace
               | LEFTBRACE wspace
              ;

tkprefixend:   RIGHTBRACE
             | wspace RIGHTBRACE
              ;

apivaluetype:   pcentdefine wspace API DOT VALUE DOT TYPE wspace LEFTBRACE RIGHTBRACE
              | pcentdefine wspace API DOT VALUE DOT TYPE wspace valuetypestart valuetype valuetypeend
              ;

valuetypestart:   LEFTBRACE
                | LEFTBRACE wspace
              ;

valuetype:   NAMESTRING
f36 
           | valuetype wspace NAMESTRING
f37 
 void f37 ()
{
    char *str2 = ((YYSTYPE *)(gills->act_val[2]))->str;
    char *str3 = ((YYSTYPE *)(gills->act_val[3]))->str;

    gillsgen->typelen += sprintf(valuetype + gillsgen->typelen, "%s%s", str2, str3);
}             ;

valuetypeend:   RIGHTBRACE
              | wspace RIGHTBRACE
              ;

unionnamevalue:   pcentdefine wspace API DOT VALUE DOT UNION DOT NAME wspace unionname
              ;

unionname:   NAMESTRING
f42 
           | NAMESTRING wspace
f43 
              ;


}, 

##############
} }, 

token_read_t *gillsgendata = {
    { "input", 201, 0, 0, { 1, { 4 }, { 0 }, { 0 }, { NULL }, { input_rule1, } } }, 

    { "leftbracepcent", 202, 0, 0, { 3, { 3, 2, 1 }, { 0, 0, 0 }, { 0, 0, 0 }, { NULL, NULL, NULL }, { leftbracepcent_rule1, leftbracepcent_rule2, leftbracepcent_rule3, } } }, 

    { "pcentrightbrace", 203, 0, 0, { 3, { 3, 2, 1 }, { 0, 0, 0 }, { 0, 0, 0 }, { NULL, NULL, NULL }, { pcentrightbrace_rule1, pcentrightbrace_rule2, pcentrightbrace_rule3, } } }, 

    { "firstblock", 204, 0, 0, { 1, { 3 }, { 0 }, { 0 }, { NULL }, { firstblock_rule1, } } }, 

    { "codeblock", 205, 0, 0, { 9, { 1, 1, 1, 1, 2, 2, 2, 2, 3 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }, { codeblock_rule1, codeblock_rule2, codeblock_rule3, codeblock_rule4, codeblock_rule5, codeblock_rule6, codeblock_rule7, codeblock_rule8, codeblock_rule9, } } }, 

    { "codewords", 206, 0, 0, { 2, { 0, 0 }, { 0, 0 }, { 0, 0 }, { f1, f2 }, { codewords_rule1, codewords_rule2, } } }, 

    { "codeword", 207, 0, 0, { 25, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, f2_1 }, { codeword_rule1, codeword_rule2, codeword_rule3, codeword_rule4, codeword_rule5, codeword_rule6, codeword_rule7, codeword_rule8, codeword_rule9, codeword_rule10, codeword_rule11, codeword_rule12, codeword_rule13, codeword_rule14, codeword_rule15, codeword_rule16, codeword_rule17, codeword_rule18, codeword_rule19, codeword_rule20, codeword_rule21, codeword_rule22, codeword_rule23, codeword_rule24, codeword_rule25, } } }, 

    { "slinecommentline", 208, 0, 0, { 1, { 3 }, { 0 }, { 0 }, { f3 }, { slinecommentline_rule1, } } }, 

    { "slinecommentstart", 209, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f4, f5 }, { slinecommentstart_rule1, slinecommentstart_rule2, } } }, 

    { "slinecomment", 210, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f6, f6_1 }, { slinecomment_rule1, slinecomment_rule2, } } }, 

    { "slinecommentword", 211, 0, 0, { 34, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }, { slinecommentword_rule1, slinecommentword_rule2, slinecommentword_rule3, slinecommentword_rule4, slinecommentword_rule5, slinecommentword_rule6, slinecommentword_rule7, slinecommentword_rule8, slinecommentword_rule9, slinecommentword_rule10, slinecommentword_rule11, slinecommentword_rule12, slinecommentword_rule13, slinecommentword_rule14, slinecommentword_rule15, slinecommentword_rule16, slinecommentword_rule17, slinecommentword_rule18, slinecommentword_rule19, slinecommentword_rule20, slinecommentword_rule21, slinecommentword_rule22, slinecommentword_rule23, slinecommentword_rule24, slinecommentword_rule25, slinecommentword_rule26, slinecommentword_rule27, slinecommentword_rule28, slinecommentword_rule29, slinecommentword_rule30, slinecommentword_rule31, slinecommentword_rule32, slinecommentword_rule33, slinecommentword_rule34, } } }, 

    { "commentline", 212, 0, 0, { 1, { 3 }, { 0 }, { 0 }, { NULL }, { commentline_rule1, } } }, 

    { "commentstart", 213, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f5, f5_1 }, { commentstart_rule1, commentstart_rule2, } } }, 

    { "commentend", 214, 0, 0, { 61, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f7, f8 }, { commentend_rule1, commentend_rule1, } } }, 

    { "comment", 215, 0, 0, { 2, { 1, 3 }, { 0, 0 }, { 0, 0 }, { f9, f10 }, { comment_rule1, comment_rule2, } } }, 

    { "commentword", 216, 0, 0, { 34, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }, { commentword_rule1, commentword_rule2, commentword_rule3, commentword_rule4, commentword_rule5, commentword_rule6, commentword_rule7, commentword_rule8, commentword_rule9, commentword_rule10, commentword_rule11, commentword_rule12, commentword_rule13, commentword_rule14, commentword_rule15, commentword_rule16, commentword_rule17, commentword_rule18, commentword_rule19, commentword_rule20, commentword_rule21, commentword_rule22, commentword_rule23, commentword_rule24, commentword_rule25, commentword_rule26, commentword_rule27, commentword_rule28, commentword_rule29, commentword_rule30, commentword_rule31, commentword_rule32, commentword_rule33, commentword_rule34, } } }, 

    { "qcwlines", 217, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { NULL, NULL }, { qcwlines_rule1, qcwlines_rule2, } } }, 

    { "qcwline", 218, 0, 0, { 3, { 1, 3, 3 }, { 0, 0, 0 }, { 0, 0, 0 }, { NULL, NULL, NULL }, { qcwline_rule1, qcwline_rule2, qcwline_rule3, } } }, 

    { "qcwords", 219, 0, 0, { 2, { 1, 3 }, { 0, 0 }, { 0, 0 }, { f11, f12 }, { qcwords_rule1, qcwords_rule2, } } }, 

    { "qcword", 220, 0, 0, { 34,  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }, { qcword_rule1, qcword_rule2, qcword_rule3, qcword_rule4, qcword_rule5, qcword_rule6, qcword_rule7, qcword_rule8, qcword_rule9, qcword_rule10, qcword_rule11, qcword_rule12, qcword_rule13, qcword_rule14, qcword_rule15, qcword_rule16, qcword_rule17, qcword_rule18, qcword_rule19, qcword_rule20, qcword_rule21, qcword_rule22, qcword_rule23, qcword_rule24, qcword_rule25, qcword_rule26, qcword_rule27, qcword_rule28, qcword_rule29, qcword_rule30, qcword_rule31, qcword_rule32, qcword_rule33, qcword_rule34, } } }, 

    { "quotestart", 221, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f13, f14 }, { quotestart_rule1, quotestart_rule2, } } }, 

    { "quoteend", 222, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { f15, f16 }, { quoteend_rule1, quoteend_rule2, } } }, 

    { "doublequotestart", 223, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f17, f18 }, { doublequotestart_rule1, doublequotestart_rule2, } } }, 

    { "doublequoteend", 224, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { f19, f20 }, { doublequoteend_rule1, doublequoteend_rule2, } } }, 

    { "dirblock", 225, 0, 0, { 1, { 2 }, { 0, 0 }, { 0, 0 }, { dirblock_endprocessing }, { dirblock_rule1, } } }, 

    { "dirlist", 226, 0, 0, { 10, { 1, 2, 2, 2, 2, 2, 2 }, { 0, 0 }, { 0, 0 }, { actfunc }, { dirlist_rule1, dirlist_rule2, dirlist_rule3, dirlist_rule4, dirlist_rule5, dirlist_rule6, dirlist_rule7, dirlist_rule8, dirlist_rule9, dirlist_rule10 } } }, 

    { "directives", 227, 0, 0, { 9, { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, { directives_rule1, directives_rule2, directives_rule3, directives_rule4, directives_rule5, directives_rule6, directives_rule7, directives_rule8, directives_rule9, } } }, 

    { "doublepcent", 228, 0, 0, { 3, { 1, 2, 3 }, { 0, 0, 0 }, { 0, 0, 0 }, { NULL, NULL, NULL }, { doublepcent_rule1, doublepcent_rule2, doublepcent_rule3, } } }, 

    { "ruleblock", 229, 0, 0, { 1, { 2 }, { 0 }, { 0 }, { ruleblock_processing}, { ruleblock_rule1, } } }, 

    { "symrules", 230, 0, 0, { 2, { 1, 2 }, { 0, 0 }, { 0, 0 }, { NULL, NULL }, { symrules_rule1, symrules_rule2, } } }, 

    { "symrule", 231, 0, 0, { 1, { 3 }, { 0 }, { 0 }, { NULL }, { symrule_rule1, } } }, 

    { "symrulestart", 232, 0, 0, { 1, { 0 }, { 0 }, { 0 }, { NULL }, { symrulestart_rule1, } } }, 

    { "symname", 233, 0, 0, { 2, { 2, 3 }, { 0, 0 }, { 0, 0 }, { f44, f45 }, { symname_rule1, symname_rule2, } } }, 

    { "symrulecolon", 234, 0, 0, { 4, { 1, 3, 2, 2 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { NULL, NULL, NULL, NULL }, { symrulecolon_rule1, symrulecolon_rule2, symrulecolon_rule3, symrulecolon_rule4, } } }, 

    { "rules", 235, 0, 0, { 6, { 1, 5, 4, 4, 6, 5 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { NULL, NULL, NULL, f49_1, f49_2 }, { rules_rule1, rules_rule2, rules_rule3, rules_rule4, rules_rule5, rules_rule6, } } }, 

    { "rulepcentprec", 236, 0, 0, { 3, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentprec_rule1, pcentprec_rule2, pcentprec_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { ruleact_rule1, ruleact_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { rule_rule1, rule_rule2, rule_rule3, rule_rule4, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { ruleactioncodeblock_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { rulecodeblockstart_rule1, rulecodeblockstart_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { rulecodeblockend_rule1, rulecodeblockend_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { symruleend_rule1, symruleend_rule2, symruleend_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { lastblock_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { lblockwords_rule1, lblockwords_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { lblockword_rule1, lblockword_rule2, lblockword_rule3, lblockword_rule4, lblockword_rule5, lblockword_rule6, lblockword_rule7, lblockword_rule8, lblockword_rule9, lblockword_rule10, lblockword_rule11, lblockword_rule12, lblockword_rule13, lblockword_rule14, lblockword_rule15, lblockword_rule16, lblockword_rule17, lblockword_rule18, lblockword_rule19, lblockword_rule20, lblockword_rule21, lblockword_rule22, lblockword_rule23, lblockword_rule24, lblockword_rule25, lblockword_rule26, lblockword_rule27, lblockword_rule28, lblockword_rule29, lblockword_rule30, lblockword_rule31, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { dollarnum_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { wspace_rule1, wspace_rule2, wspace_rule3, wspace_rule4, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { tokentype_rule1, tokentype_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { symtype_rule1, symtype_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcenttoken_rule1, pcenttoken_rule2, pcenttoken_rule3, pcenttoken_rule4, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { typetokenname_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { typesymname_rule1, typesymname_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcenttype_rule1, pcenttype_rule2, pcenttype_rule3, pcenttype_rule4, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { symtypename_rule1, symtypename_rule2, symtypename_rule3, symtypename_rule4, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { leftassoc_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { rightassoc_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentleft_rule1, pcentleft_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentright_rule1, pcentright_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { precedence_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentprec_rule1, pcentprec_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { assocsymnames_rule1, assocsymnames_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { uniondirective_rule1, uniondirective_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentunion_rule1, pcentunion_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { uniontag_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionleftbrace_rule1, unionleftbrace_rule2, unionleftbrace_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionfields_rule1, unionfields_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionfield_rule1, unionfield_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionfieldsemicolon_rule1, unionfieldsemicolon_rule2, unionfieldsemicolon_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionrightbrace_rule1, unionrightbrace_rule2, unionrightbrace_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { apiprefix_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { apiprefixstart_rule1, apiprefixstart_rule2, apiprefixstart_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { apiprefixend_rule1, apiprefixend_rule2, apiprefixend_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { tokenprefix_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { pcentdefine_rule1, pcentdefine_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { tkprefixstart_rule1, tkprefixstart_rule2, tkprefixstart_rule3, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { tkprefixend_rule1, tkprefixend_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { apivaluetype_rule1, apivaluetype_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { valuetypestart_rule1, valuetypestart_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { valuetype_rule1, valuetype_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { valuetypeend_rule1, valuetypeend_rule2, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionnamevalue_rule1, } } }, 

    { "symnnnme", 60, 0, 0, { 61, { 0, 0 }, { 0, 0 }, { 0, 0 }, { actfunc }, { unionname_rule1, unionname_rule2,  } } },
}


