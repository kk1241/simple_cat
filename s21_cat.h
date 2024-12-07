#ifndef s21_cat
#define s21_cat

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct {
    int flag_e;
    int flag_n;
    int flag_b;
    int flag_s;
    int flag_t;
    int flag_v;
} catFlags;

int parse_flags(int argc, char **argv, catFlags *flags);
void print_text(FILE *file_name, catFlags flags);


#endif 