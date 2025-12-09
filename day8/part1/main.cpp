#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <format>

struct Vec3
{
    int64_t x;
    int64_t y;
    int64_t z;

    Vec3(int64_t x_, int64_t y_, int64_t z_) : x(x_), y(y_), z(z_) {}
    Vec3() : x(0), y(0), z(0) {}

    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    bool operator==(const Vec3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    static int64_t Dot(const Vec3 &a, const Vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static int64_t DistanceInt(const Vec3 &a, const Vec3 &b)
    {
        Vec3 v = a - b;
        return Dot(v, v);
    }

    std::string ToString()
    {
        return std::format("({0}, {1}, {2})", x, y, z);
    }
};

struct Pair
{
    Vec3 a;
    Vec3 b;
};

struct Circuit
{
    std::vector<Vec3> points;
};

void MergeCircuits(std::vector<Circuit> &circuits)
{
    for (size_t i = 0; i < circuits.size(); i++)
    {
        for (size_t j = i + 1; j < circuits.size();)
        {
            bool shouldMerge = false;

            for (const Vec3 &p : circuits[i].points)
            {
                if (std::find(circuits[j].points.begin(), circuits[j].points.end(), p) != circuits[j].points.end())
                {
                    shouldMerge = true;
                    break;
                }
            }

            if (shouldMerge)
            {
                for (const Vec3 &p : circuits[j].points)
                {
                    if (std::find(circuits[i].points.begin(), circuits[i].points.end(), p) == circuits[i].points.end())
                    {
                        circuits[i].points.push_back(p);
                    }
                }

                // std::printf("merged %ld-%ld\n", circuits[i].points.size(), circuits[j].points.size());
                circuits[j].points.clear();
            }
            else
            {
                ++j;
            }
        }
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

    std::vector<Vec3> points;

    while (std::getline(file, line))
    {
        Vec3 p;

        size_t split0 = line.find(",");

        p.x = std::stoi(line.substr(0, split0));
        std::string part2 = line.substr(split0 + 1);

        size_t split1 = part2.find(",");

        p.y = std::stoi(part2.substr(0, split1));
        p.z = std::stoi(part2.substr(split1 + 1));

        points.push_back(p);
    }

    std::vector<Pair> pairs;

    for (size_t i = 0; i < points.size(); i++)
    {
        for (size_t j = i + 1; j < points.size(); j++)
        {
            Pair pair;
            pair.a = points[i];
            pair.b = points[j];

            pairs.push_back(pair);
        }
    }

    std::sort(pairs.begin(), pairs.end(),
              [](const Pair &lhs, const Pair &rhs)
              {
                  return Vec3::DistanceInt(lhs.a, lhs.b) < Vec3::DistanceInt(rhs.a, rhs.b);
              });

    std::vector<Circuit> circuits;

    int32_t connectionsLeft = 1000;
    for (size_t i = 0; i < connectionsLeft; i++)
    {
        Pair p = pairs[i];

        bool circuitFound = false;
        for (size_t j = 0; j < circuits.size(); j++)
        {
            auto &points = circuits[j].points;

            bool aExists = std::find(points.begin(), points.end(), p.a) != points.end();
            bool bExists = std::find(points.begin(), points.end(), p.b) != points.end();

            if (aExists || bExists)
            {
                if (!aExists)
                {
                    circuits[j].points.push_back(p.a);
                }
                if (!bExists)
                {
                    circuits[j].points.push_back(p.b);
                }

                circuitFound = true;
                MergeCircuits(circuits);

                break;
            }
        }

        if (!circuitFound)
        {
            Circuit newCircuit;
            newCircuit.points.push_back(p.a);
            newCircuit.points.push_back(p.b);
            circuits.push_back(newCircuit);
        }
    }

    std::sort(circuits.begin(), circuits.end(),
              [](const Circuit &lhs, const Circuit &rhs)
              {
                  return rhs.points.size() < lhs.points.size();
              });

    int32_t product = circuits[0].points.size() * circuits[1].points.size() * circuits[2].points.size();

    std::printf("0: %ld\n", circuits[0].points.size());
    std::printf("1: %ld\n", circuits[1].points.size());
    std::printf("2: %ld\n", circuits[2].points.size());
    std::printf("product: %d\n", product);

    return 0;
}