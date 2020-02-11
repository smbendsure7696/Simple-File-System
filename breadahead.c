//breada ahead algorithm

#include"buffer.h"
#include<stdio.h>

BUF_H *bfreeList;

BUF_H* bufferPool(int blkno)
{
   BUF_H *temp = bfreeList;
   
   if(temp==NULL)
   {
      printf("\n\tNo Free Buffers...");
      return;
   }
   
   while(temp->free_next != first)
   {
      if(temp->blk_number == blkno)
      {   
         return temp;   
         break;            
      }   
      temp = temp->free_next;
   }
   return NULL;
}

int getDeviceNumber(int blkno)
{
   BUF_H *temp = bfreeList;
   
   if(temp==NULL)
   {
      printf("\n\tNo Free Buffers...");
      return;
   }
   
   while(temp->free_next != first)
   {
      if(temp->blk_number == blkno)
      {   
         return temp->device_number;   
         break;            
      }   
      temp = temp->free_next;
   }
   return 0;
}


bool isBufferValid(BUF_H *buf,devno,blkno)
{
   if(buf->device_number==devno && buf->blk_number==blkno)
      return true;
   else
      return false;   
}


BUF_H* breada(int blkno1,int blkno2)
{
   BUF_H *temp;
   BUF_H *bp = NULL, *bp2 = NULL;
   bp = bufferPool(blkno1);
   bp2 = bufferPool(blkno2);
   int devno1,devno2;
   
   if(bp==NULL)
   {
      devno1 = getDeviceNumber(blkno1);
      temp = getblk(devno1,blkno1);
      if(isBufferValid(temp)==false)
      {
         temp->device_number = devno1;
         temp->blk_number = blkno1;
      }  
   }
   else
   {
      temp = bread(blkno1);
      return temp;
   }
   if(bp2==NULL)
   {
      devno2 = getDeviceNumber(blkno2);
      bp2 = getblk(devno2,blkno2);
      if(isBufferValid(bp2)== false)
      {
         temp->device_number = devno2;
         temp->blk_number = blkno2;
      }
      else
      {
         brelease(bp2);
      }                       
   }
   
   return temp;
}
