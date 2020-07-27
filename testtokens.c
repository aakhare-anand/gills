#include "testtokens.h"

simplex_token_t testtokens[SAMPLE_TOKENS_NUM] = {
{ "[0-9]", 2 },
{ "\\+", 3 },
{ "\\-", 4 },
{ "\\*", 5 },
{ "\\/", 6 },
{ "a", 9 },
{ "b", 10 },
{ "[0-9a-zA-Z]+", 11 }
};
