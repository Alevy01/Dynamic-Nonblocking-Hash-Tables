# @Author: Michel Andy, Levy Adam
# @Date:   2017-11-05T20:54:35-05:00
# @Email:  Andymic12@gmail.com
# @Project: Dynamic-Nonblocking-Hash-Tables
# @Filename: makefile
# @Last modified by:   andy
# @Last modified time: 2017-11-17T14:02:29-05:00



CXX=g++
CFLAGS=-g -std=c++14 -pthread -Wall -m64 -march=native
INC=-I./
SRCS=hnode.cc threadpool.cc
OBJS=$(SRCS:.cc=.o)
MAIN=DNHT

#all: $(OBJS)
#	$(CXX) $(CFLAGS) $(INC) $(OBJS) -o $(MAIN)

all:
	$(CXX) $(CFLAGS) $(INC) $(SRCS) -o $(MAIN)

tpool:
	$(CXX) $(CFLAGS) $(INC) threadpool.cc -o threadpool

clean:
	rm -f $(MAIN) $(OBJS)
