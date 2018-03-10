# ListPIDs
Operating systems project - modified OpenBSD's kernel in order to add a syscall that would give information about a process and its children (the syscall is defined in the sys_generic.c file and its header in the syscalls.master).
Then, ptree_user.c prints information starting from a certain process about all the processes in a tree (DFS order).
testing.c creates a number of processes that stays in a memory for a short period of time in order to test "ptree_user" and the syscall mentioned above.
