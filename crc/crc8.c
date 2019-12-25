#include <stdio.h>
#include <stdint.h>

// Polynomial representations
#define PR_CRC8_ATM 0x07 // CRC8-ATM: x8 + x2 + x + 1

static uint8_t crc8(const void *data, size_t size) {
    uint8_t *p = (uint8_t *)data;
    uint8_t crc8 = 0x00;

    for (size_t s = size; s > 0 ; s--){
        crc8 ^= *p++;
        for (int i = 0 ; i < sizeof(uint8_t) * 8; i++){
            crc8 <<= 1;
            if (crc8 & 0x80){
                crc8 ^= PR_CRC8_ATM;
            }
        }
    }
    return crc8;
}

int main () {
    uint16_t id = 15;
    // ビッグエンディアンにするなら id = htos(id); などとする
    uint8_t crc = crc8(&id, sizeof(uint16_t));
    printf("%d, %d\n", id, crc);
    printf("%x, %x\n", id, crc);
    return 0;
}
