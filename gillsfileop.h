#ifndef __GILLS_FILEOP_H__
#define __GILLS_FILEOP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define GILLS_FREAD_BUF_DEF_SIZE    4096
#define GILLS_FREAD_MIN_DEF_SIZE    1024

#define GILLS_FWRITE_BUF_DEF_SIZE    4096
#define GILLS_FWRITE_MIN_DEF_SIZE    1024

#define GILLS_MIN_FREAD_BUF_AVAIL  -2
#define GILLS_FEOF_BUFEMPTY        -3
#define GILLS_FERROR               -4

typedef struct gills_fread_ {
    char *infilepath;
    FILE *infileptr;
    int readbuf_size;
    char *readbuf;
    int min_bufread_len;
    int infile_eof;
    int infile_error;
    int head_idx;
    int end_idx;
} gills_fread_t;

typedef struct gills_fwrite_ {
    char *outfilepath;
    FILE *outfileptr;
    int write_len;
    int outfile_error;
    int outfile_eof;
    int writebuf_size;
    char *writebuf;
    int head_idx;
    int min_bufwrite_len;
//    int outfile_error;
} gills_fwrite_t;

gills_fread_t* gills_fileropen(char *filepath, int readbufsize,
               int min_fread_len);
int gills_fread_buf_consume(gills_fread_t *gf, int len);
gills_fwrite_t* gills_filewopen(char *filepath);
gills_fwrite_t* gills_filewopen_buffered(char *filepath,
                              int writebuf_size,
                              int min_write_len);
int gills_fwrite_buffered(gills_fwrite_t *gf, char *buf, int buflen);
int gills_fread(gills_fread_t *gf);
int gills_fwrite(gills_fwrite_t *gf, char *buf, int buflen);
int gills_frclose(gills_fread_t *gf);
int gills_fwclose(gills_fwrite_t *gf);

#endif /* __GILLS_FILEOP_H__ */
