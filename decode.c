#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Decode 1 byte from 8 LSBs */
char decode_byte_from_lsb(char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch |= ((image_buffer[i] & 1) << i);
    }
    return ch;
}

/* Decode 32-bit integer from 32 LSBs */
int decode_size_from_lsb(FILE *fptr_stego)
{
    int size = 0;
    unsigned char buffer[32];

    fread(buffer, 32, 1, fptr_stego);

    for (int i = 0; i < 32; i++)
    {
        size |= ((buffer[i] & 1) << i);
    }
    return size;
}

/* Validate decode args */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{

    /* Check Stego image */
    if (!strstr(argv[2], ".bmp"))
        return e_failure;

    decInfo->stego_image_fname = argv[2];
    decInfo->output_fname = argv[3];

    return e_success;
}

/* Open files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    char buffer[8];
    char magic[3];
    magic[2] = '\0';

    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        magic[i] = decode_byte_from_lsb(buffer);
    }

    if (strcmp(magic, MAGIC_STRING) == 0)
        return e_success;

    return e_failure;
}

/* Decode extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decInfo->extn_size = decode_size_from_lsb(decInfo->fptr_stego_image);

    printf("Decoded extension size = %d\n", decInfo->extn_size);

    return e_success;
}

/* Decode extension string */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    for (int i = 0; i < decInfo->extn_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decInfo->extn[i] = decode_byte_from_lsb(buffer);
    }

    decInfo->extn[decInfo->extn_size] = '\0';

    return e_success;
}

/* Decode file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->file_size = decode_size_from_lsb(decInfo->fptr_stego_image);

    printf("Decoded secret file size = %ld bytes\n", decInfo->file_size);

    return e_success;
}

/* Decode and write file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];

    for (int i = 0; i < decInfo->file_size; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        char ch = decode_byte_from_lsb(buffer);
        fputc(ch, decInfo->fptr_output);
    }

    return e_success;
}

/* Complete decoding process */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Files opened successfully\n");

        if (decode_magic_string(decInfo) == e_success)
        {
            printf("Magic string decoded successfully\n");

            if (decode_secret_file_extn_size(decInfo) == e_success)
            {
                printf("Decoded secret file extension size successfully\n");

                if (decode_secret_file_extn(decInfo) == e_success)
                {
                    printf("Decoded secret file extension successfully\n");
                    printf("Decoded extension: %s\n", decInfo->extn);

                    if (decode_secret_file_size(decInfo) == e_success)
                    {
                        printf("Decoded secret file size successfully\n");

                        if (decode_secret_file_data(decInfo) == e_success)
                        {
                            printf("Decoded secret file data successfully\n");
                            printf("Decoding completed successfully\n");
                        }
                        else
                        {
                            printf("Failed to decode secret file data\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to decode secret file extension\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to decode secret file extension size\n");
                return e_failure;
            }
        }
        else
        {
            printf("Magic string mismatch! Not a valid stego image\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open decode files\n");
        return e_failure;
    }

    return e_success;
}

