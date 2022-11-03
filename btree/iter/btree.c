/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

// #include "../btree.c"
// #include "../test.c"
// #include "../test_util.h"
// #include "../test_util.c"
// #include "stack.c"

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
  tree[0]=NULL;
  return;
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
  bst_node_t *cur = tree;
  while (cur!=NULL)
  {
    if(key == cur->key)//found nod with same key
    {
      *value = cur->value;
      return true;
    }
    if (key < cur->key)//go left, value is lesser
    {
      cur=cur->left;
      continue;
    }
    if (key > cur->key)//go right, value is greater
    {
        cur=cur->right;
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
  if (tree == NULL)//invalid
    return;
  if (tree[0] == NULL) //empty tree
  {
    tree[0] = malloc(sizeof(bst_node_t));
    tree[0]->key = key;
    tree[0]->value =value;
    tree[0]->left=NULL;
    tree[0]->right=NULL;
    return;
  }

  bst_node_t *cur = tree[0];//current node
  bst_node_t *prev = tree[0];//previous node
  bool isLeft = false;

  while (cur!=NULL)
  {
    prev=cur;//catch previous
    if(key == cur->key)//found nod with same key
    {
      cur->value = value;
      return;
    }
    if (key < cur->key)//go left, value is lesser
    {
      cur=cur->left;
      isLeft = true;
      continue;
    }
    if (key > cur->key)//go right, value is greater
    {
        cur=cur->right;
        isLeft = false;
    }  
  }
  //init new node
  bst_node_t *new = malloc(sizeof(bst_node_t));
  new->left=NULL;
  new->right=NULL;
  new->key=key;
  new->value=value;
  if (isLeft)
    prev->left = new;
  else  
    prev->right = new;
  return;
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

/* checkout */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  /* ak ma laveho potomka, attachne ho na previous. */
  bst_node_t *cur = tree[0]->right;//current node
  bst_node_t *prev = tree[0];//previous node
  while (cur!=NULL)
  {
    if(cur->right != NULL) 
    {
      prev=cur;//catch previous
      cur = cur->right;
    }
    else //there is not another left node
      break;
  }
  if (cur->left!=NULL)//cur is not a leaf
    prev->right = cur->left;//attach to tree

  target->key=cur->key;
  target->value=cur->value;
  free(cur);  
  prev->right =NULL;
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
  bst_node_t *cur = tree[0];
  bst_node_t *prev = cur;
  bool isLeft = false;
  
  if (key == cur->key) // root
  {
    bst_replace_by_rightmost(cur,&cur->left); //replace by rightmost
    tree[0]->key = cur->key;
    tree[0]->value = cur->value;
    return;
  }
  
  
  //search
  while (cur!=NULL)
  {
    if(key == cur->key)//found nod with same key
    {
      if((cur->left == NULL && cur->right == NULL)) // no children
      {
        if(isLeft)
          prev->left =NULL;
        else 
          prev->right = NULL;
        free(cur);
      }
      else if (cur->left != NULL && cur->right != NULL) //2 children
      {
        bst_replace_by_rightmost(cur,&cur->left); //replace by rightmost
        if (isLeft)
          prev->left = cur;
        else  
          prev->right = cur;
      }       
      else //1 children
      {
        if(cur->left != NULL) //has left subtree
        {
          if (isLeft)
            prev->left=cur->left;
          else  
            prev->right=cur->left;
        }
        else  //has right subtree
        {
          if (isLeft)
            prev->left=cur->right;
          else  
            prev->right=cur->right;
        }
      }
    return;
    }

    prev  = cur;//keep previous node

    if (key < cur->key)//go left, value is lesser
    {
      cur=cur->left;
      isLeft = true;
      continue;
    }
    if (key > cur->key)//go right, value is greater
    {
      cur=cur->right;
      isLeft = false;
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
  if (tree==NULL)
    return;
  if (tree[0]==NULL)
    return;

  bst_node_t *cur = tree[0];//current node
  bst_node_t *prev = tree[0];//previous node
  bool isLeft = false;
  
  while (tree[0]!=NULL)
  {
    isLeft = false;
    cur=tree[0];//catch previous
    while (cur->left!=NULL || cur->right!=NULL)
    {
      if (cur->left != NULL) //do you have left child, go left
      {
        prev=cur;
        cur =cur->left;
        isLeft=true;//i am left
      }
      else if(cur->right!= NULL) //do you have right child, go right 
      {
        prev=cur;
        cur=cur->right;
        isLeft=false;//i am right
      }
      if (cur->left==NULL && cur->right==NULL)
      {
        free(cur);
        if(isLeft)
          prev->left=NULL;
        else  
          prev->right=NULL;
        break; //start from the beginning
      }
    }
    if (tree[0]->left==NULL && tree[0]->right==NULL)
    {
      free(tree[0]);
      tree[0]=NULL;
      break;
    }  
  }

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
  bst_node_t *current = tree;//currentrent node
  while (current != NULL)
  {
    if (current->left != NULL)
    {
      bst_print_node(current);
      stack_bst_push(to_visit,current);
      current = current->left;
    }
    else break;
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
  if (tree == NULL)
    return;  
  bst_node_t *cur = tree;//current node
  stack_bst_t *stack;
  stack_bst_init(stack);

  bst_leftmost_preorder(cur,stack);
  while (cur != NULL)
  {
    cur = stack_bst_pop(stack);
    if (cur->right == NULL)
      continue;
    bst_leftmost_preorder(cur->right,stack);
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
}






// int main(int argc, char const *argv[])
// {
//   const int base_data_count = 15;
//   const char base_keys[] = {'H', 'D', 'L', 'B', 'F', 'J', 'N', 'A',
//                             'C', 'E', 'G', 'I', 'K', 'M', 'O'};
//   const int base_values[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 16};

//   const int additional_data_count = 6;
//   const char additional_keys[] = {'S', 'R', 'Q', 'P', 'X', 'Y', 'Z'};
//   const int additional_values[] = {10, 10, 10, 10, 10, 10};

//   const int traversal_data_count = 5;
//   const char traversal_keys[] = {'D', 'B', 'A', 'C', 'E'};
//   const int traversal_values[] = {1, 2, 3, 4, 5};
  
//   bst_node_t *test_tree = NULL;
  
// bst_init(&test_tree);
// bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
// bst_print_tree(test_tree);

// bst_preorder(test_tree);
// printf("\n");
// bst_print_tree(test_tree);



// bst_dispose(&test_tree);

//   return 0;
// }
