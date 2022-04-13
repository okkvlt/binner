#include <stdio.h>
#include <string.h>
#include "handler.h"

// coded by okkvlt
// use ./binner [bin_file]

/*
 * use: $ ./binner --help
 */

int main(int argc, char **argv)
{
    banner();

    if (argc == 1)
    {
        puts("\033[0;32m[!]\033[0m Use: $ ./binner -h");
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        puts("\033[0;32m[!] Options:");
        puts("\033[0;32m[!]   File info:\033[0m -f, --fileinfo [bin_file]");
        puts("\033[0;32m[!]   Byte reader:\033[0m -r, --read [bin_file]");
        puts("\033[0;32m[!]   Byte changer:\033[0m -c, --change [bin_file]");
        puts("\033[0;32m[!]   Byte inserter:\033[0m -i, --insert [bin_file]");
        puts("\033[0;32m[!]   Byte deleter:\033[0m -d, --delete [bin_file]");
        return 0;
    }
    if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--change") == 0)
    {
        change_binary_file(argv[2]);
        return 0;
    }
    if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--read") == 0)
    {
        show_bytes_from_buffer(argv[2]);
        return 0;
    }
    if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--fileinfo") == 0)
    {
        get_info_from_binary(argv[2]);
        return 0;
    }
    if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--insert") == 0)
    {
        insert_or_delete_byte_at(argv[2], 1);
        return 0;
    }
    if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--delete") == 0)
    {
        insert_or_delete_byte_at(argv[2], 2);
        return 0;
    }

    puts("\033[1;31m[-] Invalid argument!\033[0m");
    puts("\033[0;32m[!]\033[0m Use: $ ./binner -h");

    return 0;
}