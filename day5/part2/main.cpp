#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>

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

    // remove overlaps
    for (size_t i = 0; i < ranges.size(); i++)
    {
        Range &a = ranges[i];

        if (a.end == 0 && a.start == 0)
        {
            continue;
        }

        for (size_t j = 0; j < ranges.size(); j++)
        {
            if (i == j)
            {
                continue;
            }

            Range &b = ranges[j];

            if (b.end == 0 && b.start == 0)
            {
                continue;
            }

            // b is after a
            if (b.start > a.end)
            {
                continue;
            }

            // a is after b
            if (a.start > b.end)
            {
                continue;
            }

            // merge
            a.start = std::min(a.start, b.start);
            a.end = std::max(a.end, b.end);
            b.start = 0;
            b.end = 0;

            j = 0;
        }
    }

    for (Range &r : ranges)
    {
        if (r.end == 0 && r.start == 0)
        {
            continue;
        }

        int64_t range = (r.end - r.start) + 1i64;

        count += range;
    }

    std::printf("count: %lld\n", count);

    return 0;
}