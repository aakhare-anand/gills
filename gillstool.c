#include <stdio.h>

#define FREAD_BUF_SIZE 4096

int read_parse_file (char *parse_filename)
{
    FILE *finptr;
    int start_file = 1, i, readbytes;
    char buf[FREAD_BUF_SIZE];

    finptr = fopen(filename, "r");
    if (!finptr) {
        printf("failed to open file %s ...exiting\n", parse_filename);
        return -1;
    }

    foutptr = fopen(out_filename, "wx");
    if (!foutinptr) {
        printf("failed to open file %s ...exiting\n", out_filename);
        return -1;
    }

    readbytes = fread(buf, 1, FREAD_BUF_SIZE, finptr);
    if (readbytes < FREAD_BUF_SIZE) {
        if (ferror(finptr)) {
            printf("error in reading file %s ...exiting\n", filename);
            exit(-1);
        }
        if (feof(finptr)) {
            end_of_file = 1;
        }
    }

    for (i = 0; i < readbytes; i++) {
        if (start_file) {
            if (buf[0] != '%' && buf[1] != '{' && buf[2] != '\n') {
                printf("parse file error : start has to be %{ ...exiting\n");
                exit(-1);
            } else {
                start_file = 0;
                start_block = 1;
            }
        }

        if (start_block) {
            




       
