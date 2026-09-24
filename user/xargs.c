#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "usage: xargs command [args...]\n");
        exit(1);
    }

    char *args[32];
    int fixed = argc - 1;

    for(int i = 1; i < argc; i++){
        args[i-1] = argv[i];
    }

    char buf[512];
    int pos = 0;

    while(read(0, &buf[pos], 1) == 1){
        if(buf[pos] == ' '){
            buf[pos] = '\0';

            if(pos != 0){
                args[fixed] = buf;
                fixed++;
                pos++;
            }
            continue;
        }

        if(buf[pos] == '\n' && pos != 0){
            buf[pos] = '\0';
            args[fixed] = buf;
            fixed++;
            args[fixed] = 0;
            exec(args[0], args);
            fprintf(2,"xargs: exec failed\n");
            exit(1);
        }
        pos++;
    }

    if(pos > 0){
        buf[pos] = '\0';
        args[fixed] = buf;
        fixed++;
        args[fixed] = 0;
        exec(args[0], args);
    }

    exit(0);
}