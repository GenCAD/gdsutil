PKGNAME  = gdsutil
PKGVERN  = v2017.9

# Final Releae direcotry
LIBS_INC = ../../include
LIBS_DIR = ../../lib
UTIL_BIN = ../../bin

# Local Compile directory
SRC_DIR   = src
OBJ_DIR   = obj
OUT_DIR   = lib

#
#
#
MKDIR   = mkdir -p
CP	= cp -f
CAT	= cat

#
# for cc
#
CC      = gcc -g -fPIC 
#-D_LARGEFILE64_SOURCE
CFLAGS	=   \
                -DPKGNAME=\"$(PKGNAME)\" \
                -DPKGVERN=\"$(PKGVERN)\" \
		-I$(LIBS_INC) \
		-L$(LIBS_DIR)
EXTFLAGS= 
LEX     = flex -l
LFLAGS  =
YACC    = bison -y
YFLAGS  = -d 
AR      = ar -cr
RANLIB	= ranlib

####
