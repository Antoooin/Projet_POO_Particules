#pragma once
#include <cmath>

// Vecteur 2D en double précision.
// Utilisé pour positions, vitesses et forces de toutes les particules.
struct Vec2 {
    double x, y;

    Vec2() : x(0.0), y(0.0) {}
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
    Vec2 operator-(const Vec2& v) const { return {x - v.x, y - v.y}; }
    Vec2 operator*(double s)      const { return {x * s,   y * s};   }
    Vec2 operator/(double s)      const { return {x / s,   y / s};   }

    Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
    Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
    Vec2& operator*=(double s)      { x *= s;   y *= s;   return *this; }

    double norm2() const { return x*x + y*y; }

    // norm() coûte un sqrt -> préférer norm2() dans les boucles O(N²)
    double norm()  const { return std::sqrt(norm2()); }
};

// Multiplication scalaire-vecteur (scalaire à gauche)
inline Vec2 operator*(double s, const Vec2& v) { return v * s; }

// Produit scalaire
inline double dot(const Vec2& a, const Vec2& b) { return a.x*b.x + a.y*b.y; }