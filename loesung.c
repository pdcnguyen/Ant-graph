#define _POSIX_C_SOURCE 200809L
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

typedef struct
{
    char *ID;
    Array *neighbourNodes;
    size_t timesVisited;
    size_t listed;
} node;

typedef struct
{
    node *array;
    size_t used;
    size_t size;
} nodeArray;

//========================================================================================

void initArray(Array *a)
{
    a->array = malloc(5 * sizeof(int));
    a->used = 0;
    a->size = 5;
}

void insertNeighbourArray(nodeArray *nodes, Array *a, int index)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }

    for (int i = 0; i < a->used; i++)
    {
        if (strcmp(nodes->array[a->array[i]].ID, nodes->array[index].ID) > 0)
        {
            int tmp = a->array[i];
            a->array[i] = index;
            int tmp2 = 0;
            for (int j = i + 1; j < a->used; j++)
            {
                tmp2 = a->array[j];
                a->array[j] = tmp;
                tmp = tmp2;
            }
            a->array[a->used++] = tmp;
            return;
        }
    }

    a->array[a->used++] = index;
}

int insertArray(Array *a, int index)
{
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = realloc(a->array, a->size * sizeof(int));
    }

    for (int i = 0; i < a->used; i++)
    {
        if (a->array[i] == index)
        {
            return -1;
        }
    }

    a->array[a->used++] = index;
    return 0;
}

