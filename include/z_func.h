#pragma once
#include "ZMathHeader.h"
#include <string>

typedef double T;

namespace ZMath {
	//T vecSum(const std::vector<T>& data);

	ZMATH_API T sqrt(T x);
	
	ZMATH_API T sin(T x);
	ZMATH_API T cos(T x);

	ZMATH_API T exp(T x);
	ZMATH_API T log(T x);
	ZMATH_API T log(T x, T y);

	ZMATH_API T pow(T x, T y);
	ZMATH_API T pow(T x, int n);

	ZMATH_API uint64_t factorial(int n);

	ZMATH_API void hano(int n, char F, char A, char T);

    
	template <int Bits> struct IntType;
	template <> struct IntType<32> { using type = uint32_t; };
	template <> struct IntType<64> { using type = uint64_t; };

	template <int Bits>
	ZMATH_API std::string int2binary(typename IntType<Bits>::type value);

	template <int Bits> struct FloatType;
	template <> struct FloatType<32> { using type = float; };
	template <> struct FloatType<64> { using type = double; };

	template <int Bits>
	ZMATH_API std::string float2binary(typename FloatType<Bits>::type value);

}

