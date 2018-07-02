#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include <unistd.h>
#include"global_setting.h"
#include<thread>

using namespace std;

class udp_client
{
public:
    udp_client(string address,int port);
    string start_listen();

private:
    struct sockaddr_in addr;
    int sockfd;
};

#endif // UDP_CLIENT_H
