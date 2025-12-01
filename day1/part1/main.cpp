#include <iostream>
#include <fstream>
#include <string>

int main()
{

    std::ifstream file("input.txt");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    int32_t dial = 50;

    int32_t zeroTimes = 0;

    std::string line;
    while (std::getline(file, line))
    {
        bool left = line[0] == 'L';

        int32_t value = std::stoi(line.substr(1));

        if (left)
        {
            dial -= value % 100;
        }
        else
        {
            dial += value % 100;
        }

        if (dial < 0)
        {
            dial = 100 + dial;
        }
        if (dial >= 100)
        {
            dial = dial - 100;
        }

        zeroTimes += dial == 0;

        std::printf("dial: %d\n", dial);
    }

    std::printf("zeroTimes: %d\n", zeroTimes);

    return 0;
}