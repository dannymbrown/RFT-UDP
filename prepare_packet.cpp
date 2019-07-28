//
// Created by hrh11 on 7/21/2019.
//

#include "prepare_packet.h"

using namespace std;

prepare_packet::prepare_packet(bool alt_bit, vector<char> pack_data) {

  // Need to put all other header info in this section
  ack_bit = 0;
  check_bit = 0;
  packet_data = pack_data;
  seq_bit = 0;
  set_seq_num(alt_bit);

  //    std::vector<int> results;
  //    results.reserve(arr1.size() + arr2.size());
  //    results.insert(results.end(), arr1.begin(), arr1.end());
  //    results.insert(results.end(), arr2.begin(), arr2.end());
}

bool prepare_packet::get_seq_num() { return seq_bit; }

void prepare_packet::set_seq_num(bool set_seq) {
  if (set_seq == true) {
    seq_num_char = '1';
    seq_bit = 1;
  } else {
    seq_num_char = '0';
    seq_bit = 0;
  }
}

char prepare_packet::checkSum(vector<char> chars) {

  string s(chars.begin(), chars.end());
  string checksum = "";
  string bitString = "";
  string bits = "00011";

  for (size_t i = 0; i < s.size(); i++) {
    if (s.c_str()[0] == '\0') {
      return '\0';
    }
    if (s.c_str()[i] != '\0') {
      bitset<8> bits(s.c_str()[i]);
      bitString += bits.to_string();
    } else {
      bitString += "0";
    }
  }

  string carry = "0";
  for (size_t i = bits.size(); i > 0; i--) {
    if (bits.c_str()[i] == '0' && bitString.c_str()[i] == '0') {
      if (carry == "0") {
        checksum += "0";
      } else {
        checksum += "1";
      }
      carry = "0";
    } else if ((bits.c_str()[i] == '0' && bitString.c_str()[i] == '1') ||
               (bits.c_str()[i] == '1' && bitString.c_str()[i] == '0')) {
      if (carry == "0") {
        checksum += "1";
        carry = "0";
      } else {
        checksum += "0";
        carry = "1";
      }
    } else {
      if (carry == "0") {
        checksum += "0";
        carry = "1";
      } else {
        checksum += "1";
        carry = "1";
      }
    }
  }
  checksum += carry;
  reverse(checksum.begin(), checksum.end());
  bitset<6> checksumBits(checksum);
  char val = static_cast<char>(checksumBits.to_ulong() + 64);
  return val;
}

bool prepare_packet::get_ack_bit() { return ack_bit; }

void prepare_packet::set_ack_bit(int set_ack) { ack_bit = set_ack; }

char prepare_packet::get_checksum() { return check_bit; }

void prepare_packet::set_check_bit(int set_check) { check_bit = set_check; }

std::vector<char> prepare_packet::get_packet_data() { return packet_data; }

std::vector<char> prepare_packet::get_full_packet() {
  std::cout << "Retrieving Full Packet" << '\n';
  std::vector<char> full_packet;
  full_packet.push_back(seq_bit);
  full_packet.push_back(ack_bit);
  full_packet.push_back(check_bit);
  for (int i = 3; i < PACKET_NO_HEADER; i++) {
    full_packet.push_back(packet_data[i - 3]);
  }
  for (std::vector<char>::const_iterator j = full_packet.begin();
       j != full_packet.end(); ++j) {
    std::cout << *j << ' ';
  }
  return full_packet;
}
