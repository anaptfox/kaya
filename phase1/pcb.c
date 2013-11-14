#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

static pcb_t *freePcb_tp;

void debugA (int a, int b, int c) {
  int foo = 42;
}
void debugB (int a, int b, int c) {
  int foo = 42;
}
void debugC (int a, int b, int c) {
  int foo = 42;
}
void debugD (int a, int b, int c) {
  int foo = 42;
}

/* This method is used to initialize a variable to be tail pointer to a
process queue.
Return a pointer to the tail of an empty process queue; i.e. NULL. */
pcb_t *mkEmptyProcQ() {
	return(NULL);
}

/* Return TRUE if the queue whose tail is pointed to by tp is empty.
Return FALSE otherwise. */
int emptyProcQ (pcb_t *tp){
	return (tp == NULL); 
}

/* Insert the ProcBlk pointed to by p into the process queue whose
tail-pointer is pointed to by tp. Note the double indirection through
tp to allow for the possible updating of the tail pointer as well. */
void insertProcQ(pcb_t **tp, pcb_t *p){
	/*Case 1: ProcQ is empty*/
	if(emptyProcQ((*tp))){
		(*tp) = p;
		p->p_next = p;
		p->p_prev = p;
	}/*Case 2: ProcQ has only one Procblk*/
	else if((*tp)->p_next == (*tp)){
		p->p_next = (*tp);
		p->p_prev = (*tp);
		(*tp)->p_prev = p;
		(*tp)->p_next = p;
		(*tp) = p;
	}/*Case 3: ProcQ has more than one ProcBlk*/
	else{
		debugD (10, 10, 10);
		p->p_next = (*tp)->p_next;
		debugA (10, 10, 10);
		p->p_next->p_prev = p;
		debugB (10, 10, 10);
		p->p_prev = (*tp);
		debugC (10, 10, 10);
		(*tp)->p_next = p;
		debugD (10, 10, 10);
		(*tp) = p;
		debugA (10, 10, 10);
	}
	debugA (10, 10, 10);
}

/* Return a pointer to the ﬁrst ProcBlk from the process queue whose
tail is pointed to by tp. Do not remove this ProcBlkfrom the process
queue. Return NULL if the process queue is empty. */
pcb_t *headProcQ(pcb_t *tp){
	pcb_t *head = NULL;
	if((tp) != NULL) {
		head = (tp)->p_next;
	}
	return(head);
}

/* removeProcQ takes the pointer to the tail pointer and removes the 
ProcBlk associated with it, then reassigns pointers around the removed ProcBlk*/
pcb_t *removeProcQ(pcb_t **tp){
	/* Case 1: ProcQ is empty*/
	if(emptyProcQ(*tp)){
		return(NULL);
		
	}/* Case 2: Only one ProcBlk in ProcQ*/
	else if((*tp)->p_next == *tp){
		pcb_t *old = (*tp);
		*tp = mkEmptyProcQ();
		return(old);
	}/* Case 3: More than 1 ProcBlk in ProcQ*/
	else{
		pcb_t *old = (*tp)->p_next;
		(*tp)->p_next->p_next->p_prev = *tp;
		(*tp)->p_next = (*tp)->p_next->p_next;
		return(old);
	}
}

/* Remove the ProcBlk pointed to by p from the process queue whose
tail-pointer is pointed to by tp. Update the process queue’s tail
pointer if necessary. If the desired entry is not in the indicated queue
(an error condition), return NULL; otherwise, return p. Note that p
can point to any element of the process queue. */
pcb_t *outProcQ(pcb_t **tp, pcb_t *p){
	/* Case 1: ProcQ is empty.*/
	if(emptyProcQ(*tp)){
		return(NULL);
	} /* Case 2: ProcQ has only 1 ProcBlk*/
	else if((*tp)->p_prev == *tp){
		if(*tp == p){
			pcb_t *outproc = *tp;
			*tp = mkEmptyProcQ();
			return(outproc);
		}else{
			return(NULL);
		}
	}/* Case 3: ProcQ has more than one ProcBlk*/
	else{/*Subcase 1: Removing the ProcBlk pointed to by the tail pointer*/
		if(*tp == p){
			pcb_t *outproc = *tp;
			(*tp)->p_prev->p_next = (*tp)->p_next;
			(*tp)->p_next->p_prev = (*tp)->p_prev;
			*tp = (*tp)->p_prev;
			return(outproc);
		}else{
			pcb_t *index = (*tp)->p_next;
			while(index != *tp){/*Subcase 2: Removing the head*/
				if(p == index && index == (*tp)->p_next){
					return removeProcQ(tp);
				}/*Subcas3: Removing a middle ProcBlk*/
				else if(p == index){
					index->p_prev->p_next = index->p_next;
					index->p_next->p_prev = index->p_prev;
					return(index);
				}else{
					index = index->p_next;
				}
			}
			return(NULL);
		}
	}
	
}

