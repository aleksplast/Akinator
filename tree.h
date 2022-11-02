#ifndef TREE_H
#define TREE_H

#define DEBUG

#ifndef DEBUG
#define DBG if(false)
#else
#define DBG if(true)
#endif

typedef char* elem_t;

static int Piccounter = 1;

struct Tree;

struct Node
{
    elem_t val;

    Node* ancestor;
    Node* leftchild;
    Node* rightchild;

    Tree* tree;
};

struct Tree
{
    Node* anchor;
    int size;

    char* graphlog;
};

int TreeCtor(Tree* tree, elem_t rootval, char* graphfile);

int TreeVerr(Tree* tree);

int NodeVerr (Node* node, int* errors);

int TreeGraphDump (Tree* tree, int errors, int line, const char* func, const char* file);

int NodeDump(Node* node, int* counter, FILE* pic);

Node* TreeDepthSearch(Tree* tree, Node* node, elem_t searchval);

Node* AddRightChild(Tree* tree, Node* ancestor, elem_t val);

Node* AddLeftChild(Tree* tree, Node* ancestor, elem_t val);

int NodeDetor(Tree* tree, Node* node);

int TreeDetor(Tree* tree);

enum TreeErrors
{
    ANCHORERR       = 1 << 1,
    VALERR          = 1 << 2,
    SIZEERR         = 1 << 3,
    NODETREEERR     = 1 << 4,
};

#endif //TREE_H
