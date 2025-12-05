#include <iostream>
#include <fstream>
#include <string>
#include <charconv>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>

constexpr int32_t kWidth = 140;
constexpr int32_t kHeight = 140;

void SaveAsPNG(const bool *data, int width, int height, const char *filename)
{
    std::vector<unsigned char> pixels(width * height);

    for (int i = 0; i < width * height; i++)
        pixels[i] = data[i] ? 255 : 0;

    stbi_write_png(filename, width, height, 1, pixels.data(), width);
}

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

    int32_t iteration = 0;
    while (true)
    {
        bool rollsToRemove[kWidth * kHeight] = {};
        int32_t previousAccessible = accessible;

        for (int32_t x = 0; x < kWidth; x++)
        {
            for (int32_t y = 0; y < kHeight; y++)
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
                        rollsToRemove[Get1DIndex(x, y)] = true;
                        // std::printf("x");
                    }
                    else
                    {
                        // std::printf("@");
                    }
                }
                else
                {
                    // std::printf(".");
                }
            }

            // std::printf("\n");
        }

        if (previousAccessible == accessible)
        {
            break;
        }

        std::string filename = "frame_" + std::to_string(iteration) + ".png";

        SaveAsPNG(rolls, kWidth, kHeight, filename.c_str());

        std::printf("removing %d rolls\n", accessible - previousAccessible);

        for (int32_t x = 0; x < kWidth; x++)
        {
            for (int32_t y = 0; y < kHeight; y++)
            {
                int32_t i = Get1DIndex(x, y);
                bool center = rollsToRemove[i];
                if (center)
                {
                    rolls[i] = false;
                }
            }
        }

        iteration++;
    }

    std::printf("accessible: %lld", accessible);

    return 0;
}