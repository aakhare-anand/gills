#ifndef __GILLS_FILEOP_H__
#define __GILLS_FILEOP_H__

#define GILLS_FREAD_BUF_DEF_SIZE    4096
#define GILLS_FREAD_MIN_DEF_SIZE    1024

#define GILLS_FWRITE_BUF_DEF_SIZE    4096
#define GILLS_FWRITE_MIN_DEF_SIZE    1024

#define GILLS_MIN_FREAD_BUF_AVAIL  -2

typedef struct gills_fread_ {
    char *infilepath;
    FILE *infileptr;
    int readbuf_size;
    char *readbuf;
    int bufread_len;
    int min_bufread_len;
    int infile_eof;
    int infile_error;
    int head_idx;
} gills_read_t;

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
    int outfile_error;
} gills_fwrite_t;

#endif /* __GILLS_FILEOP_H__ */
