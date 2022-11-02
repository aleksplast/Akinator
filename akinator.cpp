#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <ctype.h>

#include "akinator.h"

int AkinatorMain()
{
    Tree akinatortree = DataParser();
    int mode = 0;

    while (mode != EXIT)
    {
        GetMode(&mode);
        if (mode == GUESS)
            GuessMode(akinatortree);
        else if (mode == EXIT)
        {
            DataPrint(&akinatortree);
            continue;
        }
    }
    return NOERR;
}

int GetMode(int* mode)
{
    printf("Enter the number of a mode you want to enter:\n");
    printf("[%d] for guessing the character mode\n", GUESS);
    printf("[%d] for definition mode\n", DEFINE);
    printf("[%d] for differencies mode\n", DIFFERENCIES);
    printf("[%d] for text to speak mode\n", TEXTTOSPEAK);
    printf("[%d] for exiting the programm\n", EXIT);

    scanf("%d", mode);

    while (*mode <= 0 || *mode > 5)
    {
        printf("You entered the wrong number, please, enter correct number\n");
        scanf("%d", mode);
    }

    return *mode;
}

int GuessMode(Tree akinatortree)
{
    Guess(akinatortree.anchor);

    printf("Nice game, you want to play more?\n");
    while (AnswerCheck())
    {
        Guess(akinatortree.anchor);

        printf("Nice game, you want to play more?\n");
    }

    return NOERR;
}

int Guess(Node* node)
{
    if (!node->leftchild && !node->rightchild)
    {
        printf("Is it %s?\n", node->val);

        if (AnswerCheck())
            printf("I knew it!!!\n");
        else
            AnswerAdd(node);
    }
    else
    {
        printf("%s?\n", node->val);

        if (AnswerCheck())
        {
            node = node->leftchild;
            Guess(node);
        }
        else
        {
            node = node->rightchild;
            Guess(node);
        }
    }
}

int AnswerAdd(Node* node)
{
    fflush(stdin);

    char* answer = (char*) calloc(20, sizeof(char));
    char* difference = (char*) calloc(20, sizeof(char));

    printf("You defeated me, i don't know your character\n");
    printf("Please, tell me, who was your character?\n");

    gets(answer);

    printf("Now tell me, whats the difference between %s and %s\n", answer, node->val);

    gets(difference);

    AddLeftChild(node->tree, node, answer);
    AddRightChild(node->tree, node, node->val);

    node->val = difference;

    return NOERR;
}

int AnswerCheck()
{
    char answer[10] = "";

    scanf("%s", answer);

    if (stricmp(answer, "no") == 0)
        return 0;
    else if (stricmp(answer, "yes") == 0)
        return 1;
    else
    {
        printf("Uncorrect answer, please enter \"yes\" or \"no\"\n");
        return AnswerCheck();
    }
}

Tree DataParser()
{
    source src = {};

    TextReader("data.txt", &src, "r");
    LinesSeparator(&src, '\n');

    Tree akinatortree = {};
    char* anchor = src.Strings[1].ptr;

    while (isspace(*anchor))
        anchor += 1;

    TreeCtor(&akinatortree, anchor, "graphlog.htm");

    Node* currnode = akinatortree.anchor;
    int counter = 2;

    while (counter < src.nlines)
    {
        char* curr = src.Strings[counter].ptr;

        while(isspace(*curr))
            curr += 1;

        if (strcmp(curr, "}") == 0)
        {
            currnode = currnode->ancestor;
        }
        else if (strcmp(curr, "{") == 0){}
        else
        {
            if (currnode->leftchild)
            {
                AddRightChild(&akinatortree, currnode, curr);
                currnode = currnode->rightchild;
            }
            else
            {
                AddLeftChild(&akinatortree, currnode, curr);
                currnode = currnode->leftchild;
            }
        }

        counter += 1;
    }

    return akinatortree;
}

int DataPrint(Tree* tree)
{
    FILE* data = fopen("data.txt", "w");

    NodePrint(data, tree->anchor);

    fclose(data);
}

int NodePrint(FILE* data, Node* node)
{
    fprintf(data, "{\n");
    fprintf(data, "%s\n", node->val);

    if (node->leftchild)
        NodePrint(data, node->leftchild);

    if (node->rightchild)
        NodePrint(data, node->rightchild);

    fprintf(data, "}\n");

    return NOERR;
}
