gcc -c hdr.c
gcc -c bmphdr.c
gcc -o bmphdr.exe hdr.o bmphdr.o

bmphdr testnoise.bmp

gcc -c zz.c
gcc -o zz.exe hdr.o zz.o
zz.exe testnoise.bmp zz.bmp

del *.o
pause