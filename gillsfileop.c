#include <stdio.h>
#include "gillsfileop.h"

FILE *infile = NULL;

int gills_fileropen (gills_fread_t *gf, char *filepath, int readbufsize,
               int min_fread_len)
{
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return -1;
    }
    gf->min_fread_len = min_fread_len;
    gf->infilepath = (char *)calloc(strlen(filepath), sizeof(char));
    strcpy(gf->infilepath, filepath);

    gf->infileptr = fopen(gf->infilepath, "r");
    if (!gf->infileptr) {
        printf("File open for file %s failed: ", gf->filepath);
        perror("");
        return -1;
    }

    gf->readbuf = NULL;
    gf->readbuf_size = readbufsize;
    gf->readbuf = (char *)calloc(gf->readbuf_size, sizeof(char));

    if (!gf->readbuf) {
        printf("calloc failed for file read buffer ...exiting\n");
        return -1;
    }

    return 0;
}

int gills_filewopen (gills_fwrite_t *gf, char *filepath)
{
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return -1;
    }
    gf->outfilepath = (char *)calloc(strlen(filepath), sizeof(char));
    if (!gf->outfilepath) {
        printf("mem allocation for filepath %s for write file failed ...exiting",
               filepath);
        return -1;
    }
    gf->outfile = fopen(gf->outfilepath, "w");
    if (!gf->outfileptr) {
        printf("File open for write for file %s failed", gf->outfilepath);
        perror(: ");
        return -1;
    }
    return 0;
}

int gills_filewopen_buffered (gills_fwrite_t *gf, char *filepath,
                              int writebuf_size,
                              int min_write_len)
{
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return -1;
    }
    gf->outfile = fopen(gf->outfilepath, "w");
    if (!gf->outfileptr) {
        printf("File open for write for file %s failed", gf->outfilepath);
        perror(: ");
        return -1;
    }
    gf->outfilepath = (char *)calloc(strlen(filepath), sizeof(char));
    if (!gf->outfilepath) {
        printf("mem allocation for filepath %s for write file failed ...exiting",
               filepath);
        return -1;
    }
    gf->writebuf_size = writebuf_size;
    gf->min_bufwrite_len = min_bufwrite_len;
    gf->writebuf = (char *)calloc(gf->min_bufwrite_len, sizeof(char));
    if (!gf->writebuf) {
        printf("Mem alloc failed for file write for file %s ...exiting\n",
               gf->outfilepath);
        return -1;
    }
    return 0;
}

int gills_fwrite_buffered (gills_fwrite_t *gf, char *buf, int buflen)
{
    if (gf->writebuf_size - gf->head_idx > buflen) {
        memcopy(gf->writebuf + gf->head_idx, buf, buflen);
        gf->head_idx += buflen;
    }
    if (gf->writebuf_size - gf->head_idx < gf->min_bufwrite_len) {
        len = fwrite(gf->writebuf, 1, gf->head_idx, gf->outfileptr);
        if (len < gf->head_idx) {
            gf->outfile_eorror = ferror(gf->outfileptr);
            if (gf->outfile_eof) {
                printf("Error in file write for file %s ", gf->outfilepath);
                perror(": ");
                return -1;
            }
        }
    }
    gf->head_idx = 0;
    return 0;
}

int gills_fread (gills_fread_t *gf)
{
    int len, start_len;

    start_len = fread->readbuf_size - fread->head_idx;

    if (start_len >= fread->min_fread_len) {
        return GILLS_MIN_FREAD_BUF_AVAIL;
    }

    memmove(fread->readbuf, fread->readbuf + fread->head_idx,
           fread->readbuf_size - fread->head_idx);

//    gf->infile_error = gf->infile_eof = 0;
//    clearerr(gf->infileptr);
    len = fread(gf->readbuf + start_len, 1, gf->readbuf_size, gf->infileptr);

    if (len < gf->readbuf_size) {
        if (ferror(gf->infileptr)) {
            printf("File read on file %s failed", gf->infilepath);
            perror(": ");
            gf->infile_error = 1;
            return -1;
        }
        gf->infile_feof = feof(gf->infileptr);
    }

    gf->bufread_len = len;
    gf->head_idx = 0;
    gf->readbuf[len] = '\0';

    return 0;
}

int gills_fwrite (gills_fwrite_t *gf, char *buf, int buflen)
{
    gf->write_len = fwrite(buf, 1, buflen, gf->outfileptr);

    if (ferror(gf->outfileptr)) {
        printf("File write failed for file %s", gf->outfilepath);
        perror(": ");
        gf->outfile_error = 1;
        return -1;
    }
    if (gf->write_len < gf->buflen) {
        gf->outfile_eof = feof(gf->outfileptr);
    }

    return 0;
}

int gills_frclose (gills_fread_t *gf)
{
    free(gf->readbuf);
    free(gf->infilepath);
    gf->readbuf_size = 0;
    fclose(gf->infileptr);
    memset(gf, 0, sizeof(gills_fread_t));
}

int gills_fwclose (gills_fwrite_t *gf)
{
    fclose(gf->outfileptr);
    free(gf->outfilepath);
    if (gf->writebuf)
        free(gf->writebuf);
    memset(gf, 0, sizeof(gills_fwrite_t));
}  

