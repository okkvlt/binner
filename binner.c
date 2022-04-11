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

int main(int argc, char **argv)
{
    if (argc == 1)
        return 0;

    FILE *f = fopen(argv[1], "rb");

    long len = len_offsets(f);
    unsigned char *buff = buffer(f, len);

    int lines = len / 16;

    struct bytes_char
    {
        int bytes[len];
        char c[len];
    };

    typedef struct bytes_char BYTES;

    BYTES *dict_bytes = malloc(sizeof(BYTES));

    for (int i = 0; i < len; i++)
    {
        dict_bytes->bytes[i] = (int)buff[i];
        dict_bytes->c[i] = buff[i];
    }

    for (int i = 0; i < lines + 1; i++)
    {
        printf("|%.8x| ", i * 16);

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (dict_bytes->bytes[j] < 256)
                printf("%.2x ", dict_bytes->bytes[j]);
        }

        printf(" |");

        for (int j = i * 16; j < 16 * (i + 1); j++)
        {
            if (dict_bytes->c[j] < 256)
            {
                if (dict_bytes->c[j] > 32 && dict_bytes->c[j] < 127)
                    printf("%c", dict_bytes->c[j]);
                else if (dict_bytes->c[j] == '\0')
                    printf(".");
                else if (dict_bytes->c[j] == '\0')
                    printf(" ");
                else
                    putchar('?');
            }
        }

        putchar('|');
        putchar('\n');
    }

    return 0;
}