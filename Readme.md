#  JH as wasm

An yet another cryptographic function from cryptonight spec named JH. 

# Purpose #
static library which would be used for further iterative modifications/optimizations and cross-compiled to few other non-standard platforms, like 'browser's asm.js (see emscripten folder in project root)

# howto
`cmake .` and `make ` should work for you. CLion should autogenerate build targets if you import project from source. 

For wasm compilation you need either:
- docker installed on pc (see emscripten/makefile) to build wasm and wrappers using emscripten SDK
- npm, build-tools-essential, clang, cmake to build wasm using node-webassembly project (see package.json for details)


