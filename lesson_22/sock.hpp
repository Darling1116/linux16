#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define BACKLOG 5

//---��socket�׽��ֽ��з�װ---
struct Sock{
	static int Socket(){
		int lsock = socket(AF_INET, SOCK_STREAM, 0);
		if(lsock < 0){
			std::cout << "socket error!" << std::endl;
			exit(2);
		}
		return lsock;
		std::cout << "lsock:" << lsock << std::endl;
	}


	static void Bind(int lsock, int port){
		struct sockaddr_in local;
		bzero(&local, sizeof(local));  //�ַ�������
		local.sin_family = AF_INET;  //16λ��ַ����
		local.sin_port = htons(port);
		local.sin_addr.s_addr = INADDR_ANY;
		if(bind(lsock, (struct sockaddr*)&local, sizeof(local)) < 0){
			std::cout << "bind error!" << std::endl;
			exit(3);
		}
	}
	

	//ʹ��setsockopt�������ö˿ں�Ϊ����״̬
	static void Setsockopt(int lsock){
		int opt = 1;  //???
		setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}


	static void Listen(int lsock){
		if(listen(lsock, BACKLOG) < 0){
			std::cout << "listen error!" << std::endl;
			exit(4);
		}
	}


	static int Accept(int lsock){
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int fd = accept(lsock, (struct sockaddr*)&peer, &len);
		if(fd < 0){
			std::cout << "accept error!" << std::endl;
			exit(5);
		}
		return fd;  //��������ͨ�����ӵ��ļ�������
	}

};