match: main.o ksum.o
	g++ main.o ksum.o -o match -g
ksum.o: ksum.cc ksum.h
	g++ -c --std=c++0x ksum.cc -g
main.o: main.cc
	g++ -c main.cc -g
clean:
	rm -f *.o match
