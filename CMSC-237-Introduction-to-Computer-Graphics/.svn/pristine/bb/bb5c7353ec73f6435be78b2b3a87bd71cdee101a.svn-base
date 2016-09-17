# Makefile for CMSC 23700 Lab 1
# Autumn 2015
#

TARGET =	lab1

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
COMMON_DIR =	../../common
DATA_DIR =	$(BUILD_DIR)/../data

ifeq ($(OS),Darwin)
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include -I/opt/local/include
  LDFLAGS = -L$(COMMON_DIR)/lib -L/usr/local/lib -L/opt/local/lib
  LIBS = -lcs237 -L/usr/local/lib -l glfw3 -framework OpenGL -framework Foundation -framework IOKit -framework CoreVideo -framework Cocoa
  CXX = clang++ -std=c++11
else
  CPPFLAGS = -I$(COMMON_DIR)/include -I/usr/local/include
  LDFLAGS = -L$(COMMON_DIR)/lib —L/usr/local/lib
  LIBS = -lcs237 -lglfw -lGL -lGLU -lm
  CXX = g++ -std=c++11
endif
CPPFLAGS +=	-DDATA_DIR=\"$(DATA_DIR)\"
#CXXFLAGS =	-O2 -Wall -pedantic
CXXFLAGS =	-g -Wall -pedantic

# where to find the source code
#
VPATH =		../src

SRCS =		$(wildcard ../src/*.cxx)
INCLUDES =	$(wildcard ../src/*.hxx)
DOC_SRCS =	$(SRCS) $(INCLUDES) ../main-page

COMMON_SRCS =	$(wildcard $(COMMON_DIR)/cs237-lib/*.hxx) \
		$(wildcard $(COMMON_DIR)/json/*.hxx) \
		$(wildcard $(COMMON_DIR)/obj/*.hxx)
COMMON_INCLUDES = $(wildcard $(COMMON_DIR)/include/*.hxx)

OBJS =		$(notdir $(SRCS:.cxx=.o))

$(TARGET):	$(OBJS) $(COMMON_DIR)/lib/libcs237.a .depend
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(COMMON_DIR)/lib/libcs237.a:
	(cd $(COMMON_DIR)/build; $(MAKE))

# include-file dependency information
#
.depend:	$(SRCS) $(INCLUDES)
	- $(CXX) $(CPPFLAGS) -MM $(SRCS) > .depend

ifneq ($(MAKECMDGOALS),clean)
sinclude .depend
endif

# generate documentation from sources using doxygen
#
.PHONY:		doc
doc:		../doc

../doc:		$(DOC_SRCS)
	(cd ..; $(DOXYGEN) Doxyfile)
	- touch ../doc

# cleanup by removing generated files
#
.PHONY:		clean
clean:
		rm -rf *.o $(TARGET) ../doc .depend

