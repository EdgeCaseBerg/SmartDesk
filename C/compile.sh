

gcc -c -o obj/network.o src/network.c
gcc -c -o obj/engine.o src/engine.c -lpthread
gcc -o ./bin/engine ./obj/engine.o ./obj/network.o -lpthread