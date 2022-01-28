#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "list.h"

void read_Command(char *command, char *par_1, char *par_2)
{
	char cmd[100], *p;

	fgets(cmd, 100, stdin);
	p = strtok(cmd, " ");
	command = (char *)realloc(command, strlen(p)*sizeof(char));
	strcpy(command, p);
	if (strchr(command, '\n') != NULL)
		command[strlen(command)-1] = '\0';
	p = strtok(NULL, " ");
	if (p == NULL) {
		par_1[0] = '\0';
		par_2[0] = '\0';
		return;
	}
	par_1 = (char *)realloc(par_1, strlen(p)*sizeof(char));
	strcpy(par_1, p);
	if (strchr(par_1, '\n') != NULL)
		par_1[strlen(par_1)-1] = '\0';
	p = strtok(NULL, " ");
	if (p == NULL) {
		par_2[0] = '\0';
		return;
	}
	par_2 = (char *)realloc(par_2, strlen(p)*sizeof(char));
	strcpy(par_2, p);
	if (strchr(par_2, '\n') != NULL)
		par_2[strlen(par_2)-1] = '\0';
}

int main(void)
{
	Directory *currDir, *root = NULL;
	char *command, *par_1, *par_2;

	command = (char *)malloc(sizeof(char));
	par_1 = (char *)malloc(sizeof(char));
	par_2 = (char *)malloc(sizeof(char));
	while (1) {
		read_Command(command, par_1, par_2);
		if ((root == NULL) && ((strcmp(command, "create") == 0) &&
			(strcmp(par_1, "fs") == 0))) {
			root = create_FS(root);
			currDir = root;
		}
		if (root == NULL)
			continue;
		else {
			if ((strcmp(command, "delete") == 0) &&
				(strcmp(par_1, "fs") == 0)) {
				free(root);
				return 0;
			}
			if ((strcmp(command, "touch") == 0) &&
				(par_1 != NULL)) {
				touch(currDir,	par_1,	par_2);
			}
			if ((strcmp(command, "mkdir") == 0) &&
				(par_1 != NULL)) {
				mkdir(currDir, par_1);
			}
			if (strcmp(command, "ls") == 0)
				ls(currDir);
			if (strcmp(command, "pwd") == 0) {
				if (currDir->name[0] == '/')
					printf("/\n");
				else {
					pwd(currDir);
					puts("");
				}
			}
			if (strcmp(command, "cd") == 0)
				currDir = cd(currDir, par_1);
			if (strcmp(command, "tree") == 0)
				tree(currDir);
			if (strcmp(command, "rm") == 0)
				rm(currDir, par_1);
			if (strcmp(command, "rmdir") == 0)
				rmdir(currDir, par_1);
		}
	}
	return 0;
}
