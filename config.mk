##############################################################################
# Configuration for Makefile
#

PROJECT := freeverb
//PROJECT_TYPE := revfx
PROJECT_TYPE := delfx

##############################################################################
# Sources
#

# C sources 
UCSRC = header.c

# C++ sources 
UCXXSRC = allpass.cc comb.cc revmodel.cc freeverb.cc

# List ASM source files here
UASMSRC = 

UASMXSRC = 

##############################################################################
# Include Paths
#

UINCDIR  = 

##############################################################################
# Library Paths
#

ULIBDIR = 

##############################################################################
# Libraries
#

ULIBS  = -lm

##############################################################################
# Macros
#

UDEFS = 

