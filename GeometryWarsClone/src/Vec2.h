#pragma once

#include <SFML/Graphics.hpp>


template <typename T> // meaning whatever type we construct this with will be the type of the vector

class Vec2
{
public:
    
    T x = 0;
    T y = 0;

    Vec2();
    Vec2(T xin, T yin);


    Vec2 operator + (const Vec2 & rhs) const;
    Vec2 operator - (const Vec2 & rhs) const;
    Vec2 operator / (const Vec2 & rhs) const;
    Vec2 operator * (const Vec2 & rhs) const;

    bool operator == (const Vec2 & rhs) const;
    bool operator != (const Vec2 & rhs) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const T val);
    void operator /= (const T val);


    // allow automatic conversion to sf::Vector2
    // this lets us pass Vec2 into sfml functions

    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x,y);
    }

    float dist(const Vec2& rhs) const;
    float length() const;
    
    Vec2 normalise() const;

};