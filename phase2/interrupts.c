#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"

#define bitZero 0x00000001

state_t *int_new = (state_t *) INT_NEW;

state_t *int_old = (state_t *) INT_OLD;

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

void debugTimer( int p, int b, int c) {
  int foo = 42;
}

/*p is the interrupting Bit*/

int findLine(memaddr p){
	memaddr device = bitZero;
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
	memaddr device = bitZero;
	memaddr p;

	if(lineNumber == 3) {/*lineNumber 3 word 0*/
	 p = 0x1000003C;
	}
	if(lineNumber == 4) { /*lineNumber 4 word 1*/
	 p = 0x10000040;
	}
	if(lineNumber == 5) { /*lineNumber 5 word 2*/
	 p = 0x10000044;
	}
	if(lineNumber == 6) { /*lineNumber 6 word 3*/
	 p = 0x10000048;
	}
	if(lineNumber == 7) { /* lineNumber 7 word 4 */
	 p = 0x1000004C;
	}

	if((p & DEVICE_ADDRESS_0) == 0) return 0;
	else if((p & DEVICE_ADDRESS_1) == 0) return 1;
	else if((p & DEVICE_ADDRESS_2) == 0) return 2;
	else if((p & DEVICE_ADDRESS_3) == 0) return 3;
	else if((p & DEVICE_ADDRESS_4) == 0) return 4;
	else if((p & DEVICE_ADDRESS_5) == 0) return 5;
	else if((p & DEVICE_ADDRESS_6) == 0) return 6;
	else if((p & DEVICE_ADDRESS_7) == 0) return 7;

	return NULL;
}

void intHandler(){

	/* set up devregarea*/
	devregarea_t *devregarea;

	devregarea = (devregarea_t *) 0x10000000;

	device_t *deviceWord;

	pcb_t *p;

	int terminalRead = 0;

	/*Interrupt Handler

	In order to occur two things have to happen
	
	Ints enable bit needs to beon*/
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
		debugCause(line , 10, 10);

		if(line == 1 || line == 2){

			debugTimer(6666, 10, 10);

			if(line == 1){

				debugTimer(5555, 10, 10);

				moveState( int_old , &(currentProc->p_s) );

				insertProcQ( & readyQue ,  currentProc );

				setTIMER(5000);

				softBlkCnt = softBlkCnt + 1;

				currentProc = NULL;
			}
			

		}else{

			int device = findDevice(line);
			debugCause(device , 10, 10);

			if(device == NULL){
				PANIC();
			}

			int deviceWordIndex = (((line - 3) * 8) + device);


			deviceWord = &(devregarea->devreg[deviceWordIndex]);
			
			unsigned int deviceStatus;

			if(line == TERMINT){

				deviceStatus = deviceWord->t_transm_status ;

				if((deviceWord->t_recv_status & 0xFF) == 1){

					

					/* ack the int */
					deviceWord->t_transm_command = ACK;


				}else{
					debugTimer(5555, deviceWord->t_recv_status, 10);
					debugTimer(9999, deviceWord->t_transm_status, 10);

					terminalRead = 1;

					/* ack the int */
					deviceWord-> t_recv_command = ACK;

				}
		

			}else{

				/* read the status */
				deviceStatus = deviceWord->d_status;
				/* ack the int */
				deviceWord-> d_command = ACK;

			}		
			



			int lineIndex; 

			lineIndex = line;

			if(line == TERMINT){

				if(terminalRead == 1){
					
					lineIndex = lineIndex - 3;
				
				}else{
					
					lineIndex = lineIndex - 2;
					
				}

			}else{

				lineIndex = lineIndex - 3;

			}

			debugB(lineIndex, device, 123);

			/* Increment sema accociated with device */
			deviceSemas[lineIndex][device] += 1;

					
			p = removeBlocked(&(deviceSemas[lineIndex][device]));

			if(p == NULL){
				debugB(1010101010, device, 123);

				if(line == TERMINT){

					if(terminalRead == 1){
								
						deviceStatuses[line - 3][device] = deviceStatus;
					
					}else{
						
						deviceStatuses[line - 3 + 1][device] = deviceStatus;

					}

				}else{

					deviceStatuses[line - 3][device] = deviceStatus;
				
				}

			}else{
				insertProcQ (&readyQue, p);
				debugB(232323, device, 123);

				softBlkCnt = softBlkCnt - 1;


				if(line == TERMINT){

					if(terminalRead == 1){
								
						p->p_s.s_v0 = deviceStatus;
					
					}else{
						
						p->p_s.s_v0 = deviceStatus;

					}

				}else{

					p->p_s.s_v0 = deviceStatus;
				
				}


			} 

		}

		/* Reset terminal read */
		terminalRead = 0;

		if(currentProc != NULL){

			debugA(10,22,10);

			continueWithCurrent(&(currentProc->p_s));

		}else{

			debugA(100000,33223,10);
			scheduler();
		}
		
			
	}
}


