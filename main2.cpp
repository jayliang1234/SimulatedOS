/*
	If you find any mistakes in this file
	please let me know, so I can fix them and reupload
	fixed version for everyone.
*/
#include "SimulatedOS.h"

int main()
{
	SimulatedOS osSim{ 1, 300, 100 };

	osSim.NewProcess(10);
	osSim.NewProcess(5);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0        0       1
	// 1		0		2
	// 2		
	osSim.FetchFrom(120);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		0		1
	// 1		0		2
	// 2		1		1
	osSim.DiskReadRequested(0, "job.docx");
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		0		1
	// 1		0		2
	// 2		1		1

	osSim.FetchFrom(777);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		0		2
	// 2		1		1

	osSim.NewProcess(1);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		0		2
	// 2		0		3

	osSim.DiskJobCompleted(0);
	// The following happens after this command:
	// The process 1 returns to ready-queue after the hard disk finished reading file for it.
	// Since the process 1 has the higher priority than the one currently running, the process 1 starts using the CPU immedeately
	// To use the CPU, the process one need its last used page (page 1) in memory (and thats what your simulation should provide)
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		1		1
	// 2		0		3

	osSim.Exit();
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		
	// 2		0		3

	osSim.FetchFrom(740);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		
	// 2		0		3
	
	osSim.FetchFrom(3350);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		33		2
	// 2		0		3
	osSim.FetchFrom(740);
	// The page 7 of the process 2 is already in memory. RAM snapshot doesn't change but 
	// your simulation remembers that the page 7 is now freshly used.
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		33		2
	// 2		0		3

	osSim.NewProcess(20);
	osSim.FetchFrom(100);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		1		4
	// 2		0		4
	return 0;
}