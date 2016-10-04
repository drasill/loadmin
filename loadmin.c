/*
loadmin: a wrapper with a load average limit
Based on ctx-loadmax
Copyright (C) 2000 LibertySurf Telecom
Copyright (C) 2006 Yes-Co Nederland
Licensed under the GPL v2
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <syslog.h>
#include <assert.h>
#include <sys/wait.h>

#include <stdio.h>
#include <fcntl.h>

main(int argc, char **argv, char **env) {

	pid_t pid;
	int status;
	int loadmax;
	bool stopped = false;
	
	if (argc < 3) {
		printf("usage: loadmin [loadmax] [command] [arguments]\n");
		exit(1);
	}

	loadmax = (int)strtol(argv[1], NULL, 10);
	if (loadmax < 1 || loadmax > 1000) {
		printf("Invalid loadmax\n");
		exit(1);
	}
	
	pid = fork();
	if (pid == -1) {
		printf("could not fork\n");
		exit(1);
	}
	if (pid == 0) {
		// child
		execvp(argv[2], argv+2);
		printf("exec failed\n");
		exit(1);
	} else {
		// parent
		do {
			if (lowload(loadmax)) {
				if (stopped) {
					printf("Loadmin: resuming\n");
					kill(pid, SIGCONT);
					stopped = false;
				}
			} else {
				if (!stopped) {
					printf("Loadmin: stopping\n");
					kill(pid, SIGSTOP);
					stopped = true;
				}
			}
			sleep(1);
		} while (0 == waitpid(pid, &status, WNOHANG));
	}
}

int lowload(int loadmax) {
	int fd;
	int n;
	char buf[16];
	double load;
	char* loadavgfile = "/proc/loadavg";
	
	fd = open(loadavgfile, O_RDONLY);
	if (fd < 0) {
		printf("can't open %s, %m", loadavgfile);
		return -1;
	}
	
	n = read(fd, buf, sizeof(buf) - 1);
	if (n < 0) {
		printf("can't read %s, %m", loadavgfile);
		return -1;
	}
	
	if (sscanf(buf, "%lf", &load) != 1) {
		printf("can't scanf load, \"%s\"", buf);
		return -1;
	}

	return (load < loadmax);
}
