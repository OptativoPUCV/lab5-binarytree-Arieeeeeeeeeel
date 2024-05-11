#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    void* key;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *map = (TreeMap*)malloc(sizeof(TreeMap));
    if (map){
        map->root = NULL;
        map->lower_than = lower_than;
    }
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL){
        return; 
    }

    TreeNode* auxiliar = tree->root;
    TreeNode* parent = NULL;
    while (auxiliar != NULL){
        parent = auxiliar;
        if(is_equal(tree,auxiliar->pair->key,key)){
            return;
        }
        if(tree->lower_than(auxiliar->pair->key,key)){
            auxiliar = auxiliar->right;
        }
        else{
            auxiliar = auxiliar->left;
        }
    }
    TreeNode* nuevoTreeNode = createTreeNode(key, value);
    nuevoTreeNode->parent = parent;
    if (parent == NULL){
        tree->root = nuevoTreeNode;
    }
    else{
        if(tree->lower_than(parent->pair->key,key)){
            parent->right = nuevoTreeNode;
        }
        else{
            parent->left = nuevoTreeNode;
        }
    }
    tree->current = nuevoTreeNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL){
        return NULL;
    }
    while (x->left != NULL){
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL){
            return NULL;
        }
        TreeNode *auxiliar = tree->root;

        while (auxiliar != NULL){
        if (is_equal(tree, key, auxiliar->pair->key)){
            tree->current = auxiliar;
            return auxiliar->pair;
        }
        else if (tree->lower_than(key, auxiliar->pair->key)){
            auxiliar = auxiliar->left;
        }
        else{
            auxiliar = auxiliar->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL){
        return NULL;
    }
    TreeNode *current = tree->root;
    TreeNode *nodo_auxiliar = NULL;
    while (current != NULL) {
        int cmp = tree->lower_than(current->pair->key, key);

        if (cmp >= 0) {
            nodo_auxiliar= current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (nodo_auxiliar == NULL) {
        return NULL;
    }

    if (tree->lower_than(nodo_auxiliar->pair->key, key) == 0) {
        return nodo_auxiliar->pair;
    }
    return nodo_auxiliar->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * minimo = minimum(tree->root);
    if(minimo!= NULL) return minimo->pair;
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL){
        return NULL;
    }

    TreeNode* next = tree->current;
    if(next == tree->root || next->right != NULL) {
        next = minimum(next->right);
        tree->current = next;
        return tree->current->pair;
      } 
    else{
        if (next->right == NULL){
            TreeNode *aux = next;
            while(next->parent != NULL){
                aux = next;
                next = next->parent;
                if(next->left == aux){
                tree->current = next;
                return tree->current->pair;
                }
            }
            return NULL;
        }
    }
    return NULL;
}