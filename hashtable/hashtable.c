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
  for (int i = 0; i < length; i++) { // sum every character into result
    result += key[i];
  }
  return (result % HT_SIZE); 
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++)
    table[0][i]= NULL;
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *item = table[0][hash]; 
  while (item != NULL)
  {
    if (item->key == key)
      return item;
    if (item->next != NULL)
      item = item->next;
    else 
      return NULL;
  }
  return item;//either NULL or poinetr
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
  ht_item_t *item = ht_search(table,key); // look if exists
  if (item == NULL) // key doesnt exists in the table
  {
    int hash = get_hash(key);

    ht_item_t *new = malloc(sizeof(ht_item_t)); //create new
    new->key=key;
    new->value=value;
    new->next=table[0][hash];
    table[0][hash] = new; //cur


    if (item == NULL) //attach 
    {
      table[0][hash] = new;
      return;
    }

    while (item != NULL)
    {
      if (item->next != NULL)
        item = item->next;
      else //attach to the list of synonyms
      {
        item->next = new;
        return;
      }
    }
  }
  else //there already exists key
  {
    item->value = value;
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
  ht_item_t *item = ht_search(table, key);
  if (item != NULL)
    return &item->value;
  
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
  int hash = get_hash(key);
  ht_item_t *item = table[0][hash];
  ht_item_t *prev = table[0][hash];
  
  while (item != NULL) // it might exist 
  {
    if (item->key == key)// del me
    {
      if (prev == item) // meaning the very first
      {
        if (item->next != NULL)
          table[0][hash] = item->next;
        else
          table[0][hash] = NULL;
        free(item);
        return;
      }
      else  
      {
        if (item->next != NULL)
          prev->next = item->next;
        else
          prev->next = NULL;
        free(item);
        return;
      }
    }
    prev = item;
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
  for (int i = 0; i < HT_SIZE; i++)
  {
    ht_item_t *destroyLater[30];
    destroyLater[0]=NULL;
    int x = 0;
    ht_item_t *cur = table[0][i];

    while (cur != NULL)
    {
      destroyLater[x++]=cur; //attach to destroy later
      if (cur->next != NULL)
        cur =cur->next;
      else
        break;
    }
    if (destroyLater[0]!=NULL)
      for (int j = 0; j < x; j++)
        free(destroyLater[j]);
    table[0][i] = NULL; //to init
  }
  
}
