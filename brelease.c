#include"buffer.h"
#include<stdio.h>

extern BUF_H *bfreelist,*hashque[5];

void insert(BUF_H *bp)
{ 
	if(bfreelist==NULL)
	   bfreelist=bp;
    
	else
	{
	  bp->free_next=bfreelist;
	  bfreelist->free_prev->free_next=bp;
	  bp->free_prev=bfreelist->free_prev;
	  bfreelist->free_prev=bp;

	}
}


void brelease(BUF_H *bp)
{    
	if(bp==NULL)
		return;
	/*
     printf("\n waking up all the process.....");
	 printf("\n waking up all the process waiting for this buffer....");
	 printf("\n incresing cpu execution level to disk int");
	 */

	 if(bp->status & B_DWRITE)
	 {
		printf("\n --  insert at end");
		insert(bp);
	 }
	 else if(bp->status & B_OLD)
	 {
		printf("\n --  insert at start");
		insert(bp);
		bfreelist=bp; //insertion at the first
	 }

	 bp->status &= ~(B_BUSY | B_DWRITE | B_OLD);
}
