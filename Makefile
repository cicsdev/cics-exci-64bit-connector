#*********************************************************************
# Licensed Materials - Property of IBM                               *
#                                                                    *
# SAMPLE                                                             *
#                                                                    *
# (c) Copyright IBM Corp. 2017 All Rights Reserved                   *
#                                                                    *
# US Government Users Restricted Rights - Use, duplication or        *
# disclosure restricted by GSA ADP Schedule Contract with IBM Corp   *
#*********************************************************************

ASM = /bin/as
CC = /bin/xlc
CXX = /bin/cxx

BLD=build
SRC=src
INC=inc

MACLIB=maclib
	
test:
	$(CC) -Wc,LP64 -Wl,LP64 -I./inc -o test.o testProgram.c cicsExci64Connector.x

static:
	ar -rc libCicsExci64Connector.a entry.o glue.o exci.o

dll: entry.o glue.o exci.o
	$(CXX) -Wl,LP64,DLL -o cicsExci64Connector.so entry.o glue.o exci.o

exci.o: exci.s
	$(ASM) -mgoff -I./$(MACLIB) -o exci.o exci.s

glue.o: $(SRC)/glue.s
	$(ASM) -mgoff -I./$(MACLIB) -o glue.o $(SRC)/glue.s
	
exci.s: $(SRC)/exci.c
	$(CXX) -S -Wc,CICS,RENT,NOARGPARSE,GOFF,LONGNAME,NOSEARCH,METAL -I /usr/include/metal/ -I ./inc/ -o exci.s $(SRC)/exci.c
	
entry.o: $(SRC)/entry.c
	$(CC) -c -Wc,LP64 -qexportall -I./$(INC) -o entry.o $(SRC)/entry.c

full: clean dll static test

clean:
	rm -f exci.s *.o
