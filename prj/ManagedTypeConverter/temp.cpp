
/*
template<typename _NativeType, typename = void>
class ManagedTypeConverter
{
public:
	static constexpr bool enable = false;
};

template<>
class ManagedTypeConverter<void>
{
public:
	static constexpr bool enable = true;
	using NativeType = void;
	using ManagedType = void;
};

template<>
class ManagedTypeConverter<int8_t>
{
public:
	static constexpr bool enable = true;
	using NativeType = int8_t;
	using ManagedType = int8_t;

};

template<>
class ManagedTypeConverter<int16_t>
{
public:
	static constexpr bool enable = true;
	using NativeType = int16_t;
	using ManagedType = int16_t;
};

template<>
class ManagedTypeConverter<int32_t>
{
public:
	static constexpr bool enable = true;
	using NativeType = int32_t;
	using ManagedType = int32_t;

};

template<>
class ManagedTypeConverter<int64_t>
{
public:
	static constexpr bool enable = true;
	using NativeType = int64_t;
	using ManagedType = int64_t;
};

template<>
class ManagedTypeConverter<float>
{
public:
	static constexpr bool enable = true;
	using NativeType = float;
	using ManagedType = float;
};

template<>
class ManagedTypeConverter<double>
{
public:
	static constexpr bool enable = true;
	using NativeType = double;
	using ManagedType = double;

};

template<>
class ManagedTypeConverter<std::string>
{
public:
	static constexpr bool enable = true;
	using NativeType = std::string;
	using ManagedType = std::string;
};

template<typename NativeEnum>
class ManagedTypeConverter<NativeEnum, typename std::enable_if<std::is_enum<NativeEnum>::value>::type>
{
public:
	static constexpr bool enable = true;
	using NativeType = NativeEnum;
	using ManagedType = int32_t;
};

template<typename NativeObject>
class ManagedTypeConverter<NativeObject, typename std::enable_if<std::is_class<NativeObject>::value>::type>
{
public:
	static constexpr bool enable = true;
	using NativeType = NativeObject;
	using ManagedType = void*;
};

template<class T>
struct IsNativeTypeCallback
{
	static constexpr bool value = false;
};

template<class NativeReturnType, class... NativeArgType>
struct IsNativeTypeCallback<std::function<NativeReturnType(NativeArgType...)>>
{
	static constexpr bool value = true;
};

template<typename NativeCallback>
class ManagedTypeConverter<NativeCallback, typename std::enable_if<IsNativeTypeCallback<NativeCallback>::value>::type>
{
public:
	static constexpr bool enable = true;
	using NativeType = NativeObject;
	using ManagedType = void*;

};

template<typename NativeType, size_t size>
class ManagedTypeConverter<std::array<NativeType, size>, typename std::enable_if<ManagedTypeConverter<NativeType>::enable>::type>
{

};

template<typename NativeType>
class ManagedTypeConverter<std::vector<NativeType>, typename std::enable_if<ManagedTypeConverter<NativeType>::enable>::type>
{

};
*/



