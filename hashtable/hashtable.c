/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  //null check
  if(table == NULL){
    return;
  }
  // set up table according to max size
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  //null check
  if(table == NULL || key == NULL){
    return NULL;
  }
  //save the searched item based on the index in the table by key
  ht_item_t *item = (*table)[get_hash(key)];
  //is in table
  while(item != NULL){
    //compare keys
    if(strcmp(item->key,key) == 0){
      return item;
    }
    else{
      // move
      item = item->next;
    }
  }
  //nothing found
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  //null check
  if(table == NULL || key == NULL){
    return;
  }
  int h_key = get_hash(key);
  //search item with key in table
  ht_item_t *item = ht_search(table,key);
  // is in table
  if(item != NULL){
    item->value = value;
  }
  else{ // not in table
    item = (ht_item_t*)malloc(sizeof(ht_item_t));
    item->key = key;
    item->value = value;
    if((*table)[h_key] == NULL){
      item->next = NULL;
    }
    else{
      item->next = (*table)[h_key];
    }
    (*table)[h_key]= item;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if(table == NULL || key == NULL){
    return NULL;
  }
  //search item with key in table
  ht_item_t *item = ht_search(table,key);
  // is in table
  if(item != NULL){
    return &(item->value);
  }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if(table == NULL || key == NULL){
    return;
  }
  int h_key = get_hash(key); //key hash
  // take item from table based on the index from key
  ht_item_t *item = (*table)[h_key];
  ht_item_t *tmp = NULL;
  while(item != NULL){
    if(item->key == key){ //is found
      if(item == (*table)[h_key]){
        (*table)[h_key] = item->next;
      }
      if(tmp != NULL){
        tmp->next = item->next;
      }
      free(item);
      return;
    }
    tmp = item;
    item = item->next;
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if(table == NULL){
    return;
  }
  ht_item_t *item = NULL; //active
  ht_item_t *next_item = NULL; //next
  //go through all cells
  for(int i = 0; i < HT_SIZE; i++){
    item = (*table)[i];
    while(item != NULL){ //delete all items
      next_item = item->next;
      free(item);
      item = next_item;
    }
    (*table)[i] = NULL;
  }
}
