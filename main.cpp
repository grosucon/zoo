#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct node_s {
    struct node_s *left;
    struct node_s *right;
    int count;
    int allocatedCount;
    unsigned long int power;
    unsigned long int* identifier;
};

typedef struct node_s node_t;

struct tree_s {
    struct node_s *root;
};

typedef struct tree_s tree_t;

void add_animal(node_s* node, unsigned long int power, unsigned long int identifier) {
    if(node){
        node->power = power;
        node->count++;
        if(node->allocatedCount < node->count){
            node->allocatedCount++;
            node->identifier = (unsigned long int*)realloc(node->identifier, node->count*sizeof(unsigned long int));
        }
        node->identifier[node->count-1] = identifier;
    }
}

unsigned long int get_animal(node_s* node) {
    unsigned long int minimalIdentifier = node->identifier[0];
    int index = 0;
    for(int i=1; i<node->count; i++){
        if(node->identifier[i] < minimalIdentifier){
            minimalIdentifier = node->identifier[i];
            index = i;
        }
    }
    node->identifier[index] = node->identifier[node->count-1];
    node->count--;
    return minimalIdentifier;
}

/* Create a new AVL tree. */
tree_t *create() {
    tree_t *tree = (tree_t*)malloc( sizeof( tree_t ) );
    if(tree == nullptr)  {
        return nullptr;
    }
    tree->root = nullptr;
    return tree;
}

/* Initialize a new node. */
node_t *create_node() {
    node_t *node = (node_t*)malloc(sizeof(node_t));
    if( node == nullptr ) {
        return nullptr;
    }
    node->left = nullptr;
    node->right = nullptr;
    node->count = 0;
    node->allocatedCount = 0;
    node->power = 0;
    node->identifier = 0;
    return node;
}

/* Find the height of an AVL node recursively */
int node_height( node_t *node ) {
    int height_left = 0;
    int height_right = 0;

    if( node->left ) height_left = node_height( node->left );
    if( node->right ) height_right = node_height( node->right );

    return height_right > height_left ? ++height_right : ++height_left;
}

/* Find the balance of an AVL node */
int balance_factor( node_t *node ) {
    int bf = 0;

    if( node->left  ) bf += node_height( node->left );
    if( node->right ) bf -= node_height( node->right );

    return bf ;
}

/* Left Left Rotate */
node_t *rotate_leftleft( node_t *node ) {
    node_t *a = node;
    node_t *b = a->left;

    a->left = b->right;
    b->right = a;

    return( b );
}

/* Left Right Rotate */
node_t *rotate_leftright( node_t *node ) {
    node_t *a = node;
    node_t *b = a->left;
    node_t *c = b->right;

    a->left = c->right;
    b->right = c->left;
    c->left = b;
    c->right = a;

    return( c );
}

/* Right Left Rotate */
node_t *rotate_rightleft( node_t *node ) {
    node_t *a = node;
    node_t *b = a->right;
    node_t *c = b->left;

    a->right = c->left;
    b->left = c->right;
    c->right = b;
    c->left = a;

    return( c );
}

/* Right Right Rotate */
node_t *rotate_rightright( node_t *node ) {
    node_t *a = node;
    node_t *b = a->right;

    a->right = b->left;
    b->left = a;

    return( b );
}

/* Balance a given node */
node_t *balance_node( node_t *node ) {
    node_t *newroot = NULL;

    /* Balance our children, if they exist. */
    if( node->left )
        node->left  = balance_node( node->left  );
    if( node->right )
        node->right = balance_node( node->right );

    int bf = balance_factor( node );

    if( bf >= 2 ) {
        /* Left Heavy */

        if( balance_factor( node->left ) <= -1 )
            newroot = rotate_leftright( node );
        else
            newroot = rotate_leftleft( node );

    } else if( bf <= -2 ) {
        /* Right Heavy */

        if( balance_factor( node->right ) >= 1 )
            newroot = rotate_rightleft( node );
        else
            newroot = rotate_rightright( node );

    } else {
        /* This node is balanced -- no change. */

        newroot = node;
    }

    return( newroot );
}

/* Balance a given tree */
void balance( tree_t *tree ) {

    node_t *newroot = NULL;

    newroot = balance_node( tree->root );

    if( newroot != tree->root )  {
        tree->root = newroot;
    }
}

/* Insert a new node. */
void insert( tree_t *tree, unsigned long int power, unsigned long int identifier ) {
    node_t *node = NULL;
    node_t *next = NULL;
    node_t *last = NULL;

    /* Well, there must be a first case */
    if( tree->root == NULL ) {
        node = create_node();
        add_animal(node, power, identifier);
        tree->root = node;

        /* Okay.  We have a root already.  Where do we put this? */
    } else {
        next = tree->root;

        while( next != NULL ) {
            last = next;

            if( power < next->power ) {
                next = next->left;

            } else if( power > next->power ) {
                next = next->right;

                /* Have we already inserted this node? */
            } else if( power == next->power ) {
                break;
            }
        }

        node = create_node();
        add_animal(node, power, identifier);

        if( power < last->power ) last->left = node;
        if( power > last->power ) last->right = node;
        if( power == last->power ) add_animal(last, power, identifier);

    }

    balance( tree );
}

node_t* find_min(node_t* node) {
    if(node){
        if(node->left != nullptr){
            return find_min(node->left);
        }else{
            return node;
        }
    }
    return nullptr;
}

node_t* find_max(node_t* node) {
    if(node){
        if(node->right != nullptr){
            return find_max(node->right);
        }else{
            return node;
        }
    }
    return nullptr;
}

node_t* remove_min(node_t* node) {
    if(node->left == nullptr){
        return node->right;
    }
    node->left = remove_min(node->left);
    return balance_node(node);
}

node_t* remove_max(node_t* node) {
    if(node->right == nullptr){
        return node->left;
    }
    node->right = remove_max(node->right);
    return balance_node(node);
}

unsigned long int delete_max( tree_t *tree, int* war ) {
    node_t *tmpNode = find_max(tree->root);
    if(tmpNode){
        unsigned long int iden = get_animal(tmpNode);
        if(tmpNode->count == 0)
            tree->root = remove_max(tree->root);
        return iden;
    }
    *war = 1;
    return 0;
}

unsigned long int delete_min( tree_t *tree, int* war ) {
    node_t *tmpNode = find_min(tree->root);
    if(tmpNode){
        unsigned long int iden = get_animal(tmpNode);
        if(tmpNode->count == 0)
            tree->root = remove_min(tree->root);
        return iden;
    }
    *war = 1;
    return 0;
}

int main( int argc, char **argv ) {
    tree_t** trees = (tree_t**)malloc(10000*sizeof(tree_t*));
    for(int i=0; i<10000; i++){
        trees[i] = (tree_t*)malloc(sizeof(tree_t));
    }
    while ( 1 ) {
        int end = 0;
        int prikaz;
        scanf("%d", &prikaz);
        switch(prikaz){
            case 0: {
                int tree_number;
                unsigned long int animalIdentifier, animalPower;
                scanf("%d %lu %lu", &tree_number, &animalIdentifier, &animalPower);
                insert(trees[tree_number-1], animalPower, animalIdentifier);
                break;
            }
            case 1: {
                int war = 0;
                int tree_number;
                scanf("%d", &tree_number);
                unsigned long int identifier = delete_max(trees[tree_number-1], &war);
                if(war == 1){
                    printf("empty\n");
                }else{
                    printf("%ld\n", identifier);
                }
                break;
            }
            case 2: {
                int war = 0;
                int tree_number;
                scanf("%d", &tree_number);
                unsigned long int identifier = delete_min(trees[tree_number-1], &war);
                if(war == 1){
                    printf("empty\n");
                }else{
                    printf("%ld\n", identifier);
                }
                break;
            }
            case 4:
                end = 1;
                break;
            default:
                break;
        }
        if(end){
            break;
        }
    }

    return 0;
}
