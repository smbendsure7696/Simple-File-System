#include"buffer.h"
#include<stdio.h>

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


BUF_H* bread(int blkno)
{
   BUF_H *temp;
   int devno = getDeviceNumber(blkno);
   temp = getblk(devno,blkno);
   
   if(isBufferValid(temp)==false)
   {
      temp->device_number = devno;
      temp->blk_number = blkno;
   }
   
   return temp;   
}
