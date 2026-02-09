#pragma once
#include <cmath>

struct Vec2 {
    double x,y;

    // Constructeurs
    Vec2() : x(0.0), y(0.0) {}
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    // Operateurs
    Vec2 operator+(const Vec2& v) const{
        return {x+v.x, y+v.y};
    }
    Vec2 operator-(const Vec2& v) const{
        return {x-v.x,y-v.y};
    }
    Vec2 operator*(double s) const {
        return {x*s,y*s};
    }
    //Peut être overkill mais pratique pour LJ
    Vec2& operator+=(const Vec2& v){
        x+=v.x;
        y+=v.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& v){
        x-=v.x;
        y-=v.y;
        return *this;
    }


    //Normes
    double norm2() const{
        return x*x+y*y;
    }
    //WARNING : sqrt LENTE, comme on a du O(N^2) vaudra mieux utiliser la norm2
    double norm() const{
        return std::sqrt(norm2());
    }
};
//Pour avoir la multiplication à gauche. Hors structure 
inline Vec2 operator*(double s, const Vec2& v){
    return v*s;
}
//Produit scalaire utile pour énergie cinétique, RDF etc
inline double dot(const Vec2& a, const Vec2& b){
    return a.x*b.x+a.y*b.y;
}