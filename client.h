//
// Created by hrh11 on 7/21/2019.
//

#ifndef PROJECT_1_CLIENT_H
#define PROJECT_1_CLIENT_H

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#include "prepare_packet.h"

#define TEST_FILE "TestFile.txt"
#define BUFFER_LEN 4096
#define PACKET_PLUS_HEADER 128
#define PACKET_NO_HEADER 125
#define PORT_NUM 10002

typedef unsigned char byte;

unsigned char buffer[BUFFER_LEN];
char packet[PACKET_PLUS_HEADER];
int client_sd;
std::string serv_IP_address = "192.168.0.168";
struct sockaddr_in sock_addr;
struct sockaddr_in serv_addr;
socklen_t srvaddrLen = sizeof(serv_addr);
struct hostent *h;
bool alt_bit = true;

bool alt_bit_protocol();
bool open_socket();
bool packet_sent(std::vector<char> v);
bool packet_received();
void gremlin(prepare_packet pk, double pl, double pd);

#endif // PROJECT_1_CLIENT_H
