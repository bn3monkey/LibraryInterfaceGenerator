#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_SYMBOLTYPE__

#include "../../json/json.hpp"
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
            virtual std::string toCppType() = 0;
            virtual std::string toJNIType() = 0;
            virtual std::string toKotlinType() = 0;
        };

        class SymbolTypeVoid : SymbolType
        {
        public:
            std::string toCppType() override {return "void";}
            std::string toJNIType() override {return "void";}  
            std::string toKotlinType() override {return "Unit"; }

        };
        class SymbolTypeInt8 : SymbolType
        {
        public:
            std::string toCppType() override {return "int8_t";}
            std::string toJNIType() override {return "jbyte";}  
            std::string toKotlinType() override {return "Byte"; }       
        };
        class SymbolTypeInt16 : SymbolType
        {
        public:
            std::string toCppType() override {return "int16_t";}
            std::string toJNIType() override {return "jshort";}  
            std::string toKotlinType() override {return "Short"; }
        };
        class SymbolTypeInt32 : SymbolType
        {
        public:
            std::string toCppType() override {return "int32_t";}
            std::string toJNIType() override {return "jint";}  
            std::string toKotlinType() override {return "Int"; }
        };
        class SymbolTypeInt64 : SymbolType
        {
        public:
            std::string toCppType() override {return "int64_t";}
            std::string toJNIType() override {return "jlong";}  
            std::string toKotlinType() override {return "Long"; }
        };
        class SymbolTypeDouble : SymbolType
        {
        public:
            std::string toCppType() override {return "double";}
            std::string toJNIType() override {return "jdouble";}  
            std::string toKotlinType() override {return "Double"; }
        };
        class SymbolTypeFloat : SymbolType
        {
        public:
            std::string toCppType() override {return "float";}
            std::string toJNIType() override {return "jfloat";}  
            std::string toKotlinType() override {return "Float"; }
        };
        class SymbolTypeString : SymbolType
        {
        public:
            std::string toCppType() override {return "std::string";}
            std::string toJNIType() override {return "jstring";}  
            std::string toKotlinType() override {return "String"; }
        };
        class SymbolTypeEnum : SymbolType
        {
        public:
            SymbolTypeEnum(std::string name, SymbolEnumTable& table) :
                _name(name), _table(table) {};
            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {return "jint";}  
            std::string toKotlinType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                auto ret = "";
             }
        private:
            std::string _name;
            SymbolEnumTable& _table;
        };
        class SymbolTypeObject: SymbolType
        {
        public:
            SymbolTypeObject(std::string name, SymbolObjectTable& table) :
                _name(name), _table(table) {};
            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto ret = object->getCppName();
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto ret = object->getKotlinName();
                    return ret;
                }
                return "";
            }
        private:
            std::string _name;
            SymbolObjectTable& _table;
        };

        template<class T>
        class SymbolTypeArray : SymbolType, std::false_type
        {

        };

        template<>
        class SymbolTypeArray<SymbolTypeInt8> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<char>";}
            std::string toJNIType() override {return "jbyteArray";}  
            std::string toKotlinType() override {return "ByteArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt16> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toJNIType() override {return "jshortArray";}  
            std::string toKotlinType() override {return "ShortArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt32> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeInt64> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toJNIType() override {return "jintArray";}  
            std::string toKotlinType() override {return "IntArray"; }    
        };

        template<>
        class SymbolTypeArray<SymbolTypeFloat> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<float>";}
            std::string toJNIType() override {return "jfloatArray";}  
            std::string toKotlinType() override {return "FloatArray"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeDouble> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<double>";}
            std::string toJNIType() override {return "jdoubleArray";}  
            std::string toKotlinType() override {return "DoubleArray"; }
        };

        template<>
        class SymbolTypeArray<SymbolTypeEnum> : SymbolType
        {
        public:
            SymbolTypeArray(std::string name, SymbolEnumTable& table) :
                _name(name), _table(table) {};

            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
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
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret {"std::vector<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";    
            }
        private:
            std::string _name;
            SymbolEnumTable& _table;    
        };

        template<>
        class SymbolTypeArray<SymbolTypeObject> : SymbolType
        {
        public:
            SymbolTypeArray(std::string name, SymbolObjectTable& table) :
                _name(name), _table(table) {};
            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getCppName();
                    std::string ret {"std::vector<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {return "jobjectArray";}  
            std::string toKotlinType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret {"Array<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
        private:
            std::string _name;
            SymbolObjectTable& _table;
        };

        template<class T>
        class SymbolTypeVector : SymbolType, std::false_type
        {

        };


        template<>
        class SymbolTypeVector<SymbolTypeInt8> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<char>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Byte>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt16> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int16_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Short>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt32> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int32_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Int>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeInt64> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<int64_t>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Long>"; }    
        };

        template<>
        class SymbolTypeVector<SymbolTypeFloat> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<float>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Float>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeDouble> : SymbolType
        {
        public:
            std::string toCppType() override {return "std::vector<double>";}
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {return "MutableList<Double>"; }
        };

        template<>
        class SymbolTypeVector<SymbolTypeEnum> : SymbolType
        {
        public:
            SymbolTypeVector(std::string name, SymbolEnumTable& table) :
                _name(name), _table(table) {};
            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getCppName();
                    std::string ret {"std::vector<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret {"MutableList<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
        private:
            std::string _name;
            SymbolEnumTable& _table;    
        };

        template<>
        class SymbolTypeVector<SymbolTypeObject> : SymbolType
        {
        public:
            SymbolTypeVector(std::string name, SymbolObjectTable& table) :
                _name(name), _table(table) {};
            std::string toCppType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getCppName();
                    std::string ret {"std::vector<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
            std::string toJNIType() override {return "jobject";}  
            std::string toKotlinType() override {
                auto object_iter = _table.find(_name);
                if (object_iter == _table.end())
                    return "";
                if (auto object = object_iter->second.lock())
                {
                    auto name = object->getKotlinName();
                    std::string ret {"MutableList<"};
                    ret += name;
                    ret += ">";
                    return ret;
                }
                return "";
            }
        private:
            std::string _name;
            SymbolObjectTable& _table;
        };
    }
}

#endif