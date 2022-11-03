#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>

#define TX_USE_SPEAK
#include "TXLib.h"
#include "akinator.h"

#define TRYSPEAK(line)  "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'>" #line "</speak>"

#define SPEAK(line)             if (Voice)                                              \
                                    {                                                   \
                                        txSpeak(TRYSPEAK(line));                        \
                                        printf(line);                                   \
                                    }                                                   \
                                    else                                                \
                                        printf(line);                                   \
                                        printf("\n");

#define SPEAK1(line, arg1)      if (Voice)                                              \
                                    {                                                   \
                                        txSpeak(TRYSPEAK(line), arg1);                  \
                                        printf(line, arg1);                             \
                                    }                                                   \
                                    else                                                \
                                        printf(line, arg1);                             \
                                        printf("\n");

#define SPEAK2(line, arg1, arg2)             if (Voice)                                         \
                                            {                                                   \
                                                txSpeak(TRYSPEAK(line), arg1, arg2);            \
                                                printf(line, arg1, arg2);                       \
                                            }                                                   \
                                            else                                                \
                                                printf(line, arg1, arg2);                       \
                                                printf("\n");

int AkinatorMain()
{
    Tree akinatortree = DataParser();
    int mode = 0;

    while (mode != EXIT)
    {
        GetMode(&mode);
        if (mode == GUESS)
            GuessMode(&akinatortree);
        else if (mode == DEFINE)
            DefineMode(&akinatortree);
        else if (mode == DIFFERENCIES)
            DifferenciesMode(&akinatortree);
        else if (mode == TEXTTOSPEAK)
            EnableTTS();
        else if (mode == EXIT)
            DataPrint(&akinatortree);
    }

    TreeDetor(&akinatortree);

    return NOERR;
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

        if (*curr == '}')
        {
            currnode = currnode->ancestor;
        }
        else if (*curr == '{'){}
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

int GetMode(int* mode)
{
    printf("Enter the number of a mode you want to enter:\n");
    printf("[%d] for guessing the character mode\n", GUESS);
    printf("[%d] for definition mode\n", DEFINE);
    printf("[%d] for differencies mode\n", DIFFERENCIES);
    printf("[%d] for enabling/disabling text to speak mode\n", TEXTTOSPEAK);
    printf("[%d] for exiting the programm\n", EXIT);

    scanf("%d", mode);

    while (*mode <= 0 || *mode > 5)
    {
        printf("You entered the wrong number, please, enter correct number\n");
        scanf("%d", mode);
    }

    return *mode;
}

int GuessMode(Tree* akinatortree)
{
    SPEAK("Welcome to the guessing mode");
    SPEAK("Think about any character, i will try to guess it");
    SPEAK("Lets start");

    Guess(akinatortree->anchor);

    SPEAK("Nice game, you want to play more?")

    while (AnswerCheck())
    {
        Guess(akinatortree->anchor);

        SPEAK("Nice game, you want to play more?");
    }

    return NOERR;
}

int Guess(Node* node)
{
    if (!node->leftchild && !node->rightchild)
    {
        SPEAK1("Is it %s?", node->val);

        if (AnswerCheck())
        {
            SPEAK("I knew it!!!");
        }
        else
            AnswerAdd(node);
    }
    else
    {
        SPEAK1("%s?", node->val)

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
        SPEAK("Uncorrect answer, please enter \"yes\" or \"no\"");
        return AnswerCheck();
    }
}

int AnswerAdd(Node* node)
{
    fflush(stdin);

    char* answer = (char*) calloc(20, sizeof(char));
    char* difference = (char*) calloc(20, sizeof(char));

    SPEAK("You defeated me, i don't know your character");
    SPEAK("Please, tell me, who was your character?");

    gets(answer);

    if (TreeDepthSearch(node->tree, node->tree->anchor, answer))
    {
        SPEAK("I already have this character in my database, but with another definition");
        Define(node->tree, answer);
        return NOERR;
    }

    SPEAK2("Now tell me, whats the difference between %s and %s", answer, node->val);

    gets(difference);

    AddLeftChild(node->tree, node, answer);
    AddRightChild(node->tree, node, node->val);

    node->val = difference;

    return NOERR;
}

int DefineMode(Tree* akinatortree)
{
    fflush(stdin);

    SPEAK("Welcome to the define mode");
    SPEAK("Give a name of some character, and, if it's in my database,");
    SPEAK("I will give you a definition of it");

    char character[40] = "";
    gets(character);

    if (stricmp(character, "Amogus") == 0)
    {
        system("Amogus.mp4");
        txSleep(1000);
    }

    Define(akinatortree, character);

    SPEAK("Nice game, you want to play more?");

    while (AnswerCheck())
    {
        fflush(stdin);

        SPEAK("Give a name of some character, and, if it's in my database,");
        SPEAK("i will give you a definition of it");

        gets(character);
        Define(akinatortree, character);

        SPEAK("Nice game, you want to play more?");
    }

    return NOERR;
}

int Define(Tree* akinatortree, char* character)
{
    Node* found = TreeDepthSearch(akinatortree, akinatortree->anchor, character);

    if (found)
    {
        SPEAK1("%s - ", character);
        NodeDefine(found, akinatortree->anchor);
    }
    else
        SPEAK("Whoops, seems like i don't have this character in my database");

    return NOERR;
}

int NodeDefine(Node* node, Node* end)
{
    if (node == end)
        return NOERR;
    else
        NodeDefine(node->ancestor, end);

    if (node->ancestor->leftchild == node)
        SPEAK1("%s, ", node->ancestor->val);
    if (node->ancestor->rightchild == node)
        SPEAK1("not %s, ", node->ancestor->val);

    return NOERR;;
}

int DifferenciesMode(Tree* akinatortree)
{
    SPEAK("Welcome to differencies mode");
    SPEAK("Please, enter two characters from my database,");
    SPEAK("I will tell you their differencies and their commons");

    char* character1 = (char*) calloc(40, sizeof(char));
    char* character2 = (char*) calloc(40, sizeof(char));

    fflush(stdin);
    gets(character1);

    fflush(stdin);
    gets(character2);

    Differencies(akinatortree, character1, character2);
    SPEAK("You want me to tell some more differencies?");

   while (AnswerCheck())
    {
        SPEAK("Please, enter two characters from my database,");
        SPEAK("I will tell you their differencies and their commons");

        fflush(stdin);
        gets(character1);

        fflush(stdin);
        gets(character2);

        Differencies(akinatortree, character1, character2);
        SPEAK("You want me to tell some more differencies?");
    }

    free(character1);
    free(character2);
}

int Differencies(Tree* akinatortree, char* character1, char* character2)
{
    stack char1stk = {};
    StackCtor(&char1stk, 10);

    stack char2stk = {};
    StackCtor(&char2stk, 10);

    Node* origin1 = FindPath(akinatortree, &char1stk, character1);
    Node* origin2 = FindPath(akinatortree, &char2stk, character2);

    Node* common = NULL;

    if (!origin1)
    {
        SPEAK1("Whoops, seems like i don't have %s in my database", character1);
        return NOERR;
    }

    if (!origin2)
    {
        SPEAK1("Whoops, seems like i don't have %s in my database", character2);
        return NOERR;
    }

    while   (StackPop(&char1stk) == StackPop(&char2stk))
    {
        common = (Node*) StackPop(&char1stk);
        StackPush(&char1stk, (char*) common);
    }

    SPEAK("Their commons: ");
    NodeDefine(common->ancestor, akinatortree->anchor);
    SPEAK("Their differencies:");
    SPEAK1("%s - ", character1);
    NodeDefine(origin1, common->ancestor);
    SPEAK1("%s - ", character2);
    NodeDefine(origin2, common->ancestor);
}

Node* FindPath(Tree* akinatortree, stack* stk, char* character)
{
    assert(akinatortree != NULL);
    assert(stk != NULL);

    Node* found = TreeDepthSearch(akinatortree, akinatortree->anchor, character);
    Node* origin = found;

    while (found != NULL)
    {
        StackPush(stk, (char*) found);
        found = found->ancestor;
    }

    return origin;
}

int EnableTTS()
{
    Voice = !Voice;

    return NOERR;
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
