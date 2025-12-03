#include <iostream>
#include <fstream>
#include <string>
#include <charconv>

bool IsIdValid(int64_t id)
{
    int64_t digits = (int)std::log10(id) + 1;
    if (digits % 2 != 0)
    {
        return true;
    }
    int64_t divisor = (int64_t)std::pow(10, digits / 2);

    int64_t firstHalf = id / divisor;
    int64_t secondHalf = id % divisor;

    // printf("id: %d digits: %d, firstHalf: %d, secondHalf: %d\n", id, digits, firstHalf, secondHalf);

    return firstHalf != secondHalf;
}

int64_t SumInvalidIdsRange(int64_t start, int64_t end)
{
    int64_t sum = 0;

    // std::printf("checking range %d-%d\n", start, end);

    for (int64_t id = start; id <= end; id++)
    {
        if (!IsIdValid(id))
        {
            sum += id;
            // std::printf("%d-%d has invalid ID: %d\n", start, end, id);
        }
    }

    return sum;
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

    int64_t invalidSum = 0;

    while (std::getline(file, line))
    {
        size_t index = 0;
        size_t count = 0;

        std::string_view view(line);
        int64_t previousValue = 0;
        int64_t currentValue = 0;

        size_t last = view.length() - 1;

        for (size_t i = 0; i < view.length(); i++)
        {
            char c = view[i];
            count++;

            if (c == '-' || c == ',' || i == last)
            {
                previousValue = currentValue;
                std::string_view slice = view.substr(index, count);
                std::from_chars(slice.data(), slice.data() + slice.size(), currentValue);

                index = index + count;
                count = 0;

                if (c == ',' || i == last)
                {
                    invalidSum += SumInvalidIdsRange(previousValue, currentValue);
                }
            }
        }
    }

    std::printf("invalidSum: %lld\n", invalidSum);

    return 0;
}