#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


#include "bubble_sort.h"

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
    char** copy_of_str_indexes;
};

int ReadFromFile(Text* data, const int file_descriptor);
int Compar(const void* a, const void* b);
int Compar_Reverse(const void* a, const void* b);

int main()
{
    Text data = {NULL, NULL, NULL};

    int file_descriptor = open("onegin.txt", O_RDONLY | O_BINARY);
    int real_num_of_strs = ReadFromFile(&data, file_descriptor);

    if (real_num_of_strs < 0) {printf("Error: input file isn't opened"); return 1;}

    FILE *file_out = fopen("onegin_out.txt", "w");
    if (file_out == NULL) {printf("Error: output file isn't opened"); return 1;}

    for (int i = 0; i < real_num_of_strs; i++)
        data.copy_of_str_indexes[i] = data.str_indexes[i];

    if (real_num_of_strs != 0)
        BubbleSorting(data.str_indexes, (size_t)real_num_of_strs, sizeof(data.str_indexes[0]), Compar);

    fprintf(file_out, "<<<---SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", data.str_indexes[i]);

    qsort(data.str_indexes, (size_t)real_num_of_strs, sizeof(data.str_indexes[0]), Compar_Reverse);

    fprintf(file_out, "<<<---REVERSE SORTED TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", data.str_indexes[i]);

    fprintf(file_out, "\n<<<---ORIGINAL TEXT --->>>\n");
    for (int i = 0; i < real_num_of_strs; i++)
        fprintf(file_out, "%s\n", data.copy_of_str_indexes[i]);

    fclose(file_out);
    free(data.buffer);
    free(data.str_indexes);
    free(data.copy_of_str_indexes);
}

int Compar(const void* a, const void* b)
{
    size_t i = 0;
    size_t j = 0;

    const char* lhs = *(const char* const*) a;
    const char* rhs = *(const char* const*) b;

    while (lhs[i] != '\0' && rhs[j] != '\0')
    {
        if (isalpha(lhs[i]) == 0)
        {
            i++;
            continue;
        }
        if (isalpha(rhs[j]) == 0)
        {
            j++;
            continue;
        }
        if (tolower(lhs[i]) != tolower(rhs[j])) return tolower(lhs[i]) - tolower(rhs[j]);
        i++;
        j++;
    }
    while(lhs[i] != '\0' && !isalpha(lhs[i])) i++;
    while(rhs[j] != '\0' && !isalpha(rhs[j])) j++;
    return lhs[i] - rhs[j];
}

int Compar_Reverse(const void* a, const void* b)
{
    int i = 0;
    int j = 0;

    const char* lhs = *(const char* const*) a;
    const char* rhs = *(const char* const*) b;
    while (lhs[i] != '\0') i++;
    while (rhs[j] != '\0') j++;
    while (i > 0 && j > 0)
    {
        if (isalpha(lhs[i]) == 0)
        {
            i--;
            continue;
        }
        if (isalpha(rhs[j]) == 0)
        {
            j--;
            continue;
        }
        if (tolower(lhs[i]) != tolower(rhs[j])) return tolower(lhs[i]) - tolower(rhs[j]);

        i--;
        j--;
    }
    while(i >= 0 && !isalpha(lhs[i])) i--;
    while(j >= 0 && !isalpha(rhs[j])) j--;
    if (i < 0 && j < 0) return 0;
    else return (i < 0 ? -1: 1);
}

int ReadFromFile(Text* data, const int file_descriptor)
{
    struct stat file_stat;

    if (file_descriptor == -1)
        ERR_EXIT(ERR_FILE_NOT_OPENED, file_descriptor);

    if (fstat(file_descriptor, &file_stat) == -1)
        ERR_EXIT(ERR_FSTAT_FAIL, file_descriptor);
    else if (MAX_FILE_SIZE < file_stat.st_size + 1)
        ERR_EXIT(ERR_FILE_TOO_BIG, file_descriptor);
    else data->buffer = (char*)malloc((size_t)file_stat.st_size + 1);
    if (data->buffer == NULL)
        ERR_EXIT(ERR_BUFFER_NOT_CREATED, file_descriptor);

    ssize_t real_file_size = read(file_descriptor, data->buffer, (unsigned int)file_stat.st_size);
    if (real_file_size <= 0) ERR_EXIT(ERR_FILE_DID_NOT_READ, file_descriptor);

    unsigned int num_strs = 0;
    for (int i = 0; i < real_file_size; i++) {if (data->buffer[i] == '\r') num_strs++;}
    num_strs++;

    data->str_indexes = (char**)calloc(num_strs, sizeof(char*));
    data->copy_of_str_indexes = (char**)calloc(num_strs, sizeof(char*));

    int buffer_i = 0;
    int indexes_i = 0;

    data->str_indexes[indexes_i] = data->buffer;
    indexes_i++;

    while (buffer_i < real_file_size)
    {
        if (indexes_i >= NUM_OF_STRS) break;

        if ((data->buffer)[buffer_i] == '\r' && buffer_i <= real_file_size - 1)
        {
            (data->buffer)[buffer_i] = '\0';
            if (buffer_i <= real_file_size - 2)
            {
                data->str_indexes[indexes_i] = &(data->buffer)[buffer_i + 2];
                indexes_i++;
            }
            buffer_i += 2;
        }
        else buffer_i++;
    }

    (data->buffer)[real_file_size] = '\0';

    if (*data->str_indexes[indexes_i - 1] == '\0') indexes_i--;
    for (int i = 0; i < indexes_i; i++) data->copy_of_str_indexes[i] = data->str_indexes[i];


    close(file_descriptor);
    return indexes_i;
}
