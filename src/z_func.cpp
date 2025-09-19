#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <bit>
#include <bitset>
#include "z_func.h"

constexpr T ZERO = 1.0E-4;

// 常量定义
constexpr T Pi = 3.14159265358979323846;
constexpr T TWO_PI = 2.0 * Pi;
constexpr T HALF_PI = Pi / 2.0;

constexpr T LN2 = 0.693147180559945309417232121458176568;
constexpr T INV_LN2 = 1.4426950408889634073599246810018921;

constexpr T ONE_OVER_SQRT2 = 0.707106781186547524400844362104849039;

T ZMath::sqrt(T x)
{
	if (x < 0)
	{
		return NAN;
	}
	else if (x == 0)
	{
		return 0.0;
	}

	T y = x / 2;
	T ny = y;
	T eps;
	for (; ;)
	{
		ny = 0.5 * (y + x / (y));
		eps = ny - y;
		if (eps < ZERO && eps > -ZERO)
		{
			break;
		}
		y = ny;
	}

	return ny;
}

// 自定义取模函数，将x限制在[0, y)范围内
double mod(double x, double y) {
    double quotient = x / y;
    double floored = quotient >= 0.0 ? (int)quotient : (int)(quotient - 1.0);
    return x - floored * y;
}
T cos_taylor(T x)
{
    constexpr T INV_FACT2 = 0.5;
    constexpr T INV_FACT4 = 0.041666666666666666666;
    constexpr T INV_FACT6 = 0.0013888888888888888888;
    constexpr T INV_FACT8 = 0.0000248015873015873015;
    constexpr T INV_FACT10 = 2.7557319223985890652557E-7;

    T x2 = x * x;
    T x4 = x2 * x2;
    T x6 = x4 * x2;
    T x8 = x4 * x4;
    T x10 = x8 * x2;
    return 1 - INV_FACT2 * x2 + INV_FACT4 * x4 - INV_FACT6 * x6 + INV_FACT8 * x8 - INV_FACT10 * x10;
}
T sin_taylor(T x)
{
    constexpr T INV_FACT3 = 0.166666666666666666666;
    constexpr T INV_FACT5 = 0.0083333333333333333333;
    constexpr T INV_FACT7 = 0.00019841269841269841269;
    constexpr T INV_FACT9 = 2.755731922398589065E-6;
    constexpr T INV_FACT11 = 2.505210838544171877505E-8;
    T x2 = x * x;
    T x3 = x2 * x;
    T x5 = x2 * x3;
    T x7 = x5 * x2;
    T x9 = x7 * x2;
    T x11 = x9 * x2;

    return x - INV_FACT3 * x3 + INV_FACT5 * x5 - INV_FACT7 * x7 + INV_FACT9 * x9 - INV_FACT11 * x11;
}

T ZMath::sin(T x)
{
    x = mod(x, TWO_PI); // 缩减到[0, 2pi)

    int sign = 1;
    if (x > Pi)
    {
        sign = -1;
        x -= Pi; // 约化到 [0 - pi)
    }
    if (x > HALF_PI)
    {
        x = Pi - x; // 约化到 [0, pi/2)
    }

    if (x > Pi / 4)
    {
        T cx = HALF_PI - x;
        return sign * cos_taylor(cx);
    }
    else
    {
        return sign * sin_taylor(x);
    }
}
T ZMath::cos(T x)
{
    return ZMath::sin(HALF_PI - x);
}

// 辅助函数：计算floor
T Floor(T x) {
    int n = (int)x;
    return (x < 0 && x != n) ? n - 1 : n;
}
T ZMath::exp(T x)
{
    // 处理特殊情况
    if (x != x) return x;           // NaN
    if (x > 709.782712893384) return 1e308;  // 防止溢出
    if (x < -745.133219101941) return 0.0;   // 防止下溢

    // 将x分解为整数和小数部分：x = n*ln(2) + r
    // 这样 e^x = 2^n * e^r
    double n = Floor(x * INV_LN2);
    double r = x - n * LN2;

    // 使用改进的泰勒级数计算e^r
    // 在[-ln(2)/2, ln(2)/2]区间内，使用优化的多项式逼近
    const double c1 = 1.0;
    const double c2 = 1.0;
    const double c3 = 0.5;
    const double c4 = 0.166666666666666851703837;
    const double c5 = 0.041666666666666666446627;
    const double c6 = 0.008333333333333332974823;
    const double c7 = 0.001388888888888893316143;
    const double c8 = 0.000198412698412696162806;
    const double c9 = 0.000024801587301587297458;
    const double c10 = 0.000002755731922398589065;

    // 使用Horner方法计算多项式
    double r2 = r * r;
    double result = c1 + r * (c2 + r * (c3 + r * (c4 + r * (c5 + r * (c6 +
        r * (c7 + r * (c8 + r * (c9 + r * c10))))))));

    // 调整指数部分
    int int_n = (int)n;

    // 使用位操作实现2^n的整数部分
    const long long bias = 1023LL;
    long long bits;
    if (int_n < -1022) {
        // 处理非常小的指数
        bits = (long long)(n + bias + 1022) << 52;
        result *= *((double*)&bits);
        bits = (long long)(-1022 - bias) << 52;
        result *= *((double*)&bits);
    }
    else if (int_n > 1023) {
        // 处理非常大的指数
        bits = (long long)(n - 1023 + bias) << 52;
        result *= *((double*)&bits);
        bits = (long long)(1023LL) << 52;
        result *= *((double*)&bits);
    }
    else {
        // 正常范围的指数
        bits = (long long)(n + bias) << 52;
        result *= *((double*)&bits);
    }

    return result;
}

T ZMath::log(T x) 
{
    // 处理特殊情况
    if (x <= 0) return -INFINITY;  // 返回负无穷
    if (x != x) return x;          // NaN
    if (x == 1.0) return 0.0;      // log(1) = 0

    // 使用浮点数的二进制表示来提取指数和尾数
    union {
        T d;
        long long i;
    } u = { x };

    // 提取指数和尾数
    int exp = ((u.i >> 52) & 0x7FF) - 1023;  // 提取指数部分
    u.i = (u.i & 0x800FFFFFFFFFFFFFLL) | 0x3FF0000000000000LL;  // 规范化尾数到[1,2)
    x = u.d;

    // 如果x > sqrt(2)，将x除以2并调整指数
    if (x > 2.0) {
        x *= 0.5;
        exp++;
    }

    // 现在x在[1/sqrt(2), sqrt(2)]范围内
    // 使用变换 y = (x-1)/(x+1)
    T y = (x - 1.0) / (x + 1.0);
    T y2 = y * y;

    // 使用泰勒级数计算log((1+y)/(1-y))
    // log(x) = 2(y + y³/3 + y⁵/5 + y⁷/7 + ...)
    // 使用Horner方法优化多项式计算
    constexpr T c1 = 2.0;
    constexpr T c3 = 2.0 / 3.0;
    constexpr T c5 = 2.0 / 5.0;
    constexpr T c7 = 2.0 / 7.0;
    constexpr T c9 = 2.0 / 9.0;
    constexpr T c11 = 2.0 / 11.0;
    constexpr T c13 = 2.0 / 13.0;
    constexpr T c15 = 2.0 / 15.0;

    T result = y * (c1 + y2 * (c3 + y2 * (c5 + y2 * (c7 +
        y2 * (c9 + y2 * (c11 + y2 * (c13 + y2 * c15)))))));

    // 加上指数部分
    return result + exp * LN2;
}
T ZMath::log(T x, T y)
{
    return ZMath::log(y) / ZMath::log(x);
}

T ZMath::pow(T x, T y)
{
    if (x > 0)
    {
        return ZMath::exp(y * ZMath::log(x));
    }
    else if (x == 0.0)
    {
        if (y > 0) 
        {
            return 0.0;
        }
        else if (y == 0.0)
        {
            return 1.0;
        }
        else
        {
            return NAN;
        }
    }
    else
    {
        return NAN;
    }
}

T ZMath::pow(T x, int n)// 快速幂
{
    if (n == 0) return 1.0;          // x^0 = 1

    if (x == 0) {                    // 处理 x=0 的情况
        if (n < 0) return INFINITY; // 0的负次幂为无穷大
        else return 0.0;
    }

    long long N = n;                 // 避免 n = -2^31 时取反溢出
    if (N < 0) {                     // 处理负指数
        x = 1 / x;
        N = -N;
    }

    T result = 1.0;
    while (N > 0) {
        if (N & 1) {                 // 当前二进制位为1时，累乘当前底数
            result *= x;
        }
        x *= x;                      // 底数平方
        N >>= 1;                     // 右移一位，处理下一个二进制位
    }

    return result;
}

uint64_t ZMath::factorial(int n)
{
    if (n < 0) {
        throw std::invalid_argument("n必须是非负整数");
    }
    static std::vector<unsigned long long> cache = { 1 }; // cache[0] = 1
    if (n < cache.size()) {
        return cache[n];
    }
    for (int i = cache.size(); i <= n; ++i) {
        cache.push_back(cache.back() * i);
    }
    return cache[n];
}

void ZMath::hano(int n, char F, char A, char T)
{
    if (n == 1)
    {
        printf("move %d from %c to %c\n", n, F, T);
        return;
    }
    hano(n - 1, F, T, A);
    printf("move %d from %c to %c\n", n, F, T);
    hano(n - 1, A, F, T);
}

template <int Bits>
std::string ZMath::int2binary(typename IntType<Bits>::type value) 
{
    static_assert(Bits == 32 || Bits == 64, "Bits must be 32 or 64");
    using UIntType = typename IntType<Bits>::type;

    std::string result;
    result.reserve(Bits); // 预分配空间，优化性能

    UIntType mask = static_cast<UIntType>(1) << (Bits - 1); // 从最高位开始

    for (int i = 0; i < Bits; ++i) {
        result += (value & mask) ? '1' : '0';
        mask >>= 1;
    }

    return result;
}

// 显式实例化并导出模板函数
template ZMATH_API std::string ZMath::int2binary<32>(uint32_t);
template ZMATH_API std::string ZMath::int2binary<64>(uint64_t);


template <int Bits>
std::string ZMath::float2binary(typename FloatType<Bits>::type value)
{
    static_assert(Bits == 32 || Bits == 64, "Bits must be 32 or 64");
    
    using iType = typename IntType<Bits>::type;
    using fType = typename FloatType<Bits>::type;

    // 先转化为对应的整数(相同二进制表示)
    iType i_value;
    std::memcpy(&i_value, &value, sizeof(fType));

    std::string result;
    result.reserve(Bits); // 预分配空间，优化性能

    iType mask = static_cast<iType>(1) << (Bits - 1); // 从最高位开始

    for (int i = 0; i < Bits; ++i) {
        result += (i_value & mask) ? '1' : '0';
        mask >>= 1;
    }

    return result;
}

template ZMATH_API std::string ZMath::float2binary<32>(float);
template ZMATH_API std::string ZMath::float2binary<64>(double);