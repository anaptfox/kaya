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

int i;

void debugA (int a, int b, int c) {
  int foo = 42;
}
void debugB (int a, int b, int c) {
  int foo = 42;
}
void debugC (int a, int b, int c) {
  int foo = 42;
}

void debugCause (unsigned int p, int b, int c) {
  int foo = 42;
}

/*p is the interrupting Bit*/

int deviceIterator(memaddr p){
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

int findLine(int deviceNumber){
	i = 0;
	while(i < DEVICE_LINE){
		if (deviceSemas[deviceNumber][i] < 0){
			return i;
		}
	}

	return NULL;
}

void intHandler(){

	int cause = int_old->s_cause;

	if(currentProc != NULL){

		moveState(int_old, &(currentProc->p_s));

	}

	int device = deviceIterator(cause);

	int line = findLine(device);

	debugCause(line , 10, 10);

	/*Interrupt Handler

	In order to occur two things have to happen
	
	Ints enable bit needs to beon*/

	int ints_enabled = (int_old->s_status & IEp & IM);

	debugCause(ints_enabled , 10, 10);

	if(1){
		/*he bit for the corresponding line to be on
		which device signaled the interrupt. ( there could be multiple but only handle one of higher proity )( handle int. on lowest number line)
		*/
		int cause = int_old->s_cause;
		debugCause(cause , 10, 10);

		/*Pending int line*/
		int line = deviceIterator(cause);

							
		/*find out which device number it is given the line number
		in device register area, interrupt device bitmap
		each one is a word 
		go to the appropriate word for that line , then use that word to find
		the lowest ordered bit that's on to find the disk*/
		device_t *device;

		int device_num;

		/*get the device register*/
		if(line == 3) {/*line 3 word 0*/
			device = (device_t *) 0x1000003C;
			device_num = 3;
		}
		if(line == 4) { /*line 4 word 1*/
			device = (device_t *) 0x10000040;
			device_num = 4;
		}
		if(line == 5) { /*line 5 word 2*/
			device = (device_t *) 0x10000044;
			device_num = 5;
		}
		if(line == 6) { /*line 6 word 3*/
			device = (device_t *) 0x10000048;
			device_num = 6;
		}
		if(line == 7) { /* line 7 word 4 */
			device = (device_t *) 0x1000004C;
			device_num = 7;
		}
					
		
		/* read the status */
		memaddr device_status = device->d_status;
		/* ack the int */
		device-> d_command = 1;
				
		pcb_t *p = removeBlocked(deviceSemas[device_num][line]);

		p->p_s.s_v0 = device_status;
		
		insertProcQ (&readyQue, p);
		
		currentProc = NULL;

		scheduler();
		
			
	}
}


