

gcc -c -o obj/network.o src/network.c
gcc -c -o obj/engine.o src/engine.c -lpthread
gcc -o ./bin/engine ./obj/engine.o ./obj/network.o -lpthread

gcc -c sdlex.c `sdl-config --cflags`
gcc -o sdlex sdlex.o `sdl-config --libs`
