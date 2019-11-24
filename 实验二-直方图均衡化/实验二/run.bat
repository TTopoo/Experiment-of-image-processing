gcc -c hdr.c
gcc -c bmphdr.c
gcc -o bmphdr.exe hdr.o bmphdr.o

bmphdr test.bmp

gcc -c zf.c
gcc -o zf.exe hdr.o zf.o
zf.exe test.bmp zf.bmp

del *.o
del *.exe

pause