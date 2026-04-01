#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include "Vec2.h"


// if we would define the functions directly in the class definition (which is currently in the class file), then we could write template <typename T> only once.

template <typename T> 
Vec2<T>::Vec2() = default; // the same as: Vec2<T>::Vec2(){}

template <typename T> 
Vec2<T>::Vec2(T xin, T yin)
    : x(xin), y(yin)
{
}

template <typename T> 
Vec2<T> Vec2<T>::operator + (const Vec2<T>& rhs) const
{
    //to do
    return Vec2<T>();
}

template <typename T> 
Vec2<T> Vec2<T>::operator - (const Vec2<T>& rhs) const
{
    //to do
    return Vec2<T>();
}

template <typename T> 
Vec2<T> Vec2<T>::operator / (const Vec2<T>& rhs) const
{
    //to do
    return Vec2<T><T>();
}

template <typename T> 
Vec2<T> Vec2<T>::operator * (const Vec2<T>& rhs) const
{
    //to do
    return Vec2<T>();
}

template <typename T> 
bool Vec2<T>::operator == (const Vec2<T>& rhs) const
{
    //to do
    return false;
}

template <typename T> 
bool Vec2<T>::operator != (const Vec2<T>& rhs) const
{
    //to do
    return false;
}

template <typename T> 
void Vec2<T>::operator += (const Vec2& rhs) 
{
    //to do

}

template <typename T> 
void Vec2<T>::operator -= (const Vec2& rhs) 
{
    //to do

}

template <typename T> 
void Vec2<T>::operator *= (const T val) 
{
    //to do

}

template <typename T> 
void Vec2<T>::operator /= (const T val) 
{
    //to do

}

template <typename T> 
float Vec2<T>::dist(const Vec2<T>& rhs) const
{
    // todo
}

template <typename T> 
float Vec2<T>::length() const
{

}

template <typename T> 
Vec2<T> Vec2<T>::normalise() const
{
    return Vec2<T>();
}


