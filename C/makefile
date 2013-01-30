#This make file currently explodes I need to fix it, using the compile bash script works better

CC=gcc

bin/engine: bin/engine.o bin/network.o 
	$(CC) -o bin/engine bin/network.o bin/engine.o -lpthread

bin/engine.o: src/engine.c src/engine.h src/conf.h
	$(CC) -c src/engine.c -o bin/engine.o

bin/network.o: src/conf.h src/network.h src/network.c
	$(CC) -c src/network.c -o bin/network.o


clean:
	rm -rf ./bin/*
