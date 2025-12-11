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

#include "z3++.h"

struct Machine
{
    uint32_t indicator;
    std::vector<uint32_t> buttons;
    std::vector<uint32_t> joltages;
};

bool NextCombination(std::vector<int> &indicies, int elements)
{
    int combinations = indicies.size();
    for (int i = combinations - 1; i >= 0; i--)
    {
        int maxVal = elements - (combinations - i);
        if (indicies[i] < maxVal)
        {
            indicies[i]++;

            for (int j = i + 1; j < combinations; j++)
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
        std::vector<uint32_t> joltages;

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

        i++;

        size_t index = i;
        size_t count = 0;

        for (; i < line.length(); i++)
        {
            char c = line[i];

            if (c == '}' || c == ',')
            {
                i++;
                uint32_t joltage = std::stoi(line.substr(index, count));
                joltages.push_back(joltage);

                index = i;
                count = 0;

                if (c == '}')
                {
                    break;
                }
            }
            count++;
        }

        machine.indicator = indicator;
        machine.buttons = buttons;
        machine.joltages = joltages;

        machines.push_back(machine);
    }

    // no clue what im doing down here

    z3::context ctx;
    auto z3zero = ctx.int_val(0);

    int32_t accumulator = 0;

    for (Machine &machine : machines)
    {
        z3::optimize optimizer = z3::optimize(ctx);

        z3::expr_vector buttonPresses(ctx);
        for (size_t i = 0; i < machine.buttons.size(); i++)
        {
            auto button = ctx.int_const(("b" + std::to_string(i)).c_str());
            buttonPresses.push_back(button);
            auto positiveConstraint = (button >= z3zero);
            optimizer.add(positiveConstraint);
        }

        for (size_t joltIndex = 0; joltIndex < machine.joltages.size(); joltIndex++)
        {
            z3::expr_vector joltContributions(ctx);

            for (size_t buttonIndex = 0; buttonIndex < machine.buttons.size(); buttonIndex++)
            {
                if ((machine.buttons[buttonIndex] & (0x1 << joltIndex)) != 0)
                {
                    joltContributions.push_back(buttonPresses[buttonIndex]);
                }
            }

            z3::expr targetJolt = ctx.int_val(machine.joltages[joltIndex]);

            z3::expr sum = ctx.int_val(0);
            for (auto c : joltContributions)
            {
                sum = sum + c;
            }

            z3::expr joltConstraint = sum == targetJolt;
            optimizer.add(joltConstraint);
        }

        z3::expr sumToMinimize = ctx.int_val(0);
        for (auto b : buttonPresses)
        {
            sumToMinimize = sumToMinimize + b;
        }

        z3::optimize::handle optimizationResult = optimizer.minimize(sumToMinimize);

        if (optimizer.check() != z3::sat)
        {
            throw std::runtime_error("unsatisfiable");
        }

        z3::model m = optimizer.get_model();
        z3::expr minimizedSum = m.eval(sumToMinimize, true);

        int64_t value = minimizedSum.as_int64();

        accumulator += value;

        std::cout << "minimized sum = " << value << std::endl;
    }

    std::printf("totalPresses: %d\n", accumulator);

    return 0;
}