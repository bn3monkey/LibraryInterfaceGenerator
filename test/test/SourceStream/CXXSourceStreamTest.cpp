#include <SourceStream/CXXSourceStream.hpp>
#include <gtest/gtest.h>

#include <cstdio>

TEST(CXXSourceStream, HeaderGuardCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::vector<std::string> module_names{ "sans", "papyrus" };
	{
		HeaderGuardCXXSourceScopedStream header_guard(ss, module_names, "ppap");
		ss << "int main()\n";
		ss << "{\n";
		ss << "\tprintf(\"WA! SANS!\");\n";
		ss << "}\n";
	}

	const char* result = ss.str();
	
	EXPECT_STREQ(result,

		"#if !defined(__SANS_PAPYRUS_PPAP_8Q38_5Q5D_7081__)\n"
		"#define __SANS_PAPYRUS_PPAP_8Q38_5Q5D_7081__\n"
		"\n"
		"int main()\n"
		"{\n"
		"\tprintf(\"WA! SANS!\");\n"
		"}\n"
		"\n"
		"#endif // __SANS_PAPYRUS_PPAP_8Q38_5Q5D_7081__\n"
	);

	printf(result);
	return;
}

TEST(CXXSourceStream, InternalIncludeCXXSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::string header_path1 = "undertail\\sans.hpp";
	std::string header_path2 = "undertail/papyrus.hpp";

	{
		InternalIncludeCXXSourceStream h1(ss, header_path1);
	}
	{
		InternalIncludeCXXSourceStream h2(ss, header_path2);
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"#include \"undertail/sans.hpp\"\n"
		"#include \"undertail/papyrus.hpp\"\n");
	printf(result);
	return;
}

TEST(CXXSourceStream, ExternalIncludeCXXSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::string header_path1 = "vector";
	std::string header_path2 = "string";

	{
		ExternalIncludeCXXSourceStream h1(ss, header_path1);
	}
	{
		ExternalIncludeCXXSourceStream h2(ss, header_path2);
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"#include <vector>\n"
		"#include <string>\n");
	printf(result);
	return;
}

TEST(CXXSourceStream, NamespaceCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::vector<std::string> namespace_names{ "Game", "Undertail", "SANS" };
	{
		NamespaceCXXSourceScopedStream namespaceStream(ss, namespace_names);

		ss << "class GasterBlaster\n";
		ss << "{\n";
		ss << "};\n";
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"namespace Game\n"
		"{\n"
		"\tnamespace Undertail\n"
		"\t{\n"
		"\t\tnamespace SANS\n"
		"\t\t{\n"
		"\t\t\tclass GasterBlaster\n"
		"\t\t\t{\n"
		"\t\t\t};\n"
		"\t\t}\n"
		"\t}\n"
		"}\n");

	printf(result);
	return;
}

TEST(CXXSourceStream, ClassCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	
	{
		std::vector<std::string> namespace_names{ "Game", "Undertail" };
		NamespaceCXXSourceScopedStream namespaceStream(ss, namespace_names);
		{
			ClassCXXSourceScopedStream clazz{ ss, true, "Papyrus" };
		}
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"namespace Game\n"
			"{\n"
			"\tnamespace Undertail\n"
			"\t{\n"
			"\t\tclass Papyrus;\n"
			"\t}\n"
			"}\n"
		);
		printf(result);
		ss.clear();
	}

	{
		ClassCXXSourceScopedStream clazz{ ss, false, "Papyrus" };
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"class Papyrus\n"
			"{\n"
			"};\n"
		);
		printf(result);
		ss.clear();
	}

	{
		std::vector<std::string> base_classes{ "Skeleton", "Monster" };
		ClassCXXSourceScopedStream clazz{ ss, false, "Papyrus", base_classes };
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"class Papyrus : Skeleton, Monster\n"
			"{\n"
			"};\n"
		);
		printf(result);
		ss.clear();
	}

	return;
}

TEST(CXXSourceStream, AccessCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		{
			ClassCXXSourceScopedStream clazz{ ss, false, "Papyrus" };
			{
				AccessCXXSourceScopedStream public_{ ss, AccessCXXSourceScopedStream::Specifier::Public };
				ss << "void attack();\n";
				ss << "void defence();\n";
			}
			{
				AccessCXXSourceScopedStream private_{ ss, AccessCXXSourceScopedStream::Specifier::Private };
				ss << "void think();\n";
			}
		}
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"class Papyrus\n"
			"{\n"
			"public : \n"
			"\tvoid attack();\n"
			"\tvoid defence();\n"
			"private : \n"
			"\tvoid think();\n"
			"};\n"
		);
		printf(result);
		ss.clear();
	}

	return;
}

TEST(CXXSourceStream, EnumCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		EnumCXXSourceScopedStream startingType{ ss, true, "StartingType" };
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"enum class StartingType;\n"
		);
		printf(result);
		ss.clear();
	}

	{
		EnumCXXSourceScopedStream startingType{ ss, false, "StartingType" };
		startingType.addElement("FIRE", "0");
		startingType.addElement("WATER", "1");
		startingType.addElement("GRASS", "2");
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"enum class StartingType\n"
			"{\n"
			"\tFIRE = 0, \n"
			"\tWATER = 1, \n"
			"\tGRASS = 2, \n"
			"};\n"
		);
		printf(result);
		ss.clear();
	}

	return;
}

TEST(CXXSourceStream, MethodCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		MethodCXXSourceScopedStream global_method_declaration{
			ss,
			true,
			"",
			"",
			"void",
			{},
			"main"
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"void main();\n"
		);
		printf(result);
		ss.clear();
	}

	{
		MethodCXXSourceScopedStream library_method_declaration{
			ss,
			true,
			"extern LIBRARY_API",
			"",
			"void",
			{},
			"main"
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"extern LIBRARY_API void main();\n"
		);
		printf(result);
		ss.clear();
	}

	{
		MethodCXXSourceScopedStream global_method_definition{
			ss,
			false,
			"",
			"",
			"void",
			{"Library", "Global"},
			"main"
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"void Library::Global::main()\n"
			"{\n"
			"}\n"
		);
		printf(result);
		ss.clear();
	}

	{
		MethodCXXSourceScopedStream global_method_declaration_with_parameter{
			ss,
			true,
			"",
			"",
			"void",
			{},
			"main",
			{
				ParameterNode {
					ParameterNode::VALUE,
					"int",
					"argc"
				},
				ParameterNode {
					ParameterNode::REFERENCE_IN,
					"std::vector<std::string>",
					"argv"
				},
				ParameterNode {
					ParameterNode::REFERENCE_OUT,
					"int",
					"result"
				},
			}
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"void main(int argc, const std::vector<std::string>& argv, int& result);\n"
		);
		printf(result);
		ss.clear();
	}

	{
		MethodCXXSourceScopedStream derived_method_declaration_with_parameter{
			ss,
			true,
			"",
			"override",
			"void",
			{},
			"main",
			{
				ParameterNode {
					ParameterNode::VALUE,
					"int",
					"argc"
				},
				ParameterNode {
					ParameterNode::REFERENCE_IN,
					"std::vector<std::string>",
					"argv"
				},
				ParameterNode {
					ParameterNode::REFERENCE_OUT,
					"int",
					"result"
				},
			}
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"void main(int argc, const std::vector<std::string>& argv, int& result) override;\n"
		);
		printf(result);
		ss.clear();
	}

	{
		MethodCXXSourceScopedStream global_method_definition_with_parameter{
			ss,
			false,
			"",
			"",
			"void",
			{"Library", "Global"},
			"main",
			{
				ParameterNode {
					ParameterNode::VALUE,
					"int",
					"argc"
				},
				ParameterNode {
					ParameterNode::REFERENCE_IN,
					"std::vector<std::string>",
					"argv"
				},
				ParameterNode {
					ParameterNode::REFERENCE_OUT,
					"int",
					"result"
				},
			}
		};
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"void Library::Global::main(int argc, const std::vector<std::string>& argv, int& result)\n"
			"{\n"
			"}\n"
		);
		printf(result);
		ss.clear();
	}

	return;
}

TEST(CXXSourceStream, CommentCXXSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		CommentCXXSourceStream clazzComment{ ss };
		clazzComment.addName("Monster");
		clazzComment.addDescription("A class for units to attack and defend.\n"
			"It can be inherited by various sub - monsters.");
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"/*\n"
			" * Name : Monster\n"
			" * Description :\n"
			" * A class for units to attack and defend.\n"
			" * It can be inherited by various sub - monsters.\n"
			"*/\n"
		);
		printf(result);
		ss.clear();
	}

	{
		CommentCXXSourceStream methodComment{ ss };
		methodComment.addBrief("attack other units");
		methodComment.addParameter(true, "other", "another unit");
		methodComment.addParameter(true, "damage", "the damage that another unit takes");
		methodComment.addReturn("attack result");
	}
	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"/*\n"
			" * \\brief : attack other units\n"
			" * attack other units\n"
			" * @param[in]  other  another unit\n"
			" * @param[in]  damage  the damage that another unit takes\n"
			" * @return : attack result\n"
			"*/\n"
		);
		printf(result);
		ss.clear();
	}
	return;
}

TEST(CXXSourceStream, CallCXXSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	CallCXXSourceScopedStream methodA(ss, 
		"",
		{ "Library", "A" }, 
		"functionA", 
		{
			ParameterNode{ParameterNode::REFERENCE_IN, "int", "aa"},
			ParameterNode{ParameterNode::REFERENCE_OUT, "float", "bb"},
			ParameterNode{ParameterNode::VALUE, "double", "cc"}
		}
	);

	CallCXXSourceScopedStream methodb(ss,
		"jlong",
		{ "Library", "B" },
		"functionB",
		{
			ParameterNode{ParameterNode::REFERENCE_IN, "int", "aa"},
			ParameterNode{ParameterNode::REFERENCE_OUT, "float", "bb"},
		}
	);

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"Library::A::functionA(aa, bb, cc);\n"
			"(jlong)Library::B::functionB(aa, bb);\n"
		);
		printf(result);
		ss.clear();
	}
	return;
}

TEST(CXXSourceStream, CallbackCXXSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		CallbackCXXSourceStream callback{
			ss,
			"ACallback",
			"int32_t",
			{"ObjectA", "ObjectB", "float"}
		};
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"using ACallback = std::function<int32_t(ObjectA, ObjectB, float)>;\n"
		);
		printf(result);
		ss.clear();
	}
	return;
}