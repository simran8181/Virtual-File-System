#include "headerfile.h"


//EntryPoint Function :

int main()
{
	
	char *ptr = NULL;
	int ret = 0 , fd = 0 , count = 0;
	char command[4][80];
	char str[80];       //Maximum size of command line is 80
	char arr[1024];

	char username[25]="Banthiya";
    char Password[25]="Simran12";
    
	ret=Authenticate(username,Password);
	
    if(ret==-1)
    {
        printf("\nERROR : Password Incorrect !!!\n");
    }
    
	else
   {
      printf("\nAuthentication is successful\n");

	InitialiseSuperBlock();
	CreateDILB();
	
	while(1)
	{
		fflush(stdin);
		strcpy_s(str,"");
		printf("Simran Banthiya VFS : > ");
		fgets(str,80,stdin);      // work as scanf

		count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);   // used to tokanise ie return the no.of tokens in this case it is 4

		if (count == 1)
		{
			if(_stricmp(command[0],"ls") == 0)
			{
				ls_file();
			}
			else if(_stricmp(command[0],"closeall") == 0)
			{
				CloseAllFile();
				printf("All files closed successfully\n");
				continue;
			}
			else if(_stricmp(command[0],"clear") == 0)
			{
				system("clear");
				continue;
			}
			else if(_stricmp(command[0],"help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if(_stricmp(command[0],"exit") == 0)
			{
				printf("Terminating the Marvellous Virtual File System\n");
				break;
			}
			else
			{
				printf("\nERROR : Command not found !!! \n");
				continue;
			}
		}

		else if(count == 2)
		{
			if(_stricmp(command[0],"stat") == 0)
			{
				ret = stat_file(command[1]);

				if(ret == -1)
				{
					printf("ERROR : Incorrect parameters\n");
				}
				if(ret == -2)
				{
					printf("ERROR : There is no such file\n");
				}
				continue;
			}

			else if(_stricmp(command[0],"fstat") == 0)
			{
				ret = fstat_file(atoi(command[1]));

				if(ret == -1)
				{
					printf("ERROR : Incorrect parameters\n");
				}
				if(ret == -2)
				{
					printf("ERROR : There is no such file\n");
				}
				continue;
			}

			else if(_stricmp(command[0],"close") == 0)
			{
				ret = CloseFileByName(command[1]);

				if(ret == -1)
				{
					printf("ERROR : There is no such file\n");
					continue;
				}
			}

			else if(_stricmp(command[0],"rm") == 0)
			{
				ret = rm_File(command[1]);

				if(ret == -1)
				{
					printf("ERROR : There is no such file\n");
					continue;
				}
			}

			else if(_stricmp(command[0],"man") == 0)
			{
				 man(command[1]);
			}

			else if(_stricmp(command[0],"write") == 0)
			{
				fd = GetFDFromName(command[1]);

				if(fd == -1)
				{
					printf("ERROR : Incorrect parametrs\n");
					continue;
				}

				printf("Enter the data\n");
				scanf("%[^\n]s",arr);

				ret = strlen(arr);

				if(ret == 0)
				{
					printf("ERROR : Incorrect parametrs\n");
					continue;
				}
				
				ret = WriteFile(fd,arr,ret);

				if(ret == -1)
				{
					printf("ERROR : Permission denied\n");
				}
				if(ret == -2)
				{
					printf("ERROR : There is no sufficient memory to write\n");
				}
				if(ret == -3)
				{
					printf("ERROR : It is not regular file\n");
				}
			}
			else if(_stricmp(command[0],"truncate") == 0)
			{
				ret = truncate_file(command[1]);

				if(ret == -1)
				
					printf("ERROR : Incorrect parametrs\n");
			}
				else
				{
					printf("Command not found\n");
					continue;
				}
			}

			else if(count == 3)
			{
				if(_stricmp(command[0],"create") == 0)
				{
					ret = CreateFileX(command[1],atoi(command[2]));
					if(ret >= 0)
					{
						printf("File is successfully created with file descriptor : %d\n",ret);
					}
					if(ret == -1)
					{
						printf("ERROR : Incorrect parameters\n");
					}
					if(ret == -2)
					{
						printf("ERROR : There is no nodes\n");
					}
					if(ret == -3)
					{
						printf("ERROR : File already exists\n");
					}
					if(ret == -4)
					{
						printf("ERROR : Memory allocation failure\n");
					}
					continue;
				}
					else if(_stricmp(command[0],"open") == 0)
					{
						ret = OpenFile(command[1],atoi(command[2]));
						if(ret >= 0)
						{
							printf("File is successfully created with file descriptor : %d\n",ret);
						}
						if(ret == -1)
						{
							printf("ERROR : Incorrect parameters\n");
						}
						if(ret == -2)
						{
							printf("ERROR : File not present\n");
						}
						if(ret == -3)
						{
							printf("ERROR : Permission denied\n");
						}
						continue;
					}

				else if(_stricmp(command[0],"read") == 0)
				{
					fd = GetFDFromName(command[1]);
					if(fd == -1)
					{
						printf("Incorrect parameters\n");
						continue;
					}

					ptr = (char *)malloc(sizeof(atoi(command[2]))+1);
					if(ptr == NULL)
					{
						printf("Memory allocation failure\n");
						continue;
					}

					ret = ReadFile(fd,ptr,atoi(command[2]));

					if(ret == -1)
					{
						printf("ERROR : File not exisiting\n");
					}
					if(ret == -2)
					{
						printf("ERROR : Permission denied\n");
					}
					if(ret == -3)
					{
						printf("ERROR : Reached at the end of file\n");
					}
					if(ret == -4)
					{
						printf("ERROR : It is not regular file\n");
					}
					if(ret == 0)
					{
						printf("File is empty\n");
					}
					if(ret > 0)
					{
						_write(2,ptr,ret);
					}
					continue;
				}
				else
				{
					printf("ERROR : Command not found\n");
					continue;
				}
		}

			else if(count == 4)
			{
				if(_stricmp(command[0],"lseek") == 0)
				{
					fd = GetFDFromName(command[1]);
					if(fd == -1)
					{
						printf("Incorrect parameters\n");
						continue;
					}
					
					ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));

					if(ret == -1)
					{
						printf("ERROR : Unable to perform lseek\n");
					}
				}
				else
				{
					printf("ERROR : Command not found !!!\n");
					continue;
				}
			}
			else
			{
				printf("ERROR : Command not found !!!\n");
				continue;
			}

		}//end of while
}	
	return 0;
}//end of main
