#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

static semd_t *semd_h;
static semd_t *semdFree_h;

void debugA(int i){
	return i;
}
void debugB(int i){
	return i;
}
void debugC(int i){
	return i;
}
void debugD(int i){
	return i;
}

void debugE(int i){
	return i;
}



semd_t *create(semd_t **list, int *semAdd){
	int stop = 0;
	semd_t *index = (*list);

	semd_t *newSema;
	newSema->s_next = NULL;
	newSema->s_semAdd = semAdd;
	newSema->s_procQ = mkEmptyProcQ();

	/*Check head first */
	if(index->s_semAdd > semAdd){
		(*list) = newSema;
		newSema->s_next = index;
		stop = 1;
	}
	/* Loop through everything but head.*/
	while(!stop){
		/* if semAdd is greater than the current semAdd*/
		if(index->s_next->s_semAdd > semAdd){
			newSema->s_next = index->s_next;
			index->s_next = newSema;
			stop = 1;
		/* if it is the last in the list*/
		}else if(index->s_next == NULL){
			index->s_next = newSema;
			stop = 1;
		/* Reset the index to next. */
		}else{
			index = index->s_next;
		}
	}

	return newSema;
}

semd_t *addToASL(semd_t *newSema, int *semAdd){
	int stop = 0;
	semd_t *index = (semd_h);
	newSema->s_semAdd = semAdd;

	debugC(1);
	if(semd_h == NULL){
		semd_h = newSema;
		return newSema;

	}
	debugC(1);
	/*Check head first */
	if(index->s_semAdd > semAdd){
		(semd_h) = newSema;
		newSema->s_next = index;
		stop = 1;
	}
	debugC(1);
	/*Check head first */
	if(index->s_next == NULL){
		index->s_next = newSema;
		stop = 1;
	}
	debugC(1);
	/* Loop through everything but head.*/
	while(!stop){
		/* if semAdd is greater than the current semAdd*/
		if(index->s_next->s_semAdd > semAdd){
			newSema->s_next = index->s_next;
			index->s_next = newSema;
			stop = 1;
			debugA(1);
		/* if it is the last in the list*/
		}else if(index->s_next == NULL){
			index->s_next = newSema;
			stop = 1;
			debugB(1);
		/* Reset the index to next. */
		}else{
			debugC(1);
			index = index->s_next;
		}
	}

	return newSema;
}

/*Looks through list for semAdd if not found allocNewASL*/
semd_t *find(semd_t **list, int *semAdd){
	if((*list) == NULL){
		return(NULL);
	}
	debugD(1);
	if((*list)->s_semAdd == semAdd){
		debugA(1);
		return((*list));
	}else{
		if((*list)->s_next == NULL){
			return(NULL);
		}
		debugB(1);
		semd_t *index = (*list)->s_next;
		if(index->s_semAdd == semAdd){
				return(index);
		}
		debugB(1);
		while(index->s_next != NULL){
			if(index->s_next->s_semAdd == semAdd){
				return(index->s_next);
			}
			else{
				index = index->s_next;
			}
		}
		if(index->s_next == NULL){
			if(index->s_semAdd == semAdd){
				return(index);
			}
			else{
				return(NULL);
			}
		}else{
			return(NULL);
		}
	}
}


/*Looks through list for semAdd if not found allocNewASL*/

semd_t *remove(semd_t **list, int *semAdd){
	if((*list) == NULL){
		return(NULL);
	}
	if((*list)->s_semAdd == semAdd){
		semd_t *deletedNode = (*list);
		if((*list)->s_next == NULL){
			(*list) = NULL;
		}else{
			(*list) = (*list)->s_next;
		}
		deletedNode->s_next= NULL;
		return deletedNode;
	}else{

		semd_t *index = (*list);
		semd_t *deletedNode;

		if(index->s_next == NULL){		
			return(NULL);
		}

		if(index->s_next->s_semAdd == semAdd){
			if (index->s_next->s_next != NULL){
				deletedNode = index->s_next;
				index->s_next = index->s_next->s_next;
				deletedNode->s_next = NULL;
				return deletedNode;
			}else{
				deletedNode = index->s_next;
				deletedNode->s_next = NULL;
				index->s_next = NULL;
				return deletedNode;
			}
		}
		while(index->s_next != NULL){
			if(index->s_next->s_semAdd == semAdd){

				if (index->s_next->s_next != NULL){
					deletedNode = index->s_next;
					deletedNode->s_next = NULL;
					index->s_next = index->s_next->s_next;
					return deletedNode;
				}else{
					deletedNode = index->s_next;
					deletedNode->s_next = NULL;
					index->s_next = NULL;
					return deletedNode;
				}
			}
			else{
				index = index->s_next;
			}
		}
		if(index->s_next == NULL){
			if(index->s_semAdd == semAdd){
				deletedNode = index;
				index = NULL;
				return deletedNode;
			}
		}else{
			return(NULL);
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

/* Insert the ProcBlk pointed to by p at the tail of the process queue
 associated with the semaphore whose physical address is semAdd and 
 set the semaphore address of p to semAdd. If the semaphore is currently not 
 active (i.e. there is no descriptor for it in the ASL), allo- cate a new descriptor
  from the semdFree list, insert it in the ASL (atthe appropriate position), initialize 
  all of the fields (i.e. set s semAdd to semAdd, and s procq to mkEmptyProcQ()), and proceed as above. If a new
   semaphore descriptor needs to be allocated and the semdFree list is empty, 
   return TRUE. In all other cases return FALSE. */

int insertBlocked(int *semAdd, pcb_t *p){
	semd_t *sema = find(&semd_h, semAdd);
	if(sema == NULL){
		/*remove from free (*list)*/
		sema = removeFree();
		if(sema == NULL ){
			return 1;
		}
		/* add to active list*/
		sema = addToASL(sema, semAdd);
	}
 	insertProcQ(&(sema->s_procQ), p);
 	return 0;
}

pcb_t *removeBlocked(int *semAdd){
	semd_t *sema = find(&semd_h, semAdd);
	if(sema == NULL){
		return(NULL);
	}else{
		removeProcQ(&(sema->s_procQ));
		if(emptyProcQ(sema->s_procQ)){
			sema = remove(&semd_h, semAdd);
			sema = create(&semdFree_h, semAdd );
		}
		return(sema);
	}
}

pcb_t *outBlocked(pcb_t *p){
	semd_t *sema = find(&semd_h ,p->p_semAdd);
	if(sema == NULL){
		return(NULL);
	}else{
		if(emptyProcQ(sema->s_procQ)){
			return(NULL);
		}
	}
	return outProcQ(&(sema->s_procQ), p);
}


pcb_t *headBlocked(int *semAdd){
	semd_t *sema = find(&semd_h, semAdd);
	if(sema == NULL){
		return(NULL);
	}

	return(headProcQ(sema->s_procQ));
}



void initASL(){
	static semd_t semdTable[MAXPROC];
	int i = 0;
	while(  i < (MAXPROC-1)){
		semdTable[i].s_next = &semdTable[i+1];	
		i++; 
	}
	semdTable[(MAXPROC-1)].s_next = NULL;
	semdFree_h = &semdTable[0];
	semd_h = NULL;
}