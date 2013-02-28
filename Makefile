HAS_CLANG=$(shell which clang)
ifeq ($(HAS_CLANG),)
	CC=gcc
else
	CC=clang
endif

HAS_CLANGXX=$(shell which clang++)
ifeq ($(HAS_CLANGXX),)
	CXX=g++
else
	CXX=clang++
endif

CFLAGS?=
CXXFLAGS?=-std=c++11
LIBS=

AR=ar
ARCHIVE=libderpcl.a

SYSTEM=$(shell uname -s)
ifeq ($(SYSTEM), Linux)
LIBS += -lOpenCL
else
LIBS += -framework OpenCL
CXXFLAGS += -stdlib=libc++
endif

TEST_O=build/tests.o
TEST_X=test
SRC=$(filter-out source/tests.cc, $(wildcard source/*.cc))
OBJ=$(SRC:source/%.cc=build/%.o)
HDR=$(wildcard source/*.h)
LOC_HDR=$(HDR:source/%.h=%.h)

.PHONY: all debug unit clean

all: CXXFLAGS += -O3
all: $(OBJ) $(LOC_HDR)
	$(AR) sr $(ARCHIVE) $(OBJ)


debug: CXXFLAGS += -O0 -DDEBUG -g
debug: $(OBJ) $(LOC_HDR)
	$(AR) sr $(ARCHIVE) $(OBJ)

rtest: CXXFLAGS += -O3
rtest: all $(TEST_O)
	$(CXX) $(CXXFLAGS) $(OPT) $(TEST_O) -o $(TEST_X) -L. -lderpcl $(LIBS)
	./$(TEST_X)
	cat testfile
	-rm testfile

dtest: CXXFLAGS += -O0 -DDEBUG -g
dtest: debug $(TEST_O)
	$(CXX) $(CXXFLAGS) $(OPT) $(TEST_O) -o $(TEST_X) -L. -lderpcl $(LIBS)
	./$(TEST_X)
	cat testfile
	-rm testfile

unit: CXXFLAGS += -O0 -DDEBUG -g
unit: $(OBJ) unit_test.o
	$(CXX) $(CXXFLAGS) $(OPT) $^ -o $@ $(LIBS)

build/%.o: source/%.cc $(HDR)
	$(CXX) $(CXXFLAGS) $(OPT) -c $< -o $@

%.h: source/%.h
	cp $< $@

clean:
	-rm -f build/* $(ARCHIVE) $(TEST_X) $(LOC_HDR)
