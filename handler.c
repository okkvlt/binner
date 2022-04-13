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

    char check;

    int start;
    int end;

    int offset;

    f = fopen(filename, "rb");

    if (!f)
    {
        puts("\033[1;31m[-] File not found!\033[0m");
        exit(0);
    }

    len = len_offsets(f);
    buff = buffer(f, len);

    fclose(f);

    printf("\033[0;32m[!]\033[0m Do you want to view all bytes \033[0;32m[y/n]\033[0m? ");
    scanf("%c", &check);

    if (check == 'n')
    {
        printf("\033[0;32m[!]\033[0m Start address \033[0;32m[ex. 0x1d0]\033[0m: ");
        scanf("%x", &start);
        printf("\033[0;32m[!]\033[0m End address \033[0;32m[ex. 0x1d1]\033[0m: ");
        scanf("%x", &end);

        if (start < 0 || start > len || end < 0 || end > len || start > end || start == end)
        {
            puts("\033[1;31m[-] Invalid address(es)!\033[0m");
            exit(0);
        }
    }
    else
    {
        start = 0;
        end = len;
    }

    offset = end - start;

    lines = offset / 0x10;

    if (lines * 0x10 < offset)
        lines++;

    lines *= 16;
    lines += start;

    putchar('\n');

    for (int i = start; i < lines; i += 16)
    {
        printf("\033[0;32m|0x%.8x| \033[0m", i);
        for (int j = 0 + i; j < 16 + i; j++)
        {
            if (j < end && buff[j] <= 0xff)
                printf("%.2x ", buff[j]);
        }
        printf("\033[0;32m |");
        for (int j = 0 + i; j < 16 + i; j++)
        {
            if (j < end && buff[j] <= 0xff)
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

    printf("|0x%.8x| \033[0m", end);
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

    printf("\033[0;32m[!]\033[0m Address \033[0;32m[ex. 0x10a0]\033[0m: ");
    scanf("%x", &pos);
    printf("\033[0;32m[!]\033[0m New byte \033[0;32m[ex. 0x75]\033[0m: ");
    scanf("%x", &byte);

    if (pos < 0 || pos >= len)
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

    int format;

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
        puts("\033[0;32m[!] Format: \033[0mELF");
        format = 0;
    }

    if (buff[0] == pe[0] && buff[1] == pe[1] && buff[2] == pe[2] && buff[3] == pe[3])
    {
        puts("\033[0;32m[!] Format: \033[0mWindows PE");
        format = 1;
    }

    printf("\033[0;32m[!] Bytes offset: \033[0m%x\n", len);
}

unsigned char *insert_at(unsigned char *buff, long len, int x, int pos)
{
    if (pos < 0 || pos >= len || x > 0xff)
    {
        puts("\033[1;31m[-] Invalid insertion!\033[0m");
        exit(0);
    }

    for (int i = len; i > pos; i--)
        buff[i] = buff[i - 1];

    buff[pos] = x;

    return buff;
}

unsigned char *delete_at(unsigned char *buff, long len, int pos)
{
    if (pos < 0 || pos >= len)
    {
        puts("\033[1;31m[-] Invalid deletion!\033[0m");
        exit(0);
    }

    int aux;

    for (int i = pos; i < len - 1; i++)
    {
        aux = buff[i];
        buff[i] = buff[i + 1];
        buff[i + 1] = buff[i];
    }

    buff[len - 1] = 0x0;

    return buff;
}

void insert_or_delete_byte_at(char *filename, int mode)
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

    printf("\033[0;32m[!]\033[0m Address \033[0;32m[ex. 0x10a0]\033[0m: ");
    scanf("%x", &pos);
    if (mode == 1)
    {
        printf("\033[0;32m[!]\033[0m New byte \033[0;32m[ex. 0x75]\033[0m: ");
        scanf("%x", &byte);
    }

    puts("\n\033[0;32m[!] Working...!");

    f = fopen(filename, "wb");

    if (mode == 1)
    {
        new_buff = insert_at(buff, len, byte, pos);
        len++;
    }
    else
    {
        new_buff = delete_at(buff, len, pos);
        len--;
    }

    fwrite(new_buff, 1, sizeof(char) * len, f);

    if (mode == 1)
        printf("\033[1;32m[+] Byte %.2x inserted at position %.8x!\033[0m", byte, pos);
    else
        printf("\033[1;32m[+] Byte at position %.8x deleted!\033[0m", pos);

    fclose(f);
}