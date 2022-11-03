#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include "text-sort.h"
#include "stack.h"
#include "common.h"

#define AMOGUS "https://www.youtube.com/watch?v=XIRsKGYuzng"

typedef struct poem source;

static int Voice = 0;

enum AkinatorModes
{
    GUESS           = 1,
    DEFINE          = 2,
    DIFFERENCIES    = 3,
    TEXTTOSPEAK     = 4,
    EXIT            = 5,
};

int AkinatorMain();

int GetMode(int* mode);

Tree DataParser();

int GuessMode(Tree* akinatortree);

int AnswerAdd(Node* node);

int AnswerCheck();

int Guess(Node* node);

int DefineMode(Tree* akinatortree);

int Define(Tree* akinatortree, char* character);

int NodeDefine(Node* node, Node* end);

int DifferenciesMode(Tree* tree);

int Differencies(Tree* akinatortree, char* character1, char* character2);

Node* FindPath(Tree* akinatortree, stack* stk, char* character);

int EnableTTS();

int DataPrint(Tree* tree);

int NodePrint(FILE* data, Node* node);

int BetterTxSpeak(char* line, ...);

#endif //AKINATOR_H
