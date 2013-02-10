#This make file currently explodes I need to fix it, using the compile bash script works better

CC=gcc

bin/engine: bin/engine.o bin/network.o bin/graphics.o bin/menu.o bin/button.o
	$(CC) -o bin/engine bin/network.o bin/engine.o bin/graphics.o bin/menu.o bin/button.o `sdl-config --libs` -lpthread

bin/engine.o: src/engine.c src/engine.h src/conf.h
	$(CC) -c src/engine.c -o bin/engine.o `sdl-config --cflags`

bin/network.o: src/conf.h src/network.h src/network.c
	$(CC) -c src/network.c -o bin/network.o

bin/graphics.o: src/graphics.c src/graphics.h src/conf.h src/menu.h
	$(CC) -c src/graphics.c `sdl-config --cflags` -lm -o bin/graphics.o 

bin/button.o: src/button.c src/button.h
	$(CC) -c src/button.c `sdl-config --cflags` -o bin/button.o

bin/menu.o: src/menu.c src/menu.h src/button.h
	$(CC) -c src/menu.c `sdl-config --cflags` -g -o bin/menu.o

clean:
	rm -rf ./bin/*


bin/debug: bin/engined.o bin/networkd.o bin/graphicsd.o bin/buttond.o bin/menud.o
	$(CC) -o bin/debug bin/networkd.o bin/engined.o bin/graphicsd.o bin/menud.o bin/buttond.o `sdl-config --libs` -lpthread -g

bin/engined.o: src/engine.c src/engine.h src/conf.h
	$(CC) -c src/engine.c -g -o bin/engined.o `sdl-config --cflags`

bin/networkd.o: src/conf.h src/network.h src/network.c
	$(CC) -c src/network.c -g -o bin/networkd.o

bin/graphicsd.o: src/graphics.c src/graphics.h src/conf.h src/menu.h
	$(CC) -c src/graphics.c -g `sdl-config --cflags` -lm -o bin/graphicsd.o 

bin/buttond.o: src/button.c src/button.h
	$(CC) -c src/button.c `sdl-config --cflags` -g -o bin/buttond.o

bin/menud.o: src/menu.c src/menu.h
	$(CC) -c src/menu.c `sdl-config --cflags` -g -o bin/menud.o