#ifndef AKINATOR_H
#define AKINATOR_H

#include "tree.h"
#include "text-sort.h"
#include "common.h"

typedef struct poem source;

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

int GuessMode(Tree akinatortree);

int AnswerAdd(Node* node);

int AnswerCheck();

int Guess(Node* node);

int DataPrint(Tree* tree);

int NodePrint(FILE* data, Node* node);

#endif //AKINATOR_H
