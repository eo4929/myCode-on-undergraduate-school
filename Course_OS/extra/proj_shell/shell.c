#include "string.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "wait.h"

#define MAXNUM_OF_COMARG	256
#define MAXSIZE	256

const char *prompt = "prompt> ";
const char *enter = "\n";
const char *overflow ="overflow!\n";
const char *forkerror = "fork error!\n";
const char *fopenerror = "fopenerror!\n";

char* cmdvector[MAXNUM_OF_COMARG]; // for programs and arguments
char cmdline[MAXSIZE]; // for input
char* cmdvectorbysemi[MAXNUM_OF_COMARG]; // for programs divided by semitoken and this point array is send for cmdvector

int makesemitoken(char* cmdline, const char *delimiters, char** cmdvector); // cmdline is parsed by ;delimiters ,cmdline to be stored in cmdvectorbysemi[i] and cmdvector[i]
void makecmdv(char* cmdline,const char *delimiters,char** cmdvector); // cmdline is parsed by white space delimiters to be stored in cmdvector[i]
void forkandexecvp(pid_t pid, pid_t* tokenpid,int status); // forkandexecvp is fork and execute child process

int makesemitoken(char* cmdline, const char *delimiters, char** cmdvector)
{
	int numtokens = 0;
	char *cmdlinebytoken = NULL;

	cmdlinebytoken = cmdline + strspn(cmdline, delimiters);
	//strspn : first parameter - string to be searched ,
	//		   second parameter - a set of character for searching
	//		   return - first character of first parameter not detected by a set of characters of second paramter
	if( (cmdvectorbysemi[numtokens] = strtok(cmdlinebytoken, delimiters)) == NULL )
	{
	//a token command is stored in cmdvectorbysemi[ ]
		return 0;
	}
	
	numtokens++;

	while(1)
	{
	//a token command is stored in cmdvectorbysemi[ ]
		if( (cmdvectorbysemi[numtokens] = strtok(NULL, delimiters) ) == NULL)
		{
			break;
		}

		if(numtokens == (MAXNUM_OF_COMARG -1) )
		{
			fputs(overflow, stdout);
			return -1;
		}

		numtokens++;
	}
	
	

	return numtokens;
}

void makecmdv(char* cmdline,const char *delimiters,char** cmdvector)
{
	int numtokens = 0;
	char *cmdlinebytoken = NULL;

	if( (cmdline==NULL) || (delimiters == NULL) ) // solve white space
	{
		return;
	}

	cmdlinebytoken = cmdline + strspn(cmdline, delimiters); 
	//strspn : first parameter - string to be searched ,
	//		   second parameter - a set of character for searching
	//		   return - first character of first parameter not detected by a set of characters of second paramter


	if( ( cmdvector[numtokens] = strtok(cmdlinebytoken, delimiters) ) == NULL )
	//a token command is stored in cmdvector[ ]
	{
		return;
	}

	numtokens++;



	while(1)
	{
        //a token command is stored in cmdvector[ ]
		if( (cmdvector[numtokens] = strtok(NULL, delimiters) ) == NULL)
		{
			break;
		}

		if(numtokens == (MAXNUM_OF_COMARG -1) )
		{
			fputs(overflow,stdout);
			return;
		}

		numtokens++;
	}

	return;
}


void forkandexecvp(pid_t pid, pid_t* tokenpid,int status)
{

		switch( pid = fork() )
		{
			case 0 :
				if(strchr(cmdline,';') != NULL)
				{
					int i = makesemitoken(cmdline,"';'",cmdvectorbysemi);
					for(int j = 0; j<i; j++)
					{
						switch(tokenpid[j] = fork())
						{
							case 0 :
								makecmdv(cmdvectorbysemi[j]," \t",cmdvector);

								if( strcmp(cmdvector[0], "quit") == 0 )
								{
									pid_t parentpid = getppid();
									kill(tokenpid[j],SIGQUIT);
									kill(parentpid,SIGQUIT); // kill parent process
								}
								execvp(cmdvector[0],cmdvector); // execute child process
								perror(cmdvector[0]);
								exit(1);
							case -1 :
								exit(1);
							default :
								waitpid(pid,&status,0);		
								if( WIFEXITED(&status) )// if child process'exit is nomal , return true
								{
								}
								else
								{
								}
						}
			     	}
				}
				else
				{
					makecmdv(cmdline, " \t",cmdvector);

					if( strcmp(cmdvector[0],"quit") == 0 )
					{
						pid_t parentpid = getppid();
						kill(pid,SIGQUIT);
						kill(parentpid,SIGQUIT);
					}
					execvp(cmdvector[0],cmdvector); // exec child process
					perror(cmdvector[0]);
					exit(1);
				}
			case -1 :
				exit(1);
			default :
				waitpid(pid,&status,0); // parent process wait for child process
				if( WIFEXITED(&status) )// if child process'exit is nomal , return true
				{
				}
				else
				{
				}
		}


}

int main(int argc, char** argv)
{
	pid_t tokenpid[MAXSIZE] = {0,}; //tokenpid is used for execute process divided by semicon token
	pid_t pid=0; // pid_t type may seem be int type but portable variable
    char* eofcheck; // when input ctrl + D ,and exit
    int status=0; // parent process error check


	if(argc == 2) // batch mode
	{
		FILE *fp=NULL; //File variable for reading batchfile
		int filesize=0;//file size

		if( (fp = fopen(argv[1],"r")) == NULL ) // file open with Read mode
		{
			fputs(fopenerror,stdout);
		}

		fseek(fp,0,SEEK_END); // move file pointer to file last point
		filesize = ftell(fp); // get file last point(filesize)
		fseek(fp,0,SEEK_SET); // move file pointer to file start point

		while( fgets(cmdline,filesize,fp) != NULL ) // echo and execute each line 
		{
			fputs(cmdline,stdout);
			cmdline[strlen(cmdline) -1] = '\0';
			forkandexecvp(pid,tokenpid,status);
		}

		fclose(fp);

	}
	else if( argc == 1) // interactive mode
	{

	while(1)
	{
		fputs(prompt,stdout);
		eofcheck = fgets(cmdline, MAXSIZE, stdin);
		cmdline[strlen(cmdline) -1] = '\0';
        //1. fgets absorts '\n' so, should  substitude '\n' for '\0'
		//2. when cmdline is stored to cmdvector, cmdvector must be ended with NULL because of execvp function
		if(eofcheck == NULL) // gets ctrl + D ,execute exit(1)
		{
			fputs(enter,stdout);// puts '\n'
			exit(1);
		}


		forkandexecvp(pid,tokenpid,status);


	}

	}
	else
	{
		fputs("must execute one of batch files!\n",stdout);
	}







	return 0;
}













