# fpz

a teader-only C++ library for floating point array compression

## Features

- Lossless compression
- Header-only library (But it needs to be linked to libz)

## Supported FP
- [x] IEEE Binary32 (FP32)
- [ ] IEEE Binary64 (FP64)

## Compression method

1. Divide a floating point value into exponent part and sign+mantissa part.
2. Compress the only exponent part using zlib.

## Example

See [test_fp32.cpp](./test/test_fp32.cpp).

# License
MIT
