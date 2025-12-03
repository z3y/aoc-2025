#include <iostream>
#include <fstream>
#include <string>
#include <charconv>

bool IsIdValid(int64_t id)
{
    std::string str = std::to_string(id);

    if (str.length() <= 1)
    {
        return true;
    }

    for (size_t testSize = 1; testSize <= str.length() / 2; testSize++)
    {
        std::string testSlice = str.substr(0, testSize);

        bool same = true;
        for (size_t j = testSize; j < str.length(); j += testSize)
        {
            std::string currentSlice = str.substr(j, testSize);

            same = currentSlice == testSlice;

            if (!same)
            {
                break;
            }
        }

        if (same)
        {
            return false;
        }
    }

    return true;
}

int64_t SumInvalidIdsRange(int64_t start, int64_t end)
{
    int64_t sum = 0;

    // std::printf("checking range %lld-%lld\n", start, end);

    for (int64_t id = start; id <= end; id++)
    {
        if (!IsIdValid(id))
        {
            sum += id;

            // std::printf("%lld-%lld has invalid ID: %lld\n", start, end, id);
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