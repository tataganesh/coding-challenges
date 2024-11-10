## ccwc

Count words, lines and character (single byte or multi-byte) in a text file or standard input. 

### Build ccwc

```bash
cmake -S . -B build
cmake --build build
```

### Run ccwc

```bash
./build/ccwc  test.text # With file input
cat test.txt | /path/to/file.txt # With STDIN

## Expected output
## 7145 58164 342190 test.txt
```

For more information about the command-line arguments, run `./build/ccwc  --help`. 