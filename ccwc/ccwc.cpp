#include <CLI/CLI.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <cwchar>
struct Text_Counts
{
    int character_count{0};
    int line_count{0};
    int word_count{0};
};

std::string to_string_custom(int count)
{
    if (count == 0)
        return "";
    return std::to_string(count) + " ";
}

std::istream &get_input_stream(const std::string &filepath)
{
    if (!filepath.empty())
    {
        static std::ifstream input_file(filepath);
        if (!input_file)
        {
            throw std::runtime_error("Could not open file: " + filepath);
        }
        return input_file;
    }
    else
    {
        return std::cin;
    }
}
Text_Counts get_text_counts(std::istream &input_stream, bool count_multibytechar_flag, bool count_char_flag, bool count_line_flag, bool count_word_flag)
{
    std::mbstate_t state = std::mbstate_t(); // initial state for mbrtowc
    // If none of the flags are passed, use all flags
    if (!(count_multibytechar_flag || count_char_flag || count_line_flag || count_word_flag))
    {
        count_char_flag = count_word_flag = count_line_flag = true;
    }
    Text_Counts counts;
    std::string line, word;
    // Iterate through each line of input stream
    while (std::getline(input_stream, line))
    {
        if (count_line_flag)
        {
            ++counts.line_count;
        }
        if (count_char_flag || count_multibytechar_flag)
        {
            if (count_multibytechar_flag)
            {
                // Count multi-byte characters
                const char *ptr = line.c_str();
                const char *end = ptr + std::strlen(ptr);
                int len;
                wchar_t wc;
                int index = 0;
                while ((len = std::mbrtowc(&wc, ptr, end - ptr, &state)) > 0)
                {
                    ptr += len;
                    ++counts.character_count;
                }
                ++counts.character_count;
            }
            else
            {
                counts.character_count += line.length() + 1;
            }
        }

        if (count_word_flag)
        {
            std::stringstream ss(line);
            while (ss >> word)
            {
                ++counts.word_count;
            }
        }
    }
    return counts;
}

int main(int argc, char *argv[])
{

    std::setlocale(LC_ALL, ""); // For multi-byte char counting
    CLI::App app{"wc command to count characters, word and lines"};
    argv = app.ensure_utf8(argv);

    bool count_char_flag{false};
    app.add_flag("-c", count_char_flag, "Count Characters")->take_last();
    bool count_multibytechar_flag{false};
    app.add_flag("-m", count_multibytechar_flag, "Count Multi-Byte Characters")->take_last();
    bool count_line_flag{false};
    app.add_flag("-l", count_line_flag, "Count Lines")->take_last();
    bool count_word_flag{false};
    app.add_flag("-w", count_word_flag, "Count Words")->take_last();

    std::string filepath;
    app.add_option("filepath", filepath, "Input file path");
    CLI11_PARSE(app, argc, argv);
    std::istream &input_stream = get_input_stream(filepath);
    Text_Counts output = get_text_counts(input_stream, count_multibytechar_flag, count_char_flag, count_line_flag, count_word_flag);
    std::string output_string = to_string_custom(output.line_count) + to_string_custom(output.word_count) + to_string_custom(output.character_count) + filepath;
    std::cout << output_string;
    return 0;
}
