#include"buffer.h"
#include<stdio.h>

BUF_H *bfreeList,*hashque[5];

//Function to check Buffer in HashQueue
BUF_H* inHash(int devno,int blkno)
{
   BUF_H *temp = hashque[devno];
   
   if(temp == NULL)
      return 0;
   
   for(temp=hashque[devno]; temp->hash_next != hashque[devno]; temp=temp->hash_next)
   {
      if(temp->device_number==devno && temp->blk_number==blkno)
         return temp;   
   }
   
   if(temp->device_number==devno && temp->blk_number==blkno)
         return temp;   
}


//Function to request get Buffer
BUF_H* getblk(int devno,int blkno)
{
   BUF_H *bp = NULL;
   int old;
   devno  = (devno%4)+1;
   
   while(1)
   {
      bp = inHash(devno,blkno);
      if(bp!=0)
      {
         if(bp->blk_status & B_BUSY)
         {
            printf("\n\t(%d,%d)buffer is Busy",devno,blkno);
            //continue;
            break;
         }
         else
         {
            bp->blk_status = bp->blk_status & B_BUSY;
            if(bp->free_next==bp && bp->free_prev==bp)
            {
               bfreeList = NULL;
            }
            else
            {
               bp->free_prev->free_next = bp->free_next;
				   bp->free_next->free_prev = bp->free_prev;
				   //bfreelist=bp->free_next;  //remove comment after brelese() coding
					       
				   bp->free_next=bp;
				   bp->free_prev=bp;
            }
            printf("\n(%d,%d)returning from hash que",devno,blkno);
				return(bp);
         }
      }
      else //if not in hash Queue 
      {
         bp = bfreeList;
         if(bp == NULL)
         {
            printf("\n\tEmpty FreeList");
            //continue;
            return NULL;
         }
         if(bp->blk_status & B_DWRITE)
         {
            printf("\nBuffer is Delayed Write");
            break;
         }
         
         old = bp->device_number;
         bp->blk_status |= B_BUSY;
         bp->device_number = devno;
         bp->blk_number = blkno;
         
         if(bp->free_next==bp && bp->free_prev==bp)
			{
				bfreeList=NULL;
				printf("!");
			}
			else
			{
				bp->free_prev->free_next = bp->free_next;
				bp->free_next->free_prev = bp->free_prev;
					  
				bfreeList=bp->free_next;
							//    display(bp);
				bp->free_next=bp;
				bp->free_prev=bp;
			}
			//from old hash to new hash
			
			if(bp->hash_next==bp && bp->hash_prev==bp)
			{   
				  printf("\n -------    - - - %d",old);
				hashque[old]=NULL;      
			}
			
			else
			{
				bp->hash_prev->hash_next = bp->hash_next;
				bp->hash_next->hash_prev = bp->hash_prev;
			}
			
			if(hashque[devno] == NULL)
			{
				hashque[devno]=bp;
				bp->hash_next=bp;//hashque[dev];
				bp->hash_prev= bp; //hashque[dev];
			}

			else
			{
				bp->hash_next=hashque[devno];
				bp->hash_prev= hashque[devno]->hash_prev;

				hashque[devno]->hash_prev=bp;
				bp->hash_prev->hash_next = bp;

				hashque[devno]=bp;

				hashque[devno]=bp;
			}
			
			
			printf("\nreturnig from freelist ");
			return(bp);
               
      }   
   } //end of while
}
 
