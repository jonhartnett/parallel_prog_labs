#include <cmath>
#include "vector.h"

Vector::Vector(double u) : x(u), y(u), z(u){

}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z){

}

double Vector::length() const{
    return sqrt(lengthSq());
}

double Vector::lengthSq() const{
    return x * x + y * y + z * z;
}

Vector Vector::normalized() const{
    return *this / length();
}

#define FOREACH_OPERATOR(MACRO) \
    MACRO(+) \
    MACRO(-) \
    MACRO(*) \
    MACRO(/)

#define VEC_TO_VEC(OP) \
    Vector Vector::operator OP(const Vector& v) const{ \
        return Vector(x OP v.x, y OP v.y, z OP v.z); \
    } \
    Vector& Vector::operator OP##=(const Vector& v){ \
        x OP##= v.x; \
        y OP##= v.y; \
        z OP##= v.z; \
        return *this; \
    }
    FOREACH_OPERATOR(VEC_TO_VEC)
#undef VEC_TO_VEC

#define VEC_TO_SCA(OP) \
    Vector Vector::operator OP(double u) const{ \
        return Vector(x OP u, y OP u, z OP u); \
    } \
    Vector& Vector::operator OP##=(double u){ \
        x OP##= u; \
        y OP##= u; \
        z OP##= u; \
        return *this; \
    }
    FOREACH_OPERATOR(VEC_TO_SCA)
#undef VEC_TO_SCA

#define SCA_TO_VEC(OP) \
    Vector operator OP(double u, const Vector& v){ \
        return Vector(u OP v.x, u OP v.y, u OP v.z); \
    }
    FOREACH_OPERATOR(SCA_TO_VEC)
#undef SCA_TO_VEC

Vector Vector::operator %(const Vector& v) const{
    return Vector(fmod(x, v.x), fmod(y, v.y), fmod(z, v.z));
}

Vector Vector::operator %(double u) const{
    return Vector(fmod(x, u), fmod(y, u), fmod(z, u));
}

Vector& Vector::operator %=(const Vector& v){
    x = fmod(x, v.x);
    y = fmod(y, v.y);
    z = fmod(z, v.z);
    return *this;
}

Vector& Vector::operator %=(double u){
    x = fmod(x, u);
    y = fmod(y, u);
    z = fmod(z, u);
    return *this;
}

#undef FOREACH_OPERATOR