#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "../../Auxiliary/StringHelper.hpp"
#include "SymbolObject.hpp"
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        
        class SymbolType
        {
        public:
            enum class Name
            {
                INVALID, 
                VOID,
                BOOL,
                INT8,
                INT16,
                INT32,
                INT64,
                FLOAT,
                DOUBLE,
                STRING,
                ENUM,
                OBJECT,
                CALLBACK,

                BOOLARRAY,
                INT8ARRAY,
                INT16ARRAY,
                INT32ARRAY,
                INT64ARRAY,
                FLOATARRAY,
                DOUBLEARRAY,
                STRINGARRAY,
                ENUMARRAY,
                OBJECTARRAY,
                CALLBACKARRAY,

                BOOLVECTOR,
                INT8VECTOR,
                INT16VECTOR,
                INT32VECTOR,
                INT64VECTOR,
                FLOATVECTOR,
                DOUBLEVECTOR,
                STRINGVECTOR,
                ENUMVECTOR,
                OBJECTVECTOR,
                CALLBACKVECTOR
            };

            virtual Name getTypeName() {return Name::INVALID;}
            virtual bool valid() { return false; }
            virtual bool isPrimitive() { return false; }
            virtual bool requiredDeclaration() { return false; }

            virtual std::string toNativeType() { return ""; }
            virtual std::string toManagedType() { return ""; }
            virtual std::string toJNIType() { return ""; }
            virtual std::string toKotlinType() { return ""; }

        };

        class SymbolTypeVoid : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::VOID;}
            std::string toNativeType() override {return "void";}
            std::string toManagedType() override { return "void"; }
            std::string toJNIType() override {return "void";}
            std::string toKotlinType() override {return "Unit"; }
        };

        class SymbolTypeBool : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOL;}
            std::string toNativeType() override { return "bool"; }
            std::string toManagedType() override {return "bool"; }
            std::string toJNIType() override { return "jboolean"; }
            std::string toKotlinType() override { return "Boolean"; }
        };

        class SymbolTypeInt8 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8;}
            std::string toNativeType() override { return "int8_t"; }
            std::string toManagedType() override {return "int8_t"; }
            std::string toJNIType() override { return "jbyte"; }
            std::string toKotlinType() override { return "Byte"; }

        };
        class SymbolTypeInt16 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16;}
            std::string toNativeType() override { return "int16_t"; }
            std::string toManagedType() override {return "int16_t"; }
            std::string toJNIType() override { return "jshort"; }
            std::string toKotlinType() override { return "Short"; }
        };
        class SymbolTypeInt32 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32;}
            std::string toNativeType() override { return "int32_t"; }
            std::string toManagedType() override {return "int32_t"; }
            std::string toJNIType() override { return "jint"; }
            std::string toKotlinType() override { return "Int"; }
        };
        class SymbolTypeInt64 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64;}
            std::string toNativeType() override { return "int64_t"; }
            std::string toManagedType() override {return "int64_t"; }
            std::string toJNIType() override { return "jlong"; }
            std::string toKotlinType() override { return "Long"; }
        };
        class SymbolTypeDouble : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLE;}
            std::string toNativeType() override { return "double"; }
            std::string toManagedType() override {return "double"; }
            std::string toJNIType() override { return "jdouble"; }
            std::string toKotlinType() override { return "Double"; }
        };
        class SymbolTypeFloat : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOAT;}
            std::string toNativeType() override { return "float"; }
            std::string toManagedType() override {return "float"; }
            std::string toJNIType() override { return "jfloat"; }
            std::string toKotlinType() override { return "Float"; }
        };
        class SymbolTypeString : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRING;}
            std::string toNativeType() override { return "std::string"; }
            std::string toManagedType() override {return "std::string"; }
            std::string toJNIType() override { return "jstring"; }
            std::string toKotlinType() override { return "String"; }
        };
       
        class SymbolTypeEnum : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override {return Name::ENUM;}
            SymbolTypeEnum(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};

            std::string toNativeType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toManagedType() override { return "int32_t"; }
            std::string toJNIType() override {
                return "jint";
            }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };
        class SymbolTypeObject: public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override {return Name::OBJECT;}
            SymbolTypeObject(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};

            std::string toNativeType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = "std::shared_ptr<";
                    ret += object->getCppName();
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toManagedType() override { return "void*"; }
            std::string toJNIType() override {
                return "jlong";
            }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }

        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        class SymbolTypeCallback : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override { return Name::CALLBACK; }
            SymbolTypeCallback(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toNativeType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toManagedType() override { 
                // return "void*";
                if (auto object = _obj.lock())
                {
                    std::string ret{ "_" };
                    ret += object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {
                return "jobject";
            }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }

        private:
            std::weak_ptr<SymbolObject> _obj;
        };


        class SymbolTypeBaseArray : public SymbolType
        {
        public:
            SymbolTypeBaseArray(size_t size) : size(size) {}
            virtual std::string toNativeElementType() = 0;
            virtual std::string toManagedElementType() = 0;
            virtual std::string toJNIElementType() = 0;
            virtual std::string toKotlinElementType() = 0;
        protected:
            size_t size;
        };
        
        template<class T>
        class SymbolTypeArray : public SymbolTypeBaseArray
        {
            
        };

        template<>
        class SymbolTypeArray <SymbolTypeBool> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOLARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}

            using ElementType = SymbolTypeBool;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt8> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8ARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeInt8;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt16> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16ARRAY;}
            
            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeInt16;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt32> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32ARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeInt32;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt64> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64ARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeInt64;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeFloat> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOATARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeFloat;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeDouble> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLEARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeDouble;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toJNIType().c_str());
                return std::string(buffer);
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "%sArray", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeString> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRINGARRAY;}

            SymbolTypeArray(size_t size) : SymbolTypeBaseArray(size) {}
            using ElementType = SymbolTypeString;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobjectArray";
            }
            std::string toKotlinType() override {
                return "Array<String>";
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeEnum> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override {return Name::ENUMARRAY;}

            SymbolTypeArray(std::weak_ptr<SymbolObject> obj, size_t size) :
                _obj(obj), SymbolTypeBaseArray(size) {};

            using ElementType = SymbolTypeEnum;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobjectArray";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "Array<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeArray<SymbolTypeObject> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override {return Name::OBJECTARRAY;}

            SymbolTypeArray(std::weak_ptr<SymbolObject> obj, size_t size) :
                _obj(obj), SymbolTypeBaseArray(size) {};

            using ElementType = SymbolTypeObject;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobjectArray";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "Array<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeArray<SymbolTypeCallback> : public SymbolTypeBaseArray
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override { return Name::CALLBACKARRAY; }

            SymbolTypeArray(std::weak_ptr<SymbolObject> obj, size_t size) :
                _obj(obj), SymbolTypeBaseArray(size) {};

            using ElementType = SymbolTypeObject;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toNativeElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::array<%s, %d>", toManagedElementType().c_str(), size);
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobjectArray";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "Array<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        class SymbolTypeBaseVector: public SymbolType
        {
        public:
            virtual std::string toNativeElementType() = 0;
            virtual std::string toManagedElementType() = 0;
            virtual std::string toJNIElementType() = 0;
            virtual std::string toKotlinElementType() = 0;
        };

        template<class T>
        class SymbolTypeVector : public SymbolTypeBaseVector
        {

        };

        template<>
        class SymbolTypeVector <SymbolTypeBool> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOLVECTOR;}

            using ElementType = SymbolTypeBool;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt8> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8VECTOR;}

            using ElementType = SymbolTypeInt8;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt16> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16VECTOR;}
            
            using ElementType = SymbolTypeInt16;
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt32> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32VECTOR;}
            using ElementType = SymbolTypeInt32;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt64> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64VECTOR;}
            using ElementType = SymbolTypeInt64;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeFloat> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOATVECTOR;}
            using ElementType = SymbolTypeFloat;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeDouble> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLEVECTOR;}

            using ElementType = SymbolTypeDouble;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeString> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRINGVECTOR;}

            using ElementType = SymbolTypeString;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType().toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType().toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType().toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeEnum> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::ENUMVECTOR;}
            using ElementType = SymbolTypeEnum;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeVector<SymbolTypeObject> : public SymbolTypeBaseVector
        {
        public:
            bool valid() { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::OBJECTVECTOR;}

            using ElementType = SymbolTypeEnum;            
            std::string toNativeType() override 
            {
                char buffer[128] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[128] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeVector<SymbolTypeCallback> : public SymbolTypeBaseVector
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override { return Name::CALLBACKVECTOR; }
            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            
            using ElementType = SymbolTypeEnum;            
            std::string toNativeType() override 
            {
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toNativeElementType().c_str());
                return std::string(buffer);
            }
            std::string toManagedType() override { 
                char buffer[256] {0};
                sprintf(buffer, "std::vector<%s>", toManagedElementType().c_str());
                return std::string(buffer);
            }
            std::string toJNIType() override { 
                return "jobject";
            }
            std::string toKotlinType() override {
                char buffer[256] {0};
                sprintf(buffer, "MutableList<%s>", toKotlinElementType().c_str());
                return std::string(buffer);
            }
            std::string toNativeElementType() override
            {
                return ElementType(_obj).toNativeType();
            }
            std::string toManagedElementType() override
            {
                return ElementType(_obj).toManagedType();
            }
            std::string toJNIElementType() override
            {
                return ElementType(_obj).toJNIType();
            }
            std::string toKotlinElementType() override
            {
                return ElementType(_obj).toKotlinType();
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        std::unique_ptr<LibraryInterfaceGenerator::Implementation::SymbolType> makeType(
            const std::string& type,
            const SymbolObjectTable& objectTable,
            const SymbolEnumTable& enumTable,
            const SymbolCallbackTable& callbackTable,
            ObjectReferenceSet* objectReferenceSet,
            EnumReferenceSet* enumReferenceSet,
            CallbackReferenceSet* callbackReferenceSet
            );

        class HasSymbolType
        {
        public:
            std::unique_ptr<SymbolType> type;
            Result change(SymbolObjectTable& objectTable, SymbolEnumTable& enumTable, SymbolCallbackTable& callbackTable);
            void registerReferenceSet(ObjectReferenceSet* objectReferenceSet, EnumReferenceSet* enumReferenceSet, CallbackReferenceSet* callbackReferenceSet);
        
        protected:
            std::string _type;
            ObjectReferenceSet* _objectReferenceSet{ nullptr };
            EnumReferenceSet* _enumReferenceSet{ nullptr };
            CallbackReferenceSet* _callbackReferenceSet{ nullptr };
        };

    }
}

#endif