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

        int32_t rotation = std::stoi(line.substr(1));

        int32_t rotationCount = 0;

        if (left)
        {
            if (dial - rotation < 0)
            {
                rotationCount = abs(dial - rotation) / 100 + (dial > 0);
            }
            dial -= rotation;
            if (dial == 0)
            {
                rotationCount++;
            }
        }
        else
        {
            rotationCount = (dial + rotation) / 100;
            dial += rotation;
        }

        dial %= 100;
        if (dial < 0)
        {
            dial = 100 + dial;
        }

        zeroTimes += rotationCount;
        std::printf("%s, dial: %d, rotationCount: %d\n", line.c_str(), dial, rotationCount);
    }

    std::printf("zeroTimes: %d\n", zeroTimes);

    return 0;
}