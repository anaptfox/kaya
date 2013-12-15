#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

static semd_t *semd_h;
static semd_t *semdFree_h;

void debugZ (int a, int b, int c) {
  int foo = 42;
}

void debugASL (int a, int b, int c) {
  int foo = 42;
}

void debugHEAD (int a, int b, int c) {
  int foo = 42;
}
void debugSEMADD (int a, int b, int c) {
  int foo = 42;
}

void debugS (int a, int b, int c) {
  int foo = 42;
}

void debugI (int a, int b, int c) {
  int foo = 42;
}

void debugPANIC (int a, int b, int c) {
  int foo = 42;
}

/* Add items to active semaphore list */
semd_t *addToASL(semd_t *newSema, int *semAdd){
	
	int stop = FALSE;
	
	semd_t *index = semd_h;

	if(newSema == NULL){

		debugPANIC(NULL, semd_h->s_semAdd, semd_h);

		PANIC();
	}
	
	newSema->s_semAdd = semAdd;
	
	newSema->s_next = NULL;

	debugZ(semd_h->s_next, semd_h->s_semAdd, semd_h);


	if(semd_h->s_next == NULL){

		semd_h->s_next = newSema;

		newSema->s_next = NULL;

		debugHEAD(newSema,newSema->s_next,1);

		debugZ(semd_h->s_next, semd_h->s_semAdd, semd_h);


		return newSema;

	}


	/* Loop through everything but head.*/
	while(!stop){


		if(index->s_next == NULL){

			if(index->s_next == semd_h->s_next){

				debugPANIC(NULL, semd_h->s_semAdd, semd_h);
				PANIC();
			}

			index->s_next = newSema;

			newSema->s_next = NULL;

			debugHEAD(newSema,newSema->s_next,3);
			

			stop = TRUE;

		/* Reset the index to next. */
		}else if(index->s_next->s_semAdd > semAdd){

			if(index->s_next == semd_h->s_next){
				debugSEMADD(index->s_next->s_semAdd,semAdd,4);
			}

			newSema->s_next = index->s_next;
	
			index->s_next = newSema;

			debugHEAD(newSema,newSema->s_next,4);

		
	
			stop = TRUE;
	
		/* if it is the last in the list*/
		}else{
	
			index = index->s_next;
	
		}
	
	}

debugZ(semd_h->s_next, semd_h->s_semAdd, semd_h);

	return newSema;
}

/*Looks through list for semAdd if not found allocNewASL*/
semd_t *findActive(int *semAdd){

	debugS(semd_h->s_next, semd_h->s_semAdd, semd_h);

	semd_t *index = semd_h;

	if(semd_h->s_next == NULL){

		debugS(semd_h->s_next, semd_h->s_semAdd, semd_h);

		return(NULL);

	}

	while(index->s_next != NULL){

		debugASL(index->s_next,index->s_next->s_next,semAdd);
		
		if(index->s_next->s_semAdd == semAdd){

			debugS(semd_h->s_next, semd_h->s_semAdd, semd_h);
	
			return(index->s_next);
	
		}
	
		else{
	
			index = index->s_next;
	
		}
		
	}

	debugS(semd_h->s_next, semd_h->s_semAdd, semd_h);

	return(NULL);
}


/*Looks through list for semAdd if not found allocNewASL*/

semd_t *removeActive(int *semAdd){
	
	semd_t *index = semd_h;
	
	semd_t *deletedNode;
	
	
	/*Case 1: semAdd is in element after head*/
	
	if(semd_h->s_next->s_semAdd == semAdd){
	
		deletedNode = semd_h->s_next;
	
		if(semd_h->s_next->s_next == NULL){
	
			semd_h->s_next = NULL;
	
		}else{
	
			semd_h->s_next = semd_h->s_next->s_next;
	
		}
	
		return deletedNode;
	
	}
	/*Iterate until we aren't on the last element*/
	
	while(index->s_next != NULL){
	
		/*Iterate through the list; if we find the element,
		remove it, else go to the next element*/
	
			if(index->s_next->s_semAdd == semAdd){
	
				if (index->s_next->s_next != NULL){
	
					deletedNode = index->s_next;
	
					index->s_next = index->s_next->s_next;
	
					deletedNode->s_next = NULL;
	
					return deletedNode;
	
				}else{
	
					deletedNode = index->s_next;
	
					index->s_next = NULL;
	
					deletedNode->s_next = NULL;
	
					return deletedNode;
	
				}
	
			}
	
			else{
	
				index = index->s_next;
	
			}
	
		}
	
	}


/* Return TRUE if the queue whose tail is pointed to by tp is empty.
Return FALSE otherwise. */
int emptyList(semd_t *list){

	return (list == NULL); 

}

