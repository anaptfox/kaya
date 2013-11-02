#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"

#define deviceZero 0x00000001
#define deviceOne 0x00000002
#define deviceTwo 0x00000004
#define deviceThree 0x00000008
#define deviceFour 0x00000010
#define deviceFive 0x00000020
#define deviceSix 0x00000040
#define deviceSeven 0x00000080

state_t *int_old = (state_t *) INT_OLD;

//p is the interrupting Bit
int deviceIterator(memaddr p){
	memaddr device = deviceZero;
	int temp = p & device;
	i = 8;
	while(temp == 0 && i < 15){
		temp = p & (device << 1);
		i++;
	}
	return i - 8;	
}

void inthandler(){

	//Interrupt Handler

	//In order to occur two things have to happen
	
	// Ints enable bit needs to be on

	int ints_enabled = int_old->s_status & IEc & IM;

	//the bit for the corresponding line to be on


	// which device signaled the inter. ( there could be multiple but only handle one of higher proity )( handle int. on lowest number line)
	
	int cause = int_old->s_cause; 

	// Pending int line
	int line = deviceIterator(cause);

						
	// find out which device number it is given the line number
	// in device register area, interrupt device bitmap
			// each one is a word 
			// go to the appropriate word for that line , then use that word to fine the lowest orderd bit that's on to find the disk
	device_t *device;

	int device_num;

	// get the device register
	if(line == 3) {//line 3 word 0
		device = (device_t *) 0x1000003C;
		device_num = 3;
	}
	if(line == 4) { //line 4 word 1
		device = (device_t *) 0x10000040;
		device_num = 4;
	}
	if(line == 5) { //line 5 word 2
		device = (device_t *) 0x10000044;
		device_num = 5;
	}
	if(line == 6) { //line 6 word 3
		device = (device_t *) 0x10000048;
		device_num = 6;
	}
	if(line == 7) { // line 7 word 4
		device = (device_t *) 0x1000004C;
		device_num = 7;
	}
				
	
	// read the status
	memaddr device_status = device->d_status;
	// ack the int
	device-> d_command = 1;
			
	pcb_t *p = removeBlocked(deviceSemas[device_num][line]->s_semadd);

	p->p_s->s_v0 = device_status;
	
	insertProcQ (&readyQueue, p);
	
	currentProc = NULL;

	scheduler();
	
			
	}
}


