CFLAGS=$(shell root-config --cflags)
LDFLAGS=-L /home/luole/root/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -L. -lSclass

all:libSclass.so read_write

read_write:read_write.o
	g++ -L. -o $@ $^ $(LDFLAGS)
hclient1:hclient1.o
	g++ -L. -o $@ $^ $(LDFLAGS)
hclient2:hclient2.o
	g++ -L. -o $@ $^ $(LDFLAGS)
hserv:hserv.o
	g++ -L. -o $@ $^ $(LDFLAGS)

libSclass.so:Sclass.o SclassDict.o
	g++ -shared -o $@ $^ $(LDFLAGS)

Sclass.cpp:Sclass.h

SclassDict.cpp:Sclass.h LinkDef.h
	rootcint -f $@ -c $^

%.o:%.cpp
	g++ -c -fPIC -o $@ $^ $(CFLAGS)

####################################
#run the command
####################################
run_read_write:read_write
       @LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ./read_write 
