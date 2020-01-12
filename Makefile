#######################################################################
# Homework Assignment 5, for EECS 22, Fall 2017
#
# Author: Tim Schmidt
# Date: 11/09/2017
#
# Makefile: Makefile for HW 5
#
#######################################################################

# Variables
LIBS   = libFilter.a libFileIO.a

CC     = gcc
DEBUG  = -DDEBUG
CFLAGS = -ansi -Wall -c
LFLAGS = -ansi -Wall
AR     = ar rc
RANLIB = ranlib

MOVIES = aging.yuv \
         hflip.yuv \
         edge.yuv \
         crop.yuv \
         fast.yuv \
         reverse.yuv  \
         watermark.yuv \
         spotlight.yuv \
         zoom.yuv 
         
# Default target
all: MovieLab 

######################### Generate object files #######################
# Target for MovieLab.o
MovieLab.o: MovieLab.c Image.h FileIO.h DIPs.h ImageList.h Movie.h Constants.h
	$(CC) $(CFLAGS) MovieLab.c -o MovieLab.o -lm #-lm is to link Math Library to use power function
# Target for FileIO.o
FileIO.o: FileIO.h FileIO.c Constants.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o
# Target for DIPs.o
DIPs.o: DIPs.h DIPs.c Constants.h
	$(CC) $(CFLAGS)  DIPs.c -o DIPs.o
# Target for Image.o
Image.o: Image.h Image.c Constants.h
	$(CC) $(CFLAGS) Image.c -o Image.o
# Target for ImageList.o
ImageList.o: ImageList.h ImageList.c Constants.h
	$(CC) $(CFLAGS) ImageList.c -o ImageList.o
# Target for Movie.o
Movie.o: Movie.h Movie.c Constants.h
	$(CC) $(CFLAGS) Movie.c -o Movie.o 

#target to generate libFilter.a
libFilter.a: DIPs.o 
	$(AR) libFilter.a DIPs.o
	$(RANLIB) libFilter.a

########### generate executables ###########

#target to generate MovieLab
MovieLab: MovieLab.o FileIO.o Image.o ImageList.o Movie.o libFilter.a
	$(CC) $(LFLAGS) MovieLab.o FileIO.o Image.o ImageList.o Movie.o -L. -lFilter -o MovieLab  -lm 
 

	
#target to clean the directory
clean:
	rm -f *.o $(DESIGN) $(MOVIES) $(LIBS)

