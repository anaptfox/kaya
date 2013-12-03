#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"

#define deviceZero 0x00000001

state_t *int_new = (state_t *) INT_NEW;

state_t *int_old = (state_t *) INT_OLD;

int terminalRead = NULL;

int i;

void debugA (int a, int b, int c) {
  int foo = 42;
}
void debugB (int a, int b, int c) {
  int foo = 42;
}

void debugCause (unsigned int p, int b, int c) {
  int foo = 42;
}

/*p is the interrupting Bit*/

int findLine(memaddr p){
	memaddr device = deviceZero;
	int temp;
	temp = p & device;
	i = 8;
	while(temp == 0 && i < 15){
		temp = p & (device << 1);
		i = i + 1;
	}
	return i - 8;	
}

int findDevice(int lineNumber){

	int lineIndex; 

	lineIndex = lineNumber - 3;

	if(lineNumber == TERMINT){
		/* SEARCH READ */
		i = 0;
		while(i < DEVICE_CNT){
			if (deviceSemas[lineIndex][i] < 0){
				terminalRead = 1;
				return i;
			}
			i = i + 1;
		}
		/* SEARCH TRANSMITTION */
		i = 0;
		while(i < DEVICE_CNT){
			if (deviceSemas[lineIndex + 1][i] < 0){
				return i;
			}
			i = i + 1;
		}
	}else{

		while(i < DEVICE_CNT){
			if (deviceSemas[lineIndex][i] < 0){
				return i;
			}
			i = i + 1;
		}
	}

	return NULL;
}

void intHandler(){

	/* set up devregarea*/
	devregarea_t *devregarea;

	devregarea = (devregarea_t *) 0x10000000;

	device_t *deviceWord;


	/*Interrupt Handler

	In order to occur two things have to happen
	
	Ints enable bit needs to beon*/

	int ints_enabled = (int_old->s_status & IEp & IM);

	debugCause(ints_enabled , 10, 10);

	if(1){
		/*he bit for the corresponding line to be on
		which device signaled the interrupt. ( there could be multiple but only handle one of higher proity )( handle int. on lowest number line)
		*/

		/*find out which device number it is given the line number
		in device register area, interrupt device bitmap
		each one is a word 
		go to the appropriate word for that line , then use that word to find
		the lowest ordered bit that's on to find the disk*/

		int cause = int_old->s_cause;

		if(currentProc != NULL){

			moveState(int_old, &(currentProc->p_s));

		}

		int line = findLine(cause);

		if(line == 0 || line == 1){

			/* handle clocks */

		}else{

			int device = findDevice(line);

			if(device == NULL){
				PANIC();
			}

			debugCause(device , 10, 10);


			int deviceWordIndex = ((((line - 3) * 8) - 8) + device );

				
			deviceWord = &(devregarea->devreg[deviceWordIndex]);
			

			if(line == TERMINT){

				if(terminalRead){
					
					/* read the status */
					unsigned int deviceStatus = deviceWord->t_recv_status;
					/* ack the int */
					deviceWord-> t_recv_command = 1;
				
				}else{
					
					/* read the status */
					unsigned int deviceStatus = deviceWord->t_transm_status;
					/* ack the int */
					deviceWord-> t_transm_command = 1;
					
				}

			}else{

				/* read the status */
				unsigned int deviceStatus = deviceWord->d_status;
				/* ack the int */
				deviceWord-> d_command = 1;

			}		
			



			int lineIndex; 

			lineIndex = line;

			if(line == TERMINT){

				if(terminalRead){
					
					lineIndex = lineIndex - 3;
				
				}else{
					
					lineIndex = lineIndex - 2;
					
				}

			}else{

				lineIndex = lineIndex - 3;

			}

			/* Increment sema accociated with device */
			deviceSemas[lineIndex][device] += 1;
					
			pcb_t *p = removeBlocked(deviceSemas[lineIndex][device]);


			if(line == TERMINT){

				if(terminalRead){
							
					p->p_s.s_v0 = deviceStatuses[arg1 - 3][arg2] = deviceStatus;
				
				}else{
					
					p->p_s.s_v0 = deviceStatuses[arg1 - 3 + 1][arg2] = deviceStatus;

				}

			}else{

				p->p_s.s_v0 = deviceStatuses[arg1 - 3][arg2] = deviceStatus;
			
			}

		}

	
		insertProcQ (&readyQue, p);

		softBlkCnt = softBlkCnt - 1;
		
		currentProc = NULL;

		/* Reset terminal read */
		terminalRead = NULL;

		scheduler();
		
			
	}
}


