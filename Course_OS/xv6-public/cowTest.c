#include "types.h"
#include "stat.h"
#include "user.h"
	
int a = 1;
	
void firstTest()
{
	printf(1,"%d free pages exists before forking\n",get_n_free_pages());
	printf(1,"Parent and Child share the global variable a \n");
	
	int pid = fork();

	if(pid==0){
		printf(1,"Child: a = %d\n",a);
		printf(1,"%d free pages exist before changing a\n",get_n_free_pages());
		a = 2;
		printf(1,"Child: a = %d\n",a);
		printf(1,"%d free pages exist after changing a\n",get_n_free_pages());
		exit();
	}
	
	printf(1,"Parent: a = %d\n",a);
	
	wait();
	
	printf(1,"Parent: a = %d\n",a);
	printf(1,"%d free pages exist after wait\n",get_n_free_pages());
	
	return;
}
	

void secondTest()
{
	printf(1,"%d free pages exist before fork1\n",get_n_free_pages());
	    
	if(fork()==0){

		exit();
	}else{
		printf(1,"%d free pages exist before fork2\n",get_n_free_pages());
	        
		if(fork()==0){
			printf(1,"%d free pages exist before changes in Child2\n",get_n_free_pages());
			a = 5;
			printf(1,"%d free pages exist after changes in Child2\n",get_n_free_pages());
			exit();
		}
		
		wait();
		printf(1,"%d free pages after reaping Child1\n",get_n_free_pages());
	}
	
	wait();
	printf(1,"%d free pages exist after reaping Child2\n",get_n_free_pages());
	    
	return ;
}
	

	
	
int main(void)
{
	printf(1,"firstTest running....\n");
	firstTest();
	printf(1,"firstTest finished\n");
	printf(1,"--------------------\n");
	
	printf(1,"secondTest running....\n");
	secondTest();
	printf(1,"secondTest finished\n");
	printf(1,"--------------------\n");
	
	
	exit();
}
