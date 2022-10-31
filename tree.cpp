#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tree.h"

#define TREECHECK   int errors = TreeVerr(tree);                                          \
                    DBG TreeGraphDump(tree, errors, __LINE__, __func__, __FILE__);

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
    if (node->tree == NULL)
        *errors += NODETREEERR;

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

    fprintf(pic, "DiGraph List {\n");
    fprintf(pic, "\trankdir = TB\n");
    fprintf(pic, "\t \t\"info\" [shape = \"record\", style = \"filled\", fillcolor = \"grey\", label = \"{size = %d|anchor = %p}\"]\n", tree->size, tree->anchor);

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
}

int NodeDump(Node* node, int* counter, FILE* pic)
{
    int left = 0;
    int right = 0;
    if (node->leftchild)
        left = NodeDump(node->leftchild, counter, pic);
    if (node->rightchild)
        right = NodeDump(node->rightchild, counter, pic);

    printf("left = %d", right);

    fprintf(pic, "\"node%d\" [shape = \"circle\", style = \"filled\", fillcolor = \"green\", label = %d]\n", *counter, node->val);
    printf("counter = %d\n", *counter);
    int curr = *counter;
    printf("curr = %d\n", counter);
    if (node->leftchild)
        fprintf(pic, "\"node%d\" -> \"node%d\"\n", curr, left);
    if (node->rightchild)
        fprintf(pic, "\"node%d\" -> \"node%d\"\n", curr, right);

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

int NodeDetor(Node* node)
{
    DBG assert (node != NULL);

    if (node->leftchild)
        NodeDetor(node->leftchild);

    else if (node->rightchild)
        NodeDetor(node->leftchild);

    node->tree--;

    free(node);

    return NOERR;
}

int TreeDetor(Tree* tree)
{
    DBG assert (tree != NULL);

    NodeDetor(tree->anchor);

    tree->size = -1;
    tree->graphlog = NULL;

    return NOERR;
}
