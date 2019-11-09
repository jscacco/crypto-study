rsa: rsa.o BigInt.o
	c++ -std=c++11 -DGLIBCXX_DEBUG -o rsa rsa.o BigInt.o

rsa.o: rsa.cpp BigInt.h
	c++ -std=c++11 -DGLIBCXX_DEBUG -c rsa.cpp

BigInt.o: BigInt.cpp BigInt.h
	c++ -std=c++11 -DGLIBCXX_DEBUG -c BigInt.cpp

clean:
	rm *~ *# rsa rsa.o BigInt.o
