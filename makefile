CC=gcc
CFLAGS=-I./include
OUTFILE=dob2bmp
SRC=./src/
all:
	$(CC) $(SRC)*.c -o $(OUTFILE) $(CFLAGS)

