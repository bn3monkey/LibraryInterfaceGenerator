#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_STRINGHELPER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_STRINGHELPER__
#include <string>
inline bool hasPrefix(const std::string& src, std::string prefix)
{
	return src.compare(0, prefix.size(), prefix.data()) == 0;
}
inline bool hasPostfix(const std::string& src, std::string postfix)
{
	const size_t start = src.size() - postfix.size();
	return src.compare(start, src.size(), postfix.data()) == 0;
}
#endif