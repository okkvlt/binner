#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// coded by okkvlt
// use ./binner [bin_file]

/*
 * byte reader: $ ./binner -r [bin_file]
 * byte changer: $ ./binner -c [bin_file]
 */

long len_offsets(FILE *f)
{
    long len;

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    rewind(f);

    return len;
}

unsigned char *buffer(FILE *f, long len)
{
    unsigned char *buffer = (char *)malloc(len * sizeof(char));

    fread(buffer, len, 1, f);

    return buffer;
}

void show_bytes_from_buffer(unsigned char *buff, long len)
{
    int lines = len / 16;

    if (lines * 16 < len)
        lines++;

    for (int i = 0; i < lines; i++)
    {
        printf("|0x%.8x| ", i * 16);

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (j < len && buff[j] < 256)
                printf("%.2x ", buff[j]);
        }

        printf(" |");

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (j < len && buff[j] < 256)
            {
                if (buff[j] > 32 && buff[j] < 127)
                    printf("%c", (char)buff[j]);
                else if (buff[j] == '\0')
                    printf(".");
                else if (buff[j] == '\n' || buff[j] == 32)
                    printf(" ");
                else
                    putchar('?');
            }
        }

        putchar('|');
        putchar('\n');
    }

    printf("|0x%.8x| ", len);
}

unsigned char *change_byte_from_buff(unsigned char *buff, long len, int pos, int byte)
{
    if (!(pos < 0) || !(pos > len))
    {
        buff[pos] = byte;

        return buff;
    }
}

void change_binary_file(FILE *f, unsigned char *buff, long len, int pos, int byte)
{
    puts("\n[!] changing byte!");

    unsigned char *new_buff = change_byte_from_buff(buff, len, pos, byte);

    fwrite(new_buff, 1, sizeof(char) * len, f);

    puts("[+] byte changed!");
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        puts("[!] Syntax:");
        puts("[!]   Byte reader: $ ./binner -r [bin_file]");
        return 0;
    }

    FILE *f = fopen(argv[2], "rb");

    if (!f)
        return 0;

    long len = len_offsets(f);
    unsigned char *buff = buffer(f, len);

    fclose(f);

    if (argv[1][0] == '-' && argv[1][1] == 'c')
    {
        int pos;
        int byte;

        printf("[!] Escreva o endereço (em hexa) [ex. 0x10a0]: ");
        scanf("%x", &pos);
        printf("[!] Escreva o novo byte (em hexa) [ex. 0x75]: ");
        scanf("%x", &byte);

        FILE *f = fopen(argv[2], "wb");
        change_binary_file(f, buff, len, pos, byte);
        fclose(f);
    }
    else if (argv[1][0] == '-' && argv[1][1] == 'r')
        show_bytes_from_buffer(buff, len);
    else
        puts("[-] invalid option!");

    return 0;
}