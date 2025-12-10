#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <format>
#include <bitset>
#include <numeric>

struct Machine
{
    uint32_t indicator;
    std::vector<uint32_t> buttons;
};

bool NextCombination(std::vector<int> &indicies, int combinations, int elements)
{
    for (int i = combinations - 1; i >= 0; --i)
    {
        int maxVal = elements - (combinations - i);
        if (indicies[i] < maxVal)
        {
            indicies[i]++;

            for (int j = i + 1; j < combinations; ++j)
            {
                indicies[j] = indicies[j - 1] + 1;
            }

            return true;
        }
    }

    return false;
}

int main()
{
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    std::string line;

    std::vector<Machine> machines;

    while (std::getline(file, line))
    {
        Machine machine;
        uint32_t indicator = 0;
        std::vector<uint32_t> buttons;

        size_t i = 1;
        for (; i < line.length(); i++)
        {
            char c = line[i];

            if (c == ']')
            {
                break;
            }
            else if (c == '#')
            {
                uint32_t bit = i - 1;
                indicator ^= 0x1 << bit;
            }
        }

        for (; i < line.length(); i++)
        {
            char c = line[i];

            if (c == '{')
            {
                break;
            }
            else if (c == '(')
            {
                buttons.push_back(0);
            }
            else if (c == ',' || c == ')')
            {
                uint32_t bit = line[i - 1] - '0';
                // std::printf("bit: %d\n", bit);
                buttons.back() ^= 0x1 << bit;
            }
        }

        // for (size_t i = 0; i < buttons.size(); i++)
        // {
        //     std::cout << "button: " << std::bitset<32>(buttons[i]) << std::endl;
        // }

        machine.indicator = indicator;
        machine.buttons = buttons;

        // std::cout << "indicator: " << std::bitset<32>(indicator) << std::endl;
        machines.push_back(machine);
    }

    size_t totalPresses = 0;

    for (auto &machine : machines)
    {
        size_t iterations = 0;
        auto indicator = machine.indicator;
        auto buttons = machine.buttons;

        size_t combinations = 1;

        while (combinations < buttons.size())
        {
            bool found = false;

            std::vector<int> indicies(combinations);
            std::iota(indicies.begin(), indicies.end(), 0);

            do
            {
                iterations++;
                uint32_t result = 0;

                for (int i : indicies)
                {
                    result ^= buttons[i];
                }

                if (indicator == result)
                {
                    found = true;
                    break;
                }

            } while (NextCombination(indicies, combinations, buttons.size()));

            if (found)
            {
                break;
            }

            combinations++;
        }

        totalPresses += combinations;
        std::cout << "indicator: " << std::bitset<32>(indicator) << " combinations: " << combinations << " iterations: " << iterations << std::endl;
    }

    std::printf("total presses: %ld\n", totalPresses);

    return 0;
}