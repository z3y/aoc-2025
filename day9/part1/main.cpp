#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <format>

struct Point
{
    int64_t x;
    int64_t y;
};

int64_t CalculateArea(Point &a, Point &b)
{
    int64_t height = std::abs(a.y - b.y) + 1;
    int64_t width = std::abs(a.x - b.x) + 1;
    return width * height;
}

struct Rect
{
    Point a;
    Point b;
    int64_t area;
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

    std::vector<Point> points;

    while (std::getline(file, line))
    {
        Point p;

        size_t split0 = line.find(",");

        p.x = std::stoll(line.substr(0, split0));
        p.y = std::stoll(line.substr(split0 + 1));

        points.push_back(p);
    }

    std::vector<Rect> rects;

    for (size_t i = 0; i < points.size(); i++)
    {
        for (size_t j = i + 1; j < points.size(); j++)
        {
            Rect r;
            r.a = points[i];
            r.b = points[j];
            r.area = CalculateArea(points[i], points[j]);

            rects.push_back(r);
        }
    }

    std::sort(rects.begin(), rects.end(),
              [](const Rect &lhs, const Rect &rhs)
              {
                  return rhs.area < lhs.area;
              });

    std::printf("largest rect area: %ld\n", rects[0].area);

    return 0;
}