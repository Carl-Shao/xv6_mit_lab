#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
prime_filter(int left_fd)
{
    int pipe_right[2];
    pipe(pipe_right);

    int pid = fork();

    if(pid == 0) {
        close(pipe_right[1]);
        close(left_fd);
        prime_filter(pipe_right[0]);
        exit(0);
    }
    else {
        close(pipe_right[0]);
        int number;
        if (read(left_fd, &number, sizeof(number)) == 0) {
            close(pipe_right[1]);
            close(left_fd);
            wait(0);
            exit(0);
        }
        fprintf(1, "prime %d\n", number);
        
        int left_number;
        while(read(left_fd, &left_number, sizeof(left_number)) > 0) {
            if (left_number % number == 0) { continue; }
            else { 
                write(pipe_right[1], &left_number, sizeof(left_number));
                continue; 
            }
        }

        close(pipe_right[1]);
        close(left_fd);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char* argv[])
{
    int pipe_prime[2];
    pipe(pipe_prime);

    int pid = fork();

    if (pid == 0) {
        close(pipe_prime[1]);
        prime_filter(pipe_prime[0]);
    }
    else {
        close(pipe_prime[0]);
        for (int i = 2; i <= 35; i++) {
            int number = i;
            write(pipe_prime[1], &number, sizeof(number));
        }
        close(pipe_prime[1]);
        wait(0);
        exit(0);
    }
}