#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
	int fd[2];
	char buf[100];
	pipe(fd);
	// child
	if(fork() == 0)
	{
		read(fd[0], buf, sizeof(buf)); 
		printf("%d: received ping\n", getpid());
		write(fd[1], "", 1);
		close(fd[0]);
		close(fd[1]);
	}	
	else
	{
		write(fd[1], "", 1);
		read(fd[0], buf, sizeof(buf));
		printf("%d: received pong\n", getpid());
		close(fd[0]);
		close(fd[1]);
	}

	exit(0);
}
