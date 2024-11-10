#include <CLI/CLI.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <cwchar>
struct Text_Counts
{
    int characters{0}, lines{0}, words{0};
};

struct Text_Count_Flags
{
    bool characters{false}, lines{false}, words{false}, multibyte_characters{false};
    bool no_flags_set()
    {
        return !(characters || lines || words || multibyte_characters);
    }
};

std::string counts_to_string(Text_Counts counts, const Text_Count_Flags &count_flags)
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
    if (count_flags.characters || count_flags.multibyte_characters)
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
        if (!input_file)
        {
            throw std::runtime_error("Could not open file: " + filepath);
        }
        return input_file;
    }
    else
    {
        // return stdin stream if filepath is empty
        return std::cin;
    }
}
Text_Counts get_text_counts(std::istream &input_stream, const Text_Count_Flags &count_flags)
{
    std::mbstate_t state = std::mbstate_t(); // initial state for mbrtowc
    Text_Counts counts;
    std::string line, word;
    // Iterate through each line of input stream
    while (std::getline(input_stream, line))
    {
        if (count_flags.lines)
        {
            ++counts.lines;
        }
        if (count_flags.characters || count_flags.multibyte_characters)
        {
            if (count_flags.multibyte_characters)
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
                    ++counts.characters;
                }
                ++counts.characters;
            }
            else
            {
                counts.characters += line.length() + 1;
            }
        }

        if (count_flags.words)
        {
            std::stringstream ss(line);
            while (ss >> word)
            {
                ++counts.words;
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

    Text_Count_Flags count_flags;
    app.add_flag("-c", count_flags.characters, "Count Characters")->take_last();
    app.add_flag("-m", count_flags.multibyte_characters, "Count Multi-Byte Characters")->take_last();
    app.add_flag("-l", count_flags.lines, "Count Lines")->take_last();
    app.add_flag("-w", count_flags.words, "Count Words")->take_last();

    std::string filepath;
    app.add_option("filepath", filepath, "Input file path");
    CLI11_PARSE(app, argc, argv);

    // If none of the flags are passed, use all flags
    if (count_flags.no_flags_set())
    {
        count_flags.characters = count_flags.words = count_flags.lines = true;
    }
    std::istream &input_stream = get_input_stream(filepath);
    Text_Counts counts = get_text_counts(input_stream, count_flags);
    std::string output_string = counts_to_string(counts, count_flags) + filepath;
    std::cout << output_string;
    return 0;
}
