rsa: rsa.o
	c++ -g -std=c++11 -DGLIBCXX_DEBUG -O3 -o rsa rsa.o -lgmp

rsa.o: rsa.cpp
	c++ -g -std=c++11 -DGLIBCXX_DEBUG -O3 -c rsa.cpp -lgmp

clean:
	rm *~ *# rsa rsa.o BigInt.o
