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
    int32_t x;
    int32_t y;
    int32_t z;

    Vec3(int32_t x_, int32_t y_, int32_t z_) : x(x_), y(y_), z(z_) {}
    Vec3() : x(0), y(0), z(0) {}

    Vec3 operator-(const Vec3 &other) const
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3 &other) const
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    int32_t operator[](size_t i) const
    {
        if (i == 0)
            return x;
        else if (i == 1)
            return y;
        else
            return z;
    }

    bool operator==(const Vec3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    static int32_t Dot(const Vec3 &a, const Vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static float Length(const Vec3 &a)
    {
        return std::sqrt(Dot(a, a));
    }

    static float Distance(const Vec3 &a, const Vec3 &b)
    {
        return Length(a - b);
    }

    std::string ToString()
    {
        return std::format("(x: {0}, y: {1}, z: {2})", x, y, z);
    }

    static const Vec3 zero;
};

const Vec3 Vec3::zero(0, 0, 0);

enum class KdAxis
{
    X = 0,
    Y = 1,
    Z = 2,
};

struct KdNode
{
    KdNode() = default;
    std::weak_ptr<KdNode> parent;
    std::shared_ptr<KdNode> childA;
    std::shared_ptr<KdNode> childB;
    KdAxis axis;
    Vec3 value;

    static Vec3 FindMedian(std::vector<Vec3> points, KdAxis axis)
    {
        size_t axisIndex = static_cast<size_t>(axis);

        std::sort(points.begin(), points.end(),
                  [axisIndex](const Vec3 &a, const Vec3 &b)
                  {
                      return a[axisIndex] < b[axisIndex];
                  });

        size_t medianIndex = points.size() / 2;

        return points[medianIndex];
    };

    static void SplitPoints(Vec3 &median, KdAxis axis, const std::vector<Vec3> &points, std::vector<Vec3> &a, std::vector<Vec3> &b)
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            Vec3 p = points[i];

            if (p == median)
            {
                continue;
            }

            size_t axisIndex = static_cast<size_t>(axis);

            if (p[axisIndex] < median[axisIndex])
            {
                a.push_back(p);
            }
            else
            {
                b.push_back(p);
            }
        }
    }

    static KdAxis NextAxis(KdAxis axis)
    {
        size_t axisIndex = static_cast<size_t>(axis);
        axisIndex++;
        axisIndex %= 3;
        return static_cast<KdAxis>(axisIndex);
    }

    static void CreateTreeRecusive(std::shared_ptr<KdNode> &root, const std::vector<Vec3> &points)
    {
        if (points.size() == 1)
        {
            root->value = points[0];
            return;
        }

        Vec3 median = FindMedian(points, root->axis);

        // std::printf("median: %s\n", median.ToString().c_str());

        std::vector<Vec3> a;
        std::vector<Vec3> b;

        SplitPoints(median, root->axis, points, a, b);

        root->value = median;

        KdAxis nextAxis = NextAxis(root->axis);

        if (a.size() > 0)
        {
            root->childA = std::make_shared<KdNode>();
            root->childA->axis = nextAxis;
            root->childA->parent = root;

            CreateTreeRecusive(root->childA, a);
        }

        if (b.size() > 0)
        {
            root->childB = std::make_shared<KdNode>();
            root->childB->axis = nextAxis;
            root->childB->parent = root;

            CreateTreeRecusive(root->childB, b);
        }
    }

    static std::shared_ptr<KdNode> CreateTree(const std::vector<Vec3> &points)
    {
        auto root = std::make_shared<KdNode>();
        root->axis = KdAxis::X;

        CreateTreeRecusive(root, points);

        return root;
    }

    static std::shared_ptr<KdNode> FindNode(const std::shared_ptr<KdNode> &root, Vec3 point)
    {
        if (root->value == point)
        {
            return root;
        }

        size_t axisIndex = static_cast<size_t>(root->axis);

        if (point[axisIndex] < root->value[axisIndex])
        {
            return FindNode(root->childA, point);
        }
        else
        {
            return FindNode(root->childB, point);
        }

        return nullptr;
    }

    static void FindClosestRecursive(float &closestDistance, std::shared_ptr<KdNode> &closestNode, const std::shared_ptr<KdNode> &node, Vec3 &point)
    {
        if (!node)
        {
            return;
        }

        if (node->value != point)
        {
            float d = Vec3::Distance(node->value, point);
            if (d < closestDistance)
            {
                closestDistance = d;
                closestNode = node;
            }
        }

        size_t axisIndex = static_cast<size_t>(node->axis);

        bool isLeft = point[axisIndex] < node->value[axisIndex];

        auto primary = isLeft ? node->childA : node->childB;

        FindClosestRecursive(closestDistance, closestNode, primary, point);

        if (std::abs(point[axisIndex] - node->value[axisIndex]) < closestDistance)
        {
            auto secondary = isLeft ? node->childB : node->childA;
            FindClosestRecursive(closestDistance, closestNode, secondary, point);
        }
    }

    static std::shared_ptr<KdNode> FindClosest(const std::shared_ptr<KdNode> &root, Vec3 point)
    {
        float closestDistance = std::numeric_limits<float>::max();
        std::shared_ptr<KdNode> closestNode;

        FindClosestRecursive(closestDistance, closestNode, root, point);

        return closestNode;
    }
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

        // std::printf("p: %s\n", p.ToString().c_str());
        points.push_back(p);
    }

    auto kdTree = KdNode::CreateTree(points);

    Vec3 test = Vec3(162, 817, 812);

    auto closest = KdNode::FindClosest(kdTree, test);

    std::printf("closest: %s\n", closest->value.ToString().c_str());

    // auto node = KdNode::FindNode(kdTree, Vec3(162,817,812));

    // std::printf("node: %s\n", node->value.ToString().c_str());

    std::printf("end\n");

    return 0;
}