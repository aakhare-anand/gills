#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gillsgentoken.h"
#include "simplex.h"

char gillsgen_cmd_options_string[] = "i:o::";
char parsefilename[FILENAME_MAX_LEN];
char outputfilename[FILENAME_MAX_LEN];

const struct option gillsgen_cmd_long_options[] = {
    {"inputfile", 1, NULL, 0 },
    {"outputfile", 1, NULL, 0 },
    {NULL, 0, NULL, 0 },
};

int gillsgen_get_cmd_options (int argc, char *argv[])
{
    while (1) {
        ret = getopt_long(argc, argv, cmd_options_string, gillsgen_cmd_long_options,
                       &cmd_option_index);
            if (ret == -1)
                break;
        if ((char)ret == '?') {
            printf("\n unknown option %d\n", optopt);
            return -1;
        }
        if ((char)ret == ':') {
            printf("\nno argument given to option %d\n", optopt);
            return -1;
        }

        switch ((char)ret) {
            case 'i':
                sprintf(parsefilename, "%s", optarg);
                break;

            case 'o':
                sprintf(outputfilename, "%s", optarg);
                            break;
            default:
                printf("\ninvalid invokation of gillsgen with"
                                       " argument %c\n", ret);
                return -1;
        }
    }

   return 0;
}

gillsgen_t* gillsgen_init (gillsgen_t *gillsgen, char *ifilepath,
                   char *ofilepath, int read_buf_size,
                   int min_fread_len, int write_buf_size,
                   int min_fwrite_len)
{
    gillsgen_t *gillsgen;
    int ret;

    gillsgen = (gillsgen_t *)calloc(1, sizeof(gillsgen_t));
    if (!gillsgen) {
        printf("mem alloc for gillsgen failed ...exiting\n");
        return NULL;
    }
    ret = gills_fropen(&gillsgen->gfread, ifilepath, buf_size, min_fread_len);
    if (ret)
        return NULL;

    ret = gills_fwopen_buffered(&gillsgen->gfwrite, ofilepath,
                       write_buf_size, min_fwrite_len);
    if (ret)
        return NULL;
    gillsgen->simplex = simplex_init(gillsgentokens, gillsgentokens_num,
                                     gillsgen->fread,
                                     (void *)(&gillsgen->yylval));
    
    if (!gillsgen->simplex)
        return NULL;

    gillsgen->gills = gills_create_def((void *)(gillsgen->yylval),
                                       (void *)simplex));
    if (!gillsgen->gills)
        return NULL;

    init_gills_alloc(gills);
//    init_gillsgen_gills(gills);

    ret = read_parse(gills, gillsgen_token_data, parse_tokens_num, NULL, 0, NULL, 0, NULL, 0);
    if (ret) {
        printf("parse data read failure ...exiting\n");
        exit_parse(gills, -1);
    }
    return 0;
}

/*
int get_token (gills_context_t *gills)
{
    gillsgen_t *gillsgen;

    gillsgen = (gillsgen_t *)
*/

int main (int argc, char **argv)
{

    outputfilename[0] = '\0';
    ret = gillsgen_get_cmd_options(argc, argv);

    if (ret == -1) {
        exit(0);
    }

    if (!outputfilename[0]) {
        len = strlen(parsefilename);
        if (parsefilename[len - 2] == '.' &&
            parsefilename[len - 1] == 'y') {
            sprintf(outputfilename, "%s", parsefilename);
            sprintf(outputfilename + len - 1, "%s", "tab.c");
        } else {
            sprintf(outputfilename, "%s", OUTFILE_DEF_NAME);
        }
    }

    gillsgen = gillsgen_init(inputfilename, outputfilename, 
                             GILLSGEN_FREAD_BUF_SIZE,
                             GILLSGEN_FREAD_BUF_MIN_SIZE);
    if (!gillsgen) {
        printf("gillsgen init failed ...exiting\n");
        exit(-1);
    }
    ret = parse(gills, gills->top_token);
    printf("pnodemem_idx %d\npnodeptrs_end %d \n", gills->pnodemem_idx, gills->pnodeptrs_end);
    if (ret) {
        printf("parse failed ...exiting\n");
        exit_parse(gills, -1);
    }
 //   gills_cleanup(gills);
}
