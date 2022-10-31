#include <stdio.h>

#include "tree.h"

int main()
{
    Tree tree = {};

    TreeCtor(&tree, 10, "graphlog.htm");

    Node* pos5 = AddLeftChild(&tree, tree.anchor, 5);

    Node* pos3 = AddLeftChild(&tree, pos5, 3);

    Node* pos1 = AddLeftChild(&tree, pos3, 1);

    Node* pos4 = AddRightChild(&tree, pos3, 4);

    Node* pos6 = AddRightChild(&tree, pos5, 6);

    Node* pos15 = AddRightChild(&tree, tree.anchor, 15);

    Node* pos20 = AddRightChild(&tree, pos15, 20);

    Node* pos17 = AddRightChild(&tree, pos20, 17);

    return 0;
}
