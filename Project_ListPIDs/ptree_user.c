#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

struct my_process {

	pid_t pid;
	pid_t *children_pids;
	size_t nr_children;
	
	uid_t user_id;
	sigset_t syscall_signal;
	size_t nr_open_files;

	struct memory_data {
	
		segsz_t text_size;
		caddr_t text_addr;

		segsz_t data_segm_size;
		caddr_t data_segm_addr;
		
		segsz_t stack_size;
		
	} memory;
};

void print_proc(struct my_process *proc) {
	
	fprintf(stdout, "Process ID: %d\n", proc->pid);
	fprintf(stdout, "Children: ");

	if (proc->nr_children > 0) {	
	
		int it;
		for (it = 0; it < proc->nr_children; ++it) {
		
			fprintf(stdout, "%d ", proc->children_pids[it]);
		}
	} else fprintf(stdout, "No children");

	fprintf(stdout, "\n");
	fprintf(stdout, "Called by user: %d\n", proc->user_id);
	fprintf(stdout, "Interrupting signal: %d\n", proc->syscall_signal);
	fprintf(stdout, "Number of opened files: %zu\n", proc->nr_open_files);
	fprintf(stdout, "\n---Memory---\n");
	fprintf(stdout, "Text segment size: %d\n", proc->memory.text_size);
	fprintf(stdout, "Text segment virtual address: %p\n", proc->memory.text_addr);
	fprintf(stdout, "Data segment size: %d\n", proc->memory.data_segm_size);
	fprintf(stdout, "Data segment virtual address: %p\n", proc->memory.data_segm_addr);
	fprintf(stdout, "Stack size: %d\n", proc->memory.stack_size);
	fprintf(stdout, "\n##################################################\n\n");

}

struct my_process* load_process(pid_t pid, struct my_process *proc) {

	proc = (struct my_process*) malloc(sizeof(struct my_process));
	int ret_signal = syscall(331, pid, proc, 0);
	
	// Error checking

	if (ret_signal < 0) {
		
		printf("The process with pid %d could not be found", pid);
		free(proc);
		return NULL;
	}

	proc->children_pids = malloc(sizeof(pid_t) * ret_signal);
	syscall(331, pid, proc, 1);
	return proc;
}
	
void dfs_ptree(pid_t pid) {
	
	struct my_process *proc;
	proc = load_process(pid, proc);

	// Error checking
	if (proc == NULL)
		return;

	print_proc(proc);

	// Iterating through process' children
	int i = 0;

	for (i = 0; i < proc->nr_children; i++) {
		
		dfs_ptree(proc->children_pids[i]);
	}
	
	free(proc->children_pids);
	free(proc);
}

int main(int argc, char *argv[]) {
	
	freopen("data.out", "w", stdout);
	if (argc > 1) {
	
		dfs_ptree(atoi(argv[1]));
	}
	return 0;
}
