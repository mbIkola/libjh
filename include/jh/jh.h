#ifndef JH_H
#define JH_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __WEBASSEMBLY__
#   pragma message "Building libcryptonight  for webassembly arch....."
#   include "webassembly.h"
#else
#   include <string.h>
#endif /// __WEBASSEMBLY__

#ifdef __EMSCRIPTEN__
#   pragma message "Building libcryptonight using emscripten SDK....."
#   include "emscripten.h"
#else
#   define EMSCRIPTEN_KEEPALIVE
#endif

void  __attribute__((used))  __attribute__((visibility("default"))) EMSCRIPTEN_KEEPALIVE
jh256(const uint8_t input[],
        size_t input_byte_length, uint8_t output[]
);

extern void jh(unsigned bit_len, const uint8_t input[],
    size_t input_bit_length, uint8_t output[]);

#ifdef __cplusplus
}
#endif

#endif
