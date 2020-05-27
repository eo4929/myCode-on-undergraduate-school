#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void func()
{
	//exit(1);
	return;
}

int main()
{
	func();

	printf("hi\n");

	return 0;
}


/*
	char str1[] = "ls -al;pwd";

	if(strchr(str1,';') != NULL)
	{
		int numtoken = 0;
		char* cmd[30] = {NULL,};

		if( (cmd[numtoken] = strtok(str1,"';'")) == NULL)
		{
			return 0;
		}
		printf("%s\n",cmd[numtoken]);
		numtoken++;
		
		while(1)
		{
			if( (cmd[numtoken] = strtok(NULL,"';'") ) == NULL)
				break;

			printf("%s\n",cmd[numtoken]);
			numtoken++;
		}

		
	}
	*/

/*
	char str1[] = "ls -a;pwd";

	char* parsestr1 = strtok(str1,"';' \t");
	while(parsestr1 != NULL)
	{
		printf("%s\n",parsestr1);
		parsestr1 = strtok(NULL,"';' \t");
	}
*/
/*
	char string[] = "ls -a";
	char strCharset[] = " ";
	
	//문자열에서 '1', '2', 'a', 'b' 문자가 없는 첫 위치를 검색
	printf("%d\n",strspn( string, strCharset ) );
*/

/*
char str1[]="hi hello"; // pointer로 가리키면 안됨
	
	char* nextstring = strtok(str1," ");
	while(nextstring != NULL)
	{
		printf("%s\n" ,nextstring);

		nextstring = strtok(NULL," ");
	}
*/
/*
int** matrix;
	
	matrix = (int**) malloc( 5 * sizeof(int*) );

	for(int i = 0; i<5; i++)
		matrix[i] = (int*) malloc( 4 *sizeof(int) );

	for(int j = 0; j<5; j++)
	{
		for(int k =0 ; k<4; k++)
		{
			matrix[j][k] = 10;
		}
	}

	for(int j = 0; j<5; j++)
	{
		for(int k =0 ; k<4; k++)
		{
			printf("%d\n" , &matrix[j][k]);
		}
	}
*/