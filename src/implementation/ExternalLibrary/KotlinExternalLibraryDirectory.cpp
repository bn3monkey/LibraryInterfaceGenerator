#include "KotlinExternalLibraryDirectory.hpp"
#include "KotlinTypeConverter/KotlinTypeConverter_kt.h"

std::string LibraryInterfaceGenerator::Implementation::KotlinExternalLibraryDirectory::createExternalToolCode(LibraryInterfaceGenerator::Implementation::KotlinExternalLibraryDirectory::ExternalTool tool) const
{
    switch(tool)
    {
        case LibraryInterfaceGenerator::Implementation::KotlinExternalLibraryDirectory::ExternalTool::KotlinTypeConverter:
            return KOTLINTYPECONVERTER_KT;
    }
    return "";
}
