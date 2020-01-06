# Questions

## What's `stdint.h`?

`stdint.h` - is the C language header file; defines the set of integer types, which are more specific then just simple `int` type.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

As we're going to read the `bmp` file, it is important to have proper `struct`s. A file uses Microsoft typing for it's contents. Using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` allow to define respective types (size and sign usage: signed or unsigned) and read correct amount of data from file.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

`BYTE`: 1;
`DWORD`: 4; The range is 0 through 4294967295 decimal.
`LONG`: 4;  The range is -2147483648 through 2147483647 decimal.
`WORD`: 2; The range is 0 through 65535 decimal.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes of any BMP file must be `BM`, which is stands for bit map.

## What's the difference between `bfSize` and `biSize`?

`bfSize` - is in BITMAPFILEHEADER and reflects the size of a bitmap file in bytes;
`biSize` - is in BITMAPINFOHEADER and reflects the number of bytes required by the BITMAPINFOHEADER structure;

## What does it mean if `biHeight` is negative?

Negative `biHeight` means that origin of the bitmap is at the top: upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

A color depth is specified in `biBitCount` field.

## Why might `fopen` return `NULL` in `copy.c`?

E.g. the file does not exist.

## Why is the third argument to `fread` always `1` in our code?

As we're using structures from `bmp.h` file to read data from a bitmap file it is handy to read one structure per `fread` invocation. This is what the third argument of the `fread` is for.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

`padding` will take the value of `3`.

## What does `fseek` do?

Moves the file pointer to a given location.

## What is `SEEK_CUR`?

`SEEK_CUR` is the `stdio.h` macros, defined for `fseek` function, which moves the file pointer to a given location.
