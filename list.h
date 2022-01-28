#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "file.h"

typedef struct ListF {
	struct ListF *next;
	File *file;
} ListF;

typedef struct ListD {
	struct ListD *next;
	Directory *directory;
} ListD;

Directory *create_FS(Directory *root);

void touch(Directory *currDir, char *par_1, char *par_2);

void mkdir(Directory *currDir, char *par_1);

void ls(Directory *currDir);

Directory *cd(Directory *currDir, char *dirName);

void pwd(Directory *currDir);

void tree(Directory *currDir);

ListD *findNode(Directory *currDir);

void rm(Directory *currDir, char *name);

void rmdir(Directory *currDir, char *name);

void deleteDir(Directory *currDir);
