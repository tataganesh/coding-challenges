#include <CLI/CLI.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <filesystem>
#include <cwchar>
namespace fs = std::filesystem;



std::string to_string_custom(int count)
{
    if (count == 0)
        return "";
    return std::to_string(count) + " ";
}



int countBytesInFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return -1;
    }

    // Set the locale to the user's default locale, which should handle multibyte characters
    std::locale::global(std::locale(""));

    // Read the entire file into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Count the bytes in the string
    int byteCount = 0;
    std::mbstate_t state = std::mbstate_t();
    const char* ptr = content.c_str();
    const char* end = ptr + content.size();
    wchar_t wc;

    while (ptr < end) {
        int len = std::mbrtowc(&wc, ptr, end - ptr, &state);
        if (len < 0) {
            std::cerr << "Invalid multibyte sequence encountered." << std::endl;
            return -1;
        } else if (len == 0) {
            break; // Reached the end of the string
        }
        ptr += len;
        byteCount += len;
    }

    return byteCount;
}

std::string get_text_counts(const std::string &filepath, bool count_char_flag, bool count_line_flag, bool count_word_flag)
{
            std::mbstate_t state = std::mbstate_t(); // initial state
    // If none of the flags are passed, use all flags
    if (!(count_char_flag || count_line_flag || count_word_flag))
    {
        count_char_flag = count_word_flag = count_line_flag = true;
    }
    // Read the file into memory. TODO: What are more efficient ways?
    std::ifstream  input_file(filepath);
    if (!input_file) {
        std::cout << "Unable to open file "<< filepath <<". Exiting...";
        exit(1); // terminate with error
    }
    int character_count = 0, line_count = 0, word_count = 0;
    int mb_c = 0;
    std::string line, word;
    while (std::getline(input_file, line))
    {
        if (count_line_flag)
            line_count += 1;
        if (count_char_flag)
        {
            // character_count = character_count + line.length() + 1;

            // Count multi-byte characters
            const char *ptr = line.c_str();
            const char* end = ptr + std::strlen(ptr);
            int len;
            wchar_t wc;
            int index = 0;
            while ((len = std::mbrtowc(&wc, ptr, end-ptr, &state)) > 0)
            {
                index += len;
                ptr += len;
                character_count += 1;
            }
            character_count += 1;
        }

        if (count_word_flag)
        {
            bool found_word = false;
            std::stringstream ss(line);
            while (ss >> word) {
                ++word_count;
            }
        }


    } 
    std::cout << countBytesInFile(filepath) << std::endl;
    std::cout << mb_c<<std::endl;
    std::string output = to_string_custom(character_count) + to_string_custom(line_count)  + to_string_custom(word_count) + filepath;
    return output;
}


int main(int argc, char* argv[])
{

    std::setlocale(LC_ALL, "");

    std::wcout << L"User-preferred locale setting is "
               << std::locale("").name().c_str() << L'\n';
    CLI::App app{"wc command to count characters, word and lines"};
    argv = app.ensure_utf8(argv);


    // Todo: Take last is not working as expected
    bool count_char_flag{false};
    app.add_flag("-c", count_char_flag, "Count Characters")->take_last();
    bool count_line_flag{false};
    app.add_flag("-l", count_line_flag, "Count Lines")->take_last();
    bool count_word_flag{false};
    app.add_flag("-w", count_word_flag, "Count Words")->take_last();

    std::string filepath = "";
    app.add_option("filename", filepath, "Count Characters")->required();
    CLI11_PARSE(app, argc, argv);
    std::string output = get_text_counts(filepath, count_char_flag, count_line_flag, count_word_flag);
    std::cout << output;

    


    return 0;
}

