#include "../../src/library/MemoryPool/MemoryPool.hpp"
#include "../../src/library/NativeInterfaceConverter/NativeInterfaceConverter.hpp"

#include <gtest/gtest.h>

TEST(NativerInterfaceConverter, Enum)
{
	enum class TestEnum
	{
		RIGHT= 1,
		LEFT = 2,
		UP = 3,
		DOWN = 4
	};

	{
		auto ret = EnumConverter<TestEnum>().toNativeType(1);
		EXPECT_EQ(ret, TestEnum::RIGHT);
	}
	{
		auto ret = EnumConverter<TestEnum>().toManagedType(TestEnum::UP);
		EXPECT_EQ(ret, 3);
	}
	{
		std::vector<int32_t> values{ 1, 2, 4, 3, 1 };
		auto enum_values = EnumConverter<TestEnum>().toNativeTypeArray(values);
		EXPECT_EQ(enum_values[3], TestEnum::UP);

		auto new_values = EnumConverter<TestEnum>().toManagedTypeArray(enum_values);
		
		for (size_t i = 0; i < values.size(); i++)
			EXPECT_EQ(values[i], new_values[i]);
	}
	{
		std::vector<int32_t> values{ 1, 2, 4, 3, 1 };
		auto enum_values = EnumConverter<TestEnum>().toNativeTypeVector(values);
		EXPECT_EQ(enum_values[3], TestEnum::UP);

		auto new_values = EnumConverter<TestEnum>().toManagedTypeVector(enum_values);

		for (size_t i = 0; i < values.size(); i++)
			EXPECT_EQ(values[i], new_values[i]);
	}

	{
		std::vector<int32_t> src{ 1, 2, 4,3, 1 };
		std::vector<TestEnum> dest;
		std::vector<int32_t> new_src;

		EnumConverter<TestEnum>().copyArray(src, dest);
		EnumConverter<TestEnum>().copyArray(dest, new_src);

		for (size_t i = 0; i < src.size(); i++)
			EXPECT_EQ(src[i], new_src[i]);
	}

	{
		std::vector<TestEnum> src{ TestEnum::RIGHT, TestEnum::LEFT, TestEnum::DOWN, TestEnum::UP };
		std::vector<int32_t> dest;
		std::vector<TestEnum> new_src;

		EnumConverter<TestEnum>().copyArray(src, dest);
		EnumConverter<TestEnum>().copyArray(dest, new_src);

		for (size_t i = 0; i < src.size(); i++)
			EXPECT_EQ(src[i], new_src[i]);
	}
}

