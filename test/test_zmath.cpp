#include "ZMathHeader.h"
#include "z_func.h"
#include "z_vector.h"
#include <iostream>
#include <bitset>
#include <bit>

int main()
{
	std::cout << "Hello! \n";
	
	auto a = ZMath::log(3.0);
	printf("log(3.0) = %.15f\n", a);

	{
		// 通过联合体，把浮点数转化为对应的整数（同样的二进制表示）
		union {
			float d;
			uint32_t i;
		} u = { 2.718f };// d 和 i 共用同一块内存
		std::cout << u.d << "的双关整数为：" << u.i << std::endl;

		// 用 bitset 将整数转换为 二进制字符串
		std::bitset<32> binary(u.i);
		std::cout << "该整数的二进制表示为：" << binary << std::endl;
		std::cout << std::endl;
	}
	
	{
		// 也可以用内存拷贝的方式实现
		float f = 3.14f;
		uint32_t i;
		std::memcpy(&i, &f, sizeof(float));
		std::cout << f << "的双关整数为：" << i << std::endl;
		std::cout << std::endl;
	}

	{
		// C++20支持std::bit_casr<T>()实现转换
		int i = 42;
		float f = std::bit_cast<float>(i);  // 将 int 的二进制直接复制到 float
		std::cout << "int32_t 42 的双关float为：" << f << std::endl; // 输出 5.88545e-44
		std::cout << std::endl;
	}
	
	{
		uint32_t inp = 1078523331;
		std::cout << inp << "的二进制表示为：" << ZMath::int2binary<32>(inp) << std::endl;
		float fp = 2.718f;
		std::cout << fp << "的二进制表示为：" << ZMath::float2binary<32>(fp) << std::endl;
		std::cout << std::endl;
	}

	//
	ZMath::vector<int> vec(3, ZMath::factorial(3));
	for (auto v : vec)
	{
		printf("vec[] = %d\n", v);
	}
	auto vptr = vec.data();

	//

	ZMath::hano(3, 'F', 'a', 'T');

	return 0;
}