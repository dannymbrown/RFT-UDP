//
// Created by hrh11 on 7/22/2019.
//

#ifndef PROJECT_1_SERVER_H
#define PROJECT_1_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "prepare_packet.h"

#define BUFFER_LEN 4096
#define PACKET_PLUS_HEADER 128
#define PACKET_NO_HEADER 122
#define PORT_NUM 10002

struct sockaddr_in connect_addr;
struct sockaddr_in command_addr;
struct sockaddr_in server;
socklen_t commandLen = sizeof(command_addr);
int n;
bool lastPacket;
int serv_sd;
bool expectedSeq = 1;
unsigned char buffer[BUFFER_LEN];

#endif //PROJECT_1_SERVER_H
