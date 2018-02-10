/*	    set.c
 *      Andrew Nguyen
 *      Friday Lab
 *      November 04 2017
 *         
 *      This file implements a deque using a circular, doubly linked 
 *      list using a dummy node to assert *prev to not be NULL
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "set.h"
#define alpha 20

struct set{
        int count;          /* number of elements */
        int length;         /* length of array */
        LIST **data;        /* the actual data */
        int(*compare)();    /* function to check if elements match */
        unsigned(*hash)();  /* hash function for generic pointers */
};

typedef struct set SET;

/*	createSet
 *	O(n)
 *	Returns a pointer to a new set with length maxElts/AVG_LIST_LENGTH. 
 */
SET *createSet(int maxElts, int (*compare)(), unsigned(*hash)()){
        SET *sp;
            sp = malloc(sizeof(SET));
            assert(sp != NULL);
                    
            sp->count = 0;
            sp->length = maxElts/alpha;
            sp->compare = (*compare);
            sp->hash = (*hash);
            sp->data = malloc(sizeof(LIST*)*maxElts);
                                        
            for (int i = 0; i < sp->length; i++){
                sp->data[i] = createList(sp->compare);
            }
                                            
            assert(sp->data != NULL);
            return sp;
}

/*	destroySet
 *	Complexity: O(n)
 *	Frees memory pointed to by sp.
 */
void destroySet(SET *sp){
        assert (sp != NULL);
        for (int i = 0; i < sp->length; i++){
            destroyList(sp->data[i]);
        }
        free(sp->data);
        free(sp);
}

/*	numElements
 *	O(1)
 *	Returns the number of elements
 *	in a set or total items in a list.
 */
int numElements(SET *sp){
        assert (sp != NULL);
        return (sp->count);
}

/*	addElement
 * 	O(n)
 *	New element is hashed to the appropriate list.
 *      Uses list ADT functions to check if value exists.
 *      Adds element to the set if not present.
 */
void addElement(SET *sp, void *elt){
        assert(sp != NULL);
        int hashloc = (*sp->hash)(elt) % sp->length;
        if (findItem(sp->data[hashloc], elt) == NULL)
	{
        	addFirst(sp->data[hashloc], elt);
                sp->count++;
        }
}

/*	removeElement
 *	O(n)
 * 	Uses hash function and list ADT functions to find
 *      location of element. Removes element if present.
 */
void removeElement(SET *sp, void *elt){
        assert(sp != NULL);
        int hashloc = (*sp->hash)(elt) % sp->length;
        if(findItem(sp->data[hashloc], elt) != NULL)
        	removeItem(sp->data[hashloc], elt);
                sp->count--;
}

/*	findElement
 * 	O(n)
 * 	Uses hash function and list ADT functions to search
 *      for a specific item. Returns if found, else returns NULL.
 */
void *findElement(SET *sp, void *elt){
        assert(sp != NULL);
        int hashloc = (*sp->hash)(elt) % sp->length;
        return findItem(sp->data[hashloc], elt);
}

/*	getElement
 * 	O(n)
 *      Copies all items to a new array and return pointer
 */
void *getElements(SET *sp){
        assert(sp != NULL);
        void **a;
        void **temp;
        int count = 0;
        a = malloc(sizeof(void*)*sp->count);
        assert(a != NULL);
                                
        for (int i = 0; i < sp->length; i++){
            temp = getItems(sp->data[i]);
                                                      
                for (int j = 0; j < numItems(sp->data[i]); j++){
                    a[count] = temp[j];
                    count++;
                }
            free(temp);
        }
        return a;
}

