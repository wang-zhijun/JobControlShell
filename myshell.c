void 
init_shell() {
	/*获得shell的pid*/
	SHELL_PID = getpid();
	SHELL_TERMINAL = STDIN_FILENO;

	/* determines if the fd refers to a valid 
	 * terminal type device
	 * int isatty(int fd);*/
	SHELL_IS_INTERACTIVE = isatty(SHELL_TERMINAL);

	if(SHELL_IS_INTERACTIVE) {
		/* get its initial process group ID with the 
		 * getpgrp() and comparing
		 * it to the process group ID of the current foreground
		 * job associated with its controlling terminal(retrieved using the tcgetpgrp) */
		while(tcgetpgrp(SHELL_TERMINAL) != (SHELL_PGID = getpgrp()))
			/* If the subshell is not running as a foreground job, it must stop 
			 * itself by sending a SIGTTIN signal to its 
			 * own process group.*/
			kill(-SHELL_PGID, SIGTTIN);

		/* set the action for all the stop
		 * signals to SIG_IGN
		 * When a shell enables job control, 
		 * it should set itself to ignore all 
		 * all the job control stop signals so
		 * that it doesn't accidentally stop itself */
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);

		/* int setpgid(pid, pgid);
		 * put ourselves in our own process group*/
		SHELL_PGID = getpid();
		if(setpgid (SHELL_PGID, SHELL_PGID) < 0) {
			perror("Couldn't put the shell in its own process group");
			exit(1);
		}

		/* Grab control of the terminal */
		tcsetpgrp(SHELL_TERMINAL, SHELL_PGID);
		/* Save default terminal attributes for shell */
		tcgetattr(SHELL_TERMINAL, &SHELL_TMODES);

		currentDirectory = (char *)malloc(sizeof(char)*1024);
	}
	else {
		printf("Couldn't make shell interactive, sorry...\n");
		exit(1);
	}
}



		
	
