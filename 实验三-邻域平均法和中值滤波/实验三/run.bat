gcc -c hdr.c
gcc -c bmphdr.c
gcc -o bmphdr.exe hdr.o bmphdr.o

bmphdr testnoise.bmp

gcc -c zz.c
gcc -c ly.c
gcc -o zz.exe hdr.o zz.o
gcc -o ly.exe hdr.o ly.o
zz.exe testnoise.bmp zz.bmp
ly.exe testnoise.bmp ly.bmp

del *.o
pause