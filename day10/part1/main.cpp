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

struct Machine
{
    uint32_t indicator;
    std::vector<uint32_t> buttons;
};

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
        auto indicator = machine.indicator;
        auto buttons = machine.buttons;
        size_t fewestPresses = 10;

        std::sort(buttons.begin(), buttons.end());
        do
        {
            uint32_t result = 0;

            for (size_t i = 0; i < buttons.size(); i++)
            {
                result ^= buttons[i];

                if (indicator == result)
                {
                    fewestPresses = std::min(fewestPresses, i + 1);
                    break;
                }
            }

            if (fewestPresses == 1)
            {
                // std::cout << "button:    " << std::bitset<32>(buttons[0]) << std::endl;
                break;
            }

        } while (std::next_permutation(buttons.begin(), buttons.end()));

        totalPresses += fewestPresses;
        std::cout << "indicator: " << std::bitset<32>(indicator) << " presses: " << fewestPresses << std::endl;
    }

    std::printf("total presses: %ld\n", totalPresses);

    return 0;
}