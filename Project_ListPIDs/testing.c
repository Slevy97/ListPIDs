#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	
	if (argc > 1) {
	
		int nr_children = atoi(argv[1]);
		int i = 0;
		pid_t pid = fork();

		if (pid < 0)
			return 1;
		else if (pid == 0) {
		
			fprintf(stdout, "%d\n", getpid());
		
			for (i = 0; i < nr_children; i++) {
			
				pid_t pid = fork();
				if (pid < 0)
					return 1;
				else if (pid == 0) {
				
					sleep(30);
					return 0;
				} else {
					/// parent time
				}
			}
			for (i = 0; i < nr_children; i++)
				wait(NULL);
			sleep(10);
			return 0;
		
		} else {

			/// parent time
		}
	}
	return 0;
}
