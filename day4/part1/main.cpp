#include <iostream>
#include <fstream>
#include <string>
#include <charconv>

constexpr int32_t kWidth = 140;
constexpr int32_t kHeight = 140;

int32_t Get1DIndex(int32_t x, int32_t y)
{
    if (x < 0 || x >= kWidth)
    {
        return -1;
    }
    if (y < 0 || y >= kHeight)
    {
        return -1;
    }
    return y + x * kWidth;
}

bool HasRoll(bool rolls[], int32_t x, int32_t y)
{
    int32_t i = Get1DIndex(x, y);
    return i > 0 ? rolls[i] : false;
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

    bool rolls[kWidth * kHeight];

    int row = 0;
    while (std::getline(file, line))
    {
        for (size_t i = 0; i < line.length(); i++)
        {
            char c = line[i];

            size_t index1D = Get1DIndex(row, i);

            rolls[index1D] = c == '@';
        }

        row++;
    }

    int32_t accessible = 0;
    for (int32_t y = 0; y < kHeight; y++)
    {
        for (int32_t x = 0; x < kWidth; x++)
        {
            bool center = HasRoll(rolls, x, y);

            bool u = HasRoll(rolls, x, y + 1);
            bool d = HasRoll(rolls, x, y - 1);

            bool r = HasRoll(rolls, x + 1, y);
            bool l = HasRoll(rolls, x - 1, y);

            bool ul = HasRoll(rolls, x - 1, y + 1);
            bool ur = HasRoll(rolls, x + 1, y + 1);

            bool dl = HasRoll(rolls, x - 1, y - 1);
            bool dr = HasRoll(rolls, x + 1, y - 1);

            if (center)
            {
                if (u + d + r + l + ul + ur + dl + dr < 4)
                {
                    accessible++;
                    std::printf("x");
                }
                else
                {
                    std::printf("@");
                }
            }
            else
            {
                std::printf(".");
            }
        }

        std::printf("\n");
    }

    std::printf("accessible: %lld", accessible);

    return 0;
}