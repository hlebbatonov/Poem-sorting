#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "bubble_sort.h"
#include "comparators.h"


#define NUM_OF_STRS 4500
#define MAX_FILE_SIZE 200 * 1024

#define ERR_EXIT(error, file_descriptor) do {close(file_descriptor); return error;} while(0)


enum read_errors
{
    ERR_FILE_NOT_OPENED = -1,
    ERR_FSTAT_FAIL = -2,
    ERR_BUFFER_NOT_CREATED = -3,
    ERR_FILE_TOO_BIG = -4,
    ERR_FILE_DID_NOT_READ = -5
};

struct Text
{
    char* buffer;
    char** str_indexes;
};

int ReadFromFile(Text* orig_text, Text* sorted_text, Text* rev_sorted_text, const int file_descriptor);


int main()
{
    Text orig_text = {NULL, NULL};
    Text sorted_text = {NULL, NULL};
    Text rev_sorted_text = {NULL, NULL};


    int file_descriptor = open("onegin.txt", O_RDONLY | O_BINARY);
    int real_num_of_strs = ReadFromFile(&orig_text, &sorted_text, &rev_sorted_text, file_descriptor);

    if (real_num_of_strs < 0) {printf("Error: input file isn't opened"); return 1;}

    FILE *file_out = fopen("onegin_out.txt", "w");
    if (file_out == NULL) {printf("Error: output file isn't opened"); return 1;}

    if (real_num_of_strs != 0) BubbleSorting(sorted_text.str_indexes, (size_t)real_num_of_strs, sizeof(orig_text.str_indexes[0]), Compar);

    fprintf(file_out, "<<<---SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", sorted_text.str_indexes[i]);

    qsort(rev_sorted_text.str_indexes, (size_t)real_num_of_strs, sizeof(orig_text.str_indexes[0]), Compar_Reverse);

    fprintf(file_out, "<<<---REVERSE SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", rev_sorted_text.str_indexes[i]);

    fprintf(file_out, "\n<<<---ORIGINAL TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", orig_text.str_indexes[i]);

    fclose(file_out);
    free(orig_text.buffer);
    free(orig_text.str_indexes);
    free(sorted_text.str_indexes);
    free(rev_sorted_text.str_indexes);
}

int ReadFromFile(Text* orig_text, Text* sorted_text, Text* rev_sorted_text, const int file_descriptor) //TODO: убрать от сюда инициализацию массива указателец
{
    struct stat file_stat = {};

    if (file_descriptor == -1)
        ERR_EXIT(ERR_FILE_NOT_OPENED, file_descriptor);

    if (fstat(file_descriptor, &file_stat) == -1)
        ERR_EXIT(ERR_FSTAT_FAIL, file_descriptor);

    else if (MAX_FILE_SIZE < file_stat.st_size + 1)
        ERR_EXIT(ERR_FILE_TOO_BIG, file_descriptor);

    else orig_text->buffer = (char*)malloc((size_t)file_stat.st_size + 1);

    if (orig_text->buffer == NULL)
        ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);

    ssize_t real_file_size = read(file_descriptor, orig_text->buffer, (unsigned int)file_stat.st_size);
    if (real_file_size <= 0) ERR_EXIT(ERR_FILE_DID_NOT_READ, file_descriptor);

    unsigned int num_strs = 0;
    for (int i = 0; i < real_file_size; i++) {if (orig_text->buffer[i] == '\r') num_strs++;}
    num_strs++;

    orig_text->str_indexes = (char**)calloc(num_strs, sizeof(char*));
    if (orig_text->str_indexes == NULL) ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);
    sorted_text->str_indexes = (char**)calloc(num_strs, sizeof(char*));
    if (sorted_text->str_indexes == NULL) ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);
    rev_sorted_text->str_indexes = (char**)calloc(num_strs, sizeof(char*));
    if (rev_sorted_text->str_indexes == NULL) ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);


    int buffer_i = 0;
    int indexes_i = 0;

    orig_text->str_indexes[indexes_i] = orig_text->buffer;
    indexes_i++;

    while (buffer_i < real_file_size)
    {
        if (indexes_i >= NUM_OF_STRS) break;

        if ((orig_text->buffer)[buffer_i] == '\r' && buffer_i <= real_file_size - 1)
        {
            (orig_text->buffer)[buffer_i] = '\0';
            if (buffer_i <= real_file_size - 2)
            {
                orig_text->str_indexes[indexes_i] = &(orig_text->buffer)[buffer_i + 2];
                indexes_i++;
            }
            buffer_i += 2;
        }
        else buffer_i++;
    }

    (orig_text->buffer)[real_file_size] = '\0';

    if (*orig_text->str_indexes[indexes_i - 1] == '\0') indexes_i--;
    for (int i = 0; i < indexes_i; i++)
    {
        sorted_text->str_indexes[i] = orig_text->str_indexes[i];
        rev_sorted_text ->str_indexes[i] = orig_text->str_indexes[i];
    }

    close(file_descriptor);
    return indexes_i;
}
