// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fs.h"

// char*
// fmtname(char *path)
// {
//   static char buf[DIRSIZ+1];
//   char *p;

//   // Find first character after last slash.
//   for(p=path+strlen(path); p >= path && *p != '/'; p--)
//     ;
//   p++;

//   // Return blank-padded name.
//   if(strlen(p) >= DIRSIZ)
//     return p;
//   memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//   return buf;
// }

// void find(char* path, char* filename) 
// {
//     char buf[512], *p;
//     int fd;
//     struct dirent de; // 目录项
//     struct stat st;
//     if ((fd = open(path, 0)) == 0) {
//         fprintf(2, "find: cannot open %s\n", path);
//         return;
//     }
//     if(fstat(fd, &st) < 0) {
//         fprintf(2, "find: cannot stat %s\n", path);
//         close(fd);
//         return;
//     }
//     switch (st.type)
//     {
//     case T_FILE:
//         printf("please enter dir\n");
//         break;
//     case T_DIR:
//         if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
//             printf("ls: path too long\n");
//             break;
//         }

//         strcpy(buf, path);
//         p = buf + strlen(buf);
//         *p++ = '/';
//         while (read(fd, &de, sizeof(de)) == sizeof(de)) {
//             if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
//                 continue;
//             memmove(p, de.name, DIRSIZ);
//             p[DIRSIZ] = 0;
//             if(stat(buf, &st) < 0){
//                 printf("find: cannot stat %s\n", buf);
//                 continue;
//             }
//             if (st.type == T_DIR) {
//                 find(buf, filename);
//             }
//             else if (st.type == T_FILE && (strcmp(de.name, filename) == 0)) {
//                 printf("%s\n", buf);
//             }
//         }
//         break;
//     default:
//         break;
//     }
// }

// int main(int argc, char* argv[]) 
// {
//     if (argc < 3)
//     {
//         printf("usage: find <path> <filename>\n");
//         exit(1);
//     }
//     char* path = argv[1];
//     char* filename = argv[2];
//     find(path, filename);
//     exit(0);
//     return 0;
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char* path, char* target)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
      fprintf(2, "find: cannot open %s\n", path);
      return;
    }

    if(fstat(fd, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", path);
      close(fd);
      return;
    }
    
    switch (st.type)
    {
    case T_FILE:
        printf("please enter dir\n");
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            stat(buf, &st);
            if (st.type == T_FILE && strcmp(de.name, target) == 0) {
                printf("%s\n", buf);
            }
            else if (st.type == T_DIR){
                find(buf, target);
            }
        }
        break;
    default:
        break;
    }
}

int
main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("usage: find <path> <target>\n");
        exit(1);
    }
    char* path = argv[1];
    char* target = argv[2];
    find(path, target);
    exit(0);
    return 0;
}