/*Removes the top of the Free list*/

semd_t *removeFree(){
	
	if(emptyList(semdFree_h)){
	
		return(NULL);
	
	}else{
	
		semd_t *old = (semdFree_h);
	
		if((semdFree_h)->s_next == NULL ){
	
			(semdFree_h) = NULL;
	
		}else{
	
			(semdFree_h) = (semdFree_h)->s_next;
	
		}
	
		old->s_next = NULL;
	
		old->s_procQ = mkEmptyProcQ();
	
		return(old);
	
	}

}

/*Add the top of the Free list*/

void addFree(semd_t *newSema){

	if(emptyList(semdFree_h)){

		semdFree_h = newSema;

	}else{

		newSema->s_next = semdFree_h;

		semdFree_h = newSema;

	}

}


/* Insert the ProcBlk pointed to by p at the tail of the process queue
 associated with the semaphore whose physical address is semAdd and 
 set the semaphore address of p to semAdd. If the semaphore is currently not 
 active (i.e. there is no descriptor for it in the ASL), allo- cate a new descriptor
  from the semdFree list, insert it in the ASL (atthe appropriate position), initialize 
  all of the fields (i.e. set s semAdd to semAdd, and s procq to mkEmptyProcQ()), and proceed as above. If a new
   semaphore descriptor needs to be allocated and the semdFree list is empty, 
   return TRUE. In all other cases return FALSE. */

int insertBlocked(int *semAdd, pcb_t *p){

	debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

	semd_t *sema = findActive(semAdd);

	debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

	if(sema == NULL){

		/*remove from free *semd_h*/

		sema = removeFree();

		debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

		if(sema == NULL ){

			return TRUE;

		}

		/* add to active list*/

		debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

		sema = addToASL(sema, semAdd);

	}

	debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

 	p->p_semAdd = semAdd;

 	debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

 	insertProcQ(&(sema->s_procQ), p);

 	debugI(semd_h->s_next, semd_h->s_semAdd, semd_h);

 	return FALSE;

}

/* Search the ASL for a descriptor of this semaphore. If none is
found, return NULL; otherwise, remove the ﬁrst (i.e. head) ProcBlk
from the process queue of the found semaphore descriptor and return a pointer to it. If the process queue for this semaphore becomes
empty (emptyProcQ(s procq) is TRUE), remove the semaphore
descriptor from the ASL and return it to the semdFree list. */

pcb_t *removeBlocked(int *semAdd){

	semd_t *sema = findActive(semAdd);

	if(sema == NULL){

		return(NULL);

	}else{

		pcb_t *proc = removeProcQ(&(sema->s_procQ));

		if(proc != NULL){

			proc->p_semAdd = NULL;

		}

		if(emptyProcQ(sema->s_procQ)){

			sema = removeActive(semAdd);

			addFree(sema);

		}

		return(proc);

	}

}

/* Remove the ProcBlk pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASL. If ProcBlk
pointed to by p does not appear in the process queue associated with
p’s semaphore, which is an error condition, return NULL; otherwise,
return p. */

pcb_t *outBlocked(pcb_t *p){

	
	semd_t *sema = findActive(p->p_semAdd);
	
	if(sema == NULL){
	
		return(NULL);
	
	}else{
	
		pcb_t *proc = outProcQ(&(sema->s_procQ), p);
	
		if(proc != NULL){
	
			proc->p_semAdd = NULL;
	
		}
	
		if(emptyProcQ(sema->s_procQ)){
	
			sema = removeActive(p->p_semAdd);
	
			addFree(sema);
	
		}
	
		return(proc);
	
	}


}

/* Return a pointer to the ProcBlk that is at the head of the process queue associated with the semaphore semAdd. Return NULL
if semAdd is not found on the ASL or if the process queue associated with semAdd is empty. */

pcb_t *headBlocked(int *semAdd){
	
	semd_t *sema = findActive(semAdd);
	
	if(sema == NULL){
	
		return(NULL);
	
	}
	
	return(headProcQ(sema->s_procQ));

}


/*Initialize the ASL*/
void initASL(){

	static semd_t semdTable[MAXPROC];

	semd_t *dummy;

	int i = 0;

	while ( i < MAXPROC){

		semdTable[i].s_next = &semdTable[i+1];	

		i++; 

	}

	semdTable[MAXPROC].s_next = NULL;

	semdFree_h = &semdTable[0];

	dummy->s_next = NULL;

	dummy->s_semAdd = 0;

	dummy->s_procQ = mkEmptyProcQ();

	semd_h = dummy;



}