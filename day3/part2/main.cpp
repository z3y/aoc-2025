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

    int64_t sum = 0;

    while (std::getline(file, line))
    {
        int64_t last = line.length() - 1;

        int64_t digits = 12;
        int64_t numbers[12];
        int64_t indicies[12];
        std::fill(std::begin(numbers), std::end(numbers), -1);
        std::fill(std::begin(indicies), std::end(indicies), -1);

        int64_t lastIndex = -1;

        while (digits != 0)
        {
            int64_t numberIndex = 12 - digits;
            int64_t mult = std::pow(10, digits - 1);

            for (int64_t i = 0; i < line.length() - digits + 1; i++)
            {
                int64_t currentNumber = std::stoi(line.substr(i, 1)) * mult;

                if (numbers[numberIndex] < currentNumber)
                {
                    if (i > lastIndex)
                    {
                        numbers[numberIndex] = currentNumber;
                        indicies[numberIndex] = i;
                        lastIndex = i;
                    }
                }
            }

            digits--;
        }

        for (size_t i = 0; i < 12; i++)
        {
            std::printf("%lld ", indicies[i]);
        }
        std::printf("\n");

        for (size_t i = 0; i < 12; i++)
        {
            int64_t mult = std::pow(10, 12 - i - 1);
            std::printf("%lld", numbers[i] / mult);
        }
        std::printf("\n");

        for (size_t i = 0; i < 12; i++)
        {
            sum += numbers[i];
        }
    }

    std::printf("sum: %lld\n", sum);

    return 0;
}