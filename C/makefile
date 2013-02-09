#This make file currently explodes I need to fix it, using the compile bash script works better

CC=gcc

bin/engine: bin/engine.o bin/network.o bin/graphics.o
	$(CC) -o bin/engine bin/network.o bin/engine.o bin/graphics.o `sdl-config --libs` -lpthread

bin/engine.o: src/engine.c src/engine.h src/conf.h
	$(CC) -c src/engine.c -o bin/engine.o `sdl-config --cflags`

bin/network.o: src/conf.h src/network.h src/network.c
	$(CC) -c src/network.c -o bin/network.o

bin/graphics.o: src/graphics.c src/graphics.h src/conf.h
	$(CC) -c src/graphics.c `sdl-config --cflags` -lm -o bin/graphics.o 

clean:
	rm -rf ./bin/*


bin/debug: bin/engined.o bin/networkd.o bin/graphicsd.o
	$(CC) -o bin/debug bin/networkd.o bin/engined.o bin/graphicsd.o `sdl-config --libs` -lpthread -g

bin/engined.o: src/engine.c src/engine.h src/conf.h
	$(CC) -c src/engine.c -g -o bin/engined.o `sdl-config --cflags`

bin/networkd.o: src/conf.h src/network.h src/network.c
	$(CC) -c src/network.c -g -o bin/networkd.o

bin/graphicsd.o: src/graphics.c src/graphics.h src/conf.h
	$(CC) -c src/graphics.c -g `sdl-config --cflags` -lm -o bin/graphicsd.o 
