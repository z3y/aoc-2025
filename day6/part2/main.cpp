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

    int width = 0;
    int height = 0;

    std::vector<char> nums;
    while (std::getline(file, line))
    {
        for (size_t i = 0; i < line.length(); i++)
        {
            nums.push_back(line[i]);
        }

        height++;
        width = line.length();
    }

    int64_t col = 0;
    char op = ' ';

    int64_t sum = 0;

    for (size_t x = 0; x < width; x++)
    {
        std::string line;

        for (size_t y = 0; y < height; y++)
        {
            int32_t index = x + y * width;

            char c = nums[index];

            if (std::isdigit(c))
            {
                line += c;
            }
            else if (c == '*' || c == '+')
            {
                std::printf("%c\n", c);
                op = c;

                std::printf("col: %lld\n", col);
                sum += col;

                col = op == '*' ? 1 : 0;
            }
        }

        if (line.empty())
        {
            continue;
        }

        int64_t num = std::stoll(line);

        if (op == '*')
        {
            col *= num;
        }
        else if (op == '+')
        {
            col += num;
        }

        std::printf("num: %lld\n", num);
    }

    sum += col;

    std::printf("sum: %lld\n", sum);

    return 0;
}