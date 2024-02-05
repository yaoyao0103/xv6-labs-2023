#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
	static char buf[DIRSIZ+1];
	char *p;

	// Find first character after last slash.
	for(p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	// Return blank-padding name.
	if(strlen(p) >= DIRSIZ) return p;
	memmove(buf, p, strlen(p));
	memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
	return buf;
}

char*
getName(char *path){
	char*p;
	for(p = path + strlen(path); p >= path && *p != '/'; p--)
		;
	p++;

	// Return blank-padding name.
	return p;
}

void
findInDir(char *path, char *keyword){
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;
	if((fd = open(path, O_RDONLY)) < 0){
		fprintf(2, "Find: cannot open %s\n", path);
		exit(1);
	}
	if(fstat(fd, &st) < 0){
		fprintf(2, "Find: cannot stat %s\n", path);
		close(fd);
		exit(1);
	}
	switch(st.type){
		case T_DEVICE:
		case T_FILE:
			break;
		case T_DIR:
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
				printf("Find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(buf);
			*p++ = '/';
			while(read(fd, &de, sizeof(de)) == sizeof(de)){
				if(de.inum == 0) continue;
				memmove(p, de.name, DIRSIZ);
				p[DIRSIZ] = 0;
				if(stat(buf, &st) < 0){
					printf("Find: cannot stat %s\n", buf);
					continue;
				}
				char* name = getName(buf);
				if(st.type == T_DIR && !(strcmp(name, "..") == 0 || strcmp(name, ".") == 0)) findInDir(buf, keyword);
				else if(strcmp(name, keyword) == 0) printf("%s\n", buf);
			}
			break;
	}
	close(fd);

}

int
main(int argc, char* argv[])
{
	if(argc != 3){
		fprintf(2, "Usage: enter path and keyword\n");
		exit(1);
	}
	findInDir(argv[1], argv[2]);
	exit(0);
}
