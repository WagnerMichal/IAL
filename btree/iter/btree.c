/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL; //init
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while(tree != NULL){
    if(key > tree->key){ // searched key is bigger than root, go right
      tree = tree->right;
    }
    else if(key < tree->key){ // searched key is smaller than root, go left
      tree = tree->left;
    }
    else if(key == tree->key){ // key found
      *value = tree->value;
      return true;
    }
    else{ //not found
      return false;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  while(*tree){
    if(key < (*tree)->key){ // searched key is smaller than root, go left
      tree = &((*tree)->left);
    }
    else if(key > (*tree)->key){ // searched key is bigger than root, go right
      tree = &((*tree)->right);
    }
    else if((*tree)->key == key){ //found, replace
      (*tree)->value = value;
      return;
    }
  }
  if((*tree) == NULL){ //new node
    bst_node_t *element = malloc(sizeof(struct bst_node));
    if (element == NULL){
      return;
    }
    (element)->key = key;
    (element)->value = value;
    (element)->left = NULL;
    (element)->right = NULL;
    *tree = element;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(target == NULL || *tree == NULL){
    return;
  }
  else{
    while((*tree)->right != NULL){ // until right sub-tree is empty
      tree = &(*tree)->right; // go right
    }
    target->value = (*tree)->value; // replace value
    target->key = (*tree)->key; // replace key

    bst_node_t *left = (*tree)->left; //left backup
    free(*tree);
    *tree = left;
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if(*tree == NULL || tree == NULL){
    return;
  }
  else{
    while(*tree != NULL && (*tree)->key != key){ // until tree is empty and root key is found
      if(key > (*tree)->key){ //searched key is bigger than root, go right
        tree = &((*tree)->right);
      }
      else if(key < (*tree)->key){ //searched key is smaller than root, go left
        tree = &((*tree)->left);
      }
    }
    if (*tree == NULL){ // node is not in tree
      return;
    }
    bst_node_t *back_up = *tree;
    if((*tree)->right == NULL && (*tree)->left != NULL){ // only right son
      back_up = (*tree)->left;
      free(*tree);
      *tree = back_up;
    }
    else if((*tree)->left == NULL && (*tree)->right != NULL){ // only left son
      back_up = (*tree)->right;
      free(*tree);
      *tree = back_up;
    }
    else if((*tree)->left == NULL && (*tree)->right == NULL){ //dont have sons
      free(*tree);
      *tree = NULL;
    }
    else{ //both sons
      if(*tree != NULL){
        bst_replace_by_rightmost(*tree, &(*tree)->left);
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if(*tree == NULL){
    return;
  }
  stack_bst_t stack;
  stack_bst_init(&stack);

  do{
    if(*tree == NULL){
      if(!stack_bst_empty(&stack)){ //take node from stack
        *tree = stack_bst_top(&stack); //save top to root
        stack_bst_pop(&stack); 
      }
    }
    else{
      if((*tree)->right != NULL){
        stack_bst_push(&stack, (*tree)->right); //push right to stack
      }
      bst_node_t *tmp = *tree;
      *tree = (*tree)->left;
      free(tmp);
    }
  } while((*tree != NULL) || (!stack_bst_empty(&stack)));
  *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  if(tree == NULL){
    return;
  }
  while(tree){
    bst_print_node(tree);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  if(tree == NULL){
    return;
  }
  while(tree){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree == NULL){
    return;
  }
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_pop(&stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  if(tree == NULL){
    return;
  }
  while(tree){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree == NULL){
    return;
  }
  stack_bst_t stack;
  stack_bool_t b_stack;
  bool is_from_left;

  stack_bst_init(&stack);
  stack_bool_init(&b_stack);
  bst_leftmost_postorder(tree, &stack, &b_stack);
  while(!stack_bst_empty(&stack)){
    tree = stack_bst_top(&stack);
    is_from_left = stack_bool_top(&b_stack);
    stack_bool_pop(&b_stack);
    //from left, go right
    if(is_from_left){
      stack_bool_push(&b_stack, false);
      bst_leftmost_postorder(tree->right, &stack, &b_stack);
    }
    else{
      //from right, remove node
      tree = stack_bst_pop(&stack);
      bst_print_node(tree);
    }
  }
}
