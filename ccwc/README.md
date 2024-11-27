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


### External Code Review(s)

* [Basic C++ implementation of linux wc command](https://codereview.stackexchange.com/questions/294364/basic-c-implementation-of-linux-wc-command) - Code Review Stack Exchange


### Todos

* [ ] Add documentation for testing with bats
* [ ] Add CI using github actions, running bats tests
* [ ] Bug: Handle trailing space in output with STDIN input