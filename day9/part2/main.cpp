#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <format>
#include <deque>

struct Point
{
    int64_t x = 0;
    int64_t y = 0;
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

bool PointInside(const Point &rectMin, const Point &rectMax, const Point &p)
{
    return p.x > rectMin.x && p.y > rectMin.y && p.x < rectMax.x && p.y < rectMax.y;
}

bool PointInsideOrOnEdge(const Point &rectMin, const Point &rectMax, const Point &p)
{
    return p.x >= rectMin.x && p.y >= rectMin.y && p.x <= rectMax.x && p.y <= rectMax.y;
}

bool EdgeIntersects(const Point &a, const Point &b, const Rect &r)
{
    Point rectMin;
    rectMin.x = std::min(r.a.x, r.b.x);
    rectMin.y = std::min(r.a.y, r.b.y);

    Point rectMax;
    rectMax.x = std::max(r.a.x, r.b.x);
    rectMax.y = std::max(r.a.y, r.b.y);

    bool startOnEdgeOrInside = PointInsideOrOnEdge(rectMin, rectMax, a);
    bool endOnEdgeOrInside = PointInsideOrOnEdge(rectMin, rectMax, b);

    if (startOnEdgeOrInside && endOnEdgeOrInside)
    {
        bool startInside = PointInside(rectMin, rectMax, a);
        bool endInside = PointInside(rectMin, rectMax, b);

        if ((startInside + endInside) == 1)
        {
            return true;
        }

        bool onEdge = !(startInside && endInside);
        if (onEdge)
        {
            bool bottomEdge = a.y == rectMin.y && b.y == rectMin.y;
            bool topEdge = a.y == rectMax.y && b.y == rectMax.y;
            bool leftEdge = a.x == rectMin.x && b.x == rectMin.x;
            bool rightEdge = a.x == rectMax.x && b.x == rectMax.x;

            // intersects on the inside of rect but doesnt lie on any edge
            if (!(bottomEdge || topEdge || leftEdge || rightEdge))
            {
                return true;
            }
        }
    }

    // both points outside but can still intersect
    if (!(startOnEdgeOrInside && endOnEdgeOrInside))
    {
        if (a.x == b.x) // vertical
        {
            if (a.x > rectMin.x && a.x < rectMax.x)
            {
                float minY = std::min(a.y, b.y);
                float maxY = std::max(a.y, b.y);
                if (minY < rectMax.y && maxY > rectMin.y)
                {
                    return true;
                }
            }
        }
        else if (a.y == b.y) // horizontal
        {
            if (a.y > rectMin.y && a.y < rectMax.y)
            {
                float minX = std::min(a.x, b.x);
                float maxX = std::max(a.x, b.x);
                if (minX < rectMax.x && maxX > rectMin.x)
                {
                    return true;
                }
            }
        }
    }

    return false;
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

            Point rectMin;
            rectMin.x = std::min(r.a.x, r.b.x);
            rectMin.y = std::min(r.a.y, r.b.y);

            Point rectMax;
            rectMax.x = std::max(r.a.x, r.b.x);
            rectMax.y = std::max(r.a.y, r.b.y);

            bool intersects = false;
            for (size_t i = 0; i + 1 < points.size(); i++)
            {
                if (EdgeIntersects(points[i], points[i + 1], r))
                {
                    intersects = true;
                    break;
                }
            }

            if (EdgeIntersects(points[0], points[points.size() - 1], r))
            {
                intersects = true;
                break;
            }

            if (!intersects)
            {
                r.area = CalculateArea(points[i], points[j]);
                rects.push_back(r);
            }
        }
    }

    std::sort(rects.begin(), rects.end(),
              [](const Rect &lhs, const Rect &rhs)
              {
                  return rhs.area < lhs.area;
              });

    std::printf("largest rect a: %ld,%ld\n", rects[0].a.x, rects[0].a.y);
    std::printf("largest rect b: %ld,%ld\n", rects[0].b.x, rects[0].b.y);

    std::printf("largest rect area: %ld\n", rects[0].area);

    return 0;
}