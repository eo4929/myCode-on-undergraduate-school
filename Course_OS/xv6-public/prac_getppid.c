#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"



int getppid(void)
{
	cprintf("My pid is %d\n",myproc()->pid);
	cprintf("My ppid is %d\n",myproc()->parent->pid);
	return 0;
}

int sys_getppid(void)
{
	getppid();
	
	return 0;
}

