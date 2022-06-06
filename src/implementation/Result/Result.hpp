#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_RESULT__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_RESULT__

#include <string>
#include <cstdio>
#if defined __linux__
#include <sstream>
#include <ios>
#endif

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
#if defined __linux__
        inline void inner_format_string(std::string& dest, const char* format)
	    {
	    	dest += std::string(format);
	    }

	    template<class Arg, class ...Args>
	    void inner_format_string(std::string& dest, const char* format, Arg arg, Args... args)
	    {
	    	std::stringstream ss;
	    	const char* start = format;
	    	const char* end = start;
	    	for (end = start; *end != '\0'; end++)
	    	{
	    		if (end[0] == '\0')
	    		{
	    			dest += std::string(start, end);
	    			return;
	    		}
	    		else if(end[0] == '%')
	    		{
	    			switch (end[1])
	    			{
	    				case '\0':
	    					dest += std::string(start, end);
	    					return;
	    				case 'c':
	    					ss << *(char*)(&arg);
	    					break;
	    				case 'd':
	    					ss << *(int *)(&arg);
	    					break;
	    				case 'l':
	    					ss << *(long *)(&arg);
	    					break;
	    				case 'X':
	    				case 'x':
	    					ss << "0x" << std::hex << (int *)(&arg);
	    					break;
	    				case 'f':
	    					ss << *(float *)(&arg);
	    					break;
	    				case 'u':
	    					ss << *(unsigned int *)(&arg);
	    					break;
	    				case 's':
	    					ss << arg;
	    					break;
	    				case '%':
	    					ss << '%';
	    					break;
	    				default:
	    					ss << '%' << end[1];
	    					break;
	    			}
	    			break;
	    		}
	    	}
	    	dest += std::string(start, end);
	    	dest += ss.str();
	    	start = end + 2;
	    	inner_format_string(dest, start, args...);
	    }

	    template<class ...Args>
	    std::string format_string(const char* format, Args... args)
	    {
	    	std::string ret = "";
	    	inner_format_string(ret, format, args...);
	    	return ret;
	    }
#endif

        class Result
        {
            public:
                enum class Code : int32_t
                {
                    SUCCESS = 0,

                    JSON_OBJECT_HAS_NO_ORDER,
                    JSON_OBJECT_HAS_NO_NAME,
					JSON_OBJECT_HAS_NO_DESCRIPTION,
					JSON_OBJECT_HAS_NO_AUTHOR,
					JSON_OBJECT_HAS_NO_CHILDS,
					JSON_OBJECT_HAS_NO_VALUE,
					JSON_OBJECT_HAS_NO_VALUES,
					JSON_OBJECT_HAS_NO_TYPE,
					JSON_OBJECT_HAS_NO_READONLY,
					JSON_OBJECT_HAS_NO_PARAMETERS,
					JSON_OBJECT_HAS_NO_IO,
					JSON_OBJECT_HAS_NO_RETURN,
                    JSON_OBJECT_IS_NOT_LIBRARY_ITEM,
                    JSON_OBJECT_IS_NOT_LIBRARY_FUNCTION,
					JSON_OBJECT_HAS_INVALID_VALUE,

                    BUFFER_OVERFLOW,
                    BAD_ALLOCATION,
					CANNOT_FIND_CLASSNAME,
					SYMBOL_TYPE_CAST_FAIL,

					DIRECTORY_CREATION_FAIL,
					FILE_CREATION_FAIL
                };



                inline operator bool() const { return _code == Code::SUCCESS; } 
                std::string toString() const {return _reason;}

                explicit Result(const Code code = Code::SUCCESS) : _code(code) {
                    _reason = default_reason[static_cast<size_t>(code)];
                }
                explicit Result(const Code code, const std::string& reason) : _code(code), _reason(reason) {}

                template<class ...Args>
                explicit Result(const Code code, const char* format, Args... args) : _code(code)
                {
#ifdef _WIN32
					char buf[8192];
					size_t size = static_cast<size_t>(snprintf(nullptr, 0, format, args ...)) + static_cast<size_t>(1); // Extra space for '\0'
					snprintf(buf, size, format, args ...);
					_reason = std::string(buf, buf + size - 1);
#elif __linux__
                    _reason = format_string(format, args...);
#endif
                }

            private:
                Code _code;
                std::string _reason;

                static constexpr char* default_reason[] = {
                    "SUCCESS",
                    "json object has no field (\"order\")",
                    "json object has no field (\"name\")",
					"json object has no field (\"description\")",
					"json object has no field (\"author\")",
					"json object has no field (\"childs\")",
					"json object has no field (\"value\")",
					"json object has no field (\"values\")",
					"json object has no field (\"type\")",
					"json object has no field (\"readonly\")",
					"json object has no field (\"parameters\")",
					"json object has no field (\"io\")",
					"json object has no field (\"return\")",
                    "json object is not package, module, class, interface",
                    "json object is not method, property, parameters",
					"json object has invalid value",
                    "buffer overflow",
					"allocation fail",
					"Cannot find class name",
					"Symbol Type cast fail",
					"directory creation fail",
					"file creation fail"
                };
        };

        class HasResult
        {
        public:
            operator bool() {return _result;}
            Result& toResult() {return _result;}
        protected:
            Result _result;
        };
    }
}
#endif