#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>

int main()
{
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::string line;

    std::vector<char> operators;

    std::getline(file, line);

    for (size_t i = 0; i < line.length(); i++)
    {
        char c = line[i];
        if (c != ' ')
        {
            operators.push_back(c);

            std::printf("char: %c\n", c);
        }
    }

    std::vector<int64_t> columns(operators.size());

    for (size_t i = 0; i < columns.size(); i++)
    {
        if (operators[i] == '*')
        {
            columns[i] = 1;
        }
    }

    // this could be a little less cursed xd
    while (std::getline(file, line))
    {
        int64_t sliceStart = 0;
        int64_t sliceCount = 0;
        int64_t digitsCount = 0;

        int64_t opIndex = 0;

        for (size_t i = 0; i < line.length(); i++)
        {
            char c = line[i];
            bool last = i == line.length() - 1;
            if (std::isdigit(c) && !last)
            {
                digitsCount++;
                sliceCount++;
                continue;
            }

            if (c == ' ' || last)
            {
                sliceCount++;
            }

            if (digitsCount > 0 || last)
            {
                // std::printf("sliceStart: %d, sliceCount: %d, line: %s\n", sliceStart, sliceCount, line.c_str());

                int64_t value = std::stoll(line.substr(sliceStart, sliceCount));

                // std::printf("value: %d\n", value);

                sliceStart = sliceStart + sliceCount;
                sliceCount = 0;
                digitsCount = 0;

                char op = operators[opIndex];

                if (op == '*')
                {
                    columns[opIndex] *= value;
                }
                else if (op == '+')
                {
                    columns[opIndex] += value;
                }

                opIndex++;
            }
        }
    }

    int64_t sum = 0;
    for (size_t i = 0; i < columns.size(); i++)
    {
        std::printf("columns[i]: %lld\n", columns[i]);
        sum += columns[i];
    }

    std::printf("sum: %lld\n", sum);

    return 0;
}