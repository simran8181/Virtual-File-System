#define _CRT_SECURE_NO_WARNIGS      //Used to hide warnings

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>
#include<conio.h>


#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 1024

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2



// Function Prototypes :


int Authenticate(char[],char[]);

void InitialiseSuperBlock();
void CreateDILB();
void ls_file();
void CloseAllFile();
void DisplayHelp();
int stat_file(char*);
int fstat_file(int);
int GetFDFromName(char*);
int CloseFileByName(char*);
int rm_File(char*);
void man(char*);
int truncate_file(char*);
void CloseFileByName(int);
int WriteFile(int,char*,int);
int CreateFileX(char*,int);
int ReadFile(int,char*,int);
int OpenFile(char*,int);
int LseekFile(int,int,int);


typedef struct superblock
{
	int TotalInodes;
	int FreeInode;

} SUPERBLOCK , *PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int ActualFileSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct inode *next;

} INODE , *PINODE , **PPINODE;

typedef struct filetable
{
	int ReadOffset;
	int WriteOffset;
	int Count;
	int Mode;
	PINODE ptrinode;

} FILETABLE , *PFILETABLE;

typedef struct ufdt
{
	PFILETABLE ptrfiletable;
} UFDT;


