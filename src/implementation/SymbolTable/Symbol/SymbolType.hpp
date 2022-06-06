#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__

#include "../../../json/json.hpp"
#include "../../Result/Result.hpp"
#include "SymbolObject.hpp"
#include <string>

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        
        class SymbolType
        {
        public:
            virtual bool valid() { return false; }
            virtual std::string toCppType() { return ""; };
            virtual std::string toJNIType() { return ""; };
            virtual std::string toKotlinType() { return ""; };
        };

        class SymbolTypeVoid : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "void";}
            std::string toJNIType() override {return "void";}  
            std::string toKotlinType() override {return "Unit"; }

        };

        class SymbolTypeBool : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override { return "bool"; }
            std::string toJNIType() override { return "jboolean"; }
            std::string toKotlinType() override { return "Boolean"; }
        };

        class SymbolTypeInt8 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "int8_t";}
            std::string toJNIType() override {return "jbyte";}  
            std::string toKotlinType() override {return "Byte"; }       
        };
        class SymbolTypeInt16 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "int16_t";}
            std::string toJNIType() override {return "jshort";}  
            std::string toKotlinType() override {return "Short"; }
        };
        class SymbolTypeInt32 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "int32_t";}
            std::string toJNIType() override {return "jint";}  
            std::string toKotlinType() override {return "Int"; }
        };
        class SymbolTypeInt64 : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "int64_t";}
            std::string toJNIType() override {return "jlong";}  
            std::string toKotlinType() override {return "Long"; }
        };
        class SymbolTypeDouble : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "double";}
            std::string toJNIType() override {return "jdouble";}  
            std::string toKotlinType() override {return "Double"; }
        };
        class SymbolTypeFloat : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "float";}
            std::string toJNIType() override {return "jfloat";}  
            std::string toKotlinType() override {return "Float"; }
        };
        class SymbolTypeString : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::string";}
            std::string toJNIType() override {return "jstring";}  
            std::string toKotlinType() override {return "String"; }
        };
       
        class SymbolTypeEnum : public SymbolType
        {
        public:
            bool valid() override { return true; }
            SymbolTypeEnum(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
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
            SymbolTypeObject(std::weak_ptr<SymbolObject> obj) : _obj(obj) {};
            std::string toCppType() override {
                if (auto object = _obj.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
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

        template<class T>
        class SymbolTypeArray : public SymbolType
        {
            
        };

        template<>
        class SymbolTypeArray <SymbolTypeBool> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override { return "std::vector<bool>"; }
            std::string toJNIType() override { return "jbooleanArray"; }
            std::string toKotlinType() override { return "BooleanArray"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt8> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<char>";}
            std::string toJNIType() override {return "jbyteArray";}  
            std::string toKotlinType() override {return "ByteArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt16> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toJNIType() override {return "jshortArray";}  
            std::string toKotlinType() override {return "ShortArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt32> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt64> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeFloat> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<float>";}
            std::string toJNIType() override {return "jfloatArray";}  
            std::string toKotlinType() override {return "FloatArray"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeDouble> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<double>";}
            std::string toJNIType() override {return "jdoubleArray";}  
            std::string toKotlinType() override {return "DoubleArray"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeString> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override { return "std::vector<std::string>"; }
            std::string toJNIType() override { return "jobjectArray"; }
            std::string toKotlinType() override { return "Array<String>"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeEnum> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            SymbolTypeArray(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

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
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeArray<SymbolTypeObject> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            SymbolTypeArray(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

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
            std::string toJNIType() override { return "jobjectArray"; }
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
            std::string toCppType() override { return "std::vector<bool>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override { return "MutableList<Boolean>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt8> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<char>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Byte>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt16> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Short>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt32> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Int>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt64> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Long>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeFloat> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<float>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Float>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeDouble> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override {return "std::vector<double>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Double>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeString> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            std::string toCppType() override { return "std::vector<std::string>"; }
            std::string toJNIType() override { return "jobject"; }
            std::string toKotlinType() override { return "MutableList<String>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeEnum> : public SymbolType
        {
        public:
            bool valid() override { return true; }
            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

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
        private:
            std::weak_ptr<SymbolObject> _obj;
        };

        template<>
        class SymbolTypeVector<SymbolTypeObject> : public SymbolType
        {
        public:
            bool valid() { return true; }
            SymbolTypeVector(std::weak_ptr<SymbolObject> obj) :
                _obj(obj) {};

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