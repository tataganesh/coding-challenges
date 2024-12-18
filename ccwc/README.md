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

### Run tests using bats-core

```bash
../bats-core/bats/bin/bats test_ccwc.bats

## Expected output
test_ccwc.bats
 ✓ Test ALL Flags
 ✓ Test ALL Flags (explicit)
 ✓ Test lines
 ✓ Test bytes
 ✓ Test words
 ✓ Test words and lines
 ✓ Test words and bytes
 ✓ Test lines and bytes
 ✓ Test ALL Flags (STDIN)
 ✓ Test bytes and lines (STDIN)
 ✓ Test lines (STDIN)
```

### 

For more information about the command-line arguments, run `./build/ccwc  --help`. 


### External Code Review(s)

* [Basic C++ implementation of linux wc command](https://codereview.stackexchange.com/questions/294364/basic-c-implementation-of-linux-wc-command) - Code Review Stack Exchange


### Todos

* [ ] Add CI using github actions, running bats tests
* [ ] Bug: Handle trailing space in output with STDIN input