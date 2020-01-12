/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Fall 2017                     */
/*                                                                   */
/* Author: Tim Schmidt                                               */
/* Date: 11/09/2017                                                  */
/*                                                                   */
/* MovieLab.c: source file for the main function                     */
/*                                                                   */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height);

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();

int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
  int framenumber = 0;
  char *watermarkfile = NULL;
  char opcode[10] = "0000000000";
	unsigned int width = 480;
	unsigned int height = 270;
  int operation = 0;
  int cropstart, cropend, fastfactor, watermarkX, watermarkY, watermarkcounter, spotlightradius, spotlightcounter, zoomcheck, zoompercentage;
  IENTRY *e;
  IMAGE *watermarkimage;
  int newlength;
  MOVIE *movie;
	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

    if (strcmp(argv[x], "-f") == 0) {
			if (x < argc - 1) {
				framenumber = atoi(argv[x + 1]);
			} /*fi*/
			else {
				printf("Missing argument for the frame number!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
   
    if (strcmp(argv[x], "-s") == 0) {
      if (x < argc - 1) {
  				if (sscanf(argv[x+1], "%ux%u", &width, &height) == 2) { 
            /* input is correct */ /* the image width is stored in width */ /* the image height is stored in height */ 
          } else { 
            printf("Input format error!\n");
          }
      } else {
  				printf("Missing argument for the width and height!\n");
  				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
   
    if (strcmp(argv[x], "-aging") == 0) {
      operation = 1;
      opcode[0] = '1';
			x += 1;
			continue;
		} /*fi*/   
   
    if (strcmp(argv[x], "-hflip") == 0) {
      operation = 2;
      opcode[1] = '1';
			x += 1;
			continue;
		} /*fi*/  
   
    if (strcmp(argv[x], "-edge") == 0) {
      operation = 3;
      opcode[2] = '1';
			x += 1;
			continue;
		} /*fi*/      

    if (strcmp(argv[x], "-crop") == 0) {
      if (x < argc - 1) {
  				if (sscanf(argv[x+1], "%u-%u", &cropstart, &cropend) == 2) { 
            operation = 4;
            opcode[3] = '1';
          } else { 
            printf("Input format error!\n");
          }
      } else {
  				printf("Missing argument for the crop operation!\n");
  				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
   
    if (strcmp(argv[x], "-fast") == 0) {
      if (x < argc - 1) {
				fastfactor = atoi(argv[x + 1]);
        operation = 5;
        opcode[4] = '1';
      } else {
  				printf("Missing argument for the fast operation!\n");
  				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/
   
    if (strcmp(argv[x], "-reverse") == 0) {
      operation = 6;
      opcode[5] = '1';
			x += 1;
			continue;
		} /*fi*/     

    if (strcmp(argv[x], "-watermark") == 0) {
      if (x < argc - 1) {
				watermarkfile = argv[x + 1];
        operation = 7;
        opcode[6] = '1';
			} else {
  				printf("Missing argument for the watermark operation!\n");
  				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

    if (strcmp(argv[x], "-spotlight") == 0) {
      operation = 8;
      opcode[7] = '1';
			x += 1;
			continue;
		} /*fi*/

    if (strcmp(argv[x], "-zoom") == 0) {
      operation = 9;
      opcode[8] = '1';
			x += 1;
			continue;
		} /*fi*/       
   
		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/
		if (strcmp(argv[x], "-test") == 0) {
        movie = LoadMovie(fin, framenumber, width, height);
        printf("The movie file %s was loaded successfully. \n", fin);
        YUV2RGBMovie(movie);
        RGB2YUVMovie(movie);
        SaveMovie(fout, movie);
        printf("The movie file %s was saved successfully. \n", fout);
			return 0;
		}
		x++;
	  } /*elihw*/

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}

	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}
  if (operation >= 1 && operation <= 9){
      if(fin == NULL || fout == NULL || framenumber == 0 || width == 0 || height == 0){
          printf("Missing necessary command lines for movie operations!\n");
          return 5;
      } else {
      
        movie = LoadMovie(fin, framenumber, width, height);
        printf("The movie file %s was loaded successfully. \n", fin);
        YUV2RGBMovie(movie);  
             if(opcode[0]=='1'){
                            e = movie->Frames->First;  
                            while(e){ 
                                e->RGBImage = Aging(e->RGBImage);
                                e = e->Next;
                            }    
                            printf("\"Aging\" operation is done!\n");
                            }
    	       if(opcode[1]=='1'){
                            e = movie->Frames->First;  
                            while(e){ 
                                e->RGBImage = HFlip(e->RGBImage);
                                e = e->Next;
                            }    
                            printf("\"HFlip\" operation is done!\n");
                            }
    	       if(opcode[2]=='1'){   
                            e = movie->Frames->First;  
                            while(e){ 
                                e->RGBImage = Edge(e->RGBImage);
                                e = e->Next;
                            }    
                            printf("\"Edge\" operation is done!\n");
                            }
             if(opcode[4]=='1'){
                            FastImageList(movie->Frames, fastfactor);
                            printf("\"Fast\" operation is done!\n");
                            newlength = movie->Frames->Length;
                            printf("New number of frames is %d \n", newlength);
    	       if(opcode[3]=='1'){ 
                            CropImageList(movie->Frames, cropstart, cropend);     
                            printf("\"Crop\" operation is done!\n");
                            }
                            }
             if(opcode[5]=='1'){
                            ReverseImageList(movie->Frames);
                            printf("\"Reverse\" operation is done!\n");
                            }
             if(opcode[6]=='1'){
                            e = movie->Frames->First; 
                            watermarkimage = LoadImage(watermarkfile);
                            srand(time(NULL));
                            watermarkcounter = 0;
                            while(e){ 
                                if(watermarkcounter%15==0){
                                    watermarkX = rand() % width;
                                    watermarkY = rand() % height;
                                }           
                                watermarkcounter ++;                            
                                e->RGBImage = Watermark(e->RGBImage, watermarkimage, watermarkX, watermarkY);
                                e = e->Next;
                            }
                            printf("\"Watermark\" operation is done!\n");
                            }
    			   if(opcode[7]=='1'){
                            e = movie->Frames->First;     
                            spotlightcounter = 0;
                            while(spotlightcounter < 40){ 
                                spotlightradius = sqrt(width*width + height*height)/40*spotlightcounter;
                                e->RGBImage = Spotlight(e->RGBImage, width/2, height/2, spotlightradius);
                                e = e->Next;
                                spotlightcounter++;
                            } 
                            while(spotlightcounter < movie->Frames->Length-20){
                                e = e->Next;
                                spotlightcounter++;
                            }
                            while(spotlightcounter < movie->Frames->Length){ 
                                spotlightradius = sqrt(width*width + height*height)/20*(movie->Frames->Length - spotlightcounter);
                                e->RGBImage = Spotlight(e->RGBImage, width/2, height/2, spotlightradius);
                                e = e->Next;
                                spotlightcounter++;
                            } 
                            printf("\"Spotlight\" operation is done!\n");
                            }
    			   if(opcode[8]=='1'){
                            e = movie->Frames->First; 
                            zoomcheck = 0;
                            zoompercentage = 0;
                            while(e){ 
                                if(zoompercentage==100){zoomcheck = 1;}
                                if(zoompercentage==0){zoomcheck = 0;}

                                e->RGBImage = Zoom(e->RGBImage, zoompercentage);
                                e = e->Next;
                                if(zoomcheck==0){
                                    zoompercentage += 2;
                                }else if(zoomcheck==1){
                                    zoompercentage -= 2;
                                }
                            }    
                            printf("\"Zoom\" operation is done!\n");
                            }
      RGB2YUVMovie(movie);
      SaveMovie(fout, movie);
      printf("The movie file %s was saved successfully. \n", fout);
      DeleteMovie(movie);
  }
}
  fin = NULL;
	fout = NULL;

	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> -s <WIDTHxHEIGHT> [options]\n"
	       "Options:\n"
	       "-aging                Activate the aging filter on every movie frame\n"
	       "-hflip                Activate horizontal flip on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-zoom                 Zoom in and out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVIMAGE* LoadOneFrame(const char* fname, int n,
                       unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVIMAGE* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
MOVIE *LoadMovie(const char *fname, int frameNum,
                 unsigned int width, unsigned height)
{ 
    int i;
    MOVIE *movie = CreateMovie();
    for(i=0; i<frameNum; i++){
        AppendYUVImage(movie->Frames, LoadOneFrame(fname, i, width, height));
    }
    return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{   
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}
	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->YUVImage, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVIMAGE *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
