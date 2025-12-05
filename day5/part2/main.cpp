#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>

struct Range
{
    int64_t start;
    int64_t end;
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

    std::vector<Range> ranges;
    std::string delimiter = "-";
    int64_t count = 0;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            break;
        }

        int32_t delimiterIndex = line.find(delimiter);
        std::string left = line.substr(0, delimiterIndex);
        std::string right = line.substr(delimiterIndex + 1);

        Range r;
        r.start = std::stoll(left);
        r.end = std::stoll(right);

        ranges.push_back(r);

        // std::printf("start: %lld end: %lld\n", r.start, r.end);
    }

    std::sort(ranges.begin(), ranges.end(),
              [](const Range &a, const Range &b)
              {
                  return a.start < b.start;
              });

    std::vector<Range> merged;
    merged.push_back(ranges[0]);

    for (size_t i = 1; i < ranges.size(); i++)
    {
        Range &last = merged.back();
        Range &current = ranges[i];

        if (current.start <= last.end)
        {
            last.end = std::max(last.end, current.end);
        }
        else
        {
            merged.push_back(current);
        }
    }

    for (Range &r : merged)
    {
        int64_t range = (r.end - r.start) + 1i64;

        count += range;
    }

    std::printf("count: %lld\n", count);

    return 0;
}