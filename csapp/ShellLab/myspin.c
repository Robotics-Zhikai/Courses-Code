/* 
 * myspin.c - A handy program for testing your tiny shell 
 * 
 * usage: myspin <n>
 * Sleeps for <n> seconds in 1-second chunks.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
void sigint_handler(int sig)
{
    exit(0);
}
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}
/*
 * Signal - wrapper for the sigaction function
 */
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

int main(int argc, char **argv) 
{
    int i, secs;
    //Signal(SIGINT,  sigint_handler);  //如果取消这个注释，那么如果Ctrl+C说明本进程是因为一个被捕获的信号而终止的，其父进程中waitpid WIFSIGNALED(status)返回假
    //而如果注释掉的话，那么sigint信号就没有被捕获，那么如果Ctrl+C说明本进程是因为一个未被捕获的信号而终止的，其父进程中waitpid WIFSIGNALED(status)返回真
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <n>\n", argv[0]);
	exit(0);
    }
    secs = atoi(argv[1]);
    for (i=0; i < secs; i++)
	sleep(1);
    exit(0);
}
