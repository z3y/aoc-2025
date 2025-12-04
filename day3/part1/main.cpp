#include <iostream>
#include <fstream>
#include <string>
#include <charconv>

int main()
{
    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::string line;

    int32_t sum = 0;

    while (std::getline(file, line))
    {
        int32_t last = line.length() - 1;
        int32_t firstNumber = -1;
        int32_t firstNumberIndex = -1;

        int32_t secondNumber = -1;

        for (size_t i = 0; i < line.length(); i++)
        {
            int32_t currentNumber = std::stoi(line.substr(i, 1));

            if (i != last)
            {
                if (currentNumber > firstNumber)
                {
                    firstNumber = currentNumber;
                    firstNumberIndex = i;
                }
            }
        }

        for (size_t i = firstNumberIndex + 1; i < line.length(); i++)
        {
            int32_t currentNumber = std::stoi(line.substr(i, 1));

            secondNumber = std::max(secondNumber, currentNumber);
        }

        std::printf("%d%d\n", firstNumber, secondNumber);

        sum += firstNumber * 10 + secondNumber;
    }

    std::printf("sum: %d\n", sum);

    return 0;
}