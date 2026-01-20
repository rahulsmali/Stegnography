#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{   
    /* Name of the stego image file (input BMP image) */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* File pointer for output secret file */
    char *output_fname;

    /* Size of the secret file extension (example: ".txt" = 4) */
    FILE *fptr_output;

    int extn_size;
    /* Secret file extension string (example: ".txt") */
    char extn[5];
    /* Size of the secret file data in bytes */
    long file_size;

} DecodeInfo;

/*Decoding function prototype*/

/* Read and validate command line arguments for decoding */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Open stego image file and output file */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode and verify magic string from stego image */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode size of secret file extension */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension (like .txt, .c) */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode size of secret file data */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data and write to output file */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Perform complete decoding process */
Status do_decoding(DecodeInfo *decInfo);

#endif
