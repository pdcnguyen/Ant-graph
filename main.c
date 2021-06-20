#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//========================================================================================
typedef struct
{
    int *array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}

void insertArray(Array *a, int element)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

//============================================================================================

int main()
{

    //    Array a;
    //    int i;

    //    initArray(&a, 5);  // initially 5 elements
    //    for (i = 0; i < 100; i++){
    //        insertArray(&a, i);  // automatically resizes as necessary
    //    }
    //    printf("%d\n", a.array[9]);  // print 10th element
    //    printf("%d\n", a.used);  // print number of elements
    //    freeArray(&a);

    //char file_name[50];
    char *file_name = "testCase/example_basic.stdin";
    char *line;
    size_t len = 0;
    FILE *fp;

    fp = fopen(file_name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, fp) != -1)
    {
        int startPoint = 0;
        char curCheckpoint = '.';

        for (int i = 0; i < strlen(line); i++)
        {
            // printf("%c\n", line[i]);
            // printf("%p\n", &line[i]);
            // printf("%c\n", *(&line[i]));
            //printf("%d\n", (&line + i));
            //char entry = **(line + i);

            if(line[i] == ':'){
                if(curCheckpoint == '.'){
                    printf("%d\n", i);
                    char* substr = malloc(i-startPoint);
                    strncpy(substr, line+startPoint, i-startPoint);
                    printf("%s\n", substr);
                    free(substr);
                    printf("%s ahihi\n", substr);

                }else{
                    EXIT_FAILURE;
                }
            }
        }
    }

    printf("\n\nMax line size: %zd\n", len);

    fclose(fp);
    return 0;
}
