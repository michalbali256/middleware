ifndef OMNIORB_HOME
	OMNIORB_HOME = ~/omniORB
endif

ifeq ($(OMNIORB_HOME),)
	# Assume omniORB is distribution package
	IDL = omniidl
	LIBDIRFLAGS =
	INCDIRFLAGS =
else
	# Assume omniORB is installed in $(OMNIORB_HOME)
	# e.g. in ~/omniORB
	IDL = $(OMNIORB_HOME)/bin/omniidl
	LIBDIRFLAGS = -Wl,-rpath $(OMNIORB_HOME)/lib -L $(OMNIORB_HOME)/lib
	INCDIRFLAGS = -I $(OMNIORB_HOME)/include
endif

CXX = g++
CXXFLAGS = -march=native -O2
LIBFLAGS = -lomniORB4 -lomnithread -lomniDynamic4
# C++ backend, custom filenames
IDLFLAGS = -bcxx -Wbh=.h -Wbs=SK.cpp

.PHONY: all clean

all: server client

master.h masterSK.cpp: master.idl
	$(IDL) $(IDLFLAGS) $<

%.o: %.cpp master.h server_impl.h
	$(CXX) $(CXXFLAGS) $(INCDIRFLAGS) -c -o $@ $<

server: masterSK.o server.o
	$(CXX) $(CXXFLAGS) -o $@ $(LIBDIRFLAGS) $(LIBFLAGS) $^

client: masterSK.o client.o
	$(CXX) $(CXXFLAGS) -o $@ $(LIBDIRFLAGS) $(LIBFLAGS) $^

clean:
	rm -f *.o server client master.h masterSK.cpp
