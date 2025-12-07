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

int64_t Trace(std::vector<bool> &pixels, std::vector<int64_t> &visited, int32_t x, int32_t y, int32_t width, int32_t height)
{
    if (!BoundsCheck(x, y, width, height))
    {
        return 0;
    }

    int32_t index = x + y * width;

    bool hit = pixels[index];

    if (hit)
    {
        if (visited[index] > 0)
        {
            return visited[index];
        }

        int64_t a = Trace(pixels, visited, x + 1, y, width, height);
        int64_t b = Trace(pixels, visited, x - 1, y, width, height);

        int64_t hits = a + b + 1;
        visited[index] = hits;
        return hits;
    }
    else
    {
        return Trace(pixels, visited, x, y + 1, width, height);
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

    std::vector<int64_t> visited(pixels.size());

    int64_t timelines = Trace(pixels, visited, rayStart, 0, width, height);

    std::printf("timelines: %lld\n", timelines + 1);

    return 0;
}