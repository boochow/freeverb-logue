##############################################################################
# Configuration for Makefile
#

PROJECT := freeverb
PROJECT_TYPE := revfx

##############################################################################
# Sources
#

# C sources 
CSRC = header.c

# C++ sources 
CXXSRC = allpass.cc comb.cc revmodel.cc freeverb.cc

# List ASM source files here
UASMSRC = 

UASMXSRC = 

##############################################################################
# Include Paths
#

UINCDIR  = 

ifeq ($(ARCH), arm)
  UINCDIR += 
else
  UINCDIR += 
endif

##############################################################################
# Library Paths
#

ULIBDIR = 

ifeq ($(ARCH), arm)
  ULIBDIR += 
else
  ULIBDIR += 
endif

##############################################################################
# Libraries
#

ULIBS  = -lm
ULIBS += -lc

##############################################################################
# Macros
#

UDEFS = 

