#This make file currently explodes I need to fix it, using the compile bash script works better

CC=gcc

bin/engine: obj/engine.o obj/network.o 
	$(CC) -I -o bin/engine obj/network.o obj/engine.o -lpthread

obj/network.o: src/conf.h src/network.h src/network.c
	$(CC) -I -c src/network.c -o obj/network.o

obj/engine.o: obj/network.o src/engine.c src/conf.h
	$(CC) -I -c src/engine.c obj/network.o -lpthread -o obj/engine.o