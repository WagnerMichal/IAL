/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
  *tree = NULL; // init
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree == NULL){ //check null
    return false;
  }
  else if(tree->key == key){ //searched element is root
    *value = tree->key;
    return true;
  }
  else if(tree->key > key){ // smaller than root
    return bst_search(tree->left, key, value);
  }
  else{ // bigget than root
    return bst_search(tree->right, key, value);
  }
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if(*tree == NULL){ // tree is empty
    *tree = malloc(sizeof(struct bst_node));
    if(*tree == NULL){ // allocation test
      return;
    }
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }
  else if((*tree)->key > key){ // searched key is smaller than root
    bst_insert(&((*tree)->left),key, value);
    return;
  }
  else if((*tree)->key < key){ // searched key is bigger than root
    bst_insert(&((*tree)->right),key, value);
    return;
  }
  (*tree)->value = value; // replace value
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(*tree == NULL || target == NULL){
    return;
  }
  else if((*tree)->right == NULL){ // right sub-tree is empty
    target->value = (*tree)->value; //value of target to value of root
    target->key = (*tree)->key; //key of root to key of root
    bst_node_t *left = (*tree)->left; // left sub-tree backup
    free(*tree);
    *tree = left;
  }
  else{
    bst_replace_by_rightmost(target,&((*tree)->right));
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if(*tree == NULL){
    return;
  }
  else if(key > (*tree)->key){ // searched element is bigger than root
    bst_delete(&(*tree)->right,key);
  }
  else if(key < (*tree)->key){ // searched element is smaller than root
    bst_delete(&(*tree)->left,key);
  }
  else{
    bst_node_t *back_up = *tree;
    if((*tree)->right == NULL){ //right sub-tree is empty
      *tree = back_up->left;
      free(back_up);
    }
    else if((*tree)->left == NULL){ //left sub-tree is empty
      *tree = back_up->right;
      free(back_up);
    }
    else{
      if(*tree != NULL){
        bst_replace_by_rightmost(*tree,&(*tree)->left);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if(*tree == NULL){
    return;
  }
  else if((*tree)->left != NULL){ // dispose left
    bst_dispose(&((*tree)->left));
  }
  else if((*tree)->right != NULL){ // dispose right
    bst_dispose(&((*tree)->right));
  }
  free(*tree);
  *tree = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if(tree == NULL){
    return;
  }
  bst_print_node(tree);
  bst_preorder(tree->left);
  bst_preorder(tree->right);
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree == NULL){
    return;
  }
  bst_inorder(tree->left);
  bst_print_node(tree);
  bst_inorder(tree->right);
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree == NULL){
    return;
  }
  bst_postorder(tree->left);
  bst_postorder(tree->right);
  bst_print_node(tree);
}