/* Insert the element pointed to by p onto the pcbFree list. */
void freePcb(pcb_t *p){
	insertProcQ(&freePcb_tp, p);
}
/*Initializes the pcbs*/
void initPcbs(){
	static pcb_t pcbs[MAXPROC];
	int i = 0;
	freePcb_tp = mkEmptyProcQ();
	while( i < MAXPROC){
		freePcb(&pcbs[i]);
		i++; 
	}
}


/* Return NULL if the pcbFree list is empty. Otherwise, remove
an element from the pcbFree list, provide initial values for ALL
of the ProcBlk’s ﬁelds (i.e. NULL and/or 0) and then return a
pointer to the removed element. ProcBlk’s get reused, so it is
important that no previous value persist in a ProcBlk when it
gets reallocated. */
pcb_t *allocPcb(){
	pcb_t *temp;
	temp = removeProcQ(&freePcb_tp);
	if(temp != NULL){
		temp->p_next = NULL;
		temp->p_prev = NULL;
		temp->p_prnt = NULL;
		temp->p_child = NULL;
		temp->p_sib = NULL;
		temp->p_prev_sib = NULL;
	}
	return(temp);
}
/*Set the child parent p points to to null*/
int emptyChild (pcb_t *p){
	return (p->p_child == NULL); 
}
/* Adds a child to the parent node p*/
void insertChild (pcb_t *prnt, pcb_t *p){
	/* Case 1: parent p has no children*/
	if(emptyChild(prnt)){
		prnt->p_child = p;
		p->p_prnt = prnt;
	}/*Case 2: parent p has other children*/
	else{
		prnt->p_child->p_prev_sib = p;
		p->p_prev_sib = NULL;
		p->p_sib = prnt->p_child;
		prnt->p_child = p;
		p->p_prnt = prnt;
	}
}
/*Removes the child pointed to by parent node p*/
pcb_t *removeChild (pcb_t *p){
	/*Case 1: Parent p has no children*/
	if(emptyChild(p)){
		return(NULL);
	}/*Case 2: Parent p has only one child*/
	else{
		pcb_t *removedChild = p->p_child;
		if(p->p_child->p_sib == NULL){
			p->p_child = NULL;
		}/*Case 3: Parent p has more than one child*/
		else{
			p->p_child->p_sib->p_prev_sib = NULL;
			p->p_child =  p->p_child->p_sib;
		}
		return(removedChild);
	}
}
/*outChild removes the child pointed to by p*/
pcb_t *outChild(pcb_t *p){
	/*Case 1: Child p has no parent*/
	if(p->p_prnt == NULL){
		return(NULL);
	}/*Case 2: CChild p has a parent, but no siblings*/
	else{
		if((p->p_sib == NULL) && (p->p_prev_sib == NULL)){
			p->p_prnt->p_child = NULL;
			p->p_prnt = NULL;
			return(p);
		}
		/* Case 3: Child p has both a parent and sibling(s)*/
	else{
		/* Subcase 1: Child p is the first child*/
			if(p->p_prev_sib == NULL){
				p->p_prnt->p_child = p->p_sib;
				p->p_sib->p_prev_sib = NULL;
				p->p_prnt = NULL;
				p->p_sib = NULL;
				return(p);
			}/*Subcase 2: Child p is the last child*/
			else if(p->p_sib == NULL){
				p->p_prev_sib->p_sib = NULL;
				p->p_prnt = NULL;
				p->p_prev_sib = NULL;
				return(p);
			}/*Subcase 3: Child p is neither the first nor last child*/
			else{
				p->p_prev_sib->p_sib = p->p_sib;
				p->p_sib->p_prev_sib = p->p_prev_sib;
				p->p_prnt = NULL;
				p->p_sib = NULL;
				p->p_prev_sib = NULL;
				return(p);
			}
		}
	
	}
}
