CC = g++

all: nptest.so
        cp nptest.so /usr/lib/mozilla/plugins/libnptest.so      
nptest.so:
        $(CC) nptest.c -fPIC -shared -o nptest.so
clean:
        rm /usr/lib/mozilla/plugins/libnptest.so
        rm *.so
        rm *.o

