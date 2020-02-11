#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
//#include<iostream>
#define MAXINODE 50
#define READ 1
#define WRITE 2
#define MAXFILESIZE 2048
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;
typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int permission;
	struct inode *next;
}INODE,*PINODE,**PPINODE;
typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE,*PFILETABLE;
typedef struct ufdt
{
	PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;
int d;
void man(char *name)
{
	if(name == NULL) return;
	if(strcmp(name,"create") == 0)
	{
		printf("Description : Used to create new regular file\n");
		printf("Usage : create File_name Permission\n");
	}
	else if(strcmp(name,"read") == 0)
	{
	   system("cat");
	}
	else if(strcmp(name,"write") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"ls") == 0)
	{
	   system("ls");
	}
	else if(strcmp(name,"stat") == 0)
	{
	   system("stat");
	}
	else if(strcmp(name,"fstat") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"truncate") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"open") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"close") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"closeall") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"lseek") == 0)
	{
	// Add description
	}
	else if(strcmp(name,"rm") == 0)
	{
	// Add description
	}
	else
	{
		printf("ERROR : No manual entry available.\n");
	}
}



void DisplayHelp()
{
	printf("ls : To List out all files\n");
	printf("clear : To clear console\n");
	printf("open : _______________________\n");
	printf("close : _______________________\n");
	printf("closeall : _______________________\n");
	printf("read : _______________________\n");
	printf("write :_______________________\n");
	printf("exit : _______________________\n");
	printf("stat : _______________________\n");
	printf("fstat :_______________________\n");
	printf("truncate : _______________________\n");
	printf("rm : _______________________\n");
}

int GetFDFromName(char *name)
{
	int i = 0;
	while(i<50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name)==0)
			break;
		i++;
	}
	if(i == 50) return -1;
	else return i;
}
int GetFD()
{
   int i = 0;
   while(i<50)
   {
      if(UFDTArr[i].ptrfiletable == NULL)
         break;
      i++;   
   }
   if(i==50)
      return -4;
   else
      return i;   
}

PINODE Get_Inode(char * name)
{
	PINODE temp = head;
	int i = 0;
	if(name == NULL)
	return NULL;
	while(temp!= NULL)
	{
   	if(strcmp(name,temp->FileName) == 0)
      	break;
   	temp = temp->next;
	}
	return temp;
}

void append(int n)
{
	INODE* temp;
	temp = (struct inode*)malloc(sizeof(struct inode));
	temp->InodeNumber = n;
	temp->ReferenceCount = 0;
	temp->LinkCount = 0;
	temp->FileSize = 0;
	temp->FileActualSize = 0;
	temp->FileType = 0;
	temp->Buffer = NULL;
	temp->next = NULL;
	
	if(head==NULL)
	{
		head = temp;
	}
	else
	{
		INODE* p;
		p = head;
		while(p->next != NULL)
		{
			p = p->next;
		}
		p->next = temp;
	}

}
void CreateDILB()
{
	// Create singly linked list of MAXINODE (50) inodes.
	// And store address of first node into head pointer which is global.
	// LinkCount, ReferenceCount , FileType, FileSize members should be set to 0
	// Buffer member should be set to NULL;
	// In InodeNumber member store the value from 1 to MAXINODE(50) which indicates node number
   int n = 0;
   INODE *temp,*nd;
   while(n<=MAXINODE)
   {
      append(n+1);
      n++;
   }
 
}

void InitialiseSuperBlock()
{
	int i=0;
   for(i=0;i<50;i++)
   {
      UFDTArr[i].ptrfiletable = NULL;  
   }
   
   SUPERBLOCKobj.TotalInodes = 50;
   SUPERBLOCKobj.FreeInode = 50;
}

INODE* iGet()
{
   INODE *node = head;
   while(node->next!=NULL)
   {
      if(node->ReferenceCount==0 && node->LinkCount==0)
      {  
         return node;
         break;
      }
      node = node->next;      
   } 
}

long int fileAsize(char *name)
{
   FILE *fp = fopen(name,"r");
   if(fp==NULL)
      return -1;
   fseek(fp,0L,SEEK_END);
   long int s = ftell(fp);
   fclose(fp);
   return s;
}

int checkName(char *name)
{
   INODE *temp = head;
   while(temp->next!=NULL)
   {
      if(strcmp(temp->FileName,name)==0)
      {   return -3;
         break;
      }
      temp = temp->next;   
   }
}

int CreateFile(char *name,int permission)
{
// Check input parameters
// Decrement free node count from superblock
// Find out empty node from Inode linked list
// Find out empty entry from UFDTArr
// Allocate memory for FileTable
// Initialise file table entries
// Allocate members of Inode
// Allocate memory for storing data of file (1024) bytes and store its address in Buffer pointer
// Return the index of UFDTArr in which address of FileTable is stored.
   int r = checkName(name);
   int d = GetFD(); 
   INODE *node = iGet();  
   if(r==-3)
   {
      return -3;
   }
   else if(d==-4)
   {
      return -4;
   }
   else if(node == NULL)
   {
      return -2;
   }
   else
   {
      int fd = creat(name,permission);
      if(fd == -1)
      {  
         return -1;
      }
      else
      {
         FILETABLE *Ft = (struct filetable*)malloc(sizeof(struct filetable));
         strcpy(node->FileName,name);
         node->FileSize = MAXFILESIZE;
         node->FileActualSize = fileAsize(name);
         node->FileType = REGULAR;
         node->Buffer="NULL";
         node->LinkCount = 1;
	      node->ReferenceCount = 1;
	      node->permission = permission;
	      Ft->ptrinode = node;
	      UFDTArr[d].ptrfiletable = Ft;
	      SUPERBLOCKobj.FreeInode -=1;
	      printf("\n\tInode Number: %d\n",node->InodeNumber);
	      return d;
	   }
   }
}

int Remove(char *name)
{
   int r = checkName(name);
   if(r!=-3)
   {
      printf("\n\t%s File is Not Present...",name);
   }
   else
   {
      struct inode *node = Get_Inode(name);
      if(node == NULL)
         return -1;
      else
      {
         int fd = GetFDFromName(name);
         printf("\n\tFD %d",fd);
         int status = remove(name);
         if(status==0)
         {
            UFDTArr[fd].ptrfiletable->ptrinode = NULL;
            printf("\n\tFile Name: %s",node->FileName);
            strcpy(node->FileName,"NULL");
            node->FileSize = 0;
            node->FileActualSize = 0;
            node->FileType = 0;
            node->Buffer=NULL;
            node->LinkCount = 0;
	         node->ReferenceCount = 0;
	         node->permission = 0;
	         SUPERBLOCKobj.FreeInode +=1;
            return 1;
         }
      }   
   }
}

void ls_file()
{
   struct inode *node = head;
   while(node->ReferenceCount!=0 && node->LinkCount!=0 && node->next!=NULL)
   {
      printf("\n\tFile Name: %s",node->FileName);
      node = node->next;
   }
}

int fstat_file(int fd)
{
// Display all information of file from file descriptor.
// From FD access FileTable
// From File table access inode
// Display information from the inode
   
   
}
int stat_file(char *name)
{
// Display all information of file from file name.
// From file name search specific inode from linked list
      INODE *temp = Get_Inode(name);
      if(temp==NULL)
         return -2;
      else
      {        
         printf("\n\tFile Name: %s",temp->FileName);
         printf("\n\tFile Inode Number: %d",temp->InodeNumber);
         printf("\n\tFile Type: %d",temp->FileType);
         printf("\n\tFile Size: %d",temp->FileSize);
         printf("\n\tFile Actual Size: %d",temp->FileActualSize);
         printf("\n\tFile Permission: %d",temp->permission);
         return 1;
      }
}

int main()
{
	char *ptr = NULL;
	int ret = 0, fd = 0, count = 0;
	char command[4][80], str[80], arr[1024];
	InitialiseSuperBlock();
	CreateDILB();
	while(1)
	{
		fflush(stdin);
		strcpy(str,"");
		printf("\nVFS : > ");
		fgets(str,80,stdin);
		count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
		if(count == 1)
		{
			if(strcmp(command[0],"ls") == 0)
			{
				ls_file();
			}
			else if(strcmp(command[0],"closeall") == 0)
			{
			}
			else if(strcmp(command[0],"clear") == 0)
			{
				system("clear");
				continue;
			}
			else if(strcmp(command[0],"help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if(strcmp(command[0],"exit") == 0)
			{
				printf("Terminating the Virtual File System\n");
				break;
			}
			else
			{
				printf("\nERROR : Command not found !!!\n");
				continue;
			}
		}
		else if(count == 2)
		{
			if(strcmp(command[0],"stat") == 0)
			{
			ret = stat_file(command[1]);
			if(ret == -1)
			printf("ERROR : Incorrect parameters\n");
			if(ret == -2)
			printf("ERROR : There is no such file\n");
			continue;
			}
			else if(strcmp(command[0],"fstat") == 0)
			{
			ret = fstat_file(atoi(command[1]));
			if(ret == -1)
			printf("ERROR : Incorrect parameters\n");
			if(ret == -2)
			printf("ERROR : There is no such file\n");
			continue;
			}
			else if(strcmp(command[0],"close") == 0)
			{
			
			}
			else if(strcmp(command[0],"rm") == 0)
			{
			   int r = Remove(command[1]);
			   if(r==-1)
			      printf("\n\tUnable to Delete...");
			   else
			      printf("\n\tSuccessfully Removed...");
			   continue;      
			}
			else if(strcmp(command[0],"man") == 0)
			{
			   man(command[1]);
			}
			else if(strcmp(command[0],"write") == 0)
			{
			}
			else if(strcmp(command[0],"truncate") == 0)
			{
			}
			else
			{
			printf("\nERROR : Command not found !!!\n");
			continue;
			}
		}
		else if(count == 3)
		{
			if(strcmp(command[0],"create") == 0)
			{
				ret = CreateFile(command[1],atoi(command[2]));
				if(ret >= 0)
				printf("File is successfully created with file descriptor : %d\n",ret);
				if(ret == -1)
				printf("ERROR : Incorrect parameters\n");
				if(ret == -2)
				printf("ERROR : There is no inodes\n");
				if(ret == -3)
				printf("ERROR : File already exists\n");
				if(ret == -4)
				printf("ERROR : Memory allocation failure\n");
				continue;
			}
			else if(strcmp(command[0],"open") == 0)
			{
			}
			else if(strcmp(command[0],"read") == 0)
			{
			}
			else
			{
			printf("\nERROR : Command not found !!!\n");
			continue;
			}
		}
		else if(count == 4)
		{
			if(strcmp(command[0],"lseek") == 0)
			{
			}
			else
			{
			printf("\nERROR : Command not found !!!\n");
			continue;
			}
			}
			else
			{
			printf("\nERROR : Command not found !!!\n");
			continue;
			}
		}
return 0;
}
