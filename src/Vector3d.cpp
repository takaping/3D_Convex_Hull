/* 
 * File:   Vector3d.cpp
 * Author: munehiro
 * 
 * Created on August 3, 2013, 7:31 PM
 */

#include <cmath>
#include "Vector3d.h"

/**
 * Constructors and destructor
 */
Vector3d::Vector3d() {
    vec[0] = 0.0; vec[1] = 0.0; vec[2] = 0.0;
}

Vector3d::Vector3d(const Vector3d& orig) {
    *this = orig;
}

Vector3d::Vector3d(double x, double y, double z) {
    set(x, y, z);
}

Vector3d::~Vector3d() {
}

/**
 * Sets the vector
 * @param x
 * @param y
 * @param z
 */
void Vector3d::set(double x, double y, double z) {
    vec[0] = x; vec[1] = y; vec[2] = z;
}

/**
 * Overloaded operators
 */
bool Vector3d::operator ==(const Vector3d& rhs) const {
    return (vec[0] == rhs.vec[0] && vec[1] == rhs.vec[1] && vec[2] == rhs.vec[2]);
}

bool Vector3d::operator !=(const Vector3d& rhs) const {
    return !(vec[0] == rhs.vec[0] && vec[1] == rhs.vec[1] && vec[2] == rhs.vec[2]);
}

Vector3d& Vector3d::operator =(const Vector3d& rhs) {
    vec[0] = rhs.vec[0]; vec[1] = rhs.vec[1]; vec[2] = rhs.vec[2];
    return *this;
}

Vector3d& Vector3d::operator +=(double rhs) {
    vec[0] += rhs; vec[1] += rhs; vec[2] += rhs;
    return *this;
}

Vector3d Vector3d::operator +(double rhs) const {
    Vector3d v(*this);
    v += rhs;
    return v;
}

Vector3d& Vector3d::operator +=(const Vector3d& rhs) {
    vec[0] += rhs.vec[0]; vec[1] += rhs.vec[1]; vec[2] += rhs.vec[2];
    return *this;
}

Vector3d Vector3d::operator +(const Vector3d& rhs) const {
    Vector3d v(*this);
    v += rhs;
    return v;
}

Vector3d& Vector3d::operator -=(const Vector3d& rhs) {
    vec[0] -= rhs.vec[0]; vec[1] -= rhs.vec[1]; vec[2] -= rhs.vec[2];
    return *this;
}

Vector3d Vector3d::operator -(const Vector3d& rhs) const {
    Vector3d v(*this);
    v -= rhs;
    return v;
}

Vector3d& Vector3d::operator *=(double rhs) {
    vec[0] *= rhs; vec[1] *= rhs; vec[2] *= rhs;
    return *this;
}

Vector3d Vector3d::operator *(double rhs) const {
    Vector3d v(*this);
    v *= rhs;
    return v;
}

Vector3d& Vector3d::operator /=(double rhs) {
    vec[0] /= rhs; vec[1] /= rhs; vec[2] /= rhs;
    return *this;
}

Vector3d Vector3d::operator /(double rhs) const {
    Vector3d v(*this);
    v /= rhs;
    return v;
}

/**
 * Dot production
 */
double Vector3d::dot(const Vector3d& rhs) const {
    return (vec[0] * rhs.vec[0] + vec[1] * rhs.vec[1] + vec[2] * rhs.vec[2]);
}

/**
 * Cross production
 */
Vector3d Vector3d::cross(const Vector3d& rhs) const {
    return Vector3d(vec[1] * rhs.vec[2] - vec[2] * rhs.vec[1],
                    vec[2] * rhs.vec[0] - vec[0] * rhs.vec[2],
                    vec[0] * rhs.vec[1] - vec[1] * rhs.vec[0]);
}

