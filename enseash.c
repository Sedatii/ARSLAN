#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void DisplayWelcomeMessage() {
    char welcome[] = "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n";
    write(STDOUT_FILENO, welcome, strlen(welcome));
}

void ChildProcess() {
	char input[200];
	char exitbye[] = "Bye bye...\n";
	input[strcspn(input, "\n")] = '\0';
	char *espace = strchr(input, ' ');
	
	if (espace == NULL) { 
		/// one argument : 
		read(STDIN_FILENO, input, 100);
		input[strcspn(input, "\n")] = '\0';
		if (strcmp(input, "exit") == 0) { 
			write(STDOUT_FILENO, exitbye, strlen(exitbye));	
		}
		else { /// all others functions
			execlp(input,input,NULL);
			perror("execlp");
			exit(EXIT_FAILURE);
		}
	}
	else{
		///two arguments : 
		char input2[200];
		read(STDIN_FILENO, input, 100);
		input[strcspn(input, "\n")] = '\0';
		
        if (strcmp(input, "exit") == 0) { 
            write(STDOUT_FILENO, "Bye bye...\n", strlen("Bye bye...\n"));
        } else { /// all others functions
            execlp(input, input, input2, NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
		
	}
}
}

int main(int argc, char **argv __attribute__((unused))){
	int status;
	char prompt[] = "enseash %";
	char status_exit[100];
	char status_sign[100];
	struct timespec start_time,end_time;
	
    DisplayWelcomeMessage();
    
    while (1) { 
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		write(STDOUT_FILENO, prompt, strlen(prompt));
		pid_t pid = fork();
		if (pid < 0) {
			perror("error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) { /// CHILD
			ChildProcess();
		}
		else { /// FATHER
            wait(&status);
            clock_gettime(CLOCK_MONOTONIC, &end_time);
            if (WIFEXITED(status)) {
				snprintf(status_exit, 100, "enseash [exit:%d|%ld ms] %% ", WEXITSTATUS(status),(end_time.tv_sec * 1000 + end_time.tv_nsec / 1000000) - (start_time.tv_sec * 1000 + start_time.tv_nsec / 1000000));
                write(STDOUT_FILENO, status_exit, strlen(status_exit));
            }
            else if (WIFSIGNALED(status)) {
				snprintf(status_sign, 100, "enseash [sign:%d|%ld ms] %% ", WTERMSIG(status), (end_time.tv_sec * 1000 + end_time.tv_nsec / 1000000) - (start_time.tv_sec * 1000 + start_time.tv_nsec / 1000000));
                write(STDOUT_FILENO, status_sign, strlen(status_sign));
            }
		}
	}
	exit(EXIT_SUCCESS);
}

