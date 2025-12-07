#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>

bool BoundsCheck(int32_t x, int32_t y, int32_t width, int32_t height)
{
    if (x < 0 || x >= width)
    {
        return false;
    }
    if (y < 0 || y >= height)
    {
        return false;
    }

    return true;
}

struct HitInfo
{
    int32_t x;
    int32_t y;
};

void Trace(std::vector<HitInfo> &hits, std::vector<bool> pixels, int32_t x, int32_t y, int32_t width, int32_t height)
{
    if (!BoundsCheck(x, y, width, height))
    {
        return;
    }

    int32_t index = x + y * width;

    bool hit = pixels[index];

    if (hit)
    {
        HitInfo newHit;
        newHit.x = x;
        newHit.y = y;

        for (size_t i = 0; i < hits.size(); i++)
        {
            HitInfo hit = hits[i];
            if (hit.x == newHit.x && hit.y == newHit.y)
            {
                // already hit
                return;
            }
        }

        hits.push_back(newHit);

        // std::printf("hits: %d\n", hits.size());

        // for (size_t y = 0; y < height; y++)
        // {
        //     for (size_t x = 0; x < width; x++)
        //     {
        //         int32_t index2 = x + y * width;

        //         if (index2 == index)
        //         {
        //             std::printf("*");
        //         }
        //         else if (pixels[index2])
        //         {
        //             std::printf("^");
        //         }
        //         else if (!pixels[index2])
        //         {
        //             std::printf(".");
        //         }
        //     }

        //     std::printf("\n", x, y);
        // }

        Trace(hits, pixels, x - 1, y, width, height);
        Trace(hits, pixels, x + 1, y, width, height);
    }
    else
    {
        Trace(hits, pixels, x, y + 1, width, height);
    }
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

    std::getline(file, line);
    uint32_t rayStart = line.find("S");

    std::printf("rayStart: %d\n", rayStart);

    std::vector<bool> pixels;

    int32_t width = line.size();
    int32_t height = 0;

    while (std::getline(file, line))
    {
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == '^')
            {
                pixels.push_back(true);
            }
            else if (line[i] == '.')
            {
                pixels.push_back(false);
            }
        }

        height++;
    }

    std::vector<HitInfo> hits;

    std::printf("pixels: %d, rayStart: %d, width: %d, height: %d\n", pixels.size(), rayStart, width, height);

    Trace(hits, pixels, rayStart, 0, width, height);

    std::printf("hits: %d\n", hits.size());

    return 0;
}