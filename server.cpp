//
// Created by hrh11 on 7/22/2019.
//

#include "server.h"

using namespace std;

int main() {
    lastPacket = false;
    server.sin_family = AF_INET;
    server.sin_addr.s_adrr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT_NUM);

    serv_sd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(serv_sd, (struct sockaddr *)&server, sizeof(server));

    while(!lastPacket) {
        n = recvfrom(serv_sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&command_addr, commandLen);
        if (n > 0) {
            if (n < PACKET_PLUS_HEADER) {
                lastPacket = true;
            }
            //buf[n] = '\0';
            //recreate packet
            //check sequence number
            //check checksums

        }

    }
    close(serv_sd);
    return 0;
}