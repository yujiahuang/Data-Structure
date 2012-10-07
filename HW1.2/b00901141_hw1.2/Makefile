p1a: hw1.2.p1a.main.o hw1.2.p1a.o
	g++ -o hw1.2.p1a hw1.2.p1a.main.o hw1.2.p1a.o
p1b: hw1.2.p1b.main.o hw1.2.p1b.o
	g++ -o hw1.2.p1b hw1.2.p1b.main.o hw1.2.p1b.o
bug: hw1.2.p1.bug.o hw1.2.p1a.o
	g++ -o hw1.2.p1.bug hw1.2.p1.bug.o hw1.2.p1a.o

hw1.2.p1a.main.o: hw1.2.p1a.main.cpp
	g++ -c hw1.2.p1a.main.cpp
hw1.2.p1a.o: hw1.2.p1a.cpp
	g++ -c hw1.2.p1a.cpp
hw1.2.p1b.main.o: hw1.2.p1b.main.cpp
	g++ -c hw1.2.p1b.main.cpp
hw1.2.p1b.o: hw1.2.p1b.cpp
	g++ -c hw1.2.p1b.cpp
hw1.2.p1.bug.o: hw1.2.p1.bug.cpp
	g++ -c hw1.2.p1.bug.cpp

clean:
	rm -rf hw1.2.p1a.main.o hw1.2.p1a.o
	rm -rf hw1.2.p1b.main.o hw1.2.p1b.o
	rm -rf hw1.2.p1.bug.o
