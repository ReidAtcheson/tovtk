CC := gcc
CFLAGS := -c -g -Wall -pedantic -I../include

XMLCONFIG := $(shell xml2-config --cflags)

tovtk.o : tovtk.c
	$(CC) $(XMLCONFIG) $(CFLAGS) tovtk.c -o tovtk.o


.PHONY : clean


clean : 
	rm -rf ./*.o
	rm -rf ./out
