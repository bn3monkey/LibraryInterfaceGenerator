#include <SourceStream/SourceStream.hpp>
#include <gtest/gtest.h>

#include <cstdio>

TEST(SourceStream, WriteFunction)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	SourceStream sourceStream;
	
	sourceStream << "int main()" << "\n";
	sourceStream << "{" << "\n";
	sourceStream.addIndent();
	sourceStream << "printf(\"Do you know Sans?\");" << "\n";
	sourceStream.removeIndent();
	sourceStream << "}\n";

	EXPECT_STREQ(sourceStream.str(),
		"int main()\n"
		"{\n"
		"\tprintf(\"Do you know Sans?\");\n"
		"}\n"
	);
	printf(sourceStream.str());
	
	return;
}

TEST(SourceStream, WriteTuple)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	const char* pocketmons[5] = {
		"PIKACHU",
		"RAICHU",
		"PAIRI",
		"GGOBUGI",
		"YADORAN"
	};

	SourceStream sourceStream;
	sourceStream << "(";
	for (auto& pocketmon : pocketmons)
	{
		sourceStream << pocketmon << ", ";
	}

	sourceStream.pop(sizeof(", ")-1);
	sourceStream << ")";


	EXPECT_STREQ(sourceStream.str(), "(PIKACHU, RAICHU, PAIRI, GGOBUGI, YADORAN)");
	printf(sourceStream.str());
	printf("\n");

	return;
}

TEST(SourceScopedStream, WriteCppFunction)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	SourceStream sourceStream;

	sourceStream << "int main()\n";
	{
		SourceScopedStream sourceScopedStream{ sourceStream, CodeStyle::Cpp };
		sourceScopedStream << "printf(\"Do you know Sans?\");" << "\n";
	}

	EXPECT_STREQ(sourceStream.str(),
		"int main()\n"
		"{\n"
		"\tprintf(\"Do you know Sans?\");\n"
		"}\n"
	);
	printf(sourceStream.str());
	return;
}

TEST(SourceScopedStream, WriteCppClass)
{
	using namespace LibraryInterfaceGenerator::Implementation;

	SourceStream sourceStream;
	{
		sourceStream << "namespace OuterScope\n";
		SourceScopedStream outer{ sourceStream, CodeStyle::Cpp };
		{
			outer << "namespace InnerScope\n";
			SourceScopedStream inner{ outer, CodeStyle::Cpp };
			{
				inner << "class Derived : ";
				char* base_classes[] = { "Base1", "Base2", "Base3" };
				for (auto base_class : base_classes)
				{
					inner << base_class << ", ";
				}
				inner.pop(sizeof(", ") - 1);
				inner << "\n";

				{
					SourceScopedStream clazz{ outer, CodeStyle::CppClass };
					UnindentedSourceScopedStream accessor{ clazz, CodeStyle::None };
					{
						accessor << "public : \n";
						{
							SourceScopedStream public_ {clazz, CodeStyle::None };
							public_ << "void func(int param1, char param2);\n";
							public_ << "void func2(int param1, char param2);\n";
						}
						accessor << "private : \n";
						{
							SourceScopedStream private_{ clazz, CodeStyle::None };
							private_ << "char _field1;\n";
							private_ << "char _field2;\n";
						}
					}
				}
			}
		}
	}

	
	EXPECT_STREQ(sourceStream.str(),
		"namespace OuterScope\n"
		"{\n"
		"\tnamespace InnerScope\n"
		"\t{\n"
		"\t\tclass Derived : Base1, Base2, Base3\n"
		"\t\t{\n"
		"\t\tpublic : \n"
		"\t\t\tvoid func(int param1, char param2);\n"
		"\t\t\tvoid func2(int param1, char param2);\n"
		"\t\tprivate : \n"
		"\t\t\tchar _field1;\n"
		"\t\t\tchar _field2;\n"
		"\t\t};\n"
		"\t}\n"
		"}\n");
	
	printf(sourceStream.str());
	printf("\n");
	return;
}