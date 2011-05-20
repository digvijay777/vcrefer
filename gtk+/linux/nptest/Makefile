CC = g++

all: nptest.o testplug.o testapp.o
	$(CC) nptest.o testplug.o testapp.o -shared -o nptest.so
	cp nptest.so /usr/lib/mozilla/plugins/libnptest.so   
	rm *.o	
   
testapp.o: testapp.cpp testapp.h
	$(CC) -c testapp.cpp -DXP_UNIX=1 -DMOZ_X11=1 -D_X86_=1
	
testplug.o: testplug.cpp testplug.h
	$(CC) -c testplug.cpp -DXP_UNIX=1 -DMOZ_X11=1 -D_X86_=1
	
nptest.o: nptest.cpp nptest.h
	$(CC) -fPIC  -DXP_UNIX=1 -DMOZ_X11=1 -D_X86_=1 -c nptest.cpp

clean:
	rm *.so
	rm /usr/lib/mozilla/plugins/libnptest.so

