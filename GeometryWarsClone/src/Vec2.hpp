#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>


template <typename T> // meaning whatever type we construct this with will be the type of the vector

class Vec2
{
public:
    
    T x = 0;
    T y = 0;

    Vec2() = default;

    Vec2(T xin, T yin)
        : x(xin), y(yin)
    {};

    // constructor to convert from sf::Vector2
    Vec2(const sf::Vector2<T>& vec)
        : x(T(vec.x)), y(T(vec.y))
    {
    }

    // allow automatic conversion to sf::Vector2
    // this lets us pass Vec2 into sfml functions

    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x,y);
    }

    Vec2 operator + (const Vec2& rhs) const
    {
        
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        //a - b gives the vector from b to a.
        return Vec2(x - rhs.x, y - rhs.y);
    }


    Vec2 operator / (const float val) const
    {
        if (val == 0.0f)
        {
            return Vec2(0.0f, 0.0f); // or handle differently
        }
        else
            return Vec2(x / val, y / val);
    }

    Vec2 operator * (const float & val) const
    {
        return Vec2(x * val, y * val);
    }

    bool operator == (const Vec2& rhs) const
    {
        return (x == rhs.x && y == rhs.y);
    }

    bool operator != (const Vec2& rhs) const
    {
        return (x != rhs.x || y != rhs.y);
    }

    void operator += (const Vec2& rhs) 
    {
      x+=rhs.x;
      y+=rhs.y;
    }

    void operator -= (const Vec2& rhs)
    {
      x-=rhs.x;
      y-=rhs.y;
    }

    void operator *= (const T val)
    {
      x*=val;
      y*=val;
    }

    void operator /= (const T val)
    {
      x/=val;
      y/=val;
    }

    float dist(const Vec2& rhs) const
    {
        float dx = x - rhs.x;
        float dy = y - rhs.y;
        return sqrtf(dx * dx + dy * dy);
    }

    float length () const
    {
        return(sqrtf(x*x + y*y));
    }

    Vec2<float> normalise() const
    {
        T len = length();
        return Vec2(x / len, y / len);
    }


};