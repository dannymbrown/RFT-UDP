#include "client.h"
#include "prepare_packet.h"

using namespace std;

int main(int argc, char *argv[]) {
  double pl, pd;
  if (argc >= 2) {
    pl = atof(argv[1]);
    pd = atof(argv[2]);
  } else {
    pl = 0.0;
    pd = 0.0;
  }

  if (open_socket() == false) {
    return 0;
  }

  cout << "PUT " << TEST_FILE << endl;
  FILE *file_data;
  file_data = fopen(TEST_FILE, "r");

  if (!(file_data == NULL)) {
    char c;
    int data_length, offset = 0;
    std::vector<char> buffer(128);
    std::cout << "Gathering Message" << '\n';
    while ((c = fgetc(file_data)) != EOF) {
      data_length++;
      buffer.push_back(c);
      if (data_length == 125) {
        alt_bit = alt_bit_protocol();
        std::cout << "NOTE: message = ";
        for (std::vector<char>::const_iterator i = buffer.begin();
             i != buffer.end(); ++i)
          std::cout << *i << ' ';
        std::cout << "Preparing Packet" << '\n';
        exit(1); // DELETE
        prepare_packet pk = prepare_packet(alt_bit, buffer);

        std::cout << "Datagram Generated" << '\n';

        std::cout << "\nPassing To Gremlin" << '\n';
        gremlin(pk, pl, pd);
      }
    }
    fclose(file_data);
  } else {
    cout << "Could not open file: " << TEST_FILE << std::endl;
    return 0;
  }
}

bool alt_bit_protocol() {
  alt_bit = !alt_bit;
  return alt_bit;
}

bool open_socket() {
  if ((client_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Could not create socket.");
    return 0;
  }

  memset((char *)&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_addr.sin_port = htons(0);

  if (bind(client_sd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
    perror("Could not bind socket.");
    return 0;
  }

  memset((char *)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT_NUM);
  inet_pton(AF_INET, serv_IP_address.c_str(), &(serv_addr.sin_addr));

  std::cout << "Server IP Address: " << inet_ntoa(serv_addr.sin_addr) << ":"
            << ntohs(serv_addr.sin_port) << std::endl;
  return true;
}

bool packet_sent(vector<char> pack) {
  std::cout << "Entering packet_sent" << '\n';
  vector<char> pack_array = pack;
  char packet_as_array[pack_array.size()];
  std::copy(pack_array.begin(), pack_array.end(), packet_as_array);

  if (sendto(client_sd, packet_as_array, pack_array.size(), 0,
             (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    std::cout << "Message Sent" << '\n';
    return 0;
  } else {
    std::cerr << "ERROR SENDING" << '\n';
  }
  return 1;
}

bool packet_received() {
  int poll_received = 0;
  // ufds = new pollfd;
  pollfd *ufds = new pollfd();
  time_t timer;

  ufds->fd = client_sd;
  ufds->events = POLLIN;
  poll_received = poll(ufds, 1, 18);

  if (poll_received == -1) {
    perror("Could not poll socket.");
    return 0;
  } else if (poll_received == 0) {
    perror("Timeout reached.");
    return 0;
  } else {
    int rec_length;
    rec_length = recvfrom(client_sd, buffer, BUFFER_LEN, 0,
                          (struct sockaddr *)&serv_addr, &srvaddrLen);
    if (rec_length > 0) {
      buffer[rec_length] = 0;
      char ack_nak = buffer[1];
      if (ack_nak == '1') {
        std::cout << "Server Message: ACK SEQ_NUM = " << buffer[0] << std::endl
                  << std::endl;
        return 1;
      } else {
        std::cout << "Server Message: NAK SEQ_NUM = " << buffer[0] << std::endl
                  << std::endl;
        return 0;
      }
    }
  }
  return 0;
}

/**
 *@function gremlin()
 *debugging operations commented out
 */
void gremlin(prepare_packet pack, double loss_probability,
             double damage_probability) {
  std::cout << "Entering Gremlin" << '\n';

  pack.get_full_packet();
  std::vector<char> pack_array;

  if (pack_array.empty()) {
    std::cerr << "CRITICAL ERROR: Packet is Empty!" << '\n';
    exit(1);
  }

  std::cout << "Returning Control to Gremlin" << '\n';
  std::cout << "NOTE: message = ";

  double pl = loss_probability;
  double pd = damage_probability;

  std::cout << "NOTE: pl = " << pl << '\n';
  std::cout << "NOTE: pd = " << pd << '\n';

  time_t t;
  srand((unsigned)time(&t));

  int rng_loss, rng_damage, num_bytes_damaged;

  rng_loss = (rand() & 100);

  if (!(rng_loss <= (pl * 100)) || (pl == 0)) {
    rng_damage = (rand() & 100);
    if (rng_damage <= (pd * 100) && (pd != 0)) {
      num_bytes_damaged = (rand() & 10);
      if (num_bytes_damaged == 9) {
        std::cout << "NOTE: THREE BYTES DAMAGED" << '\n';
        packet_sent(pack_array);

        for (int i = 0; i < 3; i++) {
          int corrupted_byte = rand() % pack_array.size();
          int corrupted_bit = rand() % 8;
          char mask = 1 << corrupted_bit;
          pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;
        }
        packet_sent(pack_array);

        /* three bytes damaged */
      } else if ((num_bytes_damaged > 6) && (num_bytes_damaged < 9)) {
        std::cout << "NOTE: TWO BYTES DAMAGED" << '\n';

        for (int i = 0; i < 2; i++) {
          int corrupted_byte = rand() % pack_array.size();
          int corrupted_bit = rand() % 8;
          char mask = 1 << corrupted_bit;
          pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;
        }
        packet_sent(pack_array);

        /* two bytes damaged */
      } else {
        std::cout << "NOTE: ONE BYTE DAMAGED" << '\n';
        packet_sent(pack_array);
        int corrupted_byte = rand() % pack_array.size();
        int corrupted_bit = rand() % 8;
        char mask = 1 << corrupted_bit;
        pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;

        packet_sent(pack_array);
      }

      /* one byte damaged */

      /* damage occurs */
    } else {
      std::cout << "SENT NORMALLY" << '\n';
      packet_sent(pack_array);
      /* send normally */
    }
  } else {
    std::cout << "LOSS OCCURED" << '\n';
    /* loss occurs */
  }
}
