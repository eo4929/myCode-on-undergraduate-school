#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
/*
#include "defs.h"
#include "date.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
*/

int main(int argc, char * argv[])
{
	int pid;
	int i=0;

	for(i=0;i<20;i++)
	{
	switch( pid = fork() )
	{
		case 0 :
			printf(1,"Child\n");

			yield();

			
		case -1 :
			exit();
		default :
			printf(1,"Parent\n");

			yield();

			wait();
	}
	}
	exit();
	return 0;
}
