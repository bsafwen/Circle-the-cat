circleThecat: main.o linkedList.o dji.o init_quit.o game.o
	gcc  -Wall  -o circleThecat main.o linkedList.o dji.o init_quit.o game.o -Iinc/ -Llib/ -lSDL -lSDL_image -lSDL_ttf  -lfmodex64
main.o: src/main.c
	gcc -Wall -Iinc/ -c src/main.c 
linkedList.o:src/linkedList.c
	gcc -Wall -Iinc/ -c src/linkedList.c
dji.o:src/dji.c
	gcc -Wall -Iinc/ -c src/dji.c
init_quit.o:src/init_quit.c
	gcc -Wall -c src/init_quit.c  -Iinc/ 
game.o:src/game.c
	gcc -Wall  -c src/game.c -Iinc/ 
clean:
	rm *.o
