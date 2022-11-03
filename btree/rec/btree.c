/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

// #include "../btree.c"
// #include "../test.c"
// #include "../test_util.h"
// #include "../test_util.c"

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
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
  if (tree == NULL)
    return false;
  if (tree->key == key) // found it
  {
    *value = tree->value;
    return true;
  }  
  else if (tree->key < key) // go left
  {
    if (tree->left != NULL) //is there another?
      return bst_search(tree->left,key,value);
    return false;
  }
  else //go right
  {
    if (tree->right != NULL) //is there another?
      return bst_search(tree->right,key,value);
    return false;
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
//po vymazani O cka pise divny subtree 
void bst_insert(bst_node_t **tree, char key, int value) {
  if (tree == NULL)
    return;
  if (tree[0] == NULL)//empty
  {
    bst_node_t * new = malloc(sizeof(bst_node_t));
    new->left=NULL;
    new->right=NULL;
    new->key=key;
    new->value=value;
    tree[0] = new;  
  }

  if (tree[0]->key == key) // found it
  {
    tree[0]->value=value;
  }

  else if (tree[0]->key > key) // go left
  {
    if (tree[0]->left != NULL) //is there another?
    {
      bst_insert(&tree[0]->left,key,value);
      return;
    }
    //insert
    bst_node_t * new = malloc(sizeof(bst_node_t));
    new->left=NULL;
    new->right=NULL;
    new->key=key;
    new->value=value;
    tree[0]->left = new;
    return;
  }
  else //go right
  {
    if (tree[0]->right != NULL) //is there another?
    {
      bst_insert(&tree[0]->right,key,value);
      return;
    }
    //insert
    bst_node_t * new = malloc(sizeof(bst_node_t));
    new->left=NULL;
    new->right=NULL;
    new->key=key;
    new->value=value;
    tree[0]->right = new;
    return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (tree == NULL)
    return;
  if(tree[0]== NULL)
    return;

  if (tree[0]->right == NULL)
  {
    target->key = tree[0]->key;
    target->value = tree[0]->value;
    bst_node_t *cur =tree[0];
    tree[0] = cur->left; 
    free(cur); //free root
  }
  else
    bst_replace_by_rightmost(target,&tree[0]->right);
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
  if (tree==NULL)
    return;
  if (tree[0]==NULL)
    return;
  
  if (tree[0]->key == key) // found it 
  {
    if (tree[0]->right == NULL && tree[0]->left == NULL) //no children
    {
      bst_replace_by_rightmost(tree[0],&tree[0]);
    }
    else if (tree[0]->right != NULL && tree[0]->left != NULL)//2 children
    {
      bst_replace_by_rightmost(tree[0],&tree[0]);
    }
    else//1 children 
    {
      bst_node_t *cur = tree[0];
      if(tree[0]->right)
      tree[0]= tree[0]->right; //attach to parent
          else
      tree[0]= tree[0]->left;
      free(cur);//delete 
    }
  }
  else if (tree[0]->key > key) // go left
    bst_delete(&tree[0]->left,key);
  else
    bst_delete(&tree[0]->right,key);
  return;
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
  if (tree==NULL)
    return;
  if (tree[0]==NULL)
    return;
  
  if (tree[0]->left != NULL) //dell left
    bst_dispose(&tree[0]->left);
  
  if (tree[0]->right != NULL) //dell right
    bst_dispose(&tree[0]->right);
  
  free(tree[0]); //free me
  
  tree[0]=NULL; //set me to null
  return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if (tree == NULL)
    return;
  bst_print_node(tree); // print node
  bst_preorder(tree->left); //first left
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
  if (tree == NULL)
    return;
  bst_inorder(tree->left); //first left
  bst_print_node(tree); // print node
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
  if (tree == NULL)
    return;
  bst_postorder(tree->left); //first left
  bst_postorder(tree->right);
  bst_print_node(tree); // print node
}
