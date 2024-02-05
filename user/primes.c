#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXNUM 36

void
helper(int read_fd, int write_fd)
{
	char nums[MAXNUM];
	read(read_fd, nums, sizeof(nums));
	int temp = 0;
	for(int i = 0; i < MAXNUM; i++)
		if(nums[i] == '0'){
			printf("prime %d\n", i);
			nums[i] = '1';
			temp = i;	
			break;
		}
	if(temp == 0) exit(0);
	for(int i = 0; i < MAXNUM; i++) if(i % temp == 0) nums[i] = '1';
	int pid = fork();
	if(pid == -1){
		fprintf(1, "fork error\n");
		exit(1);
	}
	// parent
	else if(pid > 0){
		write(write_fd, nums, sizeof(nums));
		wait(0);
	}
	else if(pid == 0){
		helper(read_fd, write_fd);
		wait(0);
	}
}

int
main(int argc, char* argv[])
{
	int fd[2];
	char nums[MAXNUM];

	memset(nums, '0', sizeof(nums));
	pipe(fd);
	int pid = fork();
	if(pid == -1){
		fprintf(1, "fork error\n");
		exit(1);
	}
	// parent
	else if(pid > 0){
		nums[0] = '1';
		nums[1] = '1';
		write(fd[1], nums, sizeof(nums));
		wait(0);
	}
	// child
	else if(pid == 0){
		helper(fd[0], fd[1]);
		wait(0);
	}
	exit(0);
}
