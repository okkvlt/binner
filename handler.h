#ifndef HANDLER_H
#define HANDLER_H

long len_offsets(FILE *);

unsigned char *buffer(FILE *, long);

void show_bytes_from_buffer(char *);

unsigned char *change_byte_from_buff(unsigned char *, long, int, int);

void change_binary_file(char *);

void get_info_from_binary(char *);

void banner();

#endif