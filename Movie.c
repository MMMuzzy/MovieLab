/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* Movie.c: source file for basic movie manipulations                */
/*                                                                   */
/*********************************************************************/
#include <stdlib.h>
#include <assert.h>
#include "Movie.h"


/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
MOVIE *CreateMovie(void)
{
  MOVIE *movie = (MOVIE *)malloc(sizeof(MOVIE));
  ILIST *frames = CreateImageList();
  movie->Frames = frames;
  return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
  assert(movie);
  DeleteImageList(movie->Frames);
  movie->Frames = NULL;
  free(movie);
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(MOVIE *movie)
{ 
  int x,y;
  int Y,U,V,R,G,B,C,D,E;
  IENTRY *e;
  e = movie->Frames->First;
  while(e){ 
    e->RGBImage = CreateImage(e->YUVImage->W, e->YUVImage->H);
     	for (x = 0; x < e->YUVImage->W; x++) {
		      for (y = 0; y < e->YUVImage->H; y++) {
        			Y = GetPixelY(e->YUVImage, x, y);
    			    U = GetPixelU(e->YUVImage, x, y);
          	  V = GetPixelV(e->YUVImage, x, y);
              C = Y - 16; D = U - 128; E = V - 128;
              R = ( 298 * C + 409 * E + 128) >> 8;
              G = ( 298 * C - 100 * D - 208 * E + 128) >> 8 ;
              B = ( 298 * C + 516 * D + 128) >> 8;
              if (R < 0){R = 0;} if (R > 255){R = 255;}
              if (G < 0){G = 0;} if (G > 255){G = 255;}
              if (B < 0){B = 0;} if (B > 255){B = 255;}
        			SetPixelR(e->RGBImage, x, y, R);
        			SetPixelG(e->RGBImage, x, y, G);
              SetPixelB(e->RGBImage, x, y, B);
		      }
      }
      /*DeleteYUVImage(e->YUVImage);
      e->YUVImage = NULL;*/
      e = e->Next;
  }
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(MOVIE *movie)
{
  int x,y;
  int Y,U,V,R,G,B;
  IENTRY *e;
  e = movie->Frames->First;
  while(e){ 
      e->YUVImage = CreateYUVImage(e->YUVImage->W, e->YUVImage->H);
     	for (x = 0; x < e->RGBImage->W; x++) {
		      for (y = 0; y < e->RGBImage->H; y++) {
        			R = GetPixelR(e->RGBImage, x, y);
    			    G = GetPixelG(e->RGBImage, x, y);
          	  B = GetPixelB(e->RGBImage, x, y);
           		Y = ( ( 66 * R + 129 * G + 25 * B + 128) >> 8) + 16 ;
           		U = ( ( -38 * R - 74 * G + 112 * B + 128) >> 8) + 128 ;
           		V =  ( ( 112 * R - 94 * G - 18 * B + 128) >> 8) + 128 ;
              if (Y < 0){Y = 0;} if (Y > 255){Y = 255;}
              if (U < 0){U = 0;} if (U > 255){U = 255;}
              if (V < 0){V = 0;} if (V > 255){V = 255;}
        			SetPixelY(e->YUVImage, x, y, Y);
        			SetPixelU(e->YUVImage, x, y, U);
              SetPixelV(e->YUVImage, x, y, V);
		      }
      }
      /*DeleteImage(e->RGBImage); 
      e->RGBImage = NULL;*/
      e = e->Next;
  }
}


/* EOF */
