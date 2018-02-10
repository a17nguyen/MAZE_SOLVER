/*	list.c
 *	Andrew Nguyen
 *	Friday Lab
 *	November 04 2017
 *
 *      This file implements a deque using a circular, doubly linked 
 *      list using a dummy node to assert *prev to not be NULL
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

/* creates next and previous nodes */
struct node {
	void *data;		/* data */
	struct node *next;	/* pointer to next node */
	struct node *prev;	/* pointer to previous node */
};
typedef struct node NODE;

/* create head pointer and declares compare function */
struct list {
	int count;		/* num elements */
	NODE *head;		/* head pointer */
	int(*compare)();	/* compare function */
};
typedef struct list LIST;

/*	createList
 *	O(1)
 *	initializes pointer and returns list pointer
 */
LIST *createList(int (*compare)()) {
	//initialize and allocate memory
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert (lp != NULL);

	NODE *dummy;
	dummy = malloc(sizeof(NODE));
	assert(dummy!=NULL);

	//initialize pointers
	lp->count = 0;
	lp->head = dummy;
	lp->compare = (*compare);

	dummy->data = NULL;
	dummy->next = dummy;
	dummy->prev = dummy;
	return lp;
}

/*	destroyList
 *	O(n)
 *	frees memory pointed to by lp
 */
void destroyList(LIST *lp){
	assert(lp != NULL);	
	NODE *np;	
	NODE *dummy;
	
	//implements sentinel node as head pointer 
	dummy = lp->head;
	np = dummy->next;

	//free linked lists and then the head pointer
	while (dummy->next != lp->head){
		np = dummy->next;
		free(dummy);
		dummy = np;
	}
	free(dummy);
	free(lp);
}

/*	numItems
 *	O(1)
 *     	Returns the number of items in the list
 */
int numItems(LIST *lp){
	assert(lp!=NULL);
	return (lp->count);
}

/*	addFirst
 *	O(1)
 *	adds item to the front of list, using a 
 *	dummy node for *pPrev to not be NULL
 */
void addFirst(LIST *lp, void *item){
	NODE *dummy;
	dummy = lp->head;

	//allocate memory for node pointer
	NODE *np;
	np = malloc(sizeof(NODE));

	assert(lp!=NULL && item !=NULL && np!=NULL);

	//adds item to the front of list
	np->data = item;
	np->next = dummy->next;
	np->prev = dummy;
	dummy->next->prev = np;
	dummy->next = np;

	//increase count
	lp->count++;
}

/*	addLast
 *      O(1)
 *      Adds item to the end of the list, using the 
 *      sentinel node's for *pPrev to not be NULL
 */
void addLast(LIST *lp, void *item){
	NODE *dummy;
	dummy = lp->head;
	
	//allocates memory for node pointer
	NODE *np;
	np = malloc(sizeof(NODE));

	assert(lp!=NULL && item!=NULL && np!= NULL);

	//adds item to end of list
	np->data = item;
	np->prev = dummy->prev;
	dummy->prev = np;
	np->next = dummy;
	np->prev->next = np;

	//increase count
	lp->count++;
}

/*	removeFirst
 *    	O(1)
 *      Removes the first item in the list.
 */
void *removeFirst(LIST *lp){
	assert(lp!=NULL);
	void *val;
	
	//creates dummy node for head pointer
	NODE *dummy;
	dummy = lp->head;
	
	//creates temporary pointer for deleted element
	NODE *del;

	//remove item from front
	del = dummy->next;
	val = del->data;
	dummy->next = del->next;
	del->next->prev = dummy;
	free(del);

	//decrease count
	lp->count--;
	return val;
}

/*	removeLast
 *      O(1)
 *      Removes the last item in the list, using 
 *      the sentinel node's previous pointer.
 */
void *removeLast(LIST *lp){
	assert(lp!=NULL);
	void *val;

	//creates dummy pointer as head
	NODE *dummy;
	dummy = lp->head;

	//creates temporary pointer for deleted item
	NODE *del;
	del = dummy->prev;

	//delete item from end of list
	val = del->data;
	dummy->prev = del->prev;
	del->prev->next = dummy;
	free(del);

	//decrease count
	lp->count--;
	return val;
}

/*	getFirst
 *      O(1)
 *      Returns the first item of the list.
 */
void *getFirst(LIST *lp){
	assert(lp!=NULL && lp->count!=0);
	void *item;

	NODE *dummy;
	dummy = lp->head;
	
	item = dummy->data;
	return(item);
}

/*	getLast
 *      O(1)
 *      Returns the last item of the list.
 */
void *getLast(LIST *lp){
	assert(lp!=NULL && lp->count!=0);
	void *item;
	
	NODE *dummy;
	dummy = lp->head;

	item = dummy->prev->data;
	return(item);
}

/*	removeItem
 *      O(n)
 *      Removes an item of the list.
 */
void removeItem(LIST *lp, void *item){
	NODE *pCur;
	NODE *dummy;

	dummy = lp->head;
	pCur = dummy->next;

	while (pCur != dummy){
		if((*lp->compare)(item, pCur->data)==0){
			pCur->prev->next = pCur->next;
			pCur->next->prev = pCur->prev;
			free(pCur);
			lp->count--;
			return;
		}
		pCur = pCur->next;
	}
}

/*	findItem
 *      O(n)
 *      Searches for an item in the list. 
 *      Returns if found, else returns NULL.
 */
void *findItem(LIST *lp, void *item){
	NODE *pCur;
	NODE *dummy;
	
	dummy = lp->head;
	pCur = dummy->next;
	
	while (pCur != dummy){
		if((*lp->compare)(item, pCur->data)==0){
			return pCur->data;
		}
		pCur = pCur->next;
	}
	return NULL;
}

/*	getItems
 *      O(n)
 *      Returns an array with all the items in the list.
 */
void *getItems(LIST *lp){
	assert(lp != NULL);
	
	//initializes new pointer
	void **newloc;
	int i = 0;
	NODE *p;
	NODE *dummy;

	//allocate memory for new pointer
	dummy = lp->head;
	p = dummy->next;
	newloc = malloc(sizeof(void*)*lp->count);
	assert (newloc != NULL);

	//copies over all items to new memory location
	for (i=0; i < lp->count; i++){
		newloc[i] = p->data;
		p = p->next;
	}
	return newloc;
}

