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

    # Set path to test file
    TEST_FILE="$WORKING_DIR/../ccwc/test.txt"

    # Export variables for all tests
    export WORKING_DIR EXECUTABLE TMP_DIR TEST_FILE
}

setup(){
    load 'test_helper/bats-support/load'
    load 'test_helper/bats-assert/load'
}

teardown_file() {
    rm -rf $TMP_DIR
}

## All flags tests
@test "Test ALL Flags" {
    run $EXECUTABLE $TEST_FILE
    assert_output "7145 58164 342190 $TEST_FILE"
}

@test "Test ALL Flags (explicit)" {
    run $EXECUTABLE -l -c -w $TEST_FILE
    assert_output "7145 58164 342190 $TEST_FILE"
}

## Single flag tests
@test "Test lines" {
    run $EXECUTABLE -l $TEST_FILE
    assert_output "7145 $TEST_FILE"
}

@test "Test bytes" {
    run $EXECUTABLE -c $TEST_FILE
    assert_output "342190 $TEST_FILE"
}

@test "Test words" {
    run $EXECUTABLE -w $TEST_FILE
    assert_output "58164 $TEST_FILE"
}

## Two flag tests
@test "Test words and lines" {
    run $EXECUTABLE -w -l $TEST_FILE
    assert_output "7145 58164 $TEST_FILE"
}

@test "Test words and bytes" {
    run $EXECUTABLE -w -c $TEST_FILE
    assert_output "58164 342190 $TEST_FILE"
}

@test "Test lines and bytes" {
    run $EXECUTABLE -c -l $TEST_FILE
    assert_output "7145 342190 $TEST_FILE"
}

## STDIN tests
# bats test_tags=stdin
@test "Test ALL Flags (STDIN)" {
    run bash -c "$EXECUTABLE < $TEST_FILE"
    assert_output "7145 58164 342190 "
}

# bats test_tags=stdin
@test "Test bytes and lines (STDIN)" {
    run bash -c "$EXECUTABLE -c -l < $TEST_FILE"
    assert_output "7145 342190 "
}

# bats test_tags=stdin
@test "Test lines (STDIN)" {
    run bash -c "$EXECUTABLE -l < $TEST_FILE"
    assert_output "7145 "
}

