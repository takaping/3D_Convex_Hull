/* 
 * 3d vector class
 *  - implements 3d vector operations
 * File:   Vector3d.h
 * Author: munehiro
 *
 * Created on August 3, 2013, 7:31 PM
 */

#ifndef VECTOR3D_H
#define	VECTOR3D_H

class Vector3d {
public:
    // less in x order
    static bool lessX(const Vector3d& left, const Vector3d& right) {
        if (left.vec[0] != right.vec[0]) {
            return (left.vec[0] < right.vec[0]);
        } else if (left.vec[1] != right.vec[1]) {
            return (left.vec[1] < right.vec[1]);
        } else {
            return (left.vec[2] < right.vec[2]);
        }
    };
    Vector3d();
    Vector3d(const Vector3d& orig);
    Vector3d(double x, double y, double z);
    virtual ~Vector3d();
    // vector
    const double* get() const { return vec; };
    // x of the vector
    double x() const { return vec[0]; };
    // y of the vector
    double y() const { return vec[1]; };
    // z of the vector
    double z() const { return vec[2]; };
    void set(double x, double y, double z);
    bool operator ==(const Vector3d& rhs) const;
    bool operator !=(const Vector3d& rhs) const;
    Vector3d& operator =(const Vector3d& rhs);
    Vector3d& operator +=(double rhs);
    Vector3d operator +(double rhs) const;
    Vector3d& operator +=(const Vector3d& rhs);
    Vector3d operator +(const Vector3d& rhs) const;
    Vector3d& operator -=(const Vector3d& rhs);
    Vector3d operator -(const Vector3d& rhs) const;
    Vector3d& operator *=(double rhs);
    Vector3d operator *(double rhs) const;
    Vector3d& operator /=(double rhs);
    Vector3d operator /(double rhs) const;
    double dot(const Vector3d& rhs) const;
    Vector3d cross(const Vector3d& rhs) const;
private:
    double vec[3];  // x, y and z of the vector

};

#endif	/* VECTOR3D_H */

