all:
	g++ -c src/*.cpp -I include
	g++ *.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	
clean:
	rm -f *.o sfml-app

run: all
	./sfml-app