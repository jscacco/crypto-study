rsa: rsa.o
	c++ -DGLIBCXX_DEBUG -o rsa rsa.o

rsa.o: rsa.cpp InfInt.h
	c++ -DGLIBCXX_DEBUG -c rsa.cpp

clean:
	rm *~ *# rsa rsa.o
