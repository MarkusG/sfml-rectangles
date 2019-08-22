make:
	g++ -c *.cpp
	g++ *.o -o app -lsfml-graphics -lsfml-window -lsfml-system
	rm *.o
