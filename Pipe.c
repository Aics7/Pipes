//Parent and child processes communicating using pipes
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 100
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[])
{
    printf("#######___*******START*******___#######\n");
	printf("\nParent process %d accepting numbers from terminal.", (int) getpid());
	printf("\n...\n%d number(s) passed\n",argc-1);
	
	int fd[2];
	/*creating pipe*/
	if(pipe(fd)==-1)
	{
	    fprintf(stderr, "Pipe failed");
	    return 1;
	}
	/*creating child process*/
	int new_child_process = fork();
	
	/*If child process is not created*/
	if(new_child_process<0)
	{
		fprintf(stderr, "\nChild process could not be created. The fork attempt failed!!!\n");
	}
	/*If child process is created, it does computations*/
	else if (new_child_process == 0)
	{
	    printf("\n#######___*******NEXT*******___#######\n");
	    printf("\nChild process %d performing computations.\n", (int) getpid());
	    float sum, product, difference, division,x;
	    /*If at least one argument is passed*/
	    if(argc>1)
	    {
	        x=atof(argv[1]);//converts string to float
	        sum=product=difference=division=x;//assigned to first input
	        /*performs computation on rest of input*/
	        for(int i=2; i<argc;i++)
    		{
    		    x=atof(argv[i]);
    		    sum += x;
    		    product *= x;
    		    difference -= x;
    		    division /= x;
    		}
    		close(fd[READ_END]); //closes unsused end of pipe
    		/*Preparing result string to be written to pipe*/
    		char bufferStr[20];
    		char msg[BUFFER_SIZE] = "sum -> "; gcvt(sum, 9, bufferStr); strcat(msg,bufferStr);
    		strcat(msg,"\nproduct -> "); gcvt(product, 9, bufferStr); strcat(msg,bufferStr);
    		strcat(msg,"\ndifference -> "); gcvt(difference, 9, bufferStr); strcat(msg,bufferStr);
    		strcat(msg,"\ndivision -> "); gcvt(division, 9, bufferStr); strcat(msg,bufferStr);
    		write(fd[WRITE_END],msg,strlen(msg)+1);//write message to pipe
    		close(fd[WRITE_END]);//closed write end
    		printf("\n...\n...\n...\nComputation done!\n");
    	}
	    else
    	{
    	    printf("Fewer or no aruguments passed");
    	}
	}
	/*If all child processes are done, parent process runs again*/
	else
	{
		int wait_value = wait(NULL); 
		if(argc>1)
		{
		    printf("\n#######___*******NEXT*******___#######\n");
		    printf("Parent process %d receiving and printing results.\n...\n", (int) getpid());
		    close(fd[WRITE_END]);//closes unsused end of pipe
		    char msg[BUFFER_SIZE]="";
		    printf("%s\n",msg);
		    read(fd[READ_END], msg, BUFFER_SIZE);//reads message from pipe
		    printf("%s\n",msg);
		    close(fd[READ_END]);//closes read end
		    printf("\n#######___*******END*******___#######\n");
		}
	}
	return 0;
}                                                                                              