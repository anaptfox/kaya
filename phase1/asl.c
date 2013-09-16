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

/*Looks through list for semAdd if not found allocNewASL*/
semd_t *find(semd_t **list, int *semAdd){
	if((*list)->s_semAdd == semAdd){
		return((*list));
	}else{
		semd_t *index = (*list)->s_next;
		if(index->s_semAdd == semAdd){
				return(index);
		}else if(index->s_next == NULL){
			if(index->s_semAdd == semAdd){
				return(index);
			}
			else{
				return(NULL);
			}
		}else{
			return(NULL);
		}
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
		(*list) = (*list)->s_next;
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

		if (index->s_next->s_next == NULL){
			return(NULL);
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

int insertBlocked(int *semAdd, pcb_t *p){
	semd_t *sema = find(&semd_h, semAdd);
	if(sema == NULL){
		/*remove from free (*list)*/
		sema = remove(&semdFree_h, semAdd);
		if(sema == NULL){
			return 1;
		}
		/* add to active list*/
		sema = create(&semd_h, semAdd);
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
	int i = MAXPROC;
	while(  i < MAXPROC){
		create(&semdFree_h, &semdTable[i]);
		i++; 
	}
}