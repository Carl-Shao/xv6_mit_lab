#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    char buf = 'x';
    int pipe_ptc[2];
    int pipe_ctp[2];

    pipe(pipe_ptc);
    pipe(pipe_ctp);

    int pid = fork();

    if(pid == 0) {
        close(pipe_ptc[1]);
        close(pipe_ctp[0]);
        if (read(pipe_ptc[0], &buf, 1) == 1) {
            printf("ping\n");
        }
        write(pipe_ctp[1], &buf, 1);
        exit(0);
    }
    else {
        close(pipe_ptc[0]);
        close(pipe_ctp[1]);       
        write(pipe_ptc[1], &buf, 1);
        if (read(pipe_ctp[0], &buf, 1) == 1) {
            printf("pong\n");
        }
        wait(0);
        exit(0);
    }
}
