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

    bool parsingRanges = true;
    std::vector<Range> ranges;

    std::string delimiter = "-";

    int64_t freshCount = 0;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            parsingRanges = false;
            continue;
        }

        if (parsingRanges)
        {
            int32_t delimiterIndex = line.find(delimiter);
            std::string left = line.substr(0, delimiterIndex);
            std::string right = line.substr(delimiterIndex + 1);

            Range r;
            r.start = std::stoll(left);
            r.end = std::stoll(right);

            ranges.push_back(r);

            std::printf("start: %lld end: %lld\n", r.start, r.end);

            continue;
        }

        int64_t id = std::stoll(line);
        for (Range &r : ranges)
        {
            if (id >= r.start && id <= r.end)
            {
                freshCount++;
                break;
            }
        }
    }

    std::printf("freshCount: %lld\n", freshCount);

    return 0;
}