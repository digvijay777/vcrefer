CC = g++

all: nptest.o testplug.o testapp.o
	$(CC) nptest.o testplug.o testapp.o -fPIC -shared -o nptest.so
	cp nptest.so /usr/lib/mozilla/plugins/libnptest.so   
	   
testapp.o: TestApp.cpp TestApp.h
	$(CC) -c Test.cpp -o testapp.o
	
testplug.o: TestPlug.o TestPlug.h
	$(CC) -c TestPlug.cpp -o testplug.o
	
nptest.o: nptest.cpp nptest.h
	$(CC) -c nptest.cpp -o nptest.o

clean:
	rm /usr/lib/mozilla/plugins/libnptest.so
	rm *.so
	rm *.o

