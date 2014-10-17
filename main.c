#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>

//Dan Miller ---- Project 2

char** getinput();
int mode_command(char **cmd, int *parallel_mode);


int main(int argc, char **argv) {
	int parallel_mode = 0; //holds the mode: 0 means sequential, 1 means parallel
	int new_mode = 0;
	int switch_mode = 0;
	int childrv = 0;
	int num_children = 0;
	int running = 1;

   	while(1) {
		char** cmd = getinput(parallel_mode);
     		for (int i = 0; cmd[i] != NULL; i++) {
			char **new_array = malloc(512 * sizeof(char*));
    			char *temps = strdup(cmd[i]);
    			char *token = strtok(temps," \n\t");
    			if (token!=NULL) {
    		    		new_array[0] = strdup(token);
    			}
    			int j = 1;
    			while (token!=NULL) {
				token = strtok(NULL," \n\t");
				if (token!=NULL) {
					new_array[j] =strdup(token); 
					j++;
				}
    			}
			for (int k = 0; new_array[k]!= NULL; k++) {
	     			int match = strcasecmp(new_array[k], "mode");
	     			running = strcasecmp(new_array[k], "exit");
	     			printf("Item being run: %s\n", new_array[k]);
	     			if (match == 0) {
	        			new_mode = mode_command(new_array, &parallel_mode);
					switch_mode = 1;
	     			}
	    			if (running == 0) {
					if ((k==0) && (j==1)) {
		     				exit(1);
					}
	    			}
			}
		
    			new_array[j] = NULL;
    			free(temps);

			//code for sequential mode: default setting
			pid_t child = fork();
			num_children ++;
        		if (child == 0) {
				if ((execv(new_array[0], new_array) < 0) && switch_mode==0 && running!=0) {
            				fprintf(stderr, "execv failed: %s\n", strerror(errno)); 
					num_children --;
        			}
        		}	
        		else if (parallel_mode == 0) {
				wait(&childrv);
				printf("Ran in sequential mode \n");
       			}
		}

		//code for parallel mode
        	if (parallel_mode == 1) {
	    		int count = 0;
	    		while (count < num_children) {
				wait(&childrv);
				count++;
		   	}
			printf("Ran in parallel mode \n");
		}
   		if (running == 0) {
      			exit(1);
      		}
    		parallel_mode = new_mode;
    		running = 1;
    		}
    	return 0;
}



char** getinput(int parallel_mode){
     	printf("Type a command: ");
     	fflush(stdout);
     	char command[1024];
     	char **new_array = malloc(512 * sizeof(char*));
     	if (fgets(command, 1024, stdin)!= NULL) { 
		command[strlen(command)-1] = '\0'; 
		for (int i = 0; i < strlen(command); i++) {
			if (command[i] == '#'){
				command[i] = '\0';
				break;
			}
		}
    		char *temps = strdup(command);
   		char *token = strtok(temps,";");
    		if (token!=NULL) {
    			new_array[0] = strdup(token);
    		}
    		int j = 1;
    		while (token!=NULL) {
			token = strtok(NULL,";");
			if (token!=NULL) {
				new_array[j] =strdup(token); 
				j++;
			}	
    		}
    		new_array[j] = NULL;
    		free(temps);
     	}  
     	else {
		printf("\n");
		exit(1);
	}
     	return new_array;   
}



int mode_command(char **cmd, int *parallel_mode){
    	if (cmd[1] == NULL) {
		if (*parallel_mode == 0) {
	    		printf("Running in s mode \n");
	    	}
		else {
	    		printf("Running in p mode \n");
		}
    	}
    	else if ((strcasecmp(cmd[1], "parallel") == 0) || (strcasecmp(cmd[1], "p") == 0)) {       
		printf("p mode\n");
		return 1;
    	}
    	else if ((strcasecmp(cmd[1], "sequential") == 0) || (strcasecmp(cmd[1], "s") == 0)) {   
		printf("s mode \n");
        	return 0;
    	}
    	else {
		printf(stderr, "Not a mode");
		exit(1);
    	}
    	return *parallel_mode;
}


