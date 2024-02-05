#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 36


void
helper(int fd[])
{
	char nums[MAX];
	read(fd[0], nums, sizeof(nums));
	int first = 0;
	for(int i = 0; i < MAX; i++){
		if(nums[i] == '0' && first == 0){
			printf("prime %d\n", i);
			first = i;
			nums[i] = '1';
		}
		else if(first != 0 && i % first == 0) nums[i] = '1';
	}
	if(first == 0) exit(0);
	
	int pid;	
	if((pid = fork()) == 0){
		helper(fd);
		wait(0);
	}
	else if(pid > 0){
		write(fd[1], nums, sizeof(nums));
		wait(0);		
	}
	else{
		fprintf(2, "Error: fork error\n");
		exit(1);
	}
}

int
main(int argc, char* argv[])
{
	char nums[MAX];
	memset(nums, '0', sizeof(nums));
	int fd[2];
	pipe(fd);
	int pid;
	if((pid = fork()) == 0){
		helper(fd);
		wait(0);
	}
	else if(pid > 0){
		nums[0] = '1';
		nums[1] = '1';
		write(fd[1], nums, sizeof(nums));
		wait(0);
	}
	else{
		fprintf(2, "Error: fork error\n");
		exit(1);
	}
	exit(0);
}
