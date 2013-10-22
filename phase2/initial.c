#include <libumps.e>

// Global var

	semd_t *semd_h[] = semaQue;
	
	pct_t* readyQue;
	
	pcb_t* currentProc = removeProc(&readyque);
	
	int processCnt = 0;

	int deviceSemas[deviceCnts];
	



void int main(int argc, char const *argv[])
{
	
	
	//inilize the four new area
	// Set the PC to the address of your nucleus function that is to handle exceptions of that type
		
	
	initPcbs()
	initASLs()

	//inilize semaphores to 0

	//get a pcb
	pct_t p = allocPcb();

	//populate this pcb


	insertProcQ(&readyQue, p);
	scheduler();

}

