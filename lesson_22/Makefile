.PHONY:all
all: selectServer clienttest

selectServer:selectServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread

clienttest:clienttest.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f selectServer clienttest