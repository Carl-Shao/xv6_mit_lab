#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char* path, char* target)
{
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    int fd = open(path, 0);

    if (fd < 0) {
        fprintf(2, "find: can not open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: can not stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        fprintf(2, "find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "can not stat %s\n", buf);
            continue;
        }
        if (strcmp(de.name, target) == 0) {
            fprintf(1, "%s\n", buf);
        }
        if (st.type == T_DIR) {
            find(buf, target);
        }
    }
    close(fd);
}

int
main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find directory file");
        exit(1);
    }

    struct stat st;
    if (stat(argv[1], &st) < 0) {
        fprintf(2, "find not stat %s\n", argv[1]);
        exit(1);
    }

    if (st.type != T_DIR) {
        fprintf(2, "usage: find directory file");
        exit(1);
    }
    else {
        find(argv[1], argv[2]);
    }
    exit(0);
}
