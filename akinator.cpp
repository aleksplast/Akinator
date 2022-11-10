#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <ctype.h>
#include <stdarg.h>

#define TX_USE_SPEAK
#include "TXLib.h"
#include "akinator.h"

#define STRSIZE 40

#define TRYSPEAK(line)  "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'>" #line "</speak>"

#define SPEAK(line, endline, ...)   if (Voice)                                          \
                                    {                                                   \
                                        txSpeak(TRYSPEAK(line) __VA_ARGS__);            \
                                        printf(line __VA_ARGS__);                       \
                                        if (endline) printf("\n");                      \
                                    }                                                   \
                                    else                                                \
                                    {                                                   \
                                        printf(line __VA_ARGS__);                       \
                                        if (endline) printf("\n");                      \
                                    }

#define COMMA ,

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
        else if (mode == SHOWDATA)
            ShowData(&akinatortree);
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

    char* log = (char*) calloc(STRSIZE, sizeof(char));
    log = "graphlog.htm";
    TreeCtor(&akinatortree, anchor, log);

    Node* currnode = akinatortree.anchor;

    for (int counter = 2; counter < src.nlines; counter++)
    {
        char* curr = src.Strings[counter].ptr;

        while(isspace(*curr))
            curr += 1;

        if (*curr == '}')
            {
            currnode = currnode->ancestor;
            continue;
            }
        else if (*curr == '{')
        {
            continue;
        }
        else
            {
            if (currnode->leftchild)
            {
                AddRightChild(&akinatortree, currnode, curr);
                currnode = currnode->rightchild;
                continue;
            }
            else
            {
                AddLeftChild(&akinatortree, currnode, curr);
                currnode = currnode->leftchild;
                continue;
            }
        }
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
    printf("[%d] for showing the database of this AI\n", SHOWDATA);
    printf("[%d] for exiting the programm\n", EXIT);

    scanf("%d", mode);

    while (*mode <= 0 || *mode > 6)
    {
        printf("You entered the wrong number, please, enter correct number\n");
        scanf("%d", mode);
    }

    return *mode;
}

int GuessMode(Tree* akinatortree)
{
    SPEAK("Welcome to the guessing mode", 1);
    SPEAK("Think about any character, i will try to guess it", 1);
    SPEAK("Lets start", 1);

    Guess(akinatortree->anchor);

    SPEAK("Nice game, you want to play more?", 1)

    while (AnswerCheck())
    {
        Guess(akinatortree->anchor);

        SPEAK("Nice game, you want to play more?", 1);
    }

    return NOERR;
}

int Guess(Node* node)
{
    if (!node->leftchild && !node->rightchild)
    {
        SPEAK("Is it %s?", 1, COMMA node->val);

        if (AnswerCheck())
        {
            SPEAK("I knew it!!!", 1);
        }
        else
            AnswerAdd(node);
    }
    else
    {
        SPEAK("%s?", 1, COMMA node->val)

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
        SPEAK("Uncorrect answer, please enter yes or no", 1);
        return AnswerCheck();
    }
}

int AnswerAdd(Node* node)
{
    fflush(stdin);

    char* answer = (char*) calloc(STRSIZE, sizeof(char));
    char* difference = (char*) calloc(STRSIZE, sizeof(char));

    SPEAK("You defeated me, i don't know your character", 1);
    SPEAK("Please, tell me, who was your character?", 1);

    gets(answer);

    if (TreeDepthSearch(node->tree, node->tree->anchor, answer))
    {
        SPEAK("I already have this character in my database, but with another definition", 1);
        Define(node->tree, answer);
        return NOERR;
    }

    SPEAK("Now tell me, whats the difference between %s and %s", 1, COMMA answer COMMA node->val);

    gets(difference);

    AddLeftChild(node->tree, node, answer);
    AddRightChild(node->tree, node, node->val);

    node->val = difference;

    return NOERR;
}

