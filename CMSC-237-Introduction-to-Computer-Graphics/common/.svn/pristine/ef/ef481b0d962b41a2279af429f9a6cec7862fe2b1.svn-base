# Makefile for CMSC 23700 Common Code Library
# Autumn 2015
#
# COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
# All rights reserved.

SHELL =		/bin/sh
OS =		$(shell uname -s)

# the location of doxygen depends on the system that you are running,
# but you may have to edit this path.
#
ifeq ($(OS),Darwin)
  # standard location for MacLab machines; installed from Mac Ports
  DOXYGEN =	/opt/local/bin/doxygen
else
  DOXYGEN =	/usr/bin/doxygen
endif

BUILD_DIR =	$(shell pwd)
COMMON_DIR =	..
CS237_LIB_DIR =	$(COMMON_DIR)/cs237-lib
JSON_LIB_DIR =	$(COMMON_DIR)/json
OBJ_LIB_DIR =	$(COMMON_DIR)/obj
TQT_LIB_DIR =	$(COMMON_DIR)/tqt
DOC_DIR =	$(COMMON_DIR)/doc

TARGET =	$(COMMON_DIR)/lib/libcs237.a

ifeq ($(OS),Darwin)
  CPPFLAGS = -I$(COMMON_DIR)/include -I$(COMMON_DIR)/obj -I/usr/local/include -I/opt/local/include
  LDFLAGS = -L/usr/local/lib -L/opt/local/lib
  CXX = clang++ -std=c++11
else
  CPPFLAGS = -I$(COMMON_DIR)/include -I$(COMMON_DIR)/obj -I/usr/local/include
  LDFLAGS = —L/usr/local/lib
  CXX = g++ -std=c++11
endif
CXXFLAGS =	-O2 -g -Wall -pedantic

VPATH =		$(CS237_LIB_DIR):$(JSON_LIB_DIR):$(OBJ_LIB_DIR):$(TQT_LIB_DIR)

DIRSRCS =	$(wildcard $(CS237_LIB_DIR)/*.cxx) \
		$(wildcard $(JSON_LIB_DIR)/*.cxx) \
		$(wildcard $(OBJ_LIB_DIR)/*.cxx) \
		$(wildcard $(TQT_LIB_DIR)/*.cxx)

SRCS =		$(notdir $(DIRSRCS))

INCLUDES =	$(wildcard ../include/*.hxx)  $(wildcard ../obj/*.hxx)

OBJS =		$(SRCS:.cxx=.o)

ifeq ($(OS),Darwin)
$(TARGET):	$(OBJS) .depend
	libtool -static -o $(TARGET) $(OBJS)
else
$(TARGET):	$(OBJS) .depend
	ar rcs $(TARGET) $(OBJS)
endif

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# include-file dependency information
#
.depend:	$(DIRSRCS) $(INCLUDES)
	- $(CXX) $(CPPFLAGS) -MM $(DIRSRCS) > .depend

ifneq ($(MAKECMDGOALS),clean)
sinclude .depend
endif

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -rf *.o $(TARGET) .depend
