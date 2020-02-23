#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}



int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;

  myproc()->sz += n; //assignment3_1
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


// mycodes inserted by assignment2

extern struct proc *getptable_proc(void);

void 
sys_getptable_proc(void)
{
	getptable_proc();
}

//extern int sys_getppid(void);

extern void setpriority(int, int);

void 
sys_setpriority(void)
{
	int pid, priority;

	if(argint(0,&pid) < 0)
		exit();
	if(argint(1,&priority) < 0)
		exit();

	setpriority(pid,priority);
}

extern int getlev(void);

void
sys_getlev(void)
{
	getlev();
}

int
sys_get_n_free_pages(void)
{
	return get_n_free_pages();
}