/*
template<typename _NativeType, typename _ManagedType>
class ManagedTypeConverter
{
public:

	using NativeType = _NativeType;
	using ManagedType = _ManagedType;

	using NativeTypeArray = std::vector<NativeType>;
	using ManagedTypeArray = std::vector<ManagedType>;

	using NativeTypeVector = std::vector<NativeType>;
	using ManagedTypeVector = std::vector<ManagedType>;

	virtual NativeType toNativeType(const ManagedType& value) = 0;
	virtual ManagedType toManagedType(const NativeType& value) = 0;

	NativeTypeArray toNativeTypeArray(const ManagedTypeArray& values)
	{
		NativeTypeArray ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const ManagedType& value) {
			return toNativeType(value);
			});
		return ret;
	}

	ManagedTypeArray toManagedTypeArray(const NativeTypeArray& values)
	{
		ManagedTypeArray ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const NativeType& value) {
			return toManagedType(value);
			});
		return ret;
	}

	NativeTypeVector toNativeTypeVector(const ManagedTypeVector& values)
	{
		NativeTypeVector ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const ManagedType& value) {
			return toNativeType(value);
			});
		return ret;
	}

	ManagedTypeVector toManagedTypeVector(const NativeTypeVector& values)
	{
		ManagedTypeVector ret;
		std::transform(values.begin(), values.end(), std::back_inserter(ret), [&](const NativeType& value) {
			return toManagedType(value);
			});
		return ret;
	}

	void copyArray(const NativeTypeArray& src, ManagedTypeArray& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const NativeType& value) {
			return toManagedType(value);
			});
	}

	void copyArray(const ManagedTypeArray& src, NativeTypeArray& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const ManagedType& value) {
			return toNativeType(value);
			});
	}

	void copyVector(const NativeTypeVector& src, ManagedTypeVector& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const NativeType& value) {
			return toManagedType(value);
			});
	}

	void copyVector(const ManagedTypeVector& src, NativeTypeVector& dest)
	{
		dest.clear();
		std::transform(src.begin(), src.end(), std::back_inserter(dest), [&](const ManagedType& value) {
			return toNativeType(value);
			});
	}
protected:
#ifdef __BN3MONKEY_MEMORY_POOL__
		Bn3Monkey::Bn3Tag handleTag() {
			static int num{ 0 };
			char tag[32]{ 0 };
			sprintf(tag, "managedhandle_%d", num++);
			return Bn3Monkey::Bn3Tag(tag);
		}
		Bn3Monkey::Bn3Tag objectTag() {
			static int num{ 0 };
			char tag[32]{ 0 };
			sprintf(tag, "managedobject_%d", num++);
			return Bn3Monkey::Bn3Tag(tag);

		}
#endif
	};

	template<typename NativeEnum, typename ManagedEnum = int32_t>
	class EnumConverter : public ManagedTypeConverter<NativeEnum, ManagedEnum>
	{
	public:
		using BaseConverter = ManagedTypeConverter<NativeEnum, ManagedEnum>;
		using NativeType = typename BaseConverter::NativeType;
		using ManagedType = typename BaseConverter::ManagedType;

		NativeType toNativeType(const ManagedType& value) override
		{
			return static_cast<NativeType>(value);
		}
		ManagedType toManagedType(const NativeType& value) override
		{
			return static_cast<ManagedType>(value);
		}

	};

	template<typename Type, typename NativeObject = std::shared_ptr<Type>, typename ManagedObject = void*>
	class ObjectConverter : public ManagedTypeConverter<NativeObject, ManagedObject>
	{
	public:
		using BaseConverter = ManagedTypeConverter<NativeObject, ManagedObject>;
		using NativeType = typename BaseConverter::NativeType;
		using ManagedType = typename BaseConverter::ManagedType;


		NativeType toNativeType(const ManagedType& value) override
		{
			NativeType* ref = reinterpret_cast<NativeType*>(value);
			return *ref;
		}

		ManagedType toManagedType(const NativeType& value) override
		{
			return clone(value);
		}

		template<class... Args>
		ManagedObject construct(Args... args)
		{
			NativeObject* ret = createManagedHandle();

#ifdef __BN3MONKEY_MEMORY_POOL__
			* ret = NativeObject(Bn3Monkey::Bn3MemoryPool::construct<Type>(BaseConverter::objectTag(), std::forward<Args>(args)...), [](Type* p) {
				Bn3Monkey::Bn3MemoryPool::destroy<Type>(p);
				});
#else
			* ret = new Type(args...);
#endif
			return reinterpret_cast<ManagedObject>(ret);
		}

		ManagedObject clone(const NativeObject& src)
		{
			NativeObject* ret = createManagedHandle();
			*ret = src;
			return reinterpret_cast<ManagedObject>(ret);
		}

		void release(const ManagedObject& ref)
		{
			NativeObject* ret = reinterpret_cast<NativeObject*>(ref);
#ifdef __BN3MONKEY_MEMORY_POOL__
			Bn3Monkey::Bn3MemoryPool::destroy(ret);
#else
			delete ret;
#endif
		}


	private:

		NativeObject* createManagedHandle()
		{
#ifdef __BN3MONKEY_MEMORY_POOL__
			return Bn3Monkey::Bn3MemoryPool::construct<NativeObject>(BaseConverter::handleTag());
#else
			return new NativeObject();
#endif
		}

	};


	template<typename NativeCallback, typename ManagedCallback = void*>
	class CallbackConverter : public ManagedTypeConverter<NativeCallback, ManagedCallback>
	{
	public:
		using BaseConverter = ManagedTypeConverter<NativeCallback, ManagedCallback>;
		using NativeType = typename BaseConverter::NativeType;
		using ManagedType = typename BaseConverter::ManagedType;

		NativeType toNativeType(const ManagedType& value) override
		{
			auto callback = reinterpret_cast<NativeType*>(value);
			return *callback;
		}
		ManagedType toManagedType(const NativeType& value) override
		{
			// NOT USED
			// return reinterpret_cast<BaseConverter::ManagedType>(&value);
			return nullptr;
		}

	};
}
*/