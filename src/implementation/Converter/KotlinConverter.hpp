#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINCONVERTER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_KOTLINCONVERTER__

#include "../SymbolTable/SymbolTable.hpp"

namespace LibraryInterfaceGenerator
{
	namespace Implementation
	{
        inline void createKotlinComment(SourceStream& ss, const SymbolPackage& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addDescription(object.description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolModule& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addDescription(object.description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolClass& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addBrief(object.description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolEnum& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addBrief(object.description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolMethod& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addBrief(object.description);
                for (auto& parameter : object.parameters)
                {
                    comment.addParameter((parameter->io == SymbolParameter::IO::IN ? true : false), parameter->name, parameter->name);
                }
                comment.addReturn(object.return_description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolProperty& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addBrief(object.description);
            }
        }
        inline void createKotlinComment(SourceStream& ss, const SymbolCallback& object)
        {
            {
                CommentKotlinSourceStream comment{ ss };
                comment.addBrief(object.description);
                for (auto& parameter : object.parameters)
                {
                    comment.addParameter((parameter->io == SymbolParameter::IO::IN ? true : false), parameter->name, parameter->name);
                }
            }
        }

        inline std::string createKotlinClassName(const SymbolClass& clazz)
        {
            std::string scope;
            auto& moduleNames = clazz.parentModules;
            auto& objectNames = clazz.parentObjects;
            for (size_t i = 1; i < moduleNames.size(); i++)
            {
                scope += moduleNames[i];
                scope += "/";
            }
            for (auto& objectName : objectNames)
            {
                scope += objectName;
                scope += "/";
            }
            scope += clazz.name;
            return scope;
        }
        inline std::string createKotlinClassName(const SymbolEnum& enumm)
        {
            std::string scope;
            auto& moduleNames = enumm.parentModules;
            auto& objectNames = enumm.parentObjects;
            for (size_t i = 1; i < moduleNames.size(); i++)
            {
                scope += moduleNames[i];
                scope += "/";
            }
            for (auto& objectName : objectNames)
            {
                scope += objectName;
                scope += "/";
            }
            scope += enumm.name;
            return scope;
        }

		inline std::string createKotlinWrapperScope(const SymbolClass& clazz)
		{
			std::string scope;
			auto& moduleNames = clazz.parentModules;
			auto& objectNames = clazz.parentObjects;
			for (auto& moduleName : moduleNames)
			{
				scope += moduleName;
				scope += "_";
			}
			for (auto& objectName : objectNames)
			{
				scope += objectName;
				scope += "_";
			}
			scope += clazz.name;
			scope += "_";
			return scope;
		}

		inline std::string createKotlinWrapperScope(const SymbolMethod& method)
		{
			std::string scope;
			auto& moduleNames = method.parentModules;
			for (auto& moduleName : moduleNames)
			{
				scope += moduleName;
				scope += "_";
			}
			return scope;
		}

       
	}
}
#endif