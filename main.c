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

void initArray(Array *a)
{
    a->array = malloc(5 * sizeof(int));
    a->used = 0;
    a->size = 5;
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
    size_t timesVisited;
} node;

void initNode(node *a, char *ID)
{
    a->ID = ID;
    a->timesVisited = 0;
    a->neighbourNodes = malloc(sizeof(Array));

    initArray(a->neighbourNodes);
}

void addNeighbour(node *a, int neighbourIndex)
{
    for (int i = 0; i < a->neighbourNodes->used; i++)
    {
        if (a->neighbourNodes->array[i] == neighbourIndex)
        {
            printf("Neighbour of index %d already added in %s\n", neighbourIndex, a->ID);
        }
    }

    printf("Add index %d as a neighbour of %s\n", neighbourIndex, a->ID);

    insertArray(a->neighbourNodes, neighbourIndex);
}

void updateTimeVisited(node *a, int timeVisited)
{
    a->timesVisited = timeVisited;

    printf("Update time visited of Node %s to %d\n", a->ID, timeVisited);
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

int insertNodeArray(nodeArray *a, char *ID)
{
    printf("Add Node %s\n", ID);

    for (int i = 0; i < a->used; i++)
    {
        if (strcmp(ID, a->array[i].ID) == 0)
        {
            printf("Node already exists at index %d\n", i);

            return i;
        }
    }

    printf("Adding Node %s\n", ID);

    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(node));
    }

    node newNode;
    initNode(&newNode, ID);

    a->array[a->used] = newNode;
    a->used = a->used + 1;

    return a->used - 1;
}

void freeNodeArray(nodeArray *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
//============================================================================================

void processInput(nodeArray *nodes, size_t *startingNodeIndex, size_t *numberOfSteps)
{

    char *line;
    size_t len = 0;

    while (getline(&line, &len, stdin) != -1)
    {
        int startIndex = 0;
        char curCheckpoint = '.';

        int beginNodeLine = 0;
        int stepNumLine = 0;

        size_t currentNodeIndex = 0;

        for (int i = 0; i < strlen(line); i++)
        {

            if (line[i] == ':')
            {
                if (curCheckpoint == '.')
                {
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);

                    if (*substr == 'A')
                    {
                        beginNodeLine = 1;
                        curCheckpoint = ':';
                        startIndex = i + 1;
                        printf("Starting node line\n");

                        continue;
                    }

                    if (*substr == 'I')
                    {
                        stepNumLine = 1;
                        curCheckpoint = ':';
                        startIndex = i + 1;
                        printf("Number of steps line\n");

                        continue;
                    }

                    currentNodeIndex = insertNodeArray(nodes, substr);
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
                if (curCheckpoint == ':' || curCheckpoint == ',')
                {
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    int index = insertNodeArray(nodes, substr);
                    addNeighbour(&(nodes->array[currentNodeIndex]), index);
                    addNeighbour(&(nodes->array[index]), currentNodeIndex);
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
                    int index = insertNodeArray(nodes, substr);
                    addNeighbour(&(nodes->array[currentNodeIndex]), index);
                    addNeighbour(&(nodes->array[index]), currentNodeIndex);
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
                        char *substr = malloc(i - startIndex + 1);
                        strncpy(substr, line + startIndex, i - startIndex);
                        *startingNodeIndex = insertNodeArray(nodes, substr);
                        curCheckpoint = '.';
                        startIndex = 0;
                        continue;
                    }
                    if (stepNumLine == 1)
                    {
                        char *substr = malloc(i - startIndex + 1);
                        strncpy(substr, line + startIndex, i - startIndex);
                        *numberOfSteps = atoi(substr);
                        curCheckpoint = '.';
                        startIndex = 0;
                        continue;
                    }
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    int index = insertNodeArray(nodes, substr);
                    addNeighbour(&(nodes->array[currentNodeIndex]), index);
                    addNeighbour(&(nodes->array[index]), currentNodeIndex);
                    curCheckpoint = '.';
                    startIndex = 0;
                }
                else if (curCheckpoint == ',')
                {

                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    int index = insertNodeArray(nodes, substr);
                    addNeighbour(&(nodes->array[currentNodeIndex]), index);
                    addNeighbour(&(nodes->array[index]), currentNodeIndex);
                    curCheckpoint = '.';
                    startIndex = 0;
                }
                else if (curCheckpoint == '-')
                {
                    char *substr = malloc(i - startIndex + 1);
                    strncpy(substr, line + startIndex, i - startIndex);
                    updateTimeVisited(&(nodes->array[currentNodeIndex]), atoi(substr));
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

    printf("\n\nMax line size: %zd\n", len);
    free(line);
}

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

    size_t startingNodeIndex = 0;
    size_t numberOfSteps = 0;

    processInput(&nodes, &startingNodeIndex, &numberOfSteps);

    for (int i = 0; i < nodes.used; i++)
    {
        printf("%s\n", nodes.array[i].ID);
    }

    printf("Start at %ld\n", startingNodeIndex);
    printf("Number of steps %ld\n", numberOfSteps);

    return 0;
}