int DefineMode(Tree* akinatortree)
{
    fflush(stdin);

    SPEAK("Welcome to the define mode", 1);
    SPEAK("Give a name of some character, and, if it's in my database,", 1);
    SPEAK("I will give you a definition of it", 1);

    char character[40] = "";

    fflush(stdin);
    gets(character);

    if (stricmp(character, "Amogus") == 0)
    {
        system("Amogus.mp4");
        txSleep(1000);
    }

    Define(akinatortree, character);

    SPEAK("Nice game, you want to play more?", 1);

    while (AnswerCheck())
    {
        fflush(stdin);

        SPEAK("Give a name of some character, and, if it's in my database,", 1);
        SPEAK("i will give you a definition of it", 1);

        gets(character);
        Define(akinatortree, character);

        SPEAK("Nice game, you want to play more?", 1);
    }

    return NOERR;
}

int Define(Tree* akinatortree, char* character)
{
    Node* found = TreeDepthSearch(akinatortree, akinatortree->anchor, character);

    if (found)
    {
        SPEAK("%s - ", 0, COMMA character);
        NodeDefine(found, akinatortree->anchor);
        printf("\n");
    }
    else
        SPEAK("Whoops, seems like i don't have this character in my database", 1);

    return NOERR;
}

int NodeDefine(Node* node, Node* end)
{
    if (node == end)
        return NOERR;
    else
        NodeDefine(node->ancestor, end);

    if (node->ancestor->leftchild == node)
    {
        SPEAK("%s, ", 0, COMMA node->ancestor->val);
    }
    else if (node->ancestor->rightchild == node)
    {
        SPEAK("not %s, ", 0, COMMA node->ancestor->val);
    }

    return NOERR;;
}

int DifferenciesMode(Tree* akinatortree)
{
    SPEAK("Welcome to differencies mode", 1);
    SPEAK("Please, enter two characters from my database,", 1);
    SPEAK("I will tell you their differencies and their commons", 1);

    char* character1 = (char*) calloc(STRSIZE, sizeof(char));
    char* character2 = (char*) calloc(STRSIZE, sizeof(char));

    fflush(stdin);
    gets(character1);

    fflush(stdin);
    gets(character2);

    Differencies(akinatortree, character1, character2);
    SPEAK("You want me to tell some more differencies?", 1);

    while (AnswerCheck())
    {
        SPEAK("Please, enter two characters from my database,", 1);
        SPEAK("I will tell you their differencies and their commons", 1);

        fflush(stdin);
        gets(character1);

        fflush(stdin);
        gets(character2);

        Differencies(akinatortree, character1, character2);
        SPEAK("You want me to tell some more differencies?", 1);
    }

    free(character1);
    free(character2);

    return NOERR;
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
        SPEAK("Whoops, seems like i don't have %s in my database", 1, COMMA character1);
        return NOERR;
    }

    if (!origin2)
    {
        SPEAK("Whoops, seems like i don't have %s in my database", 1, COMMA character2);
        return NOERR;
    }

    while   (StackPop(&char1stk) == StackPop(&char2stk))
    {
        common = (Node*) StackPop(&char1stk);
        StackPush(&char1stk, (char*) common);
    }

    if (common->ancestor == akinatortree->anchor)
    {
        SPEAK("%s and %s have no commons", 0, COMMA character1 COMMA character2);
    }

    SPEAK("The commons between %s and %s: ", 0, COMMA character1 COMMA character2);
    NodeDefine(common->ancestor, akinatortree->anchor);
    printf("\n");
    SPEAK("The difference between %s and %s: ", 0, COMMA character1 COMMA character2);
    SPEAK("%s is ", 0, COMMA character1);
    NodeDefine(common, common->ancestor);
    SPEAK("while %s is not", 0, COMMA character2);
//    NodeDefine(origin2, common->ancestor);
    printf("\n");

    StackDetor(&char1stk);
    StackDetor(&char2stk);

    return NOERR;
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

    return NOERR;
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

int ShowData(Tree* tree)
{
    int errors = TreeVerr(tree);
    TreeGraphDump(tree, errors, __LINE__, __func__, __FILE__);

    system(tree->graphlog);

    return NOERR;
}
