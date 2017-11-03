#ifndef JH_H
#define JH_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


extern __attribute__((visibility ("default"))) void jh256(const uint8_t input[],
        size_t input_byte_length, uint8_t output[]
);

extern void jh(unsigned bit_len, const uint8_t input[],
    size_t input_bit_length, uint8_t output[]);

#ifdef __cplusplus
}
#endif

#endif