void freeArray(Array *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

//============================================================================================

void initNode(node *a, char *ID)
{
    a->ID = ID;
    a->timesVisited = 0;
    a->listed = 0;
    a->neighbourNodes = malloc(sizeof(Array));

    initArray(a->neighbourNodes);
}

int addNeighbour(nodeArray *nodes, node *a, int neighbourIndex)
{
    if (nodes->array[neighbourIndex].ID == a->ID)
    {
        return -1;
    }
    for (int i = 0; i < a->neighbourNodes->used; i++)
    {
        if (a->neighbourNodes->array[i] == neighbourIndex)
        {
            return 0;
        }
    }

    insertNeighbourArray(nodes, a->neighbourNodes, neighbourIndex);
    return 0;
}

int updateTimeVisited(node *a, long timeVisited)
{
    if (timeVisited > 4294967295)
    {
        return -1;
    }

    a->timesVisited = (a->timesVisited + timeVisited) % 4294967295;
    return 0;
}

void freeNode(node *a)
{
    free(a->ID);
    a->timesVisited = 0;
    freeArray(a->neighbourNodes);
    free(a->neighbourNodes);
}

//============================================================================================

void initNodeArray(nodeArray *a, size_t initialSize)
{
    a->array = malloc(initialSize * sizeof(node));
    a->used = 0;
    a->size = initialSize;
}

int insertNodeArray(nodeArray *a, char *ID)
{
    if (*ID == '\0')
    {
        free(ID);
        return -1;
    }

    for (int i = 0; i < a->used; i++)
    {
        if (strcmp(ID, a->array[i].ID) == 0)
        {
            free(ID);
            return i;
        }
    }

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
    for (int i = 0; i < a->used; i++)
    {
        freeNode(&(a->array[i]));
    }

    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
//============================================================================================

char *extractString(char *line, int startIndex, int endIndex)
{
    char *substr = malloc(endIndex - startIndex + 1);
    substr[endIndex - startIndex] = '\0';
    strncpy(substr, line + startIndex, endIndex - startIndex);

    return substr;
}

int processNeighbours(nodeArray *nodes, char *neighbour, int mainNodeIndex, Array *connectedNodes)
{
    int index = insertNodeArray(nodes, neighbour);
    if (index == -1)
    {
        return -1;
    }

    for (int i = 0; i < connectedNodes->used; i++)
    {
        if (connectedNodes->array[i] == index)
        {
            freeArray(connectedNodes);
            return -1;
        }
    }

    insertArray(connectedNodes, index);

    int status1 = addNeighbour(nodes, &(nodes->array[mainNodeIndex]), index);
    int status2 = addNeighbour(nodes, &(nodes->array[index]), mainNodeIndex);

    if (status1 == -1 || status2 == -1)
    {
        return -1;
    }
    return 0;
}

int exitFail(char *line, Array *connectedNodes)
{
    free(line);
    freeArray(connectedNodes);
    return -1;
}

int processInput(nodeArray *nodes, size_t *startingNodeIndex, size_t *numberOfSteps)
{

    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, stdin) != -1)
    {
        int startIndex = 0;
        char curCheckpoint = '.';
        int beginNodeLine = 0;
        int stepNumLine = 0;
        size_t currentNodeIndex = 0;
        Array connectedNodes;

        initArray(&connectedNodes);

        for (int i = 0; i < strlen(line); i++)
        {
            if ((96 < line[i] && line[i] < 123) || (47 < line[i] && line[i] < 58) || line[i] == 65 || line[i] == 73 || line[i] == 58 || line[i] == 44 || line[i] == 45 || line[i] == 10)
            {
                switch (line[i])
                {
                case ':':
                    if (curCheckpoint == '.')
                    {
                        char *substr = extractString(line, startIndex, i);

                        if (*substr == 'A' || *substr == 'I')
                        {
                            if (*substr == 'A')
                                beginNodeLine = 1;

                            if (*substr == 'I')
                                stepNumLine = 1;

                            free(substr);
                        }
                        else
                        {
                            currentNodeIndex = insertNodeArray(nodes, substr);

                            if (currentNodeIndex == -1)
                                return exitFail(line, &connectedNodes);

                            if (nodes->array[currentNodeIndex].listed == 0)
                            {
                                nodes->array[currentNodeIndex].listed = 1;
                            }
                            else
                            {
                                return exitFail(line, &connectedNodes);
                            }
                        }
                        curCheckpoint = ':';
                        startIndex = i + 1;
                    }
                    else
                    {
                        return exitFail(line, &connectedNodes);
                    }
                    break;

                case ',':
                    if (curCheckpoint == ':' || curCheckpoint == ',')
                    {
                        int status = processNeighbours(nodes, extractString(line, startIndex, i), currentNodeIndex, &connectedNodes);
                        if (status == -1)

                            return exitFail(line, &connectedNodes);

                        curCheckpoint = ',';
                        startIndex = i + 1;
                    }
                    else
                    {
                        return exitFail(line, &connectedNodes);
                    }
                    break;

                case '-':
                    if (curCheckpoint == ':' || curCheckpoint == ',')
                    {
                        if (startIndex == i)
                        {
                            curCheckpoint = '-';
                            startIndex = i + 1;
                        }
                        else
                        {
                            int status = processNeighbours(nodes, extractString(line, startIndex, i), currentNodeIndex, &connectedNodes);
                            if (status == -1)
                                return exitFail(line, &connectedNodes);
                            curCheckpoint = '-';
                            startIndex = i + 1;
                        }
                    }
                    else
                    {
                        return exitFail(line, &connectedNodes);
                    }
                    break;

                case '\n':
                    if (curCheckpoint == ':')
                    {
                        if (beginNodeLine == 1)
                        {
                            char *substr = extractString(line, startIndex, i);
                            int oldNumberOfNodes = nodes->used;
                            *startingNodeIndex = insertNodeArray(nodes, substr);
                            int newNumberOfNodes = nodes->used;

                            if (*startingNodeIndex == -1 || newNumberOfNodes > oldNumberOfNodes)
                                return exitFail(line, &connectedNodes);

                            beginNodeLine = 0;
                        }
                        else if (stepNumLine == 1)
                        {
                            char *substr = extractString(line, startIndex, i);
                            *numberOfSteps = atol(substr);
                            free(substr);
                            stepNumLine = 0;
                        }
                        else
                        {
                            int status = processNeighbours(nodes, extractString(line, startIndex, i), currentNodeIndex, &connectedNodes);
                            if (status == -1)
                                return exitFail(line, &connectedNodes);
                        }

                        curCheckpoint = '.';
                        startIndex = 0;
                    }
                    else if (curCheckpoint == ',')
                    {
                        int status = processNeighbours(nodes, extractString(line, startIndex, i), currentNodeIndex, &connectedNodes);
                        if (status == -1)
                            return exitFail(line, &connectedNodes);
                        curCheckpoint = '.';
                        startIndex = 0;
                    }
                    else if (curCheckpoint == '-')
                    {
                        char *substr = extractString(line, startIndex, i);
                        int status = updateTimeVisited(&(nodes->array[currentNodeIndex]), atol(substr));
                        if (status == -1)
                            return exitFail(line, &connectedNodes);
                        curCheckpoint = '.';
                        startIndex = 0;
                        free(substr);
                    }
                    else
                    {
                        return exitFail(line, &connectedNodes);
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                return exitFail(line, &connectedNodes);
            }
        }

        freeArray(&connectedNodes);
    }
    free(line);
    return 0;
}

int main()
{
    nodeArray nodes;
    initNodeArray(&nodes, 10);
    size_t startingNodeIndex = 0;
    size_t numberOfSteps = 0;

    int status = processInput(&nodes, &startingNodeIndex, &numberOfSteps);

    if (status == 0 && nodes.used != 0)
    {
        for (int i = 1; i <= numberOfSteps; i++)
        {
            node *last = &(nodes.array[startingNodeIndex]);
            int timesVisited = last->timesVisited;
            int numOfNeighbour = last->neighbourNodes->used;
            updateTimeVisited(last, 1);
            if (numOfNeighbour != 0)
            {
                startingNodeIndex = last->neighbourNodes->array[timesVisited % numOfNeighbour];
            }
        }

        for (int i = 0; i < nodes.used; i++)
        {
            printf("%s:%ld\n", nodes.array[i].ID, nodes.array[i].timesVisited);
        }

        printf("E:%s\n", nodes.array[startingNodeIndex].ID);
        freeNodeArray(&nodes);
    }
    else
    {
        printf("Error in input\n");
        freeNodeArray(&nodes);
        exit(EXIT_FAILURE);
    }

    return 0;
}
