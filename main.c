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
typedef struct
{
    char *ID;
    Array *neighbourNodes;
    size_t used;
    size_t size;
} node;

void initNode(node *a, char *ID)
{
    //printf("%ld\n", strlen(*ID));
    a->ID = ID;

    a->used = 0;
    a->size = 5;
    a->neighbourNodes = malloc(5 * sizeof(Array));
}

void addNeighbour(node *a, int index)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->neighbourNodes = realloc(a->neighbourNodes, a->size * sizeof(int));
    }
    //a->neighbourNodes[a->used++] = index;
    insertArray(a->neighbourNodes, index);
}

//============================================================================================

typedef struct
{
    node *array;
    size_t used;
    size_t size;
} nodeArray;

void initNodeArray(nodeArray *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(node));
    a->used = 0;
    a->size = initialSize;
}

int insertNodeArray(nodeArray *a, node element)
{
    //printf("%ld\n", a->used);
    //printf("%s\n", element.ID);

    for(int i=0; i<a->used; i++){
        printf("%s\n", a->array[i].ID);

    }

        printf("---------------------------\n");


    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(node));
    }
    a->array[a->used] = element;
    a->used++;

    return a->used - 1;
}

void freeNodeArray(nodeArray *a)
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

    nodeArray nodes;

    initNodeArray(&nodes, 10);

    //READING STDIN
    //char file_name[50];
    char *file_name = "testCase/example_basic.stdin";
    char *line;
    size_t len = 0;
    FILE *fp;

    //printf("Enter file path: \n");
    //fgets(file_name, 50, stdin);
    //file_name[strcspn(file_name, "\n")] = 0;

    fp = fopen(file_name, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, fp) != -1)
    {
        int startIndex = 0;
        char curCheckpoint = '.';
        int beginNodeLine = 0;
        int stepLine = 0;

        for (int i = 0; i < strlen(line); i++)
        {
            // printf("%c\n", line[i]);
            // printf("%p\n", &line[i]);
            // printf("%c\n", *(&line[i]));
            //printf("%d\n", (&line + i));
            //char entry = **(line + i);

            if (line[i] == ':')
            {
                if (curCheckpoint == '.')
                {
                    //printf("%d\n", i);
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);

                    if (*substr == 'A')
                    {
                        beginNodeLine = 1;
                    }

                    if (*substr == 'I')
                    {
                        stepLine = 1;
                    }

                    node a;
                    initNode(&a, substr);

                    int index = insertNodeArray(&nodes, a);
                    // printf("%d\n", index);
                    // printf("%s\n", substr);
                    //free(substr);
                    curCheckpoint = ':';
                    startIndex = i + 1;
                }
                else
                {
                    EXIT_FAILURE;
                }
            }

            if (line[i] == ',')
            {
                if (curCheckpoint == ':')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = ',';
                    startIndex = i + 1;
                }
                else if (curCheckpoint == ',')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = ',';
                    startIndex = i + 1;
                }
                else
                {
                    EXIT_FAILURE;
                }
            }

            if (line[i] == '-')
            {
                if (curCheckpoint == ':')
                {
                    curCheckpoint = '-';
                    startIndex = i + 1;
                }
                else if (curCheckpoint == ',')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = '-';
                    startIndex = i + 1;
                }
                else
                {
                    EXIT_FAILURE;
                }
            }

            if (line[i] == '\n')
            {
                if (curCheckpoint == ':')
                {
                    if (beginNodeLine == 1)
                    {
                        printf("OK1\n");
                    }
                    if (stepLine == 1)
                    {
                        printf("OK2\n");
                    }
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = '.';
                    startIndex = 0;
                }
                else if (curCheckpoint == ',')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = '.';
                    startIndex = 0;
                }
                else if (curCheckpoint == '-')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    printf("%s\n", substr);
                    free(substr);
                    curCheckpoint = '.';
                    startIndex = 0;
                }
                else
                {
                    EXIT_FAILURE;
                }
            }
        }
        printf("==========================\n");
    }

    // for (int i = 0; i < nodes.used; i++)
    // {
    //     printf("%s\n", nodes.array[i].ID);
    // }

    printf("\n\nMax line size: %zd\n", len);
    free(line);
    fclose(fp);

    //==============================================

    return 0;
}
