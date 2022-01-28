#include "list.h"

Directory *create_FS(Directory *root)
{
	root = (Directory *)malloc(sizeof(Directory));
	if (root == NULL) {
		printf("Alocarea dinamica a esuat");
		return root;
	}
	root->name = (char *)malloc(2*sizeof(char));
	root->name[0] = '/';
	root->name[1] = '\0';
	root->parentDir = NULL;
	root->listD = (ListD *)malloc(sizeof(ListD));
	(root->listD)->directory = NULL;
	(root->listD)->next = NULL;
	root->listF = (ListF *)malloc(sizeof(ListF));
	(root->listF)->file = NULL;
	(root->listF)->next = NULL;
	return root;
}

void touch(Directory *currDir, char *par_1, char *par_2)
{
	ListF *currNode, *prevNode, *newNode;
	File *file;

	file = (File *)malloc(sizeof(File));
	if (file == NULL) {
		printf("Alocarea dinamica a esuat");
		return;
	}
	file->name = (char *)malloc(strlen(par_1)*sizeof(char));
	strcpy(file->name, par_1);
	file->size = strlen(par_2);
	file->data = (char *)malloc(strlen(par_2)*sizeof(char));
	strcpy(file->data, par_2);
	file->dir = currDir;
	if ((currDir->listF)->file == NULL) {
		(currDir->listF)->file = file;
		(currDir->listF)->next = NULL;
		return;
	}
	currNode = (currDir->listF);
	prevNode = NULL;
	newNode = (ListF *)malloc(sizeof(ListF));
	newNode->file = file;
	while (strcmp((currNode->file)->name, file->name) < 0) {
		prevNode = currNode;
		currNode = currNode->next;
		if (currNode == NULL)
			break;
	}
	if (prevNode == NULL) {
		currDir->listF = newNode;
		newNode->next = currNode;
	} else {
		prevNode->next = newNode;
		newNode->next = currNode;
	}
}

void mkdir(Directory *currDir, char *par_1)
{
	ListD *currNode, *prevNode, *newNode;
	Directory *dir;

	dir = (Directory *)malloc(sizeof(Directory));
	if (dir == NULL) {
		printf("\nAlocarea dinamica a esuat\n");
		return;
	}
	dir->name = (char *)malloc(strlen(par_1)*sizeof(char));
	strcpy(dir->name, par_1);
	dir->parentDir = currDir;
	dir->listD = (ListD *)malloc(sizeof(ListD));
	(dir->listD)->directory = NULL;
	(dir->listD)->next = NULL;
	dir->listF = (ListF *)malloc(sizeof(ListF));
	(dir->listF)->file = NULL;
	(dir->listF)->next = NULL;
	if ((currDir->listD)->directory == NULL) {
		(currDir->listD)->directory = dir;
		(currDir->listD)->next = NULL;
		return;
	}
	currNode = (currDir->listD);
	prevNode = NULL;
	newNode = (ListD *)malloc(sizeof(ListD));
	newNode->directory = dir;
	while (strcmp((currNode->directory)->name, dir->name) < 0) {
		prevNode = currNode;
		currNode = currNode->next;
		if (currNode == NULL)
			break;
	}
	if (prevNode == NULL) {
		currDir->listD = newNode;
		newNode->next = currNode;
	} else {
		prevNode->next = newNode;
		newNode->next = currNode;
	}
}

void ls(Directory *currDir)
{
	ListF *currNode;

	currNode = currDir->listF;
	if ((currNode->file) != NULL) {
		while (currNode != NULL) {
			printf("%s ", (currNode->file)->name);
			currNode = currNode->next;
		}
	}
	ListD *CurrNode;

	CurrNode = currDir->listD;
	if ((CurrNode->directory) != NULL) {
		while (CurrNode != NULL) {
			printf("%s ", (CurrNode->directory)->name);
			CurrNode = CurrNode->next;
		}
	}
		puts("");
}

Directory *cd(Directory *currDir, char *dirName)
{
	ListD *currNode;

	if (dirName[0] == '\0')
		return currDir;
	if (strcmp(dirName, "..") == 0) {
		if (currDir->name[0] == '/')
			return currDir;
		return currDir->parentDir;
	}
	currNode = currDir->listD;
	if (currNode->directory == NULL) {
		printf("Cannot move to '%s': No such directory!\n", dirName);
		return currDir;
	}
	while (strcmp(dirName, (currNode->directory)->name) != 0) {
		currNode = currNode->next;
		if (currNode == NULL) {
			printf("Cannot move to '%s': No such directory!\n",
				dirName);
			return currDir;
		}
	}
	return currNode->directory;
}

void pwd(Directory *currDir)
{
	if (currDir->parentDir != NULL)
		pwd(currDir->parentDir);
	if (currDir->name[0] == '/')
		printf("");
	else
		printf("/%s", currDir->name);
}

void tree(Directory *currDir)
{
	ListD *currNodeD;
	ListF *currNodeF;
	int i, depth = 0;

	while (depth != -1) {
		if (currDir->name[0] != '/')
			currNodeD = findNode(currDir);
		else
			currNodeD = NULL;
		for (i = 1; i <= depth; i++)
			printf("    ");
		printf("%s\n", currDir->name);
		currNodeF = currDir->listF;
		if (currNodeF->file != NULL) {
			while (currNodeF != NULL) {
				for (i = 1; i <= depth+1; i++)
					printf("    ");
			printf("%s\n", currNodeF->file->name);
			currNodeF = currNodeF->next;
			}
		}
		if (currDir->listD->directory != NULL) {
			currDir = currDir->listD->directory;
			depth++;
		} else {
			if (depth == 0) {
				depth = -1;
				continue;
			}
			if (currDir->name[0] != '/')
				currNodeD = currNodeD->next;
			if (currNodeD != NULL)
				currDir = currNodeD->directory;
			else {
				int x = 0;

				while (x == 0) {
					if (currDir->name[0] != '/')
						currDir = currDir->parentDir;
					if (currDir->name[0] == '/') {
						x = 1;
						depth = -1;
						continue;
					}
					depth--;
					if ((depth == -1) || (depth == 0)) {
						x = 1;
						depth = -1;
						continue;
					}
					currNodeD = findNode(currDir);
					currNodeD = currNodeD->next;
					if (currNodeD != NULL) {
						currDir = currNodeD->directory;
						x = 1;
					}
				}
			}
		}
	}
}

ListD *findNode(Directory *currDir)
{
	ListD *currNode;

	currNode = currDir->parentDir->listD;
	while (strcmp(currNode->directory->name, currDir->name) != 0)
		currNode = currNode->next;
	return currNode;
}

void rm(Directory *currDir, char *name)
{
	ListF *headNode;
	ListF *currNode;
	ListF *prevNode;

	currNode = currDir->listF;
	headNode = currNode;
	prevNode = currNode;
	if (currNode->file == NULL) {
		printf("Cannot remove '%s': No such file!\n", name);
		return;
	}
	while (strcmp(name, currNode->file->name) != 0) {
		prevNode = currNode;
		currNode = currNode->next;
		if (currNode == NULL) {
			printf("Cannot remove '%s': No such file!\n", name);
			return;
		}
	}
	if (headNode == currNode) {
		if (currNode->next == NULL) {
			currNode->file = NULL;
			currNode->next = NULL;
			return;
		}
		currDir->listF = currNode->next;
		free(currNode);
		return;
	}
	prevNode->next = currNode->next;
	free(currNode);
}

void rmdir(Directory *currDir, char *name)
{
	if (name[0] == '\0')
		return;
	ListD *headNode;
	ListD *prevNode;
	ListD *currNode;

	currNode = currDir->listD;
	prevNode = currNode;
	headNode = currNode;
	if (currNode->directory == NULL) {
		printf("Cannot remove '%s': No such directory!\n", name);
		return;
	}
	while (strcmp(name, currNode->directory->name) != 0) {
		prevNode = currNode;
		currNode = currNode->next;
		if (currNode == NULL) {
			printf("Cannot remove '%s': No such directory!\n",
				name);
			return;
		}
	}
	if (headNode == currNode) {
		if (currNode->next == NULL) {
			deleteDir(currNode->directory);
			free(currNode->directory->name);
			free(currNode->directory);
			currNode->directory = NULL;
			return;
		}
		currDir->listD = currNode->next;
		deleteDir(currNode->directory);
		free(currNode->directory->name);
		free(currNode->directory);
		free(currNode);
		return;
	}
	prevNode->next = currNode->next;
	deleteDir(currNode->directory);
	free(currNode->directory->name);
	free(currNode->directory);
	free(currNode);
}

void deleteDir(Directory *currDir)
{
	ListD *nodeD;
	ListF *nodeF;

	nodeF = currDir->listF;
	if (nodeF->file != NULL) {
		currDir->listF = nodeF->next;
		free(nodeF->file->data);
		free(nodeF->file->name);
		free(nodeF->file);
		free(nodeF);
		while (currDir->listF != NULL) {
			nodeF = currDir->listF;
			currDir->listF = nodeF->next;
			free(nodeF->file->data);
			free(nodeF->file->name);
			free(nodeF->file);
			free(nodeF);
		}
	} else
		free(nodeF);
	nodeD = currDir->listD;
	if (nodeD->directory != NULL)
		deleteDir(nodeD->directory);
	else {
		free(nodeD);
		return;
	}
	nodeD = currDir->listD;
	if (nodeD->next != NULL) {
		currDir->listD = nodeD->next;
		free(nodeD->directory->name);
		free(nodeD->directory);
		free(nodeD);
	} else {
		free(nodeD->directory->name);
		free(nodeD->directory);
		free(nodeD);
		return;
	}
	while (currDir->listD != NULL) {
		deleteDir(currDir->listD->directory);
		nodeD = currDir->listD;
		currDir->listD = nodeD->next;
		free(nodeD->directory->name);
		free(nodeD->directory);
		free(nodeD);
	}
}
