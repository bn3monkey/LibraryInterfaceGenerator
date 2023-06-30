#if !defined(__BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_CXXCONVERTER__)
#define __BN3MONKEY_LIBRARY_INTERFACE_GENERATOR_CXXCONVERTER__

#include "../SymbolTable/SymbolTable.hpp"
#include "../SourceStream/CXXSourceStream.hpp"

namespace LibraryInterfaceGenerator
{
    namespace Implementation
    {
        inline void createCXXComment(SourceStream& ss, const SymbolPackage& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addDescription(object.description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolModule& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addDescription(object.description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolClass& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addBrief(object.description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolEnum& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addBrief(object.description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolMethod& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addBrief(object.description);
                for (auto& parameter : object.parameters)
                {
                    comment.addParameter(parameter->name, parameter->description);
                }
                comment.addReturn(object.return_description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolProperty& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addName(object.name);
                comment.addBrief(object.description);
            }
        }
        inline void createCXXComment(SourceStream& ss, const SymbolCallback& object)
        {
            {
                CommentCXXSourceStream comment{ ss };
                comment.addBrief(object.description);
                for (auto& parameter : object.parameters)
                {
                    comment.addParameter(parameter->name, parameter->description);
                }
            }
        }

        inline std::string createRelativeHeaderPath(const SymbolModule& self)
        {
            std::string prefix = "../";
            std::string postfix = "include/";

            for (size_t idx = 1; idx < self.moduleNames.size(); idx++)
            {
                prefix += "../";
                postfix += self.moduleNames[idx];
                postfix += "/";
            }

            postfix += self.name;
            postfix += ".hpp";

            prefix += postfix;
            return prefix;
        }
        inline std::string createRelativeHeaderPath(const SymbolClass& self)
        {
            std::string prefix = "../";
            std::string postfix = "include/";

            for (size_t idx = 1; idx < self.parentModules.size(); idx++)
            {
                prefix += "../";
                postfix += self.parentModules[idx];
                postfix += "/";
            }

            postfix += self.name;
            postfix += ".hpp";

            prefix += postfix;
            return prefix;
        }


        inline std::string createRelativeHeaderPath(const SymbolClass& src, const SymbolClass& dest)
        {
            std::string path;

            auto& src_parent_modules = src.parentModules;
            auto& dest_parent_modules = dest.parentModules;

            size_t src_depth = src_parent_modules.size();
            size_t dest_depth = dest_parent_modules.size();

            size_t depth_indicator;

            for (depth_indicator = src_depth - 1; depth_indicator > dest_depth; depth_indicator--)
            {
                path += "../";
            }
            for (; depth_indicator >= 0; depth_indicator--)
            {
                if (dest_parent_modules[depth_indicator] == src_parent_modules[depth_indicator])
                    break;
                path += "../";
            }
            for (depth_indicator += 1; depth_indicator < dest_depth; depth_indicator++)
            {
                path += src_parent_modules[depth_indicator];
                path += "/";
            }

            if (dest.parentObjects.empty())
            {
                path += dest.name;

            }
            else
            {
                path += dest.parentObjects[0];
            }
            path += ".hpp";
            return path;
        }

        inline std::vector<std::string> createScope(const SymbolMethod& src)
        {
            return src.parentModules;
        }

        inline std::vector<std::string> createScope(const SymbolClass& src)
        {
            std::vector<std::string> scopes;
            scopes.insert(scopes.end(), src.parentModules.begin(), src.parentModules.end());
            scopes.insert(scopes.end(), src.parentObjects.begin(), src.parentObjects.end());
            scopes.push_back(src.name);
            return scopes;
        }

        inline std::vector<std::string> createInterfaceScope(const std::string& root_namespace, const SymbolMethod& src)
        {
            std::vector<std::string> scopes{ root_namespace };
            scopes.insert(scopes.end(), src.parentModules.begin() + 1, src.parentModules.end());
            return scopes;
        }

        inline std::vector<std::string> createInterfaceScope(const std::string& root_namespace, const SymbolClass& src)
        {
            std::vector<std::string> scopes{ root_namespace };
            scopes.insert(scopes.end(), src.parentModules.begin() + 1, src.parentModules.end());
            scopes.insert(scopes.end(), src.parentObjects.begin(), src.parentObjects.end());
            scopes.push_back(src.name);
            return scopes;
        }

        inline std::string createNativeWrapperScope(const SymbolClass& clazz)
        {
            std::string scope;
	        auto& moduleNames = clazz.parentModules;
	        auto& objectNames = clazz.parentObjects;
	        for (auto& moduleName : moduleNames)
	        {
	        	scope += moduleName;
	        	scope += "_1";
	        }
	        for (auto& objectName : objectNames)
	        {
	        	scope += objectName;
	        	scope += "_1";
	        }
	        scope += clazz.name;
	        scope += "_1";
	        return scope;
        }
        inline std::string createNativeWrapperScope(const SymbolMethod& method)
        {
             std::string scope;
	        auto& moduleNames = method.parentModules;
	        for (auto& moduleName : moduleNames)
	        {
	        	scope += moduleName;
	        	scope += "_1";
	        }
	        return scope;
        }
        
    }
} 
#endif