// Name: Sahil Punchhi
// zID: z5204256
// Data Structures: Assignment 2: AVL Trees
// Date: 3rd April 2019

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int  value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

// declaring some functions
int InsertNode(AVLTree *T, int k, int v);
AVLTreeNode *insert (AVLTreeNode *node, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);
AVLTreeNode *SearchNode(AVLTreeNode *node, int k, int v);
#define max(x,y)((x>y)?x:y)

// SOME AUXILLIARY FUNCTIONS

// to get the height of the tree
int height(AVLTreeNode *N){
    
    if (N == NULL)
        return -1;
    return N->height;
}

// int add (int lhs, int rhs);   declare a function

//     y                                x
//    / \      Right Rotation          /  \
//   x   T3    - - - - - - - >        T1   y
//  / \        < - - - - - - -            / \
// T1  T2      Left Rotation            T2  T3

// right rotate subtree rooted with y
AVLTreeNode *rotateRight(AVLTreeNode *y) {
    
    AVLTreeNode *x = y->left;
    AVLTreeNode *T2 = x->right;
    
    //perform rotation
    x->right = y;
    y->left = T2;
    
    //update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    //update parents
    x->parent = y->parent;
    y->parent = x;
    if (T2 != NULL){
        T2->parent = y;}
    
    //return new root
    return x;
}

// left rotate subtree rooted with x
AVLTreeNode *rotateLeft(AVLTreeNode *x) {
    
    AVLTreeNode *y = x->right;
    AVLTreeNode *T2 = y->left;
    
    //perform rotation
    y->left = x;
    x->right = T2;
    
    //update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
    
    //update parents
    y->parent = x->parent;
    x->parent = y;
    if (T2 != NULL){
        T2->parent = x;}
    
    //return new root
    return y;
}

// find difference in heights to check if node is balanced
int balanceTree(AVLTreeNode *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// FUNCTION 1
// time complexity - O(nlog(n)), where n is size of Tree
// time complexity of insert function is log(n) and we iterate through n elements, so overall O(nlog(n))
AVLTree *CreateAVLTree(const char *filename)
{
    
    int m = 0; int array[9999];
    AVLTree *T = newAVLTree();
    if (strcmp(filename, "stdin") == 0) {
        char *array2 = malloc(512 * sizeof(char *));
        int digit = 0; int bit = 1; int p = 0;
        while(scanf("%[^\n]%*c", array2) == 1){
            for (int p=0 ; array2[p] != '\0'; p++) {
                if ((array2[p] <= '9' && array2[p] >= '0') || array2[p] == '-') {
                 if (array2[p + 1] <= '9' && array2[p + 1] >= '0') {
                  if (array2[p] == '-') { bit = -1;
                    } else {digit = (array2[p] - '0') + 10 *digit;}
                    } else {
                    array[m++] = ((array2[p] - '0') + digit * 10) * bit; digit = 0; bit = 1; }}}}
        free(array2);  fflush(stdin);

    } else {
        char t; int digit; FILE *fil = fopen(filename, "r");
        while (!feof(fil)) {
            if (fscanf(fil, "%d", &digit) == 1) {
                array[m++] = digit;}
            else {t = fgetc(fil); }
        }fclose(fil);
    }
    // we alternatively add key and value pairs of each node using Insert function to form tree
    int k = 0;
    for (int k=0; k < m - 1; k += 2) {
        InsertNode(T, array[k], array[k + 1]);}
    return T;
}


// auxilliary function for function 2 to recursively clone node of a tree
AVLTreeNode *CloneAVLTreeNode(AVLTreeNode *node)
{
    if (node == NULL)
        return node;
    AVLTreeNode * newNode = newAVLTreeNode(node->key, node->value);
    newNode->height = node->height;
    newNode->parent = node->parent;
    newNode->left = CloneAVLTreeNode(node->left);
    newNode->right = CloneAVLTreeNode(node->right);
    // Return root of cloned tree
    return newNode;
}

// FUNCTION 2
// time complexity - O(n), where n is size of T since we traverse through every node in above auxilliary function
AVLTree *CloneAVLTree(AVLTree *T)
{
    AVLTree * clone_tree = newAVLTree();
    clone_tree->root = CloneAVLTreeNode(T->root);
    return clone_tree;
    
}

// auxilliary function for function 3 to recursively insert union node
void AVLTreesUnion1(AVLTree *union_tree, AVLTreeNode *node) {
    if (node == NULL) {
        return;
    }
    InsertNode(union_tree, node->key, node->value);
    AVLTreesUnion1(union_tree, node->left);
    AVLTreesUnion1(union_tree, node->right);
}

// FUNCTION 3
// time complexity - O(n + mlog (n+m)), where n is size of Tree T1 and m is size of tree T2
// time complexity of clone function is O(n) where n is size of tree T1, time complexity of Insert is O(log(m+n))
// and we iterate through m nodes of Tree T2
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    AVLTree * union_tree = newAVLTree();
    union_tree = CloneAVLTree(T1);
    AVLTreesUnion1(union_tree, T2->root);
    return union_tree;
}

