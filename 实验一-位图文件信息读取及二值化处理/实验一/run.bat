gcc -c hdr.c
gcc -c bmphdr.c
gcc -o bmphdr.exe hdr.o bmphdr.o

bmphdr test.bmp

gcc -c ez.c
gcc -o ez.exe hdr.o ez.o
ez.exe test.bmp result.bmp 108

pause