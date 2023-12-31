#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define int long long

struct Node{
    int key,val,mark,degree;
    struct Node* parent;
    struct Node* next;
    struct Node* child_head;
};
struct f_heap{
    struct f_heap* next;
    struct Node* FibHeap;
    int min_key; // root key
};

struct Node save[505];
int idx = 0;

struct f_heap* head;
struct f_heap* now;
struct f_heap* check_degree[200005];
void delete_root(struct f_heap* target,int ex);
void delete_node(struct Node* target);

void print_child(struct f_heap* now)
{
    if( now -> FibHeap == NULL ) return;
    struct Node* p = now -> FibHeap -> child_head;
    while( p != NULL )
    {
        printf("child: %lld",p->key);
        p = p -> next;
    }
    return;
}
void degree_init()
{
    for(int i=0;i<=200000;i++) check_degree[i] = NULL;
    return;
}
void init_f_heap()
{
    head = calloc(1,sizeof(struct f_heap));
    head -> next =  NULL; 
    head -> FibHeap = NULL;
    head -> min_key = -1e18; // root key
    now = head;
    return;
}
void insert_child(struct Node* p,struct Node* chi) // parent and child
{
    chi -> parent = p;
    chi -> next = NULL;
    p -> degree += 1; // adding one children
    
    if( p -> child_head == NULL ) p -> child_head = chi;
    else
    {
        struct Node* now = p -> child_head;
        while( now -> next != NULL ) now = now -> next;
        now -> next = chi;
    }

    return;
}
void unite(struct f_heap* target)
{
  //  printf("unite check\n");
   // printf("target= %d\n",target->FibHeap->key);
    int num = target -> FibHeap -> degree;
    if( check_degree[num] == NULL )
    {
        check_degree[num] = target;
        return;
    }
    else if( check_degree[num] == target ) return;

    struct f_heap* a = target;
    struct f_heap* b = check_degree[num];
    struct f_heap* tmp = a;

    // a unite to b
    if( a -> min_key < b -> min_key ) a = b, b = tmp; // swap
    
    insert_child( b -> FibHeap, a -> FibHeap );
   // printf("delete: %d\n",a->FibHeap->key);
    delete_root(a,0);

    if( check_degree[num+1] != NULL ) unite(b);
    else check_degree[num+1] = b;

    check_degree[num] = NULL;

    return;
}
struct Node* create_node(struct Node* p,int target_key,int target_value)
{
    struct Node* new_node = calloc(1,sizeof(struct Node));
    new_node -> parent = p;
    new_node -> degree = 0;
    new_node -> child_head = NULL;
    new_node -> key = target_key, new_node -> val = target_value;
    new_node -> next = NULL;
    return new_node;
}
void insert_root(struct Node* target)
{
    target -> parent = NULL;
    if( head -> FibHeap == NULL )
    {
    //    printf("check head\n");
        head -> FibHeap = target, head -> min_key = target -> key;
    }
    else
    {
     //   printf("check insert root\n");
        struct f_heap* new_root = calloc(1,sizeof(struct f_heap));
        new_root -> next = NULL;
        new_root -> FibHeap = target;
        new_root -> min_key = target -> key;
        while( now -> next != NULL ) now = now -> next; // debug and make sure
        now -> next = new_root;
        now = now -> next;

        if( check_degree[target->degree] != NULL ) unite(new_root);
        else check_degree[target->degree] = new_root;
    }
    
    return;
}
void delete_root(struct f_heap* target,int ex) // ex: delete root 的操作是給 extract 的 (關係到要不要拉 children 進 list)
{
  //  printf("owo\n");
  //  print_child(target);
  //  printf("\n");
    check_degree[target->FibHeap->degree] = NULL;
    struct f_heap* p = head;
    struct f_heap* front = NULL; // 前一個人
    while( p != NULL && p != target )
    {
       // printf("list: %lld\n",p->FibHeap->key);
        front = p, p = p -> next; // 這邊好像多此一舉，但我懶得改了，包欠
    }
    if( p == NULL )
    {
        printf("Not Found: %lld\n",target->FibHeap->key);
    }

    int empty = 0;

    if( p -> next == NULL && front == NULL ) empty = 1;

    if( ex == 1 )
    {
        struct Node* chi = target -> FibHeap -> child_head;
        int total = 0;
        while( chi != NULL /*&& chi -> parent == target -> FibHeap*/ )
        {
           // printf("insert child: %lld\n",chi->key);
       //     if( chi -> key == target -> FibHeap -> key ) continue;
            insert_root(chi);
            chi -> parent = 0, chi = chi -> next;
            total++;
        }
    }

    if( front == NULL && p -> next != NULL ) head = p -> next; // target == head
    else if( p -> next == NULL && front != NULL ) now = front, now -> next = NULL; // target == last
    else if( p -> next != NULL && front != NULL ) front -> next = p -> next;

    return;
}
void insert_new_node(int target_key,int target_value)
{
    struct Node* new_node = create_node(NULL,target_key,target_value);
    if( now -> FibHeap == NULL ) now -> FibHeap = new_node, head -> min_key = target_key, check_degree[0] = head;
    else insert_root(new_node); // insert root 的時候有順便檢查 degreee 了

    return;
}
struct f_heap* extract()
{
    struct f_heap* p = head;
    struct f_heap* best = head;
    while( p != NULL )
    {
        if( p -> min_key < best -> min_key ) best = p;
        p = p -> next;
    }


