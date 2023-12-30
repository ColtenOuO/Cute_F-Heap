#include <stdio.h>
#include <stdlib.h>

#define RED   0
#define BLACK 1

struct RBNode {
    int value;
    int color;
    struct RBNode *left, *right, *parent;
};

struct RBNode *root = NULL;

struct RBNode* createNode(int value) {
    struct RBNode *newNode = (struct RBNode*)malloc(sizeof(struct RBNode));
    newNode->value = value;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

void rotateLeft(struct RBNode **root, struct RBNode *x) {
    struct RBNode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rotateRight(struct RBNode **root, struct RBNode *x) {
    struct RBNode *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void insertFixUp(struct RBNode **root, struct RBNode *z) {
    while (z != *root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct RBNode *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(root, z->parent->parent);
            }
        } else {
            struct RBNode *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void insert(struct RBNode **root, int value) {
    struct RBNode *z = createNode(value);
    struct RBNode *y = NULL;
    struct RBNode *x = *root;

    while (x != NULL) {
        y = x;
        if (z->value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL) {
        *root = z;
    } else if (z->value < y->value) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFixUp(root, z);
}

const char* getColor(struct RBNode *node) {
    return node == NULL ? "Not Found" : (node->color == RED ? "red" : "black");
}

struct RBNode* search(struct RBNode* root, int value) {
    while (root != NULL && value != root->value) {
        root = (value < root->value) ? root->left : root->right;
    }
    return root;
}

int main() {

    char s[30] = {};
    while( scanf("%s",s) )
    {
        if( s[0] == 's' )
        {
            int target;
            scanf("%d",&target);
            struct RBNode *node = search(root, target);
            printf("%s\n", getColor(node));
        }
        else if( s[0] == 'i' )
        {
            int target;
            scanf("%d",&target);
            insert(&root, target);
        }
        else break;
    }

    return 0;
}
