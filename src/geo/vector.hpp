#ifndef TOPAZ_GEO_VECTOR_HPP
#define TOPAZ_GEO_VECTOR_HPP
#include "memory/align.hpp"
#include "memory/block.hpp"

namespace tz
{
    template<typename T, std::size_t S>
    class Vector
    {
    public:
        constexpr Vector(std::array<T, S> data);
        Vector() = default;

        const T& operator[](std::size_t idx) const;
        T& operator[](std::size_t idx);

        Vector<T, S>& operator+=(const Vector<T, S>& rhs);
        Vector<T, S> operator+(const Vector<T, S>& rhs) const;
        Vector<T, S>& operator-=(const Vector<T, S>& rhs);
        Vector<T, S> operator-(const Vector<T, S>& rhs) const;
        Vector<T, S>& operator*=(T scalar);
        Vector<T, S> operator*(T scalar) const;
        Vector<T, S>& operator/=(T scalar);
        Vector<T, S> operator/(T scalar) const;

        const T* data() const;
        T* data();

        T dot(const Vector<T, S>& rhs) const;
        T length() const;
    private:
        /// 16-byte alignment to make it as easy as possible to auto-vectorise.
        alignas(16) std::array<T, S> vec;
    };

    using Vec2 = Vector<float, 2>;
    using Vec3 = Vector<float, 3>;
    using Vec4 = Vector<float, 4>;

    template<typename T = float>
    Vector<T, 3> cross(const Vector<T, 3>& lhs, const Vector<T, 3>& rhs);
}

#include "geo/vector.inl"
#endif