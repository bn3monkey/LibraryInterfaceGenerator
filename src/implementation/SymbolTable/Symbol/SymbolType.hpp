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

            virtual std::string toCppType() { return ""; }
            virtual std::string toCppInterfaceType() { return ""; }
            virtual std::string toJNIType() { return ""; }
            virtual std::string toKotlinWrapperType() { return ""; }
            virtual std::string toKotlinType() { return ""; }

            virtual std::string toCppInnerType() { return ""; }
            virtual std::string toCppInterfaceInnerType() { return ""; }
            virtual std::string toKotlinInnerType() { return ""; }
        };

        class SymbolTypeVoid : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::VOID;}
            std::string toCppType() override {return "void";}
            std::string toCppInterfaceType() override { return "void"; }
            std::string toJNIType() override {return "void";}
            std::string toKotlinWrapperType() override { return "Unit"; }
            std::string toKotlinType() override {return "Unit"; }
            std::string toKotlinInnerType() override { return "Unit"; }
        };

        class SymbolTypeBool : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOL;}
            std::string toCppType() override { return "bool"; }
            std::string toCppInterfaceType() override { return "bool"; }
            std::string toJNIType() override { return "jboolean"; }
            std::string toKotlinWrapperType() override { return "Boolean"; }
            std::string toKotlinType() override { return "Boolean"; }
            std::string toKotlinInnerType() override { return "Boolean"; }
        };

        class SymbolTypeInt8 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8;}
            std::string toCppType() override {return "int8_t";}
            std::string toCppInterfaceType() override { return "int8_t"; }
            std::string toJNIType() override {return "jbyte";}
            std::string toKotlinWrapperType() override { return "Byte"; }
            std::string toKotlinType() override {return "Byte"; }
            std::string toKotlinInnerType() override { return "Byte"; }
        };
        class SymbolTypeInt16 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16;}
            std::string toCppType() override {return "int16_t";}
            std::string toCppInterfaceType() override { return "int16_t"; }
            std::string toJNIType() override {return "jshort";}
            std::string toKotlinWrapperType() override { return "Short"; }
            std::string toKotlinType() override {return "Short"; }
            std::string toKotlinInnerType() override { return "Short"; }
        };
        class SymbolTypeInt32 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32;}
            std::string toCppType() override {return "int32_t";}
            std::string toCppInterfaceType() override { return "int32_t"; }
            std::string toJNIType() override {return "jint";}
            std::string toKotlinWrapperType() override { return "Int"; }
            std::string toKotlinType() override {return "Int"; }
            std::string toKotlinInnerType() override { return "Int"; }
        };
        class SymbolTypeInt64 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64;}
            std::string toCppType() override {return "int64_t";}
            std::string toCppInterfaceType() override { return "int64_t"; }
            std::string toJNIType() override {return "jlong";}
            std::string toKotlinWrapperType() override { return "Long"; }
            std::string toKotlinType() override {return "Long"; }
            std::string toKotlinInnerType() override { return "Long"; }
        };
        class SymbolTypeDouble : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLE;}
            std::string toCppType() override {return "double";}
            std::string toCppInterfaceType() override { return "double"; }
            std::string toJNIType() override {return "jdouble";}
            std::string toKotlinWrapperType() override { return "Double"; }
            std::string toKotlinType() override {return "Double"; }
            std::string toKotlinInnerType() override { return "Double"; }
        };
        class SymbolTypeFloat : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return true; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOAT;}
            std::string toCppType() override {return "float";}
            std::string toCppInterfaceType() override { return "float"; }
            std::string toJNIType() override {return "jfloat";}
            std::string toKotlinWrapperType() override { return "Float"; }
            std::string toKotlinType() override {return "Float"; }
            std::string toKotlinInnerType() override { return "Float"; }
        };
        class SymbolTypeString : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRING;}
            std::string toCppType() override {return "std::string";}
            std::string toCppInterfaceType() override { return "std::string"; }
            std::string toJNIType() override {return "jstring";}
            std::string toKotlinWrapperType() override { return "String"; }
            std::string toKotlinType() override {return "String"; }
            std::string toKotlinInnerType() override { return "String"; }
        };
       
        class SymbolTypeEnum : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override {return Name::ENUM;}
            SymbolTypeEnum(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "int32_t"; }
            std::string toJNIType() override {
                return "jint";
            }
            std::string toKotlinWrapperType() override { return "Int"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }
            std::string toKotlinInnerType() override { 
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
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = "std::shared_ptr<";
                    ret += object->getCppName();
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "void*"; }
            std::string toJNIType() override {
                return "jlong";
            }
            std::string toKotlinWrapperType() override { return "Long"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }

            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override { return "void*"; }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
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
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "void*"; }
            std::string toJNIType() override {
                return "jobject";
            }
            std::string toKotlinWrapperType() override { return "Any"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }

            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override { return "void*"; }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<class T>
        class SymbolTypeArray : public SymbolType
        {
            
        };

        template<>
        class SymbolTypeArray <SymbolTypeBool> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOLARRAY;}
            std::string toCppType() override { return "std::vector<bool>"; }
            std::string toCppInterfaceType() override { return "std::vector<bool>"; }
            std::string toJNIType() override { return "jbooleanArray"; }
            std::string toKotlinType() override { return "BooleanArray"; }
            std::string toKotlinWrapperType() override { return "BooleanArray"; }

            using InnerType = SymbolTypeBool;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt8> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8ARRAY;}
            std::string toCppType() override {return "std::vector<char>";}
            std::string toCppInterfaceType() override { return "std::vector<char>"; }
            std::string toJNIType() override {return "jbyteArray";}  
            std::string toKotlinType() override {return "ByteArray"; }
            std::string toKotlinWrapperType() override { return "ByteArray"; }

            using InnerType = SymbolTypeInt8;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt16> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16ARRAY;}
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int16_t>"; }
            std::string toJNIType() override {return "jshortArray";}  
            std::string toKotlinType() override {return "ShortArray"; }
            std::string toKotlinWrapperType() override { return "ShortArray"; }

            using InnerType = SymbolTypeInt16;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt32> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32ARRAY;}
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int32_t>"; }
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; }
            std::string toKotlinWrapperType() override { return "IntArray"; }

            using InnerType = SymbolTypeInt32;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt64> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64ARRAY;}
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int64_t>"; }
            std::string toJNIType() override {return "jlongArray";}  
            std::string toKotlinType() override {return "LongArray"; }
            std::string toKotlinWrapperType() override { return "LongArray"; }

            using InnerType = SymbolTypeInt64;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeFloat> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOATARRAY;}
            std::string toCppType() override {return "std::vector<float>";}
            std::string toCppInterfaceType() override { return "std::vector<float>"; }
            std::string toJNIType() override {return "jfloatArray";}  
            std::string toKotlinType() override {return "FloatArray"; }
            std::string toKotlinWrapperType() override { return "FloatArray"; }

            using InnerType = SymbolTypeFloat;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeDouble> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLEARRAY;}
            std::string toCppType() override {return "std::vector<double>";}
            std::string toCppInterfaceType() override { return "std::vector<double>"; }
            std::string toJNIType() override {return "jdoubleArray";}  
            std::string toKotlinType() override {return "DoubleArray"; }
            std::string toKotlinWrapperType() override { return "DoubleArray"; }

            using InnerType = SymbolTypeDouble;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeString> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRINGARRAY;}
            std::string toCppType() override { return "std::vector<std::string>"; }
            std::string toCppInterfaceType() override { return "std::vector<std::string>"; }
            std::string toJNIType() override { return "jobjectArray"; }
            std::string toKotlinType() override { return "Array<String>"; }
            std::string toKotlinWrapperType() override { return "Array<String>"; }

            using InnerType = SymbolTypeString;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeArray<SymbolTypeEnum> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeArray(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::ENUMARRAY;}
            std::string toCppType() override {   
                if (auto object = _obj.lock())
                {
                    auto name = object->getCppName();
                    std::string ret {"std::vector<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { 
                return "std::vector<int>"; 
            }
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getCppName();
                    std::string ret{ "Array<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toKotlinWrapperType() override { return "IntArray"; }

            using InnerType = SymbolTypeEnum;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override {
                return "int";
            }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeArray<SymbolTypeObject> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeArray(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::OBJECTARRAY;}
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getCppName();
                    std::string ret{ "std::vector<std::shared_ptr<" };
                    ret += name;
                    ret += ">*>";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override {
                return "std::vector<void*>";
            }
            std::string toJNIType() override { return "jlongArray"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret{ "Array<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toKotlinWrapperType() override { return "LongArray"; }

            using InnerType = SymbolObject;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override {
                return "void*";
            }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeArray<SymbolTypeCallback> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override { return Name::CALLBACKARRAY; }
            SymbolTypeArray(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = "std::vector<";
                    ret += object->getCppName();
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "std::vector<void*>"; }
            std::string toJNIType() override {
                return "jobjectArray";
            }
            std::string toKotlinWrapperType() override { return "Array<Any>"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret{ "Array<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }

            using InnerType = SymbolTypeCallback;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override { return "void*"; }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<class T>
        class SymbolTypeVector : SymbolType
        {

        };

        template<>
        class SymbolTypeVector <SymbolTypeBool> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::BOOLVECTOR;}
            std::string toCppType() override { return "std::vector<bool>"; }
            std::string toCppInterfaceType() override { return "std::vector<bool>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override { return "MutableList<Boolean>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Boolean>"; }

            using InnerType = SymbolTypeBool;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt8> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT8VECTOR;}
            std::string toCppType() override {return "std::vector<char>";}
            std::string toCppInterfaceType() override { return "std::vector<char>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Byte>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Byte>"; }

            using InnerType = SymbolTypeInt8;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt16> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT16VECTOR;}
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int16_t>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Short>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Short>"; }

            using InnerType = SymbolTypeInt16;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt32> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT32VECTOR;}
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int32_t>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Int>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Int>"; }

            using InnerType = SymbolTypeInt32;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt64> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::INT64VECTOR;}
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toCppInterfaceType() override { return "std::vector<int64_t>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Long>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Long>"; }

            using InnerType = SymbolTypeInt64;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeFloat> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::FLOATVECTOR;}
            std::string toCppType() override {return "std::vector<float>";}
            std::string toCppInterfaceType() override { return "std::vector<float>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Float>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Float>"; }

            using InnerType = SymbolTypeFloat;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeDouble> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::DOUBLEVECTOR;}
            std::string toCppType() override {return "std::vector<double>";}
            std::string toCppInterfaceType() override { return "std::vector<double>"; }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Double>"; }
            std::string toKotlinWrapperType() override { return "MutableList<Double>"; }

            using InnerType = SymbolTypeDouble;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeString> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return false; }

            Name getTypeName() override {return Name::STRINGVECTOR;}
            std::string toCppType() override { return "std::vector<std::string>"; }
            std::string toCppInterfaceType() override { return "std::vector<std::string>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override { return "MutableList<String>"; }
            std::string toKotlinWrapperType() override { return "MutableList<String>"; }

            using InnerType = SymbolTypeString;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
            std::string toKotlinInnerType() override {
                return InnerType().toKotlinType();
            }
        };

        template<>
        class SymbolTypeVector<SymbolTypeEnum> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::ENUMVECTOR;}
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getCppName();
                    std::string ret{ "std::vector<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "std::vector<int>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret{ "MutableList<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toKotlinWrapperType() override { return "MutableList<Int>"; }
            using InnerType = SymbolObject;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override {
                return "int";
            }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeVector<SymbolTypeObject> : public SymbolType
        {
        public:
            bool valid() { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

            Name getTypeName() override {return Name::OBJECTVECTOR;}
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getCppName();
                    std::string ret{ "std::vector<std::shared_ptr<" };
                    ret += name;
                    ret += ">>";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "std::vector<void*>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret{ "MutableList<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toKotlinWrapperType() override { return "MutableList<Long>"; }

            using InnerType = SymbolObject;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override {
                return "void*";
            }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
            }
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeVector<SymbolTypeCallback> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            bool isPrimitive() override { return false; }
            bool requiredDeclaration() override { return true; }

            Name getTypeName() override { return Name::CALLBACKVECTOR; }
            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    std::string ret = "std::vector<";
                    ret += object->getCppName();
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toCppInterfaceType() override { return "std::vector<void*>"; }
            std::string toJNIType() override {
                return "jobject";
            }
            std::string toKotlinWrapperType() override { return "MutableList<Any>"; }
            std::string toKotlinType() override {
                if (auto object = _obj.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret{ "MutableList<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }

            using InnerType = SymbolTypeCallback;
            std::string toCppInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getCppName();
                }
                return "";
            }
            std::string toCppInterfaceInnerType() override { return "void*"; }
            std::string toKotlinInnerType() override {
                if (auto object = _obj.lock())
                {
                    return object->getKotlinName();
                }
                return "";
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