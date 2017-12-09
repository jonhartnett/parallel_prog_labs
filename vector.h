#ifndef PARALLEL_PROG_LABS_VECTOR_H
#define PARALLEL_PROG_LABS_VECTOR_H

#define FOREACH_OPERATOR(MACRO) \
    MACRO(+) \
    MACRO(-) \
    MACRO(*) \
    MACRO(/) \
    MACRO(%)

class Vector{
public:
    double x;
    double y;
    double z;

    Vector(double u);
    Vector(double x, double y, double z);

    double length() const;
    double lengthSq() const;

    Vector normalized() const;

#define VEC_TO_VEC(OP) \
    Vector operator OP(const Vector& v) const; \
    Vector& operator OP##=(const Vector& v);
    FOREACH_OPERATOR(VEC_TO_VEC)
#undef VEC_TO_VEC

#define VEC_TO_SCA(OP) \
    Vector operator OP(double u) const; \
    Vector& operator OP##=(double u);
    FOREACH_OPERATOR(VEC_TO_SCA)
#undef VEC_TO_SCA
};

#define SCA_TO_VEC(OP) \
    Vector operator OP(double u, const Vector& v);
    FOREACH_OPERATOR(SCA_TO_VEC)
#undef SCA_TO_VEC

#undef FOREACH_OPERATOR

#endif //PARALLEL_PROG_LABS_VECTOR_H
