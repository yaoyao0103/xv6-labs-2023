#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARG_LEN 100

int
main(int argc, char* argv[])
{
	char args[MAXARG][MAXARG_LEN];
	int argsCount = 0;
	int limitNum = 100;
	if(argc > 1){
		for(int i = 1; i < argc; i++){
			// param -n
			if(!strcmp(argv[i], "-n")) limitNum = atoi(argv[++i]);
			// command
			else memmove(args[argsCount++], argv[i], strlen(argv[i])+1);
		}
	}
	char c;
	int idx = 0;
	while(read(0, &c, 1) != 0){
		if(c == '\n' || c == ' '){
			args[argsCount++][idx] = '\0';
			idx = 0;	
		}
		else args[argsCount][idx++] = c;
		
	}
	char* tempArgs[MAXARG];
	int count = 0;
	tempArgs[count++] = args[0];
	for(int i = 1; i < argsCount; i++){
		tempArgs[count++] = args[i];	
		if(count == limitNum + 1){
			int pid = fork();
			if(pid == 0) exec(args[0], tempArgs);
			else wait(0);
			count = 0;
			tempArgs[count++] = args[0];	
		}	
	}
	if(count > 1){
		int pid = fork();
		if(pid == 0) exec(args[0], tempArgs);
		else wait(0);
	}
	exit(0);
}
