#include <stdio.h>
#include <stdlib.h>

#define int long long
#define RED 1
#define BLACK 2

struct RB_Tree_Node{
    struct RB_Tree_Node* parent;
    struct RB_Tree_Node* left;
    struct RB_Tree_Node* right;
    int color,val;
};

struct RB_Tree_Node* root;
void rbtree_left_rotate(struct RB_Tree_Node* root,struct RB_Tree_Node* x);
void rbtree_right_rotate(struct RB_Tree_Node* root,struct RB_Tree_Node* y);
int color[1000005];

void rbtree_insert_fixup(struct RB_Tree_Node* root, struct RB_Tree_Node* node)
{
    struct RB_Tree_Node *parent, *gparent;

    // 若父節點存在，並且父節點的顏色是紅色
    while ( ( ( parent = node -> parent ) && ( parent -> color == RED ) ) )
    {
        gparent = parent -> parent;

        //若“父節點”是“祖父節點的左孩子”
        if ( parent == gparent -> left )
        {
            // Case 1條件：叔叔節點是紅色
            {
                struct RB_Tree_Node *uncle = gparent -> right;
                if( uncle && uncle -> color == RED )
                {//父、叔變黑，爺變紅,對爺進行判斷 
                    color[uncle->val] = 2;
                    color[parent->val] = 2;
                    color[gparent->val] = 1;
                    
                    uncle -> color = BLACK;
                    parent -> color = BLACK;
                    gparent -> color =RED;
                    node = gparent;
                    continue;
                }
            }

            // Case 2條件：叔叔是黑色，且當前節點是右孩子
            if ( parent -> right == node)
            {
                struct RB_Tree_Node *tmp;
                rbtree_left_rotate(root, parent);//父左旋 
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3條件：叔叔是黑色，且當前節點是左孩子。
            color[parent->val] = 2;
            color[gparent->val] = 1;
            parent->color=BLACK;
            gparent->color=RED;
            rbtree_right_rotate(root, gparent);
        } 
        else//若“z的父節點”是“z的祖父節點的右孩子”
        {
            // Case 1條件：叔叔節點是紅色
            {
                struct RB_Tree_Node *uncle = gparent -> left;
                if( uncle && ( uncle -> color == RED ) )
                {
                    color[uncle->val] = 2;
                    color[parent->val] = 2;
                    color[gparent->val] = 1;

                    uncle -> color = BLACK;
                    parent -> color = BLACK;
                    gparent -> color = RED;
                    node = gparent;
                    continue;
                }
            }

            // Case 2條件：叔叔是黑色，且當前節點是左孩子
            if ( parent -> left == node )
            {
                struct RB_Tree_Node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3條件：叔叔是黑色，且當前節點是右孩子。
            color[parent->val] = 2;
            color[gparent->val] = 1;
            parent->color=BLACK;
            gparent->color=RED;
            rbtree_left_rotate(root, gparent);
        }
    }

    // 將根節點設爲黑色
    color[root->val] = 2;
    root -> color = BLACK;
  //  printf("%d\n",node->color);
}

void rbtree_left_rotate(struct RB_Tree_Node* root,struct RB_Tree_Node* x)
{
    struct RB_Tree_Node* y = x -> right;
    // 找到 y 的左孩子，他會變成 x 的右孩子
    x -> right = y -> left;
    if( y -> left != NULL ) y -> left -> parent = x; // 如果 y 有左孩子，他的 parent 會變成 x

    y -> parent = x -> parent;
    
    if( x -> parent == NULL ) root = y; // 如果 x 是 root，交換後會變成 y 是 root
    else // 如果 x 不是 root，確定一下他是 parent 的 left child 還是 right child
    {
        if( x -> parent -> left == x ) x -> parent -> left = y;
        else x -> parent -> right = y;
    }

    y -> left = x;
    x -> parent = y;

    return;
}
void rbtree_right_rotate(struct RB_Tree_Node* root,struct RB_Tree_Node* y)
{
    struct RB_Tree_Node* x = y -> left;
    y -> left = x -> right;
    if( x -> right != NULL ) x -> right -> parent = y;

    x -> parent = y -> parent;
    if( y -> parent == NULL ) root = x;
    else
    {
        if( y -> parent -> right == y ) y -> parent -> right = x;
        else y -> parent -> left = x;  
    }

    x -> right = y;
    y -> parent = x;

    return;
}
void insert(struct RB_Tree_Node* root,struct RB_Tree_Node* node)
{
    struct RB_Tree_Node* y = NULL;
    struct RB_Tree_Node* x = root;
    while( x != NULL ) 
    {
        y = x;
        if( x -> val > node -> val ) x = x -> left;
        else x = x -> right;
    }
    node -> parent = y;

    if( y != NULL )
    {
        if( node -> val < y -> val ) y -> left = node;
        else y -> right = node;
    }
    else
    {
        root = node;
    }

   // printf("%d\n",node->val);

    color[node->val] = 1;
    node -> color = RED;

    rbtree_insert_fixup(root, node);
}
struct RB_Tree_Node* creat_rbtree()
{
    struct RB_Tree_Node *root=(struct RB_Tree_Node*)malloc(sizeof(struct RB_Tree_Node));//定義根結點，並分配空間
    return root;
}
struct RB_Tree_Node* creat_rbtree_node(int key,struct RB_Tree_Node *parent,struct  RB_Tree_Node *lchild,struct RB_Tree_Node *rchild)
{
    struct RB_Tree_Node* p;
    p = (struct RB_Tree_Node*)malloc(sizeof(struct RB_Tree_Node));
    p -> val = key;
    p -> left = lchild;
    p -> right = rchild;
    p -> color = BLACK;

   // color[p->val] = 2;

    return p;
}
signed main()
{
    char s[30] = {};
    while( scanf("%s",s) )
    {
        if( s[0] == 's' )
        {
            int target;
            scanf("%lld",&target);
            if( color[target] == 1 ) printf("red\n");
            else if( color[target] == 2 ) printf("black\n");
            else printf("not found\n");
        }
        else if( s[0] == 'i' )
        {
            int target;
            scanf("%lld",&target);
            struct RB_Tree_Node* new_node = creat_rbtree_node(target,NULL,NULL,NULL);
            insert(root,new_node);
        }
        else break;
    }

    return 0;
}