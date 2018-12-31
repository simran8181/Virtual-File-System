#include"headerfile.h"

//Global Variables :

UFDT UFDTArr[MAXINODE];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;


int Authenticate(char usr[],char pwd[])
{
    char ch;
	int i=0;
	char Currentpwd[25],username[25];
	printf("Enter username: ");
	scanf("%[^\n]",username);
	printf("Enter  password: ");
	
	for(i=0;i<=8;i++)
	{
		
		ch = getch();
		Currentpwd[i]=ch;
		printf("*");
        
	}
		
		Currentpwd[i]='\0';
		
		if(strcmp(username,usr)==0)
        {
           if(strcmp(Currentpwd,pwd)==0)
           {
              return 1;
           }
        }
       
		else
        {
           return -1;
        }
}


void InitialiseSuperBlock()
{
	int i = 0;

	while(i < MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
	
	//In starting both totalinodes and freeinodes initialise to maxinode ie no.of.files

	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;

	printf("Initialisation done successfully\n");
}

void CreateDILB()
{
	int i = 1;      // i = 1 because it is linkedlist

	PINODE newn = NULL;
	PINODE temp = head;

	while(i <= MAXINODE)
	{
		newn = (PINODE) malloc (sizeof(INODE));   // allocating memory for node

		// Initialisation members of node :

		newn -> LinkCount = 0;
		newn-> ReferenceCount = 0;
		newn -> FileType = 0;
		newn -> FileSize = 0;
		newn -> Buffer = NULL;
		newn -> next = NULL;
		newn -> InodeNumber = i;

		if(temp == NULL)  // if linkedlist is empty
		{
			head = newn;
			temp = head;
		}
		else             // atleast 1 node is present in linkedlist
		{
			temp -> next = newn;
			temp = temp -> next;
		}

		i++;

	}

	printf("DILB created successfully\n");
}

void ls_file()
{
	int i = 0;
	PINODE temp = head;

	if(SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("Error : There is no files\n");
		return;
	}

	printf("\nFile Name\tInode Number\tFile Size\tLink Count\n");
	printf("------------------------------------------------------------------------------\n");

	while(temp != NULL)
	{
		if(temp -> FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n",temp -> FileName , temp -> InodeNumber , temp -> ActualFileSize , temp -> LinkCount);
		}

		temp = temp -> next;
	}
	printf("------------------------------------------------------------------------------\n");
}

void CloseAllFile()
{
	int i = 0;
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable ->ReadOffset = 0;
			UFDTArr[i].ptrfiletable ->WriteOffset = 0;
			(UFDTArr[i].ptrfiletable ->ptrinode ->ReferenceCount)--;
			break;
		}
		i++;
	}
}

void DisplayHelp()
{
	printf("Is : To list out all files\n");
	printf("clear : To clear console\n");
	printf("open : To open the file\n");
	printf("close : To close the file\n");
	printf("closeall : To close all opened files\n");
	printf("read : To read the contents from files\n");
	printf("write : To erite the contents into file\n");
	printf("exit : To terminate file system\n");
	printf("stat : To display information of files using name\n");
	printf("fstat : To display information of files using description\n");
	printf("truncate : To remove all data from files\n");
	printf("rm : To delete files\n");
}

int stat_file(char *name)
{
	PINODE temp = head ;
	int i = 0;

	if(name == NULL)
	{
		return -1;
	}

	while(temp != NULL)
	{
		if(strcmp(name , temp -> FileName) == 0)
		break;
		temp = temp -> next;
	}

	if(temp == NULL)
	{
		return -2;
	}

	printf("\n--------Statistical Information About File---------\n");
	printf("File Name : %s\n", temp -> FileName);
	printf("Inode Number : %d\n", temp -> InodeNumber);
	printf("File Size : %d\n", temp -> FileSize);
	printf("Actual File Size : %d\n", temp -> ActualFileSize);
	printf("Link Count : %d\n", temp -> LinkCount);
	printf("Reference Count : %d\n", temp -> ReferenceCount);

	if(temp -> Permission == 1)
	{
		printf("File permission : Read only\n");
	}
	else if(temp -> Permission == 2)
	{
		printf("File permission : Write only\n");
	}
	else if(temp -> Permission == 3)
	{
		printf("File permission : Read and Write \n");
	}

	printf("--------------------------------------------------------------------\n\n");

	return 0;
}

int fstat_file(int fd)
{
	PINODE temp = head;
	int i = 0;

	if(fd < 0)
	{
		return -1;
	}
	
	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -2;
	}

	temp = UFDTArr[i].ptrfiletable -> ptrinode;

	printf("\n--------Statistical Information About File---------\n");
	printf("File Name : %s\n", temp -> FileName);
	printf("Inode Number : %d\n", temp -> InodeNumber);
	printf("File Size : %d\n", temp -> FileSize);
	printf("Actual File Size : %d\n", temp -> ActualFileSize);
	printf("Link Count : %d\n", temp -> LinkCount);
	printf("Reference Count : %d\n", temp -> ReferenceCount);

	if(temp -> Permission == 1)
	{
		printf("File permission : Read only\n");
	}
	else if(temp -> Permission == 2)
	{
		printf("File permission : Write only\n");
	}
	else if(temp -> Permission == 3)
	{
		printf("File permission : Read and Write \n");
	}

	printf("--------------------------------------------------------------------\n\n");

	return 0;
}

int CloseFileByName(char *name)
{
	int i = 0;
	i = GetFDFromName(name);

	if(i == -1)
	{
		return -1;
	}

	UFDTArr[i].ptrfiletable ->ReadOffset = 0;
	UFDTArr[i].ptrfiletable ->WriteOffset = 0;
	(UFDTArr[i].ptrfiletable ->ptrinode ->ReferenceCount)--;
	return 0;
}

int GetFDFromName(char *name)
{
	int i = 0;
	 while(i < 50)
	 {
		 if(UFDTArr[i].ptrfiletable != NULL)
		 {
			if(_stricmp((UFDTArr[i].ptrfiletable -> ptrinode -> FileName),name) == 0)
			{
				 break;
			}
		 }
		 i++;
	 }
	 
	 if(i==50)
	 {
		 return -1;
	 }
	 else
	 {
		 return i;
	 }
}

int rm_File(char *name)
{
	int fd = 0;
	
	fd = GetFDFromName(name);

	if(fd == -1)
	{
		return -1;
	}
	(UFDTArr[fd].ptrfiletable ->ptrinode ->LinkCount)--;

	if(UFDTArr[fd].ptrfiletable ->ptrinode ->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable ->ptrinode ->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);
	}

	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;
}


void man(char *name)
{
	if(name == NULL)
	{
		return ;
	}
	if(_stricmp(name,"create") == 0)
	{
		printf("Description : Used to create new regular file\n");
		printf("Usage : create File_Name Permission\n");
	}

	else if(_stricmp(name,"read") == 0)
	{
		printf("Description : Used to read data from file\n");
		printf("Usage : read File_Name No_Of_Bytes_To_Read\n");
	}

	else if(_stricmp(name,"write") == 0)
	{
		printf("Description : Used to write into regular file\n");
		printf("Usage : write File_Name\n After this enter data that we want to write\n");
	}

	else if(_stricmp(name,"Is") == 0)
	{
		printf("Description : Used to list all information of files\n");
		printf("Usage : Is\n");
	}

	else if(_stricmp(name,"stat") == 0)
	{
		printf("Description : Used to display information of file\n");
		printf("Usage : stat File_name\n");
	}

	else if(_stricmp(name,"fstat") == 0)
	{
		printf("Description : Used to display information of file\n");
		printf("Usage : stat File_Descriptor\n");
	}

	else if(_stricmp(name,"truncate") == 0)
	{
		printf("Description : Used to remove data from file\n");
		printf("Usage : truncate File_name\n");
	}

	else if(_stricmp(name,"open") == 0)
	{
		printf("Description : Used to open existing file\n");
		printf("Usage : open File_name mode\n");
	}

	else if(_stricmp(name,"close") == 0)
	{
		printf("Description : Used to close opened file\n");
		printf("Usage : close File_name\n");
	}

	else if(_stricmp(name,"closeall") == 0)
	{
		printf("Description : Used to close all opened file\n");
		printf("Usage : closeall \n");
	}

	else if(_stricmp(name,"lseek") == 0)
	{
		printf("Description : Used to change file offset\n");
		printf("Usage : lseek File_Name changeInOffset StartPoint\n");
	}
	else if(_stricmp(name,"rm") == 0)
	{
		printf("Description : Used to delete the file\n");
		printf("Usage : rm File_name\n");
	}

	else 
	{
		printf("No manual entry available\n");
	}
}


int truncate_file(char *name)
{
	int fd = GetFDFromName(name);

	if(fd == -1)
	{
		return -1;
	}

	memset(UFDTArr[fd].ptrfiletable -> ptrinode -> Buffer,0,1024);
	UFDTArr[fd].ptrfiletable ->ReadOffset = 0;
	UFDTArr[fd].ptrfiletable ->WriteOffset = 0;
	UFDTArr[fd].ptrfiletable ->ptrinode ->ActualFileSize = 0;
}


void CloseFileByName(int fd)
{
	UFDTArr[fd].ptrfiletable ->ReadOffset = 0;
	UFDTArr[fd].ptrfiletable ->WriteOffset = 0;
	(UFDTArr[fd].ptrfiletable ->ptrinode ->ReferenceCount)--;
}

int WriteFile(int fd , char *arr , int isize)
{
	if(((UFDTArr[fd].ptrfiletable -> Mode) != WRITE) && ((UFDTArr[fd].ptrfiletable -> Mode) != READ + WRITE))
	{
		return -1;
	}

	if(((UFDTArr[fd].ptrfiletable -> ptrinode -> Permission) != WRITE) && ((UFDTArr[fd].ptrfiletable -> ptrinode -> Permission) != READ + WRITE))
	{
		return -1;
	}

	if((UFDTArr[fd].ptrfiletable -> WriteOffset) == MAXFILESIZE)
	{
		return -2;
	}

	if((UFDTArr[fd].ptrfiletable -> ptrinode -> FileType) != REGULAR)
	{
		return -3;
	}

	strncpy((UFDTArr[fd].ptrfiletable -> ptrinode -> Buffer) + (UFDTArr[fd].ptrfiletable -> WriteOffset),arr,isize);

	(UFDTArr[fd].ptrfiletable -> WriteOffset) = (UFDTArr[fd].ptrfiletable -> WriteOffset) + isize;

	(UFDTArr[fd].ptrfiletable -> ptrinode -> ActualFileSize) = (UFDTArr[fd].ptrfiletable -> ptrinode -> ActualFileSize) + isize;

	return isize;
}


PINODE Get_Inode(char *name)
{
	PINODE temp = head;
	int i = 0;

	if(name == NULL)
	{
		return NULL;
	}
	while(temp != NULL)
	{
		if(strcmp(name,temp ->FileName) == 0)
		{
			break ;
		}
		temp = temp -> next;
	}

	return temp ;
}

int CreateFileX(char *name,int permission)
{
	int i = 0;
	PINODE temp = head ;

	if((name == NULL) || (permission == 0) || (permission > 3))
	{
		return -1;
	}

	if(SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}

	(SUPERBLOCKobj.FreeInode)--;

	if(Get_Inode(name) != NULL)
	{
		return -3;
	}

	while(temp != NULL)
	{
		if(temp ->FileType == 0)
		break;

		temp = temp -> next;
	}

	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		break;
		i++;
	}

	UFDTArr[i].ptrfiletable = (PFILETABLE) malloc (sizeof(FILETABLE));

	if(UFDTArr[i].ptrfiletable == NULL)
	{
		return -4;
	}
	
	UFDTArr[i].ptrfiletable ->Count= 1;
	UFDTArr[i].ptrfiletable ->Mode = permission;
	UFDTArr[i].ptrfiletable ->ReadOffset = 0;
	UFDTArr[i].ptrfiletable ->WriteOffset = 0;

	UFDTArr[i].ptrfiletable ->ptrinode = temp;
	strcpy_s(UFDTArr[i].ptrfiletable ->ptrinode ->FileName , name);

	UFDTArr[i].ptrfiletable ->ptrinode -> FileType = REGULAR;
	UFDTArr[i].ptrfiletable ->ptrinode -> ReferenceCount = 1;
	UFDTArr[i].ptrfiletable ->ptrinode -> LinkCount = 1;
	UFDTArr[i].ptrfiletable ->ptrinode -> FileSize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable ->ptrinode -> ActualFileSize = 0;
	UFDTArr[i].ptrfiletable ->ptrinode -> Permission = permission;
	UFDTArr[i].ptrfiletable ->ptrinode -> Buffer = (char *) malloc(MAXFILESIZE);
	memset(UFDTArr[i].ptrfiletable ->ptrinode -> Buffer,0,1024);

	return i;
}




int ReadFile(int fd, char *arr,int isize)
{
	int read_size = 0;

	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}

	if(((UFDTArr[fd].ptrfiletable -> Mode) != READ) && ((UFDTArr[fd].ptrfiletable -> Mode) != READ + WRITE))
	{
		return -2;
	}

	if(((UFDTArr[fd].ptrfiletable -> ptrinode -> Permission) != READ) && ((UFDTArr[fd].ptrfiletable -> ptrinode -> Permission) != READ + WRITE))
	{
		return -2;
	}

	if((UFDTArr[fd].ptrfiletable -> ReadOffset) == UFDTArr[fd].ptrfiletable -> ptrinode ->ActualFileSize )
	{
		return -3;
	}

	if(UFDTArr[fd].ptrfiletable -> ptrinode ->FileType != REGULAR)
	{
		return -4;
	}

	read_size = (UFDTArr[fd].ptrfiletable -> ptrinode ->ActualFileSize) - (UFDTArr[fd].ptrfiletable -> ReadOffset);

	if(read_size < isize)
	{
		strncpy(arr,(UFDTArr[fd].ptrfiletable -> ptrinode -> Buffer) + (UFDTArr[fd].ptrfiletable -> ReadOffset),read_size);

	    (UFDTArr[fd].ptrfiletable -> ReadOffset) = (UFDTArr[fd].ptrfiletable -> ReadOffset) + read_size;
	}

	else
	{
		strncpy(arr,(UFDTArr[fd].ptrfiletable -> ptrinode -> Buffer) + (UFDTArr[fd].ptrfiletable -> ReadOffset),isize);

	    (UFDTArr[fd].ptrfiletable -> ReadOffset) = (UFDTArr[fd].ptrfiletable -> ReadOffset) + isize;
	}
	
	return isize;
}


int OpenFile(char *name,int mode)
{
	int i = 0;
	PINODE temp = NULL;
	
	if(name == NULL || mode <= 0)
	{
		return -1;
	}

	temp = Get_Inode(name);
	
	if(temp == NULL)
	{
		return -2;
	}
	
	if(temp ->Permission < mode)
	{
		return -3;
	}

	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable == NULL)
		break;

		i++;
	}

	UFDTArr[i].ptrfiletable = (PFILETABLE) malloc (sizeof(FILETABLE));

	if(UFDTArr[i].ptrfiletable == NULL)
	{
		return -1;
	}

	UFDTArr[i].ptrfiletable ->Count = 1;
	UFDTArr[i].ptrfiletable ->Mode = mode;

	if(mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable ->ReadOffset = 0;
		UFDTArr[i].ptrfiletable ->WriteOffset = 0;
	}
	else if(mode == READ)
	{
		UFDTArr[i].ptrfiletable ->ReadOffset = 0;
	}

	else if(mode == WRITE)
	{
		UFDTArr[i].ptrfiletable ->WriteOffset = 0;
	}

	UFDTArr[i].ptrfiletable ->ptrinode = temp;
	(UFDTArr[i].ptrfiletable ->ptrinode ->ReferenceCount)++;
	
	return i;
}

int LseekFile(int fd,int size,int from)
{
	if((fd < 0) || (from >2))
	{
		return -1;
	}

	if(UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}

	if(((UFDTArr[fd].ptrfiletable -> Mode) == READ) || ((UFDTArr[fd].ptrfiletable -> Mode) == READ + WRITE))
	{
		if(from == CURRENT)
		{
			if(((UFDTArr[fd].ptrfiletable ->ReadOffset) + size) > UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize)
			{
				return -1;
			}

			if(((UFDTArr[fd].ptrfiletable ->ReadOffset) + size) < 0)
			{
				return -1;
			}

			(UFDTArr[fd].ptrfiletable ->ReadOffset) = (UFDTArr[fd].ptrfiletable ->ReadOffset) + size ;
		}

		else if(from == START)
		{
			if(size > (UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize)) 
			{
				return -1;
			}

			if(size < 0)
			{
				return -1;
			}

			(UFDTArr[fd].ptrfiletable ->ReadOffset) = size;
		}

		else if(from == END)
		{
			if((UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) + size > MAXFILESIZE) 
			{
				return -1;
			}

			if(((UFDTArr[fd].ptrfiletable ->ReadOffset) + size) < 0)
			{
				return -1;
			}

			(UFDTArr[fd].ptrfiletable ->ReadOffset) = (UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) + size;
		}
	}

	else if((UFDTArr[fd].ptrfiletable -> Mode) == WRITE)
	{
		if(from == CURRENT)
		{
			if(((UFDTArr[fd].ptrfiletable ->WriteOffset) + size) > MAXFILESIZE)
			{
				return -1;
			}

			if(((UFDTArr[fd].ptrfiletable ->WriteOffset) + size) < 0)
			{

				return -1;
			}

			if(((UFDTArr[fd].ptrfiletable ->WriteOffset) + size) > (UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize))
			{
				(UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) = ((UFDTArr[fd].ptrfiletable ->WriteOffset) + size);
				(UFDTArr[fd].ptrfiletable ->WriteOffset) = ((UFDTArr[fd].ptrfiletable ->WriteOffset) + size);
			}
		}
		else if(from == START)
		{
			if(size > MAXFILESIZE)
			{
				return -1;
			}

			if(size < 0)
			{
				return -1;
			}

			if(size > (UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize))
			{
				(UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) = size;
				(UFDTArr[fd].ptrfiletable ->WriteOffset) = size;
			}
		}
		else if(from == END)
		{
			if((UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) + size > MAXFILESIZE)
			{
				return -1;
			}
			if(((UFDTArr[fd].ptrfiletable ->WriteOffset) + size) < 0)
			{
				return -1;
			}

			(UFDTArr[fd].ptrfiletable ->WriteOffset) = (UFDTArr[fd].ptrfiletable ->ptrinode->ActualFileSize) + size;
		}
	}
}