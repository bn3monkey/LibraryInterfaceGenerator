#include "../../src/library/MemoryPool/MemoryPool.hpp"
#include "../../src/library/ManagedTypeConverter/ManagedTypeConverter.hpp"

#include <gtest/gtest.h>
#include <functional>
#include <array>

TEST(ManagedTypeConverter, Primitive)
{
	using namespace Bn3Monkey;

	{
		int8_t value {'a'};
		auto mvalue = MInt8().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MInt8::ManagedType>('a'));
		auto nvalue = MInt8().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}
	{
		int16_t value{ 2 };
		auto mvalue = MInt16().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MInt16::ManagedType>(2));
		auto nvalue = MInt16().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}
	{
		int32_t value{ 1 << 17 };
		auto mvalue = MInt32().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MInt32::ManagedType>(1 << 17));
		auto nvalue = MInt32().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}
	{
		int64_t value{ 30000 };
		auto mvalue = MInt64().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MInt64::ManagedType>(30000));
		auto nvalue = MInt64().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}
	{
		float value{ 30000.0f };
		auto mvalue = MFloat().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MFloat::ManagedType>(30000.0f));
		auto nvalue = MFloat().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	} 
	{
		double value{ 30000.0 };
		auto mvalue = MDouble().toManagedType(value);
		EXPECT_EQ(mvalue, static_cast<Bn3Monkey::MDouble::ManagedType>(30000.0));
		auto nvalue = MDouble().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}
	{
		std::string value{ "sans" };
		auto mvalue = MString().toManagedType(value);
		EXPECT_EQ(mvalue, std::string{"sans"});
		auto nvalue = MString().toNativeType(mvalue);
		EXPECT_EQ(nvalue, value);
	}

}

TEST(ManagedTypeConverter, PrimitiveArray)
{
	using namespace Bn3Monkey;
	{
		using NativeArray = std::array<int8_t, 5>;
		using ManagedArray = std::array<int8_t, 5>;
		NativeArray src{ 'a','b','c','d','e' };
		ManagedArray converted_src{ 'a','b','c','d','e' };
		ManagedArray dest;

		using ArrayConverter = MArray<MInt8, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<int16_t, 5>;
		using ManagedArray = std::array<int16_t, 5>;
		NativeArray src{ 1, 2, 3, 4, 5 };
		ManagedArray converted_src{ 1, 2, 3, 4, 5 };
		ManagedArray dest;

		using ArrayConverter = MArray<MInt16, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<int32_t, 5>;
		using ManagedArray = std::array<int32_t, 5>;
		NativeArray src{ 1, 2, 3, 4, 5 };
		ManagedArray converted_src{ 1, 2, 3, 4, 5 };
		ManagedArray dest;

		using ArrayConverter = MArray<MInt32, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<int64_t, 5>;
		using ManagedArray = std::array<int64_t, 5>;
		NativeArray src{ 1, 2, 3, 4, 5 };
		ManagedArray converted_src{ 1, 2, 3, 4, 5 };
		ManagedArray dest;

		using ArrayConverter = MArray<MInt64, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<float, 5>;
		using ManagedArray = std::array<float, 5>;
		NativeArray src{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
		ManagedArray converted_src{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
		ManagedArray dest;

		using ArrayConverter = MArray<MFloat, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<double, 5>;
		using ManagedArray = std::array<double, 5>;
		NativeArray src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedArray converted_src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedArray dest;

		using ArrayConverter = MArray<MDouble, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}

	{
		using NativeArray = std::array<std::string, 5>;
		using ManagedArray = std::array<std::string, 5>;
		NativeArray src{ "sans", "papyrus", "undyne", "alpyhis", "asgore"};
		ManagedArray converted_src{ "sans", "papyrus", "undyne", "alpyhis", "asgore" };
		ManagedArray dest;

		using ArrayConverter = MArray<MString, 5>;

		auto mvalues = ArrayConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		ArrayConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
}

TEST(ManagedTypeConverter, PrimitiveVector)
{
	using namespace Bn3Monkey;

	{
		using NativeVector = std::vector<int8_t>;
		using ManagedVector = std::vector<int8_t>;
		NativeVector src{ 'a', 'b', 'c', 'd', 'e'};
		ManagedVector converted_src{ 'a','b','c','d','e' };
		ManagedVector dest;

		using VectorConverter = MVector<MInt8>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<int16_t>;
		using ManagedVector = std::vector<int16_t>;
		NativeVector src{ 1, 2, 3, 4, 5 };
		ManagedVector converted_src{ 1, 2, 3, 4, 5 };
		ManagedVector dest;

		using VectorConverter = MVector<MInt16>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<int32_t>;
		using ManagedVector = std::vector<int32_t>;
		NativeVector src{ 1, 2, 3, 4, 5 };
		ManagedVector converted_src{ 1, 2, 3, 4, 5 };
		ManagedVector dest;

		using VectorConverter = MVector<MInt32>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<int64_t>;
		using ManagedVector = std::vector<int64_t>;
		NativeVector src{ 1, 2, 3, 4, 5 };
		ManagedVector converted_src{ 1, 2, 3, 4, 5 };
		ManagedVector dest;

		using VectorConverter = MVector<MInt64>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<float>;
		using ManagedVector = std::vector<float>;
		NativeVector src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedVector converted_src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedVector dest;

		using VectorConverter = MVector<MFloat>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<double>;
		using ManagedVector = std::vector<double>;
		NativeVector src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedVector converted_src{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		ManagedVector dest;

		using VectorConverter = MVector<MDouble>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
	{
		using NativeVector = std::vector<std::string>;
		using ManagedVector = std::vector<std::string>;
		NativeVector src{ "sans", "papyrus", "undyne", "alpyhis", "asgore" };
		ManagedVector converted_src{ "sans", "papyrus", "undyne", "alpyhis", "asgore" };
		ManagedVector dest;

		using VectorConverter = MVector<MString>;

		auto mvalues = VectorConverter().toManagedType(src);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(mvalues[i], converted_src[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(nvalues[i], src[i]);
		}

		VectorConverter().copy(src, dest);
		for (size_t i = 0; i < src.size(); i++)
		{
			EXPECT_EQ(dest[i], converted_src[i]);
		}
	}
}

TEST(ManagedTypeConverter, Enum)
{
	using namespace Bn3Monkey;
	enum class TestEnum
	{
		RIGHT= 1,
		LEFT = 2,
		UP = 3,
		DOWN = 4
	};

	{
		auto ret = MEnum<TestEnum>().toNativeType(1);
		EXPECT_EQ(ret, TestEnum::RIGHT);
	}
	{
		auto ret = MEnum<TestEnum>().toManagedType(TestEnum::UP);
		EXPECT_EQ(ret, 3);
	}
	{
		std::array<TestEnum, 5> nativeArray{ TestEnum::RIGHT, TestEnum::LEFT, TestEnum::DOWN, TestEnum::UP, TestEnum::RIGHT };
		std::array<int32_t, 5> managedArray{ 1, 2, 4, 3, 1 };
		std::array<int32_t, 5> dest;

		using ArrayConverter = MArray<MEnum<TestEnum>, 5>;

		auto mvalues = ArrayConverter().toManagedType(nativeArray);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(mvalues[i], managedArray[i]);
		}

		auto nvalues = ArrayConverter().toNativeType(mvalues);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(nvalues[i], nativeArray[i]);
		}

		ArrayConverter().copy(nativeArray, dest);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(managedArray[i], dest[i]);
		}
	}

	{
		std::vector<TestEnum> nativeArray{ TestEnum::RIGHT, TestEnum::LEFT, TestEnum::DOWN, TestEnum::UP, TestEnum::RIGHT };
		std::vector<int32_t> managedArray{ 1, 2, 4, 3, 1 };
		std::vector<int32_t> dest;

		using VectorConverter = MVector<MEnum<TestEnum>>;

		auto mvalues = VectorConverter().toManagedType(nativeArray);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(mvalues[i], managedArray[i]);
		}

		auto nvalues = VectorConverter().toNativeType(mvalues);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(nvalues[i], nativeArray[i]);
		}

		VectorConverter().copy(nativeArray, dest);
		for (size_t i = 0; i < 5; i++)
		{
			EXPECT_EQ(managedArray[i], dest[i]);
		}
	}
}

TEST(ManagedTypeConverter, Object)
{
	class TestObject
	{
	public:
		char data[32];

		TestObject(const char* str)
		{
			strcpy_s(data, str);
		}
	};

	

	using namespace Bn3Monkey;
	Bn3Monkey::Bn3MemoryPool::Analyzer analyzer;
	ManagedTypeConverter::initialize();

	
	{
		auto managed = MObject<TestObject>().construct("sans");

		auto native = MObject<TestObject>().toNativeType(managed);
		EXPECT_EQ(native.use_count(), 2);

		auto new_managed = MObject<TestObject>().toManagedType(native);
		auto new_native = MObject<TestObject>().toNativeType(new_managed);
		EXPECT_EQ(native.use_count(), 4);

		EXPECT_STREQ(native->data, "sans");
		EXPECT_STREQ(new_native->data, "sans");

		MObject<TestObject>().release(managed);
		EXPECT_EQ(native.use_count(), 3);

		MObject<TestObject>().release(new_managed);
		EXPECT_EQ(native.use_count(), 2);

		new_native.reset();
		EXPECT_EQ(native.use_count(), 1);

		native.reset();
		EXPECT_EQ(native.use_count(), 0);
	}


	std::vector<const char*> object_names{ "dog", "cat", "horse", "sheep" };

	class TestArrayObject
	{
	public:
		std::array<std::shared_ptr<TestObject>, 4> object_array;
		char data[32];

		TestArrayObject(const char* str)
		{
			strcpy_s(data, str);
		}
	};

	{
		// Array Test
		auto managed_arrayobject = MObject<TestArrayObject>().construct("animal");
		auto native_arrayobject = MObject<TestArrayObject>().toNativeType(managed_arrayobject);

		for (size_t i =0 ; i<object_names.size();i++)
		{
			auto object_name = object_names[i];
			auto managed = MObject<TestObject>().construct(object_name);
			{
				auto native = MObject<TestObject>().toNativeType(managed);
				native_arrayobject->object_array[i] = native;
			}
			EXPECT_EQ(native_arrayobject->object_array[i].use_count(), 2);

			MObject<TestObject>().release(managed);
			EXPECT_EQ(native_arrayobject->object_array[i].use_count(), 1);
		}

		auto& arrays = native_arrayobject->object_array;
		{			
			auto managed_array = MArray<MObject<TestObject>, 4>().toManagedType(arrays);
			auto native_array = MArray<MObject<TestObject>, 4>().toNativeType(managed_array);
			
			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_array)
			{
				MObject<TestObject>().release(managed);
			}
		}
		{
			std::array<void*, 4> managed_array;
			MArray<MObject<TestObject>, 4>().copy(arrays, managed_array);
			auto native_array = MArray<MObject<TestObject>, 4>().toNativeType(managed_array);
			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_array)
			{
				MObject<TestObject>().release(managed);
			}
		}

		MObject<TestArrayObject>().release(managed_arrayobject);
	}


	class TestVectorObject
	{
	public:
		std::vector<std::shared_ptr<TestObject>> object_vector;
		char data[32];

		TestVectorObject(const char* str)
		{
			strcpy_s(data, str);
		}
	};

	{
		// Array Test
		auto managed_vectorobject = MObject<TestVectorObject>().construct("animal");
		auto native_vectorobject = MObject<TestVectorObject>().toNativeType(managed_vectorobject);

		for (size_t i = 0; i < object_names.size(); i++)
		{
			auto object_name = object_names[i];
			auto managed = MObject<TestObject>().construct(object_name);
			{
				auto native = MObject<TestObject>().toNativeType(managed);
				native_vectorobject->object_vector.push_back(native);
			}
			EXPECT_EQ(native_vectorobject->object_vector[i].use_count(), 2);

			MObject<TestObject>().release(managed);
			EXPECT_EQ(native_vectorobject->object_vector[i].use_count(), 1);
		}

		auto& vector = native_vectorobject->object_vector;
		{
			auto managed_vector = MVector<MObject<TestObject>>().toManagedType(vector);
			auto native_vector = MVector<MObject<TestObject>>().toNativeType(managed_vector);

			for (size_t i = 0; i < vector.size(); i++)
			{
				EXPECT_STREQ(vector[i]->data, native_vector[i]->data);
			}

			for (auto& managed : managed_vector)
			{
				MObject<TestObject>().release(managed);
			}
		}
		{
			std::vector<void*> managed_vector;
			MVector<MObject<TestObject>>().copy(vector, managed_vector);
			auto native_array = MVector<MObject<TestObject>>().toNativeType(managed_vector);
			for (size_t i = 0; i < vector.size(); i++)
			{
				EXPECT_STREQ(vector[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_vector)
			{
				MObject<TestObject>().release(managed);
			}
		}

		MObject<TestVectorObject>().release(managed_vectorobject);
	}

	analyzer.analyzeAll();
	ManagedTypeConverter::release();
}


TEST(ManagedTypeConverter, ObjectRelease)
{
	using namespace Bn3Monkey;

	class NativeObject
	{
	public:
		int num;
		NativeObject(int num) : num(num) {}
		int getNumber() { return num; }
	};
	class ManagedObject
	{
	public:
		ManagedObject(int num) {
			_handle = MObject<NativeObject>().construct(num);
			
			std::function<void()> releaser = [&]() {
				release();
			};
			ManagedTypeConverter::addReleaser(releaser);
		}
		ManagedObject(void* handle)
		{
			_handle = handle;
		}
		~ManagedObject()
		{
			release();
		}
		void release() {
			if (_handle)
			{
				MObject<NativeObject>().release(_handle);
				_handle = nullptr;
			}
		}

		int getNumber() {
			auto nativeObject = MObject<NativeObject>().toNativeType(_handle);
			return nativeObject->getNumber();
		}

		void* _handle{ nullptr };
	};

	Bn3Monkey::Bn3MemoryPool::Analyzer analyzer;
	
	
	
	{
		// Application LifeCycle
		ManagedTypeConverter::initialize();

		ManagedObject a(1);
		ManagedObject b(2);
		ManagedObject c(3);

		EXPECT_EQ(a.getNumber(), 1);
		EXPECT_EQ(b.getNumber(), 2);
		EXPECT_EQ(c.getNumber(), 3);

		{
			ManagedObject d(4);
			ManagedObject e(5);
			EXPECT_EQ(d.getNumber(), 4);
			EXPECT_EQ(e.getNumber(), 5);

		}
		ManagedTypeConverter::release();

		EXPECT_EQ(a._handle, nullptr);
		EXPECT_EQ(b._handle, nullptr);
		EXPECT_EQ(c._handle, nullptr);

	}

	analyzer.analyzeAll();
}

TEST(ManagedTypeConverter, Callback)
{
	using namespace Bn3Monkey;

	enum class TestEnum
	{
		A, B, C
	};
	struct TestClass
	{
		TestClass(int value) : value(value) {}
		int value;
	};

	Bn3Monkey::Bn3MemoryPool::Analyzer analyzer;
	ManagedTypeConverter::initialize();

	{
		MInt32::NativeType v1{ 2 };
		MDouble::NativeType v2{ 2.0 };
		MEnum<TestEnum>::NativeType v3 = TestEnum::B;
		MObject<TestClass>::NativeType v4 = std::shared_ptr<TestClass>(new TestClass(4));
		MVector<MObject<TestClass>>::NativeType v5 = std::vector<std::shared_ptr<TestClass>>{
			std::shared_ptr<TestClass>(new TestClass(1)),
			std::shared_ptr<TestClass>(new TestClass(2)),
			std::shared_ptr<TestClass>(new TestClass(3))
		};

		auto managed_callback = [v1, v2, v3, v4, v5](
			const MInt32::ManagedType& value1,
			const MDouble::ManagedType& value2,
			const MEnum<TestEnum>::ManagedType& value3,
			const MObject<TestClass>::ManagedType& value4,
			const MVector<MObject<TestClass>>::ManagedType& value5)
		{
			{
				auto value = MInt32().toNativeType(value1);
				EXPECT_EQ(v1, value);
			}
			{
				auto value = MDouble().toNativeType(value2);
				EXPECT_EQ(v2, value);
			}
			{
				auto value = MEnum<TestEnum>().toNativeType(value3);
				EXPECT_EQ(v3, value);
			}
			{
				auto value = MObject<TestClass>().toNativeType(value4);
				EXPECT_EQ(v4->value, value->value);

				// This handle must be contained by maanged language boejct and released when the managed language object is destroyed.
				MObject<TestClass>().release(value4);
			}
			{
				auto value = MVector<MObject<TestClass>>().toNativeType(value5);
				for (size_t i = 0; i < value.size(); i++)
				{
					EXPECT_EQ(v5[i]->value, value[i]->value);

					// This handle must be contained by maanged language boejct and released when the managed language object is destroyed.
					MObject<TestClass>().release(value5[i]);
				}
			}
		};

		auto native_callback = MCallback<MVoid, MInt32, MDouble, MEnum<TestEnum>, MObject<TestClass>, MVector<MObject<TestClass>>>().toNativeType(managed_callback);
		native_callback(v1, v2, v3, v4, v5);
	}

	{
		MArray<MString, 4>::NativeType v1{"sans", "papyrus", "undyne", "alpyhs"};
		MString::NativeType v2{ "PPAP" };

		auto managed_callback = [v1, v2](const MArray<MString, 4>::NativeType& value1, const MString::NativeType& value2) -> MInt32::ManagedType {
			{
				auto value = MArray<MString, 4>().toNativeType(value1);
				for (size_t i = 0; i < 4; i++)
				{
					EXPECT_EQ(v1[i], value1[i]);
				}
			}
			{
				auto value = MString().toNativeType(value2);
				EXPECT_EQ(v2, value);
			}
			return 4;
		};

		auto native_callback = MCallback<MInt32, MArray<MString, 4>, MString>().toNativeType(managed_callback);
		auto ret = native_callback(v1, v2);
		EXPECT_EQ(ret, MInt32().toNativeType(4));
	}

	{
		struct ManagedTestClass
		{
			void* handle;

			ManagedTestClass(void* handle) : handle(handle) {}
			~ManagedTestClass() {
				MObject<TestClass>().release(handle);
			}
		} *managedObject;

		auto managed_callback = [&managedObject]() {
			auto handle = MObject<TestClass>().construct(4);
			managedObject = new ManagedTestClass(handle);

			// This handle must be contained by maanged language boejct and released when the managed language object is destroyed.
			// managedObject.handle is destoryed when managedObject is destroyted;
			return handle;
		};

		auto native_callback = MCallback<MObject<TestClass>>().toNativeType(managed_callback);
		auto ret = native_callback();
		EXPECT_EQ(ret->value, 4);

		delete managedObject;
	}

	ManagedTypeConverter::release();
	analyzer.analyzeAll();
}