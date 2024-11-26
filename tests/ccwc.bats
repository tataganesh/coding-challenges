setup_file() {
    # Get current file directory
    WORKING_DIR="$( cd "$( dirname "$BATS_TEST_FILENAME" )" >/dev/null 2>&1 && pwd )"

    # Create temporary build directory
    TMP_DIR=$(mktemp -d --tmpdir=$WORKING_DIR)
    TMP_BUILD_DIR="$TMP_DIR/build"
    # Create build files
    cmake -S "$WORKING_DIR/../ccwc/" -B $TMP_BUILD_DIR
    # Compile
    cmake --build $TMP_BUILD_DIR
    # Set path to executable
    EXECUTABLE="$TMP_BUILD_DIR/ccwc"
    # Export variables for all tests

    export WORKING_DIR EXECUTABLE TMP_DIR
}

setup(){
    load 'test_helper/bats-support/load'
    load 'test_helper/bats-assert/load'
}

teardown_file() {
    rm -rf $TMP_DIR
}

@test "Test ALL Flags" {
    input_file_path=$WORKING_DIR/../ccwc/test.txt
    run $EXECUTABLE $input_file_path
    assert_output "7145 58164 342190 $input_file_path"
}

@test "Test line check" {
    input_file_path=$WORKING_DIR/../ccwc/test.txt
    run $EXECUTABLE -l $input_file_path
    assert_output "7145 $input_file_path"
}

# @test "can run our script" {
#     input_file_path=$WORKING_DIR/../ccwc/test.txt
#     run $EXECUTABLE_PATH $input_file_path
#     assert_output "7145 58164 342190 $input_file_path"
# }

# @test "can run our script" {
#     input_file_path=$WORKING_DIR/../ccwc/test.txt
#     run $EXECUTABLE_PATH $input_file_path
#     assert_output "7145 58164 342190 $input_file_path"
# }


### Todo

### Run using other tests cases. You can also create your own files/stdin.
### Importantly test different flags, 