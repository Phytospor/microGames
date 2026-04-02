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
        //to do
        return Vec2();
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        //to do
        return Vec2();
    }

    Vec2 operator / (const Vec2& rhs) const
    {
        //to do
        return Vec2();
    }

    Vec2 operator * (const Vec2& rhs) const
    {
        //to do
        return Vec2();
    }

    bool operator == (const Vec2& rhs) const
    {
        //to do
        return false;
    }

    bool operator != (const Vec2& rhs) const
    {
        //to do
        return false;
    }

    void operator += (const Vec2& rhs) const
    {
        //to do

    }

    void operator -= (const Vec2& rhs) const
    {
        //to do

    }

    void operator *= (const T val) const
    {
        //to do

    }

    void operator /= (const T val) const
    {
        //to do

    }

    float dist(const Vec2& rhs) const
    {
        // todo
    }

    float lenght() const
    {

    }

    void normalise()
    {

    }


};