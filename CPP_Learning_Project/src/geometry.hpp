#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <utility>

template<const int size,typename T>
struct Point
{
    std::array<T, size> values {};
    Point() {}
    template <typename... Args>
    Point(T first,Args&&... args) : values { first,std::forward<Args>(args)... }
    {
        static_assert(sizeof...(args) + 1 == size,"The number of arguments doesn't match with these type of Point !");
    }

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y()
    {
        static_assert(size >=2);
        return values[1];
    }
    float y() const
    {
        static_assert(size >=2);
        return values[1];
    }

    float& z()
    {
        static_assert(size >= 3);
        return values[2];
    }
    float z() const
    {
        static_assert(size >= 3);
        return values[2];
    }

    Point& operator+=(const Point& other)
    {
        std::transform(values.begin(),values.end(),other.values.begin(),values.begin(),std::plus<float> {});
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(),values.end(),other.values.begin(),values.begin(),std::minus<float> {});
        return *this;
    }

    Point& operator*=(const float scalar)
    {
        std::transform(values.begin(),values.end(),values.begin(),[scalar](float c) { return c*scalar; });
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const T scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator-() const { return Point { -x(), -y(), -z() }; }

    float length() const {
        return std::sqrt(std::accumulate(values.begin(),values.end(),0.0,[](float count,float a){ return count+(a*a);}));
    }

    float distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point& operator*=(const Point& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    T* toArray() const
    {
        return (T*) values.data();
    }
};

using Point3D = Point<3,float>;
using Point2D = Point<2,float>;


// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}