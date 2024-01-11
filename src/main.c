#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

int main(int argc, char **argv){
	char *endptr = NULL; //for strtoul
	long line_number;
	int remove_flag = 0;
	int view_flag = 0;
	int save_flag = 0;
	char cwd[PATH_MAX]; //current working directory
	int c;
	FILE *pF, *pTemp;
	int count = 1;
	char buffer[255];

	/* Handling arguments */
	while ((c = getopt (argc, argv, "vsr:")) != -1){
		switch (c){
			case 'v':
				view_flag = 1;
				break;
			case 's':
				save_flag = 1;
				break;
			case 'r':
				/* Line to be removed */
				line_number = strtoul(optarg, &endptr, 10);
				remove_flag = 1;
				/* Handle errors */
				if (endptr == optarg) {
					fprintf(stderr, "Error: %s is not a valid number\n", optarg);
					return 1;
				}
				if (*endptr != '\0') {
			        fprintf(stderr, "Error: %s contains extra characters\n", optarg);
			        return 1;
			    }
			    if (strlen(optarg) == strlen(endptr)) {
			        fprintf(stderr, "Error: %s is not a valid number\n", optarg);
			        return 1;
			    }
				break;
			/* Handle more errors */
			case '?':
				if (optopt == 'r'){
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				}
				else if (isprint(optopt)){
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				}
				else{
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
				}
				return 1;
		}
	}

	
	/* Open files + error handling */
	pF = fopen(".dirs", "a+");
	if(pF == NULL){
		printf("Error: Unable to open dirs file.");
		exit(1);
	}
	pTemp = fopen(".tmp", "w");
	if(pTemp == NULL){
		printf("Error: Unable to open tmp file.");
		exit(1);
	}

	if(view_flag != 0){
		while(fgets(buffer, 255, pF) != NULL){
			printf("%s",buffer);
		}
	}

	if(save_flag != 0){
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			fprintf(pF, "%s\n", cwd);
		}
		else {
			perror("getcwd() error");
			return 1;
		}
	}
	
	/* Write .tmp file line by line */
	while(remove_flag != 0 && fgets(buffer, 255, pF) != NULL){
		if (line_number != 0 && count != line_number){
			fprintf(pTemp,buffer);
		}
		count++;
	}

	/* Close files and replace .dirs file with .tmp file */
	fclose(pF);
	fclose(pTemp);
	if (remove_flag != 0){
		remove(".dirs");
		rename(".tmp", ".dirs");	
	}
	return 0;
}