    if( best -> FibHeap == NULL ) return NULL;

    return best;
}
struct Node* find(struct Node* pos,int target_key,int target_val)
{
    if( pos == NULL ) return NULL;
    else if( pos -> key == target_key && pos -> val == target_val ) return pos;
    
    struct Node* p = pos -> child_head;
    while( p != NULL )
    {
        struct Node* ans = find(p,target_key,target_val);
        if( ans != NULL ) return ans;
        else p = p -> next;
    }
    
    return NULL; // NOT FOUND
}
void delete_node(struct Node* target)
{
 //   printf("delete node: %lld, parent: %lld, parent of parent: %lld\n",target->key,target->parent->key,target->parent->parent->key);
    struct Node* now = target -> child_head;
    struct Node* front = NULL;

    while( now != NULL )
    {
        insert_root(now);
        front = now;
        now = now -> next;
        front -> next = NULL;
    }

    if( target -> parent != NULL )
    {
        struct Node* p = target -> parent;
        struct Node* pos = p -> child_head;
        struct Node* f = NULL; // front
        while( pos != target ) f = pos, pos = pos -> next;

        if( f == NULL && pos -> next == NULL ) p -> child_head = NULL;
        else if( f == NULL && pos -> next != NULL ) p -> child_head = pos -> next;
        else if( f != NULL && pos -> next != NULL ) f -> next = pos -> next, pos -> next = NULL;
        else if( f != NULL && pos -> next == NULL ) f -> next = NULL;
    }

  //  printf("delete node: %lld, parent: %lld, parent of parent: %lld\n",target->key,target->parent->key,target->parent->parent->key);

    return;
}
void delete_target(int target_key,int target_val)
{
    struct f_heap* now_pos = head;
    struct Node* p = NULL;
    while( p == NULL && now_pos != NULL )
    {
        p = find(now_pos->FibHeap,target_key,target_val);
        if( p != NULL ) break;
        now_pos = now_pos -> next;
    }
    if( p == NULL ) printf("Debug: NOT FOUND\n");

    if( p -> parent == NULL )
    {
        delete_root(now_pos,1);
    }
    else
    {
        p -> parent -> degree -= 1;
        if( p -> parent == NULL ) // parent is a root
        {
            struct f_heap* now_pos = head;
            while( now_pos != NULL && now_pos -> FibHeap != p -> parent ) now_pos = now_pos -> next;
            check_degree[now_pos->FibHeap->degree] = NULL;
            now_pos -> FibHeap -> degree -= 1;

            if( check_degree[now_pos->FibHeap->degree] != NULL ) unite(now_pos);
            else check_degree[now_pos->FibHeap->degree] = now_pos;
        }
       // printf("delete %lld\n",p->key);
        delete_node(p);
    }

    return;
}
void decrease(int target_key,int target_val,int d)
{
    struct f_heap* now_pos = head;
    struct Node* p = NULL;
    while( p == NULL && now_pos != NULL ) p = find(now_pos->FibHeap,target_key,target_val), now_pos = now_pos -> next;

    if( p == NULL ) printf("NOT FOUND\n");
    if( p -> parent == NULL )
    {
        struct f_heap* pos = head;
        while( pos != NULL )
        {
            if( pos -> FibHeap == p )
            {
                pos -> min_key -= d;
                break;
            }
            pos = pos -> next;
        }
    }

    p -> key -= d;
    if( p -> key < p -> parent -> key )
    {
        struct Node* tmp = p -> parent;
        insert_root(p);
        p -> parent = tmp;
        delete_node(p);
        p -> parent = NULL;
    }


    return;
}
signed main()
{
    init_f_heap(), degree_init();
    char s[30] = {};
    int cnt = 0;
    while( scanf("%s",s) )
    {
        if( s[0] == 'q' ) break;
        if( s[0] == 'e' )
        {
            struct f_heap* ans = extract();
            if( ans == NULL ) continue;
            printf("(%lld)%lld\n",ans->FibHeap->key,ans->FibHeap->val);
            delete_root(ans,1);
            cnt--;
        }
        if( s[0] == 'i' )
        {
            int target_key,target_val;
            scanf("%lld %lld",&target_key,&target_val);
            insert_new_node(target_key,target_val);
            cnt++;
        }
        if( s[0] == 'd' && s[1] == 'e' && s[2] == 'c' )
        {
            int target_key,target_val,d;
            scanf("%lld %lld %lld",&target_key,&target_val,&d);
            decrease(target_key,target_val,d);
        }
        if( s[0] == 'd' && s[1] == 'e' && s[2] == 'l' )
        {
            int target_key,target_val;
            scanf("%lld %lld",&target_key,&target_val);
            delete_target(target_key,target_val);
            cnt--;
        }

        if( cnt == 0 ) init_f_heap(),degree_init();
    }
}