#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
char *name;
struct ListF *listF;
struct ListD *listD;
struct Directory *parentDir;
} Directory;

typedef struct File {
char *name;
int size;
char *data;
Directory *dir;
} File;

#endif
