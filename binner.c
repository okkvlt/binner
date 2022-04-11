#include <stdio.h>
#include <stdlib.h>

//coded by okkvlt
//use ./binner [bin_file]

/*
 * soon I will add the function to change bytes.
 */

long len_offsets(FILE *f)
{
    long len;

    fseek(f, 0, SEEK_END);
    len = ftell(f);

    return len;
}

unsigned char *buffer(FILE *f, long len)
{
    unsigned char *buffer;

    rewind(f);

    buffer = (char *)malloc(len * sizeof(char));

    fread(buffer, len, 1, f);

    return buffer;
}

void show_bytes_from_buffer(unsigned char *buff, long len)
{
    int lines = len / 16;
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
                    printf("%c", (char) buff[j]);
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

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        puts("Syntax:");
        puts("  Byte reader: $ ./binner -r [bin_file]");
        return 0;
    }

    FILE *f = fopen(argv[2], "rb");

    long len = len_offsets(f);
    unsigned char *buff = buffer(f, len);

    show_bytes_from_buffer(buff, len);

    return 0;
}