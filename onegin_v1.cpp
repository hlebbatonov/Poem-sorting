#include <cerrno>
#include <corecrt.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <io.h>
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

int ReadFromFileToBuffer(Text* text, const int file_descriptor);
int GetStrsIndexesFromBuffer(Text* text);

int main()
{
    Text orig_text = {NULL, NULL};
    Text sorted_text = {NULL, NULL};
    Text rev_sorted_text = {NULL, NULL};

    int file_descriptor = open("onegin.txt", O_RDONLY | O_BINARY);
    int real_num_of_strs = ReadFromFileToBuffer(&orig_text, file_descriptor);
    if (real_num_of_strs < 0) {printf("Error: input file isn't opened"); return 1;}

    sorted_text = orig_text;
    sorted_text.lines = (Line*)calloc((size_t)sorted_text.num_of_strs, sizeof(Line));
    rev_sorted_text = orig_text;
    rev_sorted_text.lines = (Line*)calloc((size_t)rev_sorted_text.num_of_strs, sizeof(Line));

    for (int i = 0; i < orig_text.num_of_strs; i++)
    {
        sorted_text.lines[i] = orig_text.lines[i];
        rev_sorted_text.lines[i] = orig_text.lines[i];
    }


    FILE *file_out = fopen("onegin_out.txt", "w");
    if (file_out == NULL) {printf("Error: output file isn't opened"); return 1;}

    if (real_num_of_strs != 0) BubbleSorting(sorted_text.lines, (size_t)real_num_of_strs, sizeof(orig_text.lines[0]), Compar);

    fprintf(file_out, "<<<---SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", sorted_text.lines[i].str_index);

    qsort(rev_sorted_text.lines, (size_t)real_num_of_strs, sizeof(orig_text.lines[0]), Compar_Reverse);

    fprintf(file_out, "<<<---REVERSE SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", rev_sorted_text.lines[i].str_index);

    fprintf(file_out, "\n<<<---ORIGINAL TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", orig_text.lines[i].str_index);

    fclose(file_out);
    free(orig_text.buffer);
    free(orig_text.lines);
    free(sorted_text.lines);
    free(rev_sorted_text.lines);
}

int ReadFromFileToBuffer(Text* text, const int file_descriptor)
{
    struct stat file_stat = {};

    if (file_descriptor == -1)
        ERR_EXIT(ERR_FILE_NOT_OPENED, file_descriptor);

    if (fstat(file_descriptor, &file_stat) == -1)
        ERR_EXIT(ERR_FSTAT_FAIL, file_descriptor);

    else if (MAX_FILE_SIZE < file_stat.st_size + 1)
        ERR_EXIT(ERR_FILE_TOO_BIG, file_descriptor);


    else text->buffer = (char*)malloc((size_t)file_stat.st_size + 1);//буфер
    if (text->buffer == NULL) ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);

    text->text_size = read(file_descriptor, text->buffer, (unsigned int)file_stat.st_size);//размер файла, чтение в буфер
    if (text->text_size <= 0) ERR_EXIT(ERR_FILE_DID_NOT_READ, file_descriptor);

    text->num_of_strs = 0;
    for (int i = 0; i < text->text_size; i++) {if (text->buffer[i] == '\r') text->num_of_strs++;}//подсчет колва строк
    text->num_of_strs++;

    text->num_of_strs = GetStrsIndexesFromBuffer(text);

    close(file_descriptor);
    return (int)text->num_of_strs;
}

int GetStrsIndexesFromBuffer(Text* text)
{
    text->lines = (Line*)calloc((size_t)text->num_of_strs, sizeof(Line));// создание массивов индексов
    if (text->lines == NULL) return ERR_BUFFER_NOT_CREATED;

    int buffer_i = 0;
    int indexes_i = 0;
    unsigned int len_of_curr_str = 0;

    text->lines[indexes_i].str_index = text->buffer;//разрезание на строки
    indexes_i++;

    while (buffer_i < text->text_size)
    {

        if ((text->buffer)[buffer_i] == '\r' && buffer_i <= text->text_size - 1)
        {
            (text->buffer)[buffer_i] = '\0';
            (text->lines)[indexes_i - 1].len_of_str = len_of_curr_str;
            len_of_curr_str = 0;
            if (buffer_i <= text->text_size - 2)
            {
                text->lines[indexes_i].str_index = &(text->buffer)[buffer_i + 2];
                indexes_i++;
            }
            buffer_i += 2;
        }
        else
        {
            buffer_i++;
            len_of_curr_str++;
        }
    }

    if (indexes_i > 0) text->lines[indexes_i - 1].len_of_str = len_of_curr_str;

    (text->buffer)[text->text_size] = '\0';

    if (indexes_i > 0 && *text->lines[indexes_i - 1].str_index == '\0') indexes_i--;
    return indexes_i;
}
