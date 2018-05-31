//<Ray Declarations>
class Ray{
public:
    //<Ray Public Methods>
    // constructors
    // explicitly declared constructor -->
    Ray() : tMax(Infinity), time(0.f), medium(nullptr) {}
    Ray(const Point3f &o, const Vector3f &d, Float tMax = Infinity,
        Float time = 0.f, const Medium *medium = nullptr)
        : o(o), d(d), tMax(tMax), time(time), medium(medium) {}
    //
    Point3f operator()(Float t) const {return o + d * t;}

    //<Ray Public Data>
    Point3f o;  //origin
    Vector3f d; //direction
    mutable Float tMax;
    Float time;
    const Medium *medium;
};

//<Vector Declarations>
template <typename T> class Vector3 {
public:
    // Vector3 Public Methods
    // 1. constructors
    Vector3() { x = y = z = 0; }
    Vector3(T x, T y, T z)
            : x(x), y(y), z(z) {
            Assert(!HasNaNs()); } // DCHECK(!HasNaNs());
    // 2. check NaN (not a number)
    bool HasNaNs() const {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }
    // 3. operators overload
    // [ ]
    /*
        operator to index into the components
        eg. v.v[0] == v.x
    */
    T operator[](int i) const {
        DCHECK(i>=0 && i<=2); // DCHECK(i >= 0 && i <= 1);
        if(i == 0) return x;
        if(i == 1) return y;
        return z;
    }
    T &operator[](int i) { // why & and no const???
        DCHECK(i>=0 && i<=2);
        if(i == 0) return x;
        if(i == 1) return y;
        return z;
    }
    // + addition
    Vector3<T> operator+(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    Vector3<T> &operator+=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    // - substract
    Vector3<T> operator-(const Vector3<T> &v) const {
        DCHECK(!v.HasNaNs());
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3<T> &operator-=(const Vector3<T> &v) {
        DCHECK(!v.HasNaNs());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    // * multiply a scalar
    template <typename U>
    Vector3<T> operator*(U s) const {
        return Vector3<T>(s * x, s * y, s * z);
    }
    template <typename U>
    Vector3<T> &operator*=(U s) {
        DCHECK(!isNaN(s));
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    // / devide by a scalr
    template <typename U>
    Vector3<T> operator/(U f) const {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        return Vector3<T>(x * inv, y * inv, z * inv);
    }
    template <typename U>
    Vector3<T> &operator/=(U f) {
        CHECK_NE(f, 0);
        Float inv = (Float)1 / f;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    // = compare
    bool operator==(const Vector3<T> &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    bool operator!=(const Vector3<T> &v) const {
        return x != v.x || y != v.y || z != v.z;
    }
    // unary negation
    Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
    // 4. normalization
    Float LengthSquared() const { return x * x + y * y + z * z; }
    Float Length() const { return std::sqrt(LengthSquared()); }
    // more methods are inlined, all put at the end!!!

    // Vector3 Public Data
    T x, y, z;
};

template <typename T> class Vector2 {
public:
    // Vector2 Public Methods
    // Vector2 Public Data
}

typedef Vector2<Float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector3<Float> Vector3f;
typedef Vector3<int> Vector3i;

// Geometry Inline Functions
// inline constructor ???
template <typename T>
inline Vector3<T>::Vector3(const Point3<T> &p)
    : x(p.x), y(p.y), z(p.z) {
    DCHECK(!HasNaNs());
}
// inline scalar multiply
template <typename T, typename U>
inline Vector3<T> operator*(U s, const Vector3<T> &v) {
    return v * s;
}
// inline abs
template <typename T>
Vector3<T> Abs(const Vector3<T> &v) {
    return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}
// normalization
template <typename T>
inline Vector3<T> Normalize(const Vector3<T> &v) {
    return v / v.Length();
}
// dot product
template <typename T>
inline T Dot(const Vector3<T> &v1, const Vector3<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T AbsDot(const Vector3<T> &v1, const Vector3<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    return std::abs(Dot(v1, v2));
}
// cross product
template <typename T>
inline Vector3<T> Cross(const Vector3<T> &v1, const Vector3<T> &v2) {
    DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;
    return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
                      (v1x * v2y) - (v1y * v2x));
}
// coordinate system from a vector
template <typename T>
inline void CoordinateSystem(const Vector3<T> &v1, Vector3<T> *v2,
                             Vector3<T> *v3) {
    if (std::abs(v1.x) > std::abs(v1.y))
        *v2 = Vector3<T>(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z);
    else
        *v2 = Vector3<T>(0, v1.z, -v1.y) / std::sqrt(v1.y * v1.y + v1.z * v1.z);
    *v3 = Cross(v1, *v2);
}
