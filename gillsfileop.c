#include <stdio.h>
#include "gillsfileop.h"

FILE *infile = NULL;

gills_fread_t* gills_fileropen (char *filepath, int readbufsize,
               int min_fread_len)
{
    gills_fread_t *gf;

    gf = (gills_fread_t *)calloc(1, sizeof(gills_fread_t));
    if (!gf) {
        printf("memory alloc failed for gills_fread_t\n");
        return NULL;
    }
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return NULL;
    }
    gf->min_bufread_len = min_fread_len;
    gf->infilepath = (char *)calloc(strlen(filepath), sizeof(char));
    strcpy(gf->infilepath, filepath);

    gf->infileptr = fopen(gf->infilepath, "r");
    if (!gf->infileptr) {
        printf("File open for file %s failed: ", gf->infilepath);
        perror("");
        return NULL;
    }

    gf->readbuf = NULL;
    gf->readbuf_size = readbufsize;
    gf->readbuf = (char *)calloc(gf->readbuf_size, sizeof(char));

    if (!gf->readbuf) {
        printf("calloc failed for file read buffer ...exiting\n");
        return NULL;
    }

    return gf;
}

gills_fwrite_t* gills_filewopen (char *filepath)
{
    gills_fwrite_t* gf;

    gf = (gills_fwrite_t *)calloc(1, sizeof(gills_fwrite_t));
    if (!gf) {
        printf("gills_fwrite_t memory calloc failed\n");
        return NULL;
    }
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return NULL;
    }
    gf->outfilepath = (char *)calloc(strlen(filepath), sizeof(char));
    if (!gf->outfilepath) {
        printf("mem allocation for filepath %s for write file failed ...exiting",
               filepath);
        return NULL;
    }
    gf->outfileptr = fopen(gf->outfilepath, "w");
    if (!gf->outfileptr) {
        printf("File open for write for file %s failed", gf->outfilepath);
        perror(": ");
        return NULL;
    }
    return gf;
}

gills_fwrite_t* gills_filewopen_buffered (char *filepath,
                              int writebuf_size,
                              int min_write_len)
{
    gills_fwrite_t* gf;

    gf = (gills_fwrite_t *)calloc(1, sizeof(gills_fwrite_t));
    if (!gf) {
        printf("gills_fwrite_t memory calloc failed\n");
        return NULL;
    }
    if (!filepath || filepath[0] == '\0') {
        printf("No file path for input file ...exiting\n");
        return NULL;
    }
    gf->outfileptr = fopen(gf->outfilepath, "w");
    if (!gf->outfileptr) {
        printf("File open for write for file %s failed", gf->outfilepath);
        perror(": ");
        return NULL;
    }
    gf->outfilepath = (char *)calloc(strlen(filepath), sizeof(char));
    if (!gf->outfilepath) {
        printf("mem allocation for filepath %s for write file failed ...exiting",
               filepath);
        return NULL;
    }
    gf->writebuf_size = writebuf_size;
    gf->min_bufwrite_len = min_write_len;
    gf->writebuf = (char *)calloc(gf->min_bufwrite_len, sizeof(char));
    if (!gf->writebuf) {
        printf("Mem alloc failed for file write for file %s ...exiting\n",
               gf->outfilepath);
        return NULL;
    }
    return gf;
}

int gills_fwrite_buffered (gills_fwrite_t *gf, char *buf, int buflen)
{
    int len;

    if (gf->writebuf_size - gf->head_idx > buflen) {
        memcpy(gf->writebuf + gf->head_idx, buf, buflen);
        gf->head_idx += buflen;
    }
    if (gf->writebuf_size - gf->head_idx < gf->min_bufwrite_len) {
        len = fwrite(gf->writebuf, 1, gf->head_idx, gf->outfileptr);
        if (len < gf->head_idx) {
            gf->outfile_error = ferror(gf->outfileptr);
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
    int len, readbuf_len;
    int i;

    readbuf_len = gf->end_idx - gf->head_idx;

    if (readbuf_len >= gf->min_bufread_len ||
        (gf->infile_eof && (gf->end_idx - gf->head_idx))) {
        return GILLS_MIN_FREAD_BUF_AVAIL;
    }

    if (gf->infile_eof && !(gf->end_idx - gf->head_idx)) {
        return GILLS_FEOF_BUFEMPTY;
    }

    if (readbuf_len > gf->readbuf_size - gf->end_idx) {
        for (i = 0; i < readbuf_len; i++) {
             gf->readbuf[i] = gf->readbuf[gf->head_idx + i];
        }
        gf->end_idx = readbuf_len;
        gf->head_idx = 0;
    }

/*
    memmove(gf->readbuf, gf->readbuf + gf->head_idx,
           gf->readbuf_size - gf->head_idx);
*/
//    gf->infile_error = gf->infile_eof = 0;
//    clearerr(gf->infileptr);
    len = fread(gf->readbuf + gf->end_idx, 1, gf->min_bufread_len - readbuf_len, gf->infileptr);
    if (gf->infile_eof)
        gf->readbuf[gf->end_idx + len - 1] = '\0';
    else
        gf->readbuf[gf->end_idx + len] = '\0';

    if (len < gf->readbuf_size) {
        if (ferror(gf->infileptr)) {
            printf("File read on file %s failed", gf->infilepath);
            perror(": ");
            gf->infile_error = 1;
            return -1;
        }
        gf->infile_eof = feof(gf->infileptr);
    }

//    gf->end_idx += (len - readbuf_len);
    gf->end_idx += gf->infile_eof ? len - 1 : len;
    gf->readbuf[gf->end_idx] = '\0';

    return 0;
}

inline int gills_fread_buf_consume (gills_fread_t *gf, int len)
{
    gf->head_idx += len;
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
    if (gf->write_len < buflen) {
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
    free(gf);
}

int gills_fwclose (gills_fwrite_t *gf)
{
    fclose(gf->outfileptr);
    free(gf->outfilepath);
    if (gf->writebuf)
        free(gf->writebuf);
    free(gf);
}  

