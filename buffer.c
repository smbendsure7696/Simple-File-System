#include"buffer.h"	//custom header file
#include<stdio.h>
#include<malloc.h>


BUF_H *freelist = NULL, *hashque[5] = {NULL};

void createPool(BUF_H **first,int size)
{
   register int i;
   BUF_H *temp = NULL;
   
   for(i=0;i<size;i++)
   {
      BUF_H *newBuf = NULL; 
      newBuf= (BUF_H *)calloc(sizeof(BUF_H),1);
      
      if(newBuf==NULL)
      {
         printf("\n\tUnable to create Buffer");
         return;
      }
      
      newBuf->ptr_data_area = (char *) malloc(sizeof(char)*10);
      newBuf->ptr_data_area = "SHUBHAM";
      newBuf->test = i;
      
      if(*first==NULL)
      {
         *first = newBuf;
         temp = newBuf;
         newBuf->free_next = newBuf;
         newBuf->free_prev = newBuf;
         
         newBuf->hash_next = newBuf;
         newBuf->hash_prev = newBuf;
      }
      else
      {
         temp->free_next = newBuf;
         newBuf->free_next = *first;
         newBuf->free_prev = temp;
         (*first)->free_prev = newBuf;
         
         temp->hash_next = newBuf;
         newBuf->hash_next = *first;
         newBuf->hash_prev = temp;
         (*first)->hash_prev = newBuf;
         
         temp = temp->free_next;     
      }
        
   }
}


//Function to display FreeList

void displayFreeList(BUF_H *first)
{
   BUF_H *temp = NULL;
   temp = first;
   
   if(first==NULL)
   {
      printf("\n\tNo Free Buffers...");
      return;
   }
   
   while(temp->free_next != first)
   {
      printf("    %d",temp->test);
      temp = temp->free_next;
   }
   printf("    %d",temp->test);
   temp = NULL;
   
}


//Function to display HashQueue

void displayHashQueue(BUF_H *first)
{
   BUF_H *temp = NULL;
   temp = first;
   
   if(first==NULL)
   {
      printf("\n\tNo Free Buffers...");
      return;
   }
   
   while(temp->hash_next != first)
   {
      printf("    %d",temp->test);
      temp = temp->hash_next;
   }
   printf("    %d",temp->test);
   temp = NULL;
   
}
