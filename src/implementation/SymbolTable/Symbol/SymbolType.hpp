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
            };

            virtual Name getTypeName() {return Name::INVALID;}
            virtual bool valid() { return false; }
            virtual bool isPrimitive() { return false; }
            virtual bool requiredDeclaration() { return false; }

            virtual std::string toCppType() { return ""; }
            virtual std::string toCppInterfaceType() { return ""; }
            virtual std::string toJNIType() { return ""; }
            virtual std::string toKotlinType() { return ""; }
            virtual std::string toCppInnerType() { return ""; }
            virtual std::string toCppInterfaceInnerType() { return ""; }
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
            std::string toKotlinType() override {return "Unit"; }

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
            std::string toKotlinType() override { return "Boolean"; }
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
            std::string toKotlinType() override {return "Byte"; }       
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
            std::string toKotlinType() override {return "Short"; }
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
            std::string toKotlinType() override {return "Int"; }
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
            std::string toKotlinType() override {return "Long"; }
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
            std::string toKotlinType() override {return "Double"; }
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
            std::string toKotlinType() override {return "Float"; }
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
            std::string toKotlinType() override {return "String"; }
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

            using InnerType = SymbolTypeBool;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt8;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt16;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt32;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; } 

            using InnerType = SymbolTypeInt64;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeFloat;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeDouble;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeString;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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
                    auto name = object->getCppName();
                    std::string ret{ "Array<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }

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

            using InnerType = SymbolTypeBool;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt8;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt16;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt32;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeInt64;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeFloat;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeDouble;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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

            using InnerType = SymbolTypeString;
            std::string toCppInnerType() override { return InnerType().toCppType(); }
            std::string toCppInterfaceInnerType() override { return InnerType().toCppInterfaceType(); }
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
                    auto name = object->getCppName();
                    std::string ret{ "MutableList<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
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
                    auto name = object->getCppName();
                    std::string ret{ "MutableList<" };
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
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
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        std::unique_ptr<LibraryInterfaceGenerator::Implementation::SymbolType> makeType(
            const std::string& type,
            const SymbolObjectTable& objectTable,
            const SymbolEnumTable& enumTable
            );

        class HasSymbolType
        {
        public:
            std::unique_ptr<SymbolType> type;
            Result change(SymbolObjectTable& objectTable, SymbolEnumTable& enumTable);
        protected:
            std::string _type;
        };
    }
}

#endif