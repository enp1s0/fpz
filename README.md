# fpz

A header-only C++ library for floating point array compression

## Features

- Lossless compression
- Header-only library (But it needs to be linked to libz)

## Supported FP
- [x] IEEE Binary32 (FP32)
- [x] IEEE Binary64 (FP64)

## Compression method

1. Divide a floating point value into exponent part and sign+mantissa part.
2. Compress the only exponent part using zlib.

## Example

See [compression_decompress.cpp](./test/compress_decompress.cpp).

## License
MIT
