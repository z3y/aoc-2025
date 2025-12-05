#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>

struct Event
{
    int64_t id;
    int32_t sign;
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

    std::string delimiter = "-";
    int64_t count = 0;

    std::vector<Event> events;

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            break;
        }

        int32_t delimiterIndex = line.find(delimiter);
        std::string left = line.substr(0, delimiterIndex);
        std::string right = line.substr(delimiterIndex + 1);

        Event e0;
        e0.id = std::stoll(left);
        e0.sign = 1;
        events.push_back(e0);

        Event e1;
        e1.id = std::stoll(right) + 1;
        e1.sign = -1;
        events.push_back(e1);
    }

    std::sort(events.begin(), events.end(),
              [](const Event &a, const Event &b)
              {
                  return a.id < b.id;
              });

    int32_t depth = 0;
    int64_t previousId = 0;

    for (size_t i = 0; i < events.size(); i++)
    {
        Event &e = events[i];

        if (depth == 0)
        {
            previousId = e.id;
        }

        depth += e.sign;

        if (depth == 0)
        {
            count += e.id - previousId;
        }
    }

    std::printf("count: %lld\n", count);

    return 0;
}