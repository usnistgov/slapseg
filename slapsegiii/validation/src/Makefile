# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.


INC_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

LIB_PREFIX = libslapsegiii_
ifeq ($(shell ls $(LIB_DIR) | grep -E $(LIB_PREFIX)[A-Za-z0-9]\+_\([0-9A-Fa-f]\+\|[0-9A-Fa-f]\{4\}\)\.\(so\|dylib\) | wc -l | awk '{print $$1}'),1)
        CORE_LIB := $(shell ls $(LIB_DIR) | grep -E $(LIB_PREFIX)[A-Za-z0-9]\+_\([0-9A-Fa-f]\+\|[0-9A-Fa-f]\{4\}\)\.\(so\|dylib\))
else
        ifeq ($(shell ls $(LIB_DIR) | grep -E $(LIB_PREFIX)[A-Za-z0-9]\+_\([0-9A-Fa-f]\+\|[0-9A-Fa-f]\{4\}\)\.\(so\|dylib\) | wc -l | awk '{print $$1}'),0)
                $(error No core library found in $(LIB_DIR). Please review the SlapSegIII naming conventions)
        else
                $(error More than one core library found in $(LIB_DIR). Please review SlapSegIII naming conventions)
        endif
endif
ifeq ($(CORE_LIB), )
        $(error You did not place your libraries in $(LIB_DIR))
endif

CXXFLAGS += -Wall -pedantic -std=c++11 -I$(INC_DIR) -I. -O3
LDFLAGS += -L$(LIB_DIR) -lslapsegiii -l$(patsubst lib%.so,%,$(CORE_LIB)) -Wl,-rpath,$(shell readlink -f $(LIB_DIR))

BINARY = slapsegiii_validation
SOURCES = $(BINARY).cpp $(BINARY)_validate.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(BINARY)

debug: CXXFLAGS += -g
debug: all

$(BINARY): $(OBJECTS)
	$(MAKE) -C libslapsegiii
	mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(BIN_DIR)/$(BINARY)

clean:
	$(MAKE) -C libslapsegiii clean
	$(RM) $(OBJECTS) $(BIN_DIR)/$(BINARY)
	$(RM) -r $(BIN_DIR)