// auxilliary function for function 4 to recursively insert intersecting node
void AVLTreesIntersection1(AVLTree *intersection_tree, AVLTree *T1, AVLTreeNode *node)
{
    if (node == NULL){
        return;
    }
    if (Search(T1, node->key, node->value))
        InsertNode(intersection_tree, node->key, node->value);
    
    AVLTreesIntersection1(intersection_tree, T1, node->left);
    AVLTreesIntersection1(intersection_tree, T1, node->right);
}

// FUNCTION 4
// time complexity - O(m*log (n+m)), where n is size of Tree T1 and m is size of tree T2
// time complexity of search function is log n where n is size of tree 1 and max time complexity of insert function is log(m+n) and we iterate through m nodes of tree T2
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    AVLTree * intersection_tree = newAVLTree();
    AVLTreesIntersection1(intersection_tree, T1, T2->root);
    return intersection_tree;
}

// // auxilliary function for function 5 to insert a new node
AVLTreeNode *insert (AVLTreeNode *node, int k, int v) {
    
    // normal binary search tree insertion
    if (node == NULL) {
        return(newAVLTreeNode(k, v));
    }
    
    else if (k < node->key){
        node->left = insert(node->left, k, v);
        node->left->parent = node;
    }
    
    else if (k > node->key){
        node->right = insert(node->right, k, v);
        node->right->parent = node;
    }
    
    else if (k == node->key){
        
        if (v < node->value){
            node->left = insert(node->left, k, v);
            node->left->parent = node;
        }
        
        else if (v > node->value){
            node->right = insert(node->right, k, v);
            node->right->parent = node;
        }
    }
    
    else if (k == node->key && v == node->value){
        return node;}
    
    //  update height of ancestor node
    node->height = 1 + max(height(node->left), height(node->right));
    
    //height difference to check if this node has become unbalanced
    int balance = balanceTree(node);
    
    // If this node becomes unbalanced, then there are 4 cases
    
    // Left Left Case
    if (balance > 1 && ((k < node->left->key) || (k == node->left->key && v < node->left->value)))
        return rotateRight(node);
    
    // Right Right Case
    if (balance < -1 && ((k > node->right->key) || (k == node->right->key && v > node->right->value)))
        return rotateLeft(node);
    
    // Left Right Case
    if (balance > 1 && ((k > node->left->key) || (k == node->left->key && v > node->left->value)))
    {
        node->left =  rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right Left Case
    if (balance < -1 && ((k < node->right->key) || (k == node->right->key && v < node->right->value)))
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    
    return node;
}

// FUNCTION 5
// time complexity - O(log n), where n is size of T
int InsertNode(AVLTree *T, int k, int v)
{
    if (Search(T, k, v) == NULL){
        T->root = insert(T->root, k, v); // we start from root and then recursively add, T->root is a node, not a tree
        return 1;}
    else
        return 0;
}

// given a non-empty binary search tree, return the node with minimum key value found in that tree
AVLTreeNode * minValue(AVLTreeNode * node)
{
    AVLTreeNode * current = node;
    while (current->left != NULL)
        current = current->left;
    
    return current;
}

// auxilliary function for function 6 to recursively find and delete node of a tree with given key, value from //subtree with given root returning root of the modified subtree.
AVLTreeNode *delete(AVLTreeNode * root, int key, int value)
{
    
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;
    
    // If the key to be deleted is smaller than the root’s key, then it lies in left subtree
    if ( key < root->key )
        root->left = delete(root->left, key, value);
    
    // If the key to be deleted is greater than the root’s key, then it lies in right subtree
    else if( key > root->key )
        root->right = delete(root->right, key, value);
    
    else if (key == root->key){
        
        if (value < root->value)
            root->left = delete(root->left, key, value);
        
        
        else if (value > root->value)
            root->right = delete(root->right, key, value);
        
        
        
        // if key is same as root’s key and value is same as root's value, then This is the node to be deleted
        else
        {
            // node with only one child or no child
            if( (root->left == NULL) || (root->right == NULL) )
            {
                AVLTreeNode *temp = root->left ? root->left : root->right;
                
                // No child case
                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else
                // One child case
                    *root = *temp;
                free(temp);
            }
            else
            {
                // node with two children
                AVLTreeNode* temp = minValue(root->right);
                root->key = temp->key;
                root->value = temp->value;
                root->right = delete(root->right, temp->key, temp->value);
            }
        }
    }
    // If the tree had only one node then return
    if (root == NULL)
        return root;
    
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),height(root->right));
    
    
    // STEP 3: GET THE TREE BALANCE FOR THIS NODE (to check whether this node became unbalanced)
    int balance = balanceTree(root);
    
    // If this node becomes unbalanced, then there are 4 cases
    
    // Left Left Case
    if (balance > 1 && balanceTree(root->left) >= 0)
        return rotateRight(root);
    
    // Left Right Case
    if (balance > 1 && balanceTree(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    
    // Right Right Case
    if (balance < -1 && balanceTree(root->right) <= 0)
        return rotateLeft(root);
    
    // Right Left Case
    if (balance < -1 && balanceTree(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    
    return root;
}

// FUNCTION 6
// time complexity for search is O(log n) where n is size of tree
int DeleteNode(AVLTree *T, int k, int v)
{
    if (Search(T, k, v) != NULL){
        AVLTreeNode * m = delete(T->root, k, v);
        if (m != NULL && m->parent == NULL){
            T->root = m;}
        else if (m == NULL){
            T->root = NULL;
        }
        
        return 1;}
    else
        return 0;
  
}

// auxilliary function for function 7 to recursively search node of tree
AVLTreeNode *SearchNode(AVLTreeNode *node, int k, int v)
{
    AVLTreeNode *n = NULL;
    if (node == NULL) {
        return NULL;
    }
    if (node->key == k && node->value == v){
        n = node;
    }
    else{
        if (k < node->key)
            n = SearchNode(node->left, k, v);
        else if (k > node->key)
            n = SearchNode(node->right, k, v);
        else {
            if (v < node->value)
                n = SearchNode(node->left, k, v);
            else if (v > node->value)
                n = SearchNode(node->right, k, v);
        }
    }
    return n;
}

// FUNCTION 7
// time complexity - O(log n), where n is size of T
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
    AVLTreeNode *n = SearchNode(T->root, k, v);
    return n;
}

// auxilliary function for function 8 to recursively free node of tree
void FreeAVLTreeNode(AVLTreeNode *node)
{
    if (node == NULL) {
        return;
    }
    
    FreeAVLTreeNode(node->left);
    FreeAVLTreeNode(node->right);
    free(node);
    
}

// FUNCTION 8
// time complexity - O(n), where n is size of T, follows post order traversal
// each call to free() takes O(1) time
void FreeAVLTree(AVLTree *T)
{
    FreeAVLTreeNode(T->root);
    free(T);
}

// auxilliary function for function 9 to recursively print node of tree
void PrintAVLTreeNode(AVLTreeNode *node)
{ if(node != NULL)
{ PrintAVLTreeNode(node->left);
    printf("(%d, %d) %d \n",node->key,node->value, node->height);
    PrintAVLTreeNode(node->right); }
}

// FUNCTION 9
// time complexity - O(n), where n is size of T, follows in order traversal
void PrintAVLTree(AVLTree *T)
{
    if(T != NULL){
        PrintAVLTreeNode(T->root);}
}


// display Tree sideways
void showTreeR(AVLTreeNode *n, int depth) {
    if (n != NULL) {
        showTreeR(n->right, depth+1);
        int i;
        for (i = 0; i < depth; i++)
            putchar('\t');            // TAB character
        printf("(%d, %d) %d\n", n->key, n->value, n->height);
        showTreeR(n->left, depth+1);
    }
}

void showTree(AVLTreeNode *n) {
    showTreeR(n, 0);
}

//  /Users/sahilpunchhi/Desktop/COMP_9024_DSA/Assignments/Assignment_2/
//////////////////////////////////////////////////////////////////////////////////////////////


int main() //sample main for testing
{ int i,j;
    AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
    AVLTreeNode *node1;
    
    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
//    you need to create the text file file1.txt
//    to store a set of items without duplicate items
    tree2=CreateAVLTree("File3.txt");
    PrintAVLTree(tree2);
    tree3=CloneAVLTree(tree2);
    PrintAVLTree(tree3);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);
    //Create tree4
    tree4=newAVLTree();
    j=InsertNode(tree4, 10, 10);
    for (i=0; i<15; i++)
    {
        j=InsertNode(tree4, i, i);
        if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree4);
    node1=Search(tree4,20,20);
    if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
    else
        printf("Key 20 does not exist\n");

    for (i=17; i>0; i--)
    {
        j=DeleteNode(tree4, i, i);
        if (j==0)
            printf("Key %d does not exist\n",i);
        PrintAVLTree(tree4);
    }
    FreeAVLTree(tree4);
    //Create tree5
    tree5=newAVLTree();
    j=InsertNode(tree5, 6, 25);
    j=InsertNode(tree5, 6, 10);
    j=InsertNode(tree5, 6, 12);
    j=InsertNode(tree5, 6, 20);
    j=InsertNode(tree5, 9, 25);
    j=InsertNode(tree5, 10, 25);
    PrintAVLTree(tree5);
    //Create tree6
    tree6=newAVLTree();
    j=InsertNode(tree6, 6, 25);
    j=InsertNode(tree6, 5, 10);
    j=InsertNode(tree6, 6, 12);
    j=InsertNode(tree6, 6, 20);
    j=InsertNode(tree6, 8, 35);
    j=InsertNode(tree6, 10, 25);
    PrintAVLTree(tree6);
    tree7=AVLTreesIntersection(tree5, tree6);
    tree8=AVLTreesUnion(tree5,tree6);
    PrintAVLTree(tree7);
    PrintAVLTree(tree8);
 
    return 0;
}
