all:
	g++ -c src/*.cpp -I include
	g++ *.o -o tetris -lsfml-graphics -lsfml-window -lsfml-system
	
clean:
	rm -f *.o tetris

run: all 
	./tetris