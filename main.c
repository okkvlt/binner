#include <stdio.h>
#include <string.h>
#include "handler.h"

// coded by okkvlt
// use ./binner [bin_file]

/*
 * byte reader: $ ./binner -r [bin_file]
 * byte changer: $ ./binner -c [bin_file]
 */

    /*if (argc < 3)
    {
        puts("[!] Use: $ ./binner -h\033[0m");
        return 0;
    }

    if (strcmp(argv[1],"-r") != 0 && strcmp(argv[1],"-c") != 0)
        puts("\033[1;31m[-] File not found!\033[0;32m");
        puts("[!] Syntax:");
        puts("[!]   Byte reader: $ ./binner -r [bin_file]");
        puts("[!]   Byte changer: $ ./binner -c [bin_file]\033[0m");
        return 0;*/

int main(int argc, char **argv)
{
    banner();

    if(argc == 1)
    {
        puts("\033[0;32m[!]\033[0m Use: $ ./binner -h");
        return 0;
    }
    
    if(strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)
    {
        puts("\033[0;32m[!] Options:");
        puts("\033[0;32m[!]   Byte reader:\033[0m -r, --read [bin_file]");
        puts("\033[0;32m[!]   Byte changer:\033[0m -c, --change [bin_file]");
        return 0;
    }

    if (strcmp(argv[1],"-c") == 0 || strcmp(argv[1],"--change") == 0)
    {
        change_binary_file(argv[2]);
    }
    if (strcmp(argv[1],"-r") == 0 || strcmp(argv[1],"--read") == 0)
    {
        show_bytes_from_buffer(argv[2]);
    }

    return 0;
}