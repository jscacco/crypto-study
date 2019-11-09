rsa: rsa.o
	c++ -DGLIBCXX_DEBUG -o rsa rsa.o big_int.o

rsa.o: rsa.cpp big_int.h
	c++ -DGLIBCXX_DEBUG -c rsa.cpp

big_int.o: big_int.cpp big_int.h
	c++ -DGLIBCXX_DEBUG -c big_int.cpp

clean:
	rm *~ *# rsa rsa.o
