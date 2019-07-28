#include "client.h"

using namespace std;

void gremlin(double a, double b);

int main(int argc, char *argv[]) {
  double pl, pd;
  if (argc >= 2) {
    pl = atof(argv[1]);
    pd = atof(argv[2]);
  } else {
    pl = 0.0;
    pd = 0.0;
  }
  gremlin(pl, pd);
  return 0;
}

/**
 *@function gremlin()
 *debugging operations commented out
 */
void gremlin(/*prepare_packet pack,*/ double loss_probability,
             double damage_probability) {

  vector<char> pack_array = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                             'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                             'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

  double pl = loss_probability;
  double pd = damage_probability;

  std::cout << "DEBUG: PL = " << pl << '\n';
  std::cout << "DEBUG: PD = " << pd << '\n';

  time_t t;
  srand((unsigned)time(&t));

  int rng_loss, rng_damage, num_bytes_damaged;

  rng_loss = (rand() & 100);

  // for (int k = 0; k < pack_array.size(); k++) {
  //  std::cout << pack_array.at(k) << ' ';
  //  }

  if (!(rng_loss <= (pl * 100)) || (pl == 0)) {
    rng_damage = (rand() & 100);
    if (rng_damage <= (pd * 100) && (pd != 0)) {
      num_bytes_damaged = (rand() & 10);
      if (num_bytes_damaged == 9) {
        std::cout << "DEBUG: THREE BYTES DAMAGED" << '\n';

        for (int i = 0; i < 3; i++) {
          int corrupted_byte = rand() % pack_array.size();
          int corrupted_bit = rand() % 8;
          char mask = 1 << corrupted_bit;
          pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;
        }

        // for (int q = 0; q < pack_array.size(); q++) {
        //  std::cout << pack_array.at(q) << ' ';
        //  }

        /* three bytes damaged */
      } else if ((num_bytes_damaged > 6) && (num_bytes_damaged < 9)) {
        std::cout << "DEBUG: TWO BYTES DAMAGED" << '\n';

        for (int i = 0; i < 2; i++) {
          int corrupted_byte = rand() % pack_array.size();
          int corrupted_bit = rand() % 8;
          char mask = 1 << corrupted_bit;
          pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;
        }

        // for (int p = 0; p < pack_array.size(); p++) {
        //  std::cout << pack_array.at(p) << ' ';
        //    }

        /* two bytes damaged */
      } else {
        std::cout << "DEBUG: ONE BYTE DAMAGED" << '\n';
        int corrupted_byte = rand() % pack_array.size();
        int corrupted_bit = rand() % 8;
        char mask = 1 << corrupted_bit;
        pack_array[corrupted_byte] = pack_array[corrupted_byte] ^ mask;

        //  for (int i = 0; i < pack_array.size(); i++) {
        //    std::cout << pack_array.at(i) << ' ';
        //  }

        /* one byte damaged */
      }

      /* damage occurs */
    } else {
      std::cout << "DEBUG: SENT NORMALLY" << '\n';

      // for (int m = 0; m < pack_array.size(); m++) {
      //  std::cout << pack_array.at(m) << ' ';
      //  }

      /* send normally */
    }
  } else {
    std::cout << "DEBUG: LOSS OCCURED" << '\n';
    /* loss occurs */
  }
}
