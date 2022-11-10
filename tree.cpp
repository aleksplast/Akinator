#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include "tree.h"
#include "common.h"

const char* Poison = NULL;

#define TREECHECK   if (int errors = TreeVerr(tree))                                         \
                        DBG TreeGraphDump(tree, errors, __LINE__, __func__, __FILE__);

static int Piccounter = 1;

int TreeCtor(Tree* tree, elem_t rootval, char* graphfile)
{
    DBG assert(tree != NULL);

    if ((tree->anchor = (Node*) calloc(1, sizeof(Node))) == NULL)
        return MEMERR;

    tree->anchor->val = rootval;
    tree->anchor->ancestor = NULL;
    tree->anchor->rightchild = NULL;
    tree->anchor->leftchild = NULL;
    tree->anchor->tree = tree;

    tree->size = 1;

    tree->graphlog = graphfile;

    FILE* fp = fopen(tree->graphlog, "w");
    fclose(fp);

    TREECHECK

    return NOERR;
}

int TreeVerr(Tree* tree)
{
    DBG assert(tree != NULL);

    int errors = 0;

    if (tree->anchor->ancestor != NULL)
        errors += ANCHORERR;
    if (tree->size < 0)
        errors += SIZEERR;

    errors += NodeVerr(tree->anchor, &errors);

    return errors;
}

int NodeVerr (Node* node, int* errors)
{
    DBG assert(node != NULL);

    if (node->leftchild)
        *errors += NodeVerr(node->leftchild, errors);
    if (node->rightchild)
        *errors += NodeVerr(node->rightchild, errors);

    if (node->val == Poison)
        *errors += VALERR;

    return 0;
}

int TreeGraphDump (Tree* tree, int errors, int line, const char* func, const char* file)
{
    DBG assert(tree != NULL);

    char picture[15] = "picture";
    sprintf(picture, "%d.dot", Piccounter);
    char picturesrc[15] = "picture";
    sprintf(picturesrc, "%d.png", Piccounter);

    FILE* pic = fopen(picture, "w");

    fprintf(pic, "strict graph {\n");
    fprintf(pic, "\trankdir = TB\n");
    fprintf(pic, "\t\"info\" [shape = \"record\", style = \"filled\", fillcolor = \"grey\", label = \"{size = %d|anchor = %p}\"]\n", tree->size, tree->anchor);

    int counter = 1;
    NodeDump(tree->anchor, &counter, pic);

    fprintf(pic, "}");

    fclose(pic);
    Piccounter += 1;

    char src[80] = "";
    sprintf(src, "dot -Tpng %s -o %s", picture, picturesrc);

    system(src);

    FILE* graphlog = fopen(tree->graphlog, "a");

    fprintf(graphlog, "<pre>\n");

    fprintf(graphlog, "<h2>TreeDump on %d line in %s in %s</h2>\n", line, func, file);
    fprintf(graphlog, "<img src = \"%s\">\n", picturesrc);

    fprintf(graphlog, "<hr>\n");

    fclose(graphlog);

    return NOERR;
}

int NodeDump(Node* node, int* counter, FILE* pic)
{
    int left = 0;
    int right = 0;
    if (node->leftchild)
        left = NodeDump(node->leftchild, counter, pic);
    if (node->rightchild)
        right = NodeDump(node->rightchild, counter, pic);

    fprintf(pic, "\t\"node%d\" [shape = \"circle\", style = \"filled\", fillcolor = \"green\", label = \"%s\"]\n", *counter, node->val);
    int curr = *counter;
    if (node->leftchild)
        fprintf(pic, "\t\"node%d\" -- \"node%d\"\n", curr, left);
    if (node->rightchild)
        fprintf(pic, "\t\"node%d\" -- \"node%d\"\n", curr, right);

    (*counter)++;
    return *counter - 1;
}

Node* AddRightChild(Tree* tree, Node* ancestor, elem_t val)
{
    DBG assert(ancestor != NULL);

    if (ancestor->rightchild)
        return ancestor;

    if ((ancestor->rightchild = (Node*) calloc(1, sizeof(Node))) == NULL)
        return ancestor;

    tree->size++;

    ancestor->rightchild->val = val;
    ancestor->rightchild->leftchild = NULL;
    ancestor->rightchild->rightchild = NULL;
    ancestor->rightchild->ancestor = ancestor;
    ancestor->rightchild->tree = ancestor->tree;

    TREECHECK

    return ancestor->rightchild;
}

Node* AddLeftChild(Tree* tree, Node* ancestor, elem_t val)
{
    DBG assert(ancestor != NULL);

    if (ancestor->leftchild)
        return ancestor;

    if ((ancestor->leftchild = (Node*) calloc(1, sizeof(Node))) == NULL)
        return ancestor;

    tree->size++;

    ancestor->leftchild->val = val;
    ancestor->leftchild->leftchild = NULL;
    ancestor->leftchild->rightchild = NULL;
    ancestor->leftchild->ancestor = ancestor;
    ancestor->leftchild->tree = ancestor->tree;

    TREECHECK

    return ancestor->leftchild;
}

Node* TreeDepthSearch(Tree* tree, Node* node, elem_t searchval)
{
    Node* searchleft = NULL;
    Node* searchright = NULL;
    Node* searchhere = NULL;

    if (node->leftchild)
        searchleft = TreeDepthSearch(tree, node->leftchild, searchval);
    if (node->rightchild)
        searchright = TreeDepthSearch(tree, node->rightchild, searchval);

    if (stricmp(node->val, searchval) == 0)
        return node;
    else if (searchright)
        return searchright;
    else if (searchleft)
        return searchleft;
    else
        return NULL;
}

int NodeDetor(Tree* tree, Node* node)
{
    DBG assert (node != NULL);

    if (node->leftchild)
        NodeDetor(tree, node->leftchild);

    else if (node->rightchild)
        NodeDetor(tree, node->leftchild);

    tree->size--;

    free(node);

    return NOERR;
}

int TreeDetor(Tree* tree)
{
    DBG assert (tree != NULL);

    TREECHECK

    NodeDetor(tree, tree->anchor);

    tree->size = -1;
    tree->graphlog = NULL;

    return NOERR;
}
