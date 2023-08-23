@echo off
gcc -c -o bin/main.o source/queue.c -O2
gcc -c -o bin/sllist.o source/Compose/SLList.c -O2
pause
gcc -o bin/prog bin/main.o bin/sllist.o
pause