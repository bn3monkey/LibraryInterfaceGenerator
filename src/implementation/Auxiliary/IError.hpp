#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_IERROR__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_IERROR__

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
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

        class Error
        {
            public:
                enum class Code : int32_t
                {
                    SUCCESS = 0,

                    JSON_OBJECT_HAS_NO_ORDER,
                    JSON_OBJECT_HAS_NO_NAME,
                    JSON_OBJECT_IS_NOT_LIBRARY_ITEM,
                    JSON_OBJECT_IS_NOT_LIBRARY_FUNCTION,

                    BUFFER_OVERFLOW,
                    BAD_ALLOCATION,
                };



                inline operator bool() const { return _code != Code::SUCCESS; } 
                std::string toString() const {return _reason;}

                explicit Error(const Code code = Code::SUCCESS) : _code(code) {
                    _reason = default_reason[static_cast<size_t>(code)];
                }
                explicit Error(const Code code, const std::string& reason) : _code(code), _reason(reason) {}

                template<class ...Args>
                explicit Error(const Code code, const char* format, Args... args) : _code(code)
                {
                    _reason = format_string(format, args...);
                }

            private:
                Code _code;
                std::string _reason;

                static constexpr char* default_reason[] = {
                    "SUCCESS",
                    "json object has no field (\"order\")",
                    "json object has no field (\"name\")",
                    "json object is not package, module, class, interface",
                    "json object is not method, property, parameters",
                    "buffer overflow",
                };
        };
    }
}
#endif
