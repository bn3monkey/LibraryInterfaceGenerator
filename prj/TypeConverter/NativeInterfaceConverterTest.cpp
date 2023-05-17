#include "../../src/library/MemoryPool/MemoryPool.hpp"
#include "../../src/library/NativeInterfaceConverter/NativeInterfaceConverter.hpp"

#include <gtest/gtest.h>
#include <functional>

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


TEST(NativerInterfaceConverter, Object)
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

	

	Bn3Monkey::Bn3MemoryPool::Analyzer analyzer;
	initializeNativeInterface();

	
	{
		auto managed = ObjectConverter<TestObject>().construct("sans");

		auto native = ObjectConverter<TestObject>().toNativeType(managed);
		EXPECT_EQ(native.use_count(), 2);

		auto new_managed = ObjectConverter<TestObject>().toManagedType(native);
		auto new_native = ObjectConverter<TestObject>().toNativeType(new_managed);
		EXPECT_EQ(native.use_count(), 4);

		EXPECT_STREQ(native->data, "sans");
		EXPECT_STREQ(new_native->data, "sans");

		ObjectConverter<TestObject>().release(managed);
		EXPECT_EQ(native.use_count(), 3);

		ObjectConverter<TestObject>().release(new_managed);
		EXPECT_EQ(native.use_count(), 2);

		new_native.reset();
		EXPECT_EQ(native.use_count(), 1);

		native.reset();
		EXPECT_EQ(native.use_count(), 0);
	}
	
	class TestArrayObject
	{
	public:
		std::vector<std::shared_ptr<TestObject>> objects;
		char data[32];

		TestArrayObject(const char* str)
		{
			strcpy_s(data, str);
		}
	};

	{
		auto managed_array = ObjectConverter<TestArrayObject>().construct("animal");
		auto native_array = ObjectConverter<TestArrayObject>().toNativeType(managed_array);

		std::vector<const char*> object_names{ "dog", "cat", "horse", "sheep" };
		for (auto& object_name : object_names)
		{
			auto managed = ObjectConverter<TestObject>().construct(object_name);
			{
				auto native = ObjectConverter<TestObject>().toNativeType(managed);
				native_array->objects.push_back(native);
			}
			EXPECT_EQ(native_array->objects.back().use_count(), 2);

			ObjectConverter<TestObject>().release(managed);
			EXPECT_EQ(native_array->objects.back().use_count(), 1);
		}

		auto& arrays = native_array->objects;
		{
			
			auto managed_array = ObjectConverter<TestObject>().toManagedTypeArray(arrays);
			auto native_array = ObjectConverter<TestObject>().toNativeTypeArray(managed_array);
			
			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_array)
			{
				ObjectConverter<TestObject>().release(managed);
			}

		}

		{

			auto managed_vector = ObjectConverter<TestObject>().toManagedTypeVector(arrays);
			auto native_vector = ObjectConverter<TestObject>().toNativeTypeVector(managed_vector);

			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_vector[i]->data);
			}

			for (auto& managed : managed_vector)
			{
				ObjectConverter<TestObject>().release(managed);
			}

		}

		{
			std::vector<void*> managed_array;
			ObjectConverter<TestObject>().copyArray(arrays, managed_array);

			std::vector<std::shared_ptr<TestObject>> native_array;
			ObjectConverter<TestObject>().copyArray(managed_array, native_array);

			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_array)
			{
				ObjectConverter<TestObject>().release(managed);
			}
		}

		{
			std::vector<void*> managed_array;
			ObjectConverter<TestObject>().copyVector(arrays, managed_array);

			std::vector<std::shared_ptr<TestObject>> native_array;
			ObjectConverter<TestObject>().copyVector(managed_array, native_array);

			for (size_t i = 0; i < arrays.size(); i++)
			{
				EXPECT_STREQ(arrays[i]->data, native_array[i]->data);
			}

			for (auto& managed : managed_array)
			{
				ObjectConverter<TestObject>().release(managed);
			}
		}
	}


	analyzer.analyzeAll();
	releaseNativeInterface();
}



TEST(NativerInterfaceConverter, ObjectRelease)
{
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
			_handle = ObjectConverter<NativeObject>().construct(num);
			
			std::function<void()> releaser = [&]() {
				release();
			};
			addReleaser(reinterpret_cast<void *>(&releaser));
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
				ObjectConverter<NativeObject>().release(_handle);
				_handle = nullptr;
			}
		}

		int getNumber() {
			auto nativeObject = ObjectConverter<NativeObject>().toNativeType(_handle);
			return nativeObject->getNumber();
		}

		void* _handle{ nullptr };
	};

	Bn3Monkey::Bn3MemoryPool::Analyzer analyzer;
	
	
	
	{
		// Application LifeCycle
		initializeNativeInterface();

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
		releaseNativeInterface();

		EXPECT_EQ(a._handle, nullptr);
		EXPECT_EQ(b._handle, nullptr);
		EXPECT_EQ(c._handle, nullptr);

	}

	analyzer.analyzeAll();
}

TEST(NativerInterfaceConverter, Callback)
{
	using SumCallback = std::function<int32_t(int32_t, int32_t)>;

	SumCallback callback = [](int32_t value, int32_t value2) {
		return value + value2;
	};

	// auto managed_callback = CallbackConverter<SumCallback>().toManagedType(callback);
	auto* managed_callback = reinterpret_cast<void*>(&callback);
	auto native_callback = CallbackConverter<SumCallback>().toNativeType(managed_callback);

	EXPECT_EQ(native_callback(2, 3), 5);
}