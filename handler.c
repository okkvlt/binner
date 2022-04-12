#include <stdio.h>
#include <stdlib.h>
#include "handler.h"

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

void show_bytes_from_buffer(char *filename)
{
    FILE *f;

    long len;
    unsigned char *buff;

    int lines;

    f = fopen(filename, "rb");

    if (!f)
    {
        puts("\033[1;31m[-] File not found!\033[0m");
        exit(0);
    }

    len = len_offsets(f);
    buff = buffer(f, len);

    fclose(f);

    lines = len / 16;

    if (lines * 16 < len)
        lines++;

    for (int i = 0; i < lines; i++)
    {
        printf("\033[0;32m|0x%.8x| \033[0m", i * 16);

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (j < len && buff[j] <= 0xff)
                printf("%.2x ", buff[j]);
        }

        printf("\033[0;32m |");

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (j < len && buff[j] <= 0xff)
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

    printf("|0x%.8x| \033[0m", len);
}

unsigned char *change_byte_from_buff(unsigned char *buff, long len, int pos, int byte)
{
    buff[pos] = byte;
    return buff;
}

void change_binary_file(char *filename)
{
    int pos;
    int byte;

    long len;

    unsigned char *buff;
    unsigned char *new_buff;

    FILE *f;

    f = fopen(filename, "rb");

    if (!f)
    {
        puts("\033[1;31m[-] File not found!\033[0m");
        exit(0);
    }

    len = len_offsets(f);
    buff = buffer(f, len);

    fclose(f);

    printf("\033[0;32m[!] Address (in hexadecimal) [ex. 0x10a0]: ");
    scanf("%x", &pos);
    printf("[!] New byte (in hexadecimal) [ex. 0x75]: ");
    scanf("%x", &byte);

    if (pos < 0 || pos > len)
    {
        puts("\033[1;31m[-] Invalid address!\033[0m");
        exit(0);
    }

    if (byte > 0xff)
    {
        puts("\033[1;31m[-] Invalid byte!\033[0m");
        exit(0);
    }

    puts("\n\033[0;32m[!] Changing byte!");

    f = fopen(filename, "wb");

    new_buff = change_byte_from_buff(buff, len, pos, byte);
    fwrite(new_buff, 1, sizeof(char) * len, f);

    puts("\033[1;32m[+] Byte changed!\033[0m");

    fclose(f);
}

void get_info_from_binary(char *filename)
{
    FILE *f;

    long len;
    unsigned char *buff;

    int elf[] = {0x7f, 0x45, 0x4c, 0x46};
    int pe[] = {0x4d, 0x5a, 0x90, 0x00};

    f = fopen(filename, "rb");

    if (!f)
    {
        puts("\033[1;31m[-] File not found!\033[0m");
        exit(0);
    }

    len = len_offsets(f);
    buff = buffer(f, len);

    fclose(f);

    if (buff[0] == elf[0] && buff[1] == elf[1] && buff[2] == elf[2] && buff[3] == elf[3])
    {
        puts("\033[0;32m[!] File pattern: \033[0mELF");
    }
    
    if (buff[0] == pe[0] && buff[1] == pe[1] && buff[2] == pe[2] && buff[3] == pe[3])
    {
        puts("\033[0;32m[!] File pattern: \033[0mWindows PE");
    }

    printf("\033[0;32m[!] Byte offset: \033[0m%x\n", len);
}