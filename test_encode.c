#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include"common.h"

int main(int argc, char *argv[])
{
    //validate cla
    if (argc < 2)
    {
        printf("Error: Not enough argument\n");
        return 1;
    }
    
    EncodeInfo E1;
    DecodeInfo D1;
    int choice = check_operation_type(argv);

    if(choice==e_encode)
    {
        printf("Encoding is selected\n");
        if(read_and_validate_encode_args(argv, &E1) == e_success)
        {
            printf("INfO: Read and validate is success\n");
            if(do_encoding(&E1)==e_success)
            {
                printf("Encoding is success\n");
            }
            else
            {
                printf("Failed to encode\n");
                return 0;
            }
        }
        else{
            printf("Failed to read and validate encode args");
            return 0;
        }
    }
    else if(choice==e_decode)
    {
        printf("decoding is selected\n");
        if(read_and_validate_decode_args(argv, &D1)==e_success)
        {
            printf("Info : Read and validate decode args success\n");

            if(do_decoding(&D1)==e_success)
            {
                printf("Decoding is success\n");
            }
            else 
            {
                printf("Failed to decode\n");
                return 0;
            }
            
        }
        else{
            printf("Failed to read and validate decode args\n");
            return 0;
        }
    }
    else if(choice==e_unsupported)
    {
        printf("invalid operation\nUsage:\n");
        printf("For Encoding : ./a.out -e stego.bmp outpur.txt stego.bmp\n");
        printf("For decoding : ./a.out -d stego.bmp output.txt\n");
    }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e")==0)
        return e_encode;
    else if(strcmp (argv[1], "-d")==0)
        return e_decode;
    else
        return e_unsupported;
}
    