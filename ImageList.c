
/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* ImageList.c: source file for image list manipulations             */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ImageList.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
	ILIST *list = (ILIST *)malloc(sizeof(ILIST));
  if(list == NULL){
       perror("Out of memory! Aborting...");
       exit(10);
  }
  list->Length = 0;
  list->First = NULL;
  list->Last = NULL;
  return list;
}

/* Allocate a new image entry*/
IENTRY *NewImageEntry(IMAGE *i){ 
  IENTRY *e;
  e = malloc(sizeof(IENTRY));
  if (! e){ 
    perror("Out of memory! Aborting...");
    exit(10);
  } /* fi */
  e->List = NULL;
  e->Next = NULL;
  e->Prev = NULL;
  e->RGBImage = i;
  e->YUVImage = NULL;
  return e;
}

/* Allocate a new image entry*/
IENTRY *NewYUVImageEntry(YUVIMAGE *YUVi){ 
  IENTRY *e;
  e = malloc(sizeof(IENTRY));
  if (! e){ 
    perror("Out of memory! Aborting...");
    exit(10);
  } /* fi */
  e->List = NULL;
  e->Next = NULL;
  e->Prev = NULL;
  e->RGBImage = NULL;
  e->YUVImage = YUVi;
  return e;
}

/* delete a image entry */
void DeleteImageEntry(IENTRY *e){ 
  assert(e);
  assert(e->List);
  if(e->RGBImage){assert(e->RGBImage); DeleteImage(e->RGBImage);}
  free(e);
  e->List = NULL;
  e->Prev = NULL;
  e->Next = NULL;
} 

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{       
        IENTRY *e, *n;
        e = list->First;
        while(e){ 
            n = e->Next;
            DeleteImageEntry(e);
            e = n;
        }
        assert(list);
        assert(list->First);
        assert(list->Last);
        
        free(list->First);
        free(list->Last);
        free(list);
}


/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, IMAGE *RGBimage)
{
    IENTRY *e = NULL;
    assert(list);
    assert(RGBimage);
    e = NewImageEntry(RGBimage);
    if (list->Last){ 
      e->List = list;
      e->Next = NULL;
      e->Prev = list->Last;
      e->RGBImage = RGBimage;
      list->Last->Next = e;
      list->Last = e;
    }
    else
    { e->List = list;
      e->Next = NULL;
      e->Prev = NULL;
      list->First = e;
      list->Last = e;
    }
    list->Length++;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVIMAGE *YUVimage)
{
    IENTRY *e = NULL;
    assert(list);
    assert(YUVimage);
    e = NewYUVImageEntry(YUVimage);
    if (list->Last){ 
      e->List = list;
      e->Next = NULL;
      e->Prev = list->Last;
      e->YUVImage = YUVimage;
      list->Last->Next = e;
      list->Last = e;
    }
    else
    { e->List = list;
      e->Next = NULL;
      e->Prev = NULL;
      list->First = e;
      list->Last = e;
    }
    list->Length++;
}


/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{ 
     IENTRY *e;
     IENTRY *n;
     e = list->First; 
     int counter = 0;
     while(counter < start){
         n = e->Next;
         DeleteImageEntry(e);
         e = n;
         counter ++;
     }
     list->First = e;
     list->First->Prev = NULL;
     while(counter < end){
         n = e->Next;
         e = n;
         counter ++;
     }
     list->Last = e;
     while(counter < list->Length-1){
         n = e->Next;   
         /*DeleteImageEntry(e);*/   
         e = n;
         counter ++;
     }
     list->Last->Next = NULL;
     list->Length = end - start;
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
   IENTRY *e = list->First;
   IENTRY *n = list->First->Next;
   IENTRY *c = NULL;
   int counter = 0;
   while(e){
       if(counter%factor==0){        
           if(c){c->Next = e;}
           e->Prev = c; 
           c = e;
           e = e->Next;
       }
       if(counter%factor!=0){
           n = e->Next;
           /*DeleteImageEntry(e);*/
           e = n;
       }
       counter ++;
   } 
   list->Length = (list->Length-1)/3+2;
   list->Last->Next = NULL;
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
    IENTRY *l;
    IENTRY *f;
    l = list->Last;
    f = list->First;
    list->Last = f;
    list->First = l;
    IENTRY *e;
    IENTRY *n;
    e = list->First;
    while(e){
      n = e->Prev;
      e->Prev = e->Next;
      e->Next = n; 
      e = n;
    }
    list->Last->Next = NULL;
    list->First->Prev = NULL;
}

/* EOF */