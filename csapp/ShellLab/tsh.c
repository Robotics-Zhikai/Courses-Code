/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state. 前台只能有一个job
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
//jid是本程序人为规定的ID，每加入一个job就增加1(nextjid)；而PID是操作系统内核分配的
//jid不一定是unique的，当jid累积超过max时，就归一
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    //getopt函数是解析argc argv的，"hvp"是输入参数 循环读取h v p参数并忽略其他干扰项
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }
   
    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    //Signal(SIGINT,SIG_DFL);
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    // printf("|||||||||||||||||||||||||\r\n"); //如果不加fflush，printf发出的顺序有点问题 不知道何原因 
    // fflush(stdout); //跟fflush有关系，原因未知

    } 


    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
 * 每个子进程都必须有一个唯一的进程组ID，否则如果出现前台进程和后台进程属于同一个进程组时，发送SIGTSTP就把后台进程也影响了
 * 
*/
void eval(char *cmdline) 
{
    // static int bgCount = 0; //后台运行子进程的数量
    // printf("new eval\r\n");
    char * argv[MAXARGS];
    pid_t pid;

    int bgflag = parseline(cmdline,argv);

    
    if (!builtin_cmd(argv))
    {
        sigset_t mask,prevMask;
        sigemptyset(&mask);
        sigaddset(&mask,SIGCHLD); //暂时阻塞SIGCHLD信号的接收，以免FG在waitpaid之前就被处理程序清理了，进而造成waitpaid调用错误
        sigaddset(&mask,SIGINT);//暂时阻塞SIGINT信号的接受，以免出现进入int中断时没有FG
        sigaddset(&mask,SIGTSTP);//暂时阻塞SIGTSTP信号的接收,以免出现进入tstp中断时没有FG
        sigprocmask(SIG_BLOCK,&mask,&prevMask);
        if ((pid=fork())==0) //每fork一个子进程肯定是一个新的PID
        {
            //printf("%s子进程的进程组为:%d\r\n",argv[0],getpgrp());
            //fflush(stdout); //如果不加fflush，这里的这个就直接不输出了
            sigprocmask(SIG_SETMASK,&prevMask,NULL);//子进程继承了父进程的阻塞向量,需要解除掉
            setpgid(0,0); 
            //这个和setpgid(pid,0)冗余，保证进入execve之后的进程都是属于同一进程组且与父进程的进程组不同；避免竞争
            execve(argv[0],argv,environ); //execve不返回 如果返回说明没找到命令 
            
            printf("没找到命令:%s\r\n",argv[0]);
            exit(0);
        }
        else if (pid == -1)
        {
            printf("创建子进程失败\r\n");
            exit(0);
        }

        // printf("%s父进程的进程组为:%d\r\n",argv[0],getpgrp());
        // fflush(stdout);

        setpgid(pid,0); //将新创建的子进程的进程组设置成为子进程的进程号
        //这样就使得子进程之间有着不同的进程组ID
        //子进程进入execve后就不能修改子进程的进程组了
        
        if (bgflag)
        {
            addjob(jobs,pid,BG,cmdline);
            sigprocmask(SIG_SETMASK,&prevMask,NULL); //取消sigchild、int及其他中断的堵塞
            //必须在这里取消int中断的阻塞，否则如果开始有一堆命令是后台运行的话，输入的ctrl+C会对之后前台运行的程序造成影响
            //也必须在这里取消sigchild中断的阻塞，否则如果一直运行的是后台命令的话最后的僵尸进程没办法reap


            printf("[%d] (%d) ",pid2jid(pid),pid,argv[0]);
            char ** curargv = argv;
            while (*curargv!=NULL)
            {
                printf("%s ",*curargv);
                curargv++;
            }
            printf("&\r\n");
            return; //说明要在后台运行
        }
        else
        {
            addjob(jobs,pid,FG,cmdline);

            sigdelset(&mask,SIGINT);
            sigdelset(&mask,SIGTSTP);
            sigprocmask(SIG_SETMASK,&mask,NULL);//取消int和stp堵塞

            waitfg(pid);

            sigprocmask(SIG_SETMASK,&prevMask,NULL); //取消sigchild中断的堵塞
            
        }
        // printf("返回的status=%d\r\n",status);

    }

    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    //既然是static的array了，肯定能保存住，不会销毁
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline); //第二个参数是源，第一个参数是目的
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    } //经过这个循环，就把以空格间隔开的cmdline的参数都放在了argv中
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg; //如果本函数返回1，则说明job不在前台运行
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
{
    if(strcmp(argv[0],"quit")==0)
        exit(0);
    else if (strcmp(argv[0],"jobs")==0)
    {
        listjobs(jobs); 
        return 1;
    }
    else if (strcmp(argv[0],"bg")==0 || strcmp(argv[0],"fg")==0)
    {
        if (argv[1]==NULL)
            printf("%s command requires PID or %%jobid argument\r\n",argv[0]);
        else if (argv[1][0]=='%'||(argv[1][0]>='0'&&argv[1][0]<='9'))
            do_bgfg(argv);
        else
            printf("%s: argument must be a PID or %%jobid\r\n",argv[0]);
        return 1;
    }
        
    // else if (strcmp(argv[0],"/bin/echo")==0)
    //     printf("%d\n",execve(argv[0],argv,environ));

    return 0;     /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    sigset_t mask,prevmask;
    sigfillset(&mask); //阻塞所有信号
    sigprocmask(SIG_BLOCK,&mask,&prevmask);

    int jid;
    struct job_t * job;
    if(argv[1][0]=='%')
    {
        jid = atoi(&argv[1][1]);
        if (jid==0)
        {
            printf("%s: argument must be a PID or %%jobid\r\n",argv[0]);
            return;
        }
    }
    else
    {
        pid_t pid = atoi(&argv[1][0]);
        if (pid==0)
        {
            printf("%s: argument must be a PID or %%jobid\r\n",argv[0]);
            return;
        }
        jid = pid2jid(pid);
        if (jid==0)
        {
            printf("(%d):No such process\r\n",pid);
            return;
        }
    }
    job = getjobjid(jobs,jid);
    if (job == NULL)
    {
        printf("%s:No such job\r\n",argv[1]);
        return ;
    }
        
    
    if (strcmp(argv[0],"bg")==0)
    {
        if (job->state==ST)
        {
            kill(-job->pid,SIGCONT); //这个必须是负的，才会把信号发送到该进程组中的所有进程
            job->state = BG;
        }
        printf("[%d] (%d) %s",job->jid,job->pid,job->cmdline);
    }
    else if (strcmp(argv[0],"fg")==0)
    {
        if (job->state==ST)
            kill(-job->pid,SIGCONT);
        job->state = FG;
        sigdelset(&mask,SIGINT);
        sigdelset(&mask,SIGTSTP);
        sigprocmask(SIG_SETMASK,&mask,NULL);//取消int和stp堵塞

        waitfg(job->pid);

        sigdelset(&mask,SIGCHLD);
        sigprocmask(SIG_SETMASK,&mask,NULL); //取消sigchild阻塞
    }

    sigprocmask(SIG_SETMASK,&prevmask,NULL);
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) //waitfg不应该设置阻塞向量位，由函数外边的环境决定
{
    pid_t ptmp = -2;
    int status = -2;
    //sigprocmask(SIG_SETMASK,&prevMask,NULL); 
    //sleep(10); //必须在这里堵塞sigchild信号，否则如果sleep（10），运行./myspin 5的话，会造成pid被收回

    //如果在调用waitpid时阻塞了sigchild：当在等待pid的过程中非pid终止，那么由于阻塞sigchild，
    //暂时不会对其做出响应，只会使得对应的信号为待处理，待解除sigchild阻塞后进入sigchild响应函数处理；
    //对于pid正常终止，仍旧会使得对应的信号为待处理（pending），但不立即响应，因为sigchild被阻塞，直到
    //解除阻塞，由于已经被waitpid收回了，因此在sigchild处理函数中就没有子进程了

    //如果在调用waitpid时没有阻塞sigchild：对于pid正常终止时，仍旧会使得对应的信号为待处理（pending），
    //但是先继续执行waitpid还是先执行sigchild响应函数是不确定的，如果先执行waitpid，那么没有问题，
    //但是如果先进入sigchild响应函数，把对应的进程释放掉，再出来执行waitpid时，就直接由于没有该进程进入了unix_error
    if ((ptmp = waitpid(pid,&status,WUNTRACED))<0) //挂起调用进程，直到等待集合中的一个进程变为已终止或被停止 
        unix_error("waitpid error\r\n");
    //printf("%d,%d,%d\r\n",WIFEXITED(status),WIFSIGNALED(status));

    // printf("ptmp:%d;status:%d\r\n",ptmp,status);
    // fflush(stdout);
    if (!WIFSTOPPED(status))
    {
        if (WIFSIGNALED(status)) 
        //如果子进程是因为一个未被捕获的信号终止的，那么就返回真 如果是正常退出，不进入这个if 不知道为何偏偏是未被捕获的信号就返回真这样的机制的原因
            printf("Job [%d] (%d) terminated by signal %d\r\n",pid2jid(pid),pid,WTERMSIG(status));
        deletejob(jobs,pid); //回收了阻塞进程，这只是把jobs列表移走了，真正的回收步骤不在这
    }
    else
    {
        printf("Job [%d] (%d) stopped by signal %d\r\n",pid2jid(pid),pid,WSTOPSIG(status));
        getjobpid(jobs,pid)->state = ST;
    }


    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    //printf("sigchld_handler\r\n");
    sigset_t mask,prevmask;
    sigfillset(&mask); //阻塞所有信号
    sigprocmask(SIG_BLOCK,&mask,&prevmask);

    int status;
    pid_t pidret;
    while((pidret=waitpid(-1,&status,WNOHANG|WUNTRACED))>0)
    //如果等待集合中的子进程都没有被停止或终止，则返回0 WNOHANG
    //如果等待集合中的一个进程变成已终止或者被停止，则返回导致停止的子进程pid
    {
        if (!WIFSTOPPED(status)) //只有当终止的子进程返回时才deletejob 否则对于停止的子进程不进行delete 
            deletejob(jobs,pidret);
    }
    sigprocmask(SIG_SETMASK,&prevmask,NULL);
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)  //sig存储被捕获的信号！！！！！！！
{
    sigset_t mask,prevmask;
    sigfillset(&mask); //阻塞所有信号
    sigprocmask(SIG_BLOCK,&mask,&prevmask);

    pid_t fgPID = fgpid(jobs);
    if (fgPID>0)
    {
        //printf不是异步信号安全的，因此尽量不要造信号处理函数中使用
        //kill(-fgPID,SIGKILL); //发送给fgPID号进程组，使其进程终止 这里的本意应该是要子进程调用捕获的信号
        kill(-fgPID,sig);//应该发送给子进程捕获的信号，然后让子进程自己选到底是捕获该信号按照非默认方式处理还是不捕获该信号按照默认方式处理

    }
        
    // exit(0); //不应该退出调用进程 因为还要在shell中输入命令

    sigprocmask(SIG_SETMASK,&prevmask,NULL);
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    //由于这个函数有全局变量要处理，因此为了保证数据一致性，需要临时把其他信号阻塞
    sigset_t mask,prevmask;
    sigfillset(&mask); //阻塞所有信号
    sigprocmask(SIG_BLOCK,&mask,&prevmask);

    pid_t fgPID = fgpid(jobs);
    if (fgPID>0)
    {
        //kill(-fgPID,SIGSTOP); //发送给fgPID号进程组，使其进程停止
        kill(-fgPID,SIGTSTP); //应该发送给子进程捕获的信号，然后让子进程自己选到底是捕获该信号按照非默认方式处理还是不捕获该信号按照默认方式处理
    }

    sigprocmask(SIG_SETMASK,&prevmask,NULL);
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++; //人为给定的一个jid
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    //没有找到空闲的能插入的job空位
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    //printf("deletejob,%d\r\n",pid);
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
//前台只能有一个job
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
//pid到jid是多对一映射
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
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

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}



