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

void show_bytes_from_buffer(unsigned char *buff, long len)
{
    int lines = len / 16;

    if (lines * 16 < len)
        lines++;

    for (int i = 0; i < lines; i++)
    {
        printf("|0x%.8x| \033[0m", i * 16);

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (j < len && buff[j] < 256)
                printf("%.2x ", buff[j]);
        }

        printf("\033[0;32m |");

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
    puts("\n[!] Changing byte!");

    unsigned char *new_buff = change_byte_from_buff(buff, len, pos, byte);

    fwrite(new_buff, 1, sizeof(char) * len, f);

    puts("\033[1;32m[+] Byte changed!\033[0;32m");
}