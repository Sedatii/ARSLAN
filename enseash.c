#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	/// variables definition
	int status;
	char input[100];
	char welcome[] = "Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n";
	char prompt[] = "enseash %";
	char exitbye[] = "Bye bye...\n";
	char status_exit[100];
	char status_sign[100];
	
    write(STDOUT_FILENO, welcome, strlen(welcome)); ///welcome message
    
    while (1) { /// execlp is stopping the process so need to add fork process
		write(STDOUT_FILENO, prompt, strlen(prompt));
		pid_t pid = fork();
		if (pid < 0) {
			perror("error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) { /// CHILD
			read(STDIN_FILENO, input, 100);
			input[strcspn(input, "\n")] = '\0';
			if (strcmp(input, "exit") == 0) { /// exception for exit command
				write(STDOUT_FILENO, exitbye, strlen(exitbye));
			}
			else { /// all others functions
				execlp(input, input, NULL);
			}
		}
		else { /// FATHER
            wait(&status);
            if (WIFEXITED(status)) {
				snprintf(status_exit, 100, "enseash [exit:%d] %% ", WEXITSTATUS(status));
                write(STDOUT_FILENO, status_exit, strlen(status_exit));
            }
            else if (WIFSIGNALED(status)) {
				snprintf(status_sign, 100, "enseash [sign:%d] %% ", WTERMSIG(status));
                write(STDOUT_FILENO, status_sign, strlen(status_sign));
            }
		}
	}
	exit(EXIT_SUCCESS);
}
