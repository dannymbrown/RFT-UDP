//
// Created by hrh11 on 7/21/2019.
//

#ifndef PROJECT_1_PREPARE_PACKET_H
#define PROJECT_1_PREPARE_PACKET_H

#include <algorithm>
#include <arpa/inet.h>
#include <bitset>
#include <climits>
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
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define TEST_FILE "TestFile.txt"
#define BUFFER_LEN 4096
#define PACKET_PLUS_HEADER 128
#define PACKET_NO_HEADER 125
#define PACKET_HEADER 6
#define PORT_NUM 10002

class prepare_packet {
public:
  prepare_packet(bool seq_num, std::vector<char> data_in);
  char checkSum(std::vector<char> chars);
  void set_seq_num(bool seq_set);
  bool get_seq_num();
  bool get_ack_bit();
  void set_ack_bit(int ack_set);
  char get_checksum();
  void set_check_bit(int check_set);
  std::vector<char> get_packet_data();
  std::vector<char> get_full_packet();

protected:
  char seq_num_char;
  int seq_bit;
  int ack_bit;
  int check_bit;
  std::vector<char> packet_data;
};

#endif // PROJECT_1_PREPARE_PACKET_H
