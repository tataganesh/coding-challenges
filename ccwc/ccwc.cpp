#include "CLI/CLI.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <cwchar>
#include <cstdlib>
struct Text_Counts
{
    int bytes{0};
    int characters{0};
    int lines{0};
    int words{0};
};

struct Text_Count_Flags
{
    bool bytes{false};
    bool lines{false};
    bool words{false};
    bool characters{false};

    bool no_flags_set()
    {
        return !(bytes || lines || words || characters);
    }
};

std::string counts_to_string(const Text_Counts &counts, const Text_Count_Flags &count_flags)
{
    std::string output_string;

    if (count_flags.lines)
    {
        output_string += std::to_string(counts.lines) + " ";
    }

    if (count_flags.words)
    {
        output_string += std::to_string(counts.words) + " ";
    }

    if (count_flags.bytes)
    {
        output_string += std::to_string(counts.bytes) + " ";
    }
    if (count_flags.characters)
    {
        output_string += std::to_string(counts.characters) + " ";
    }
    return output_string;
}

std::istream &get_input_stream(const std::string &filepath)
{
    if (!filepath.empty())
    {
        static std::ifstream input_file(filepath);
        input_file.exceptions(std::ifstream::badbit);
        if (!input_file.is_open())
        {
            std::cerr << "Failed to open file " << filepath << ". Exiting program..." << std::endl;
            std::exit(EXIT_FAILURE); // Todo: Find a better way to handle this error
        }
        return input_file;
    }
    else
    {
        // return stdin stream if filepath is empty
        return std::cin;
    }
}

int count_multibyte_characters(const std::string &str)
{
    std::mbstate_t state = std::mbstate_t(); // initial state for mbrtowc
    int character_count = 0;
    const char *ptr = str.c_str();
    const char *end = ptr + str.length();
    int len;
    wchar_t wc;
    int index = 0;
    while ((len = std::mbrtowc(&wc, ptr, end - ptr, &state)) > 0)
    {
        ptr += len;
        ++character_count;
    }
    ++character_count; // For \n?
    return character_count;
}

int count_words(const std::string str)
{
    std::string word;
    std::stringstream ss(str);
    int word_count = 0;
    while (ss >> word)
    {
        ++word_count;
    }
    return word_count;
}

Text_Counts get_text_counts(std::istream &input_stream, const Text_Count_Flags &count_flags)
{
    Text_Counts counts;
    std::string line;

    // Iterate through each line of input stream
    while (std::getline(input_stream, line))
    {
        if (count_flags.lines)
        {
            ++counts.lines;
        }

        if (count_flags.characters)
        {
            counts.characters += count_multibyte_characters(line);
        }

        if (count_flags.bytes)
        {
            counts.bytes += line.length() + 1;
        }

        if (count_flags.words)
        {
            counts.words += count_words(line);
        }
    }
    return counts;
}

int main(int argc, char *argv[])
{

    std::setlocale(LC_ALL, ""); // For multi-byte char counting
    CLI::App app{"wc command to count characters, word and lines"};
    argv = app.ensure_utf8(argv);

    Text_Count_Flags count_flags;
    app.add_flag("-c", count_flags.bytes, "Count Bytes")->take_last();
    app.add_flag("-m", count_flags.characters, "Count Characters")->take_last();
    app.add_flag("-l", count_flags.lines, "Count Lines")->take_last();
    app.add_flag("-w", count_flags.words, "Count Words")->take_last();

    std::string filepath;
    app.add_option("filepath", filepath, "Input file path");
    CLI11_PARSE(app, argc, argv);

    // If none of the flags are passed, use all flags
    if (count_flags.no_flags_set())
    {
        count_flags.bytes = count_flags.words = count_flags.lines = true;
    }
    std::istream &input_stream = get_input_stream(filepath);
    Text_Counts counts = get_text_counts(input_stream, count_flags);
    std::string output_string = counts_to_string(counts, count_flags) + filepath;
    std::cout << output_string;
    return EXIT_SUCCESS;
}
