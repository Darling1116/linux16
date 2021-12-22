#include "udpServer.hpp"

int main( ){

udpServer *us = new udpServer();
us->InitServer();
us->Start();

delete us;
}