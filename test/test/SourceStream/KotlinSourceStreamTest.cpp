#include <SourceStream/KotlinSourceStream.hpp>
#include <gtest/gtest.h>

#include <cstdio>

TEST(KotlinSourceStream, PackageKotlinSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::vector<std::string> module_names{ "sans", "papyrus" };
	{
		PackageKotlinSourceStream packageStream(ss, "undertail", module_names);
	}

	const char* result = ss.str();
	
    EXPECT_STREQ(result,
		"package undertail.sans.papyrus\n"		
	);
	printf(result);
	return;
}

TEST(KotlinSourceStream, ImportKotlinSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	std::vector<std::string> module_names{ "sans", "papyrus", "undyne"};
	{
		ImportKotlinSourceStream importStream(ss, "undertail", module_names);
	}
	{
		ImportKotlinSourceStream importSream2{ ss, "kotlin.js.*" };
	}

	const char* result = ss.str();

	EXPECT_STREQ(result,
		"import undertail.sans.papyrus.undyne\n"
		"import kotlin.js.*\n"
	);
	printf(result);
	return;
}

TEST(KotlinSourceStream, ClassKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		ClassKotlinSourceScopedStream classStream{ ss, "Undyne", {"monster", "guard"} };
	}

	const char* result = ss.str();

	EXPECT_STREQ(result,
		"class Undyne : monster, guard\n"
		"{\n"
		"}\n"
	);
	printf(result);
	return;
}

TEST(KotlinSourceStream, CompanionObjectKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		ClassKotlinSourceScopedStream classStream{ ss, "Undyne", {} };
		{
			CompanionObjectKotlinSourceScopedStream companionObjectStream{ ss };
			companionObjectStream.addSingleTon("Undyne");
			{
				CompanionObjectKotlinSourceScopedStream::Init initStream{ ss };
				initStream.loadLibrary("Attacking");
			}
		}
	}

	const char* result = ss.str();

	
	EXPECT_STREQ(result,
		"class Undyne\n"
		"{\n"
        "\tcompanion object\n"
        "\t{\n"
        "\t\t@Volatile private var instance : Undyne ? = null\n"
        "\t\t@JvmStatic fun getInstance() : Undyne =\n"
        "\t\t\tinstance ?: synchronized(this) { instance ?: Undyne().also{instance = it} }\n"
        "\t\tinit\n"
        "\t\t{\n"
		"\t\t\tSystem.loadLibrary(\"Attacking\")\n"
		"\t\t}\n"
		"\t}\n"
        "}\n"
	);
	
	printf(result);
	return;
}

TEST(KotlinSourceStream, EnumKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		EnumKotlinSourceScopedStream enumStream{ ss, "Named"};
		enumStream.addElement("SANS", "0");
		enumStream.addElement("PAPYRUS", "1");
		enumStream.addElement("UNDYNE", "2");
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"enum class Named(val value : Int)\n"
		"{\n"
		"\tSANS(0.toInt()),\n"
		"\tPAPYRUS(1.toInt()),\n"
		"\tUNDYNE(2.toInt()),\n"
		"}\n"
	);

	printf(result);
	return;
}

TEST(KotlinSourceStream, AdvancedEnumKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		AdvancedEnumKotlinSourceScopedStream enumStream{ ss, "Named" };
		enumStream.addElement("SANS", "0", "sans");
		enumStream.addElement("PAPYRUS", "1", "papyrus");
		enumStream.addElement("UNDYNE", "2", "undyne");
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"enum class Named(val value : Int, val fullname : String)\n"
		"{\n"
		"\tSANS(0.toInt(), \"sans\"),\n"
		"\tPAPYRUS(1.toInt(), \"papyrus\"),\n"
		"\tUNDYNE(2.toInt(), \"undyne\"),\n"
		"}\n"
	);

	printf(result);
	return;
}

TEST(KotlinSourceStream, MethodKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		InterfaceKotlinSourceScopedStream interfaceStream{ ss, "Monster" };
		MethodKotlinSourceScopedStream func{ ss, true, KotlinAccess::PUBLIC, "", "", "String", "baseFunction",
				{
					ParameterNode {
						ParameterNode::VALUE,
						"Int",
						"baseParam"
					}
				}
		};
	}

	{
		ClassKotlinSourceScopedStream classStream{ ss, "Undyne", {"monster", "guard"} };
		{
			{
				MethodKotlinSourceScopedStream func{ ss, false, KotlinAccess::PUBLIC, "", "", "Int", "func",
					{
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Int",
							"attack"
						},
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Float",
							"defence"
						},
						ParameterNode {
							ParameterNode::REFERENCE_OUT,
							"String",
							"sans"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream func{ ss, false, KotlinAccess::PRIVATE, "", "", "Int", "funcPrivate",
					{
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Int",
							"attack"
						},
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Float",
							"defence"
						},
						ParameterNode {
							ParameterNode::REFERENCE_OUT,
							"String",
							"sans"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream func{ ss, false, KotlinAccess::PROTECTED, "", "", "String", "baseFunction",
					{
						ParameterNode {
							ParameterNode::VALUE,
							"Int",
							"baseParam"
						}
					}
				};
			}

			{
				MethodKotlinSourceScopedStream internalFunction{ ss, false, KotlinAccess::INTERNAL, "", "", "StringArray", "internalFunction",
					{
						ParameterNode {
							ParameterNode::VALUE,
							"Short",
							"pickachu"
						},
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Int",
							"raichu"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream derivedFunction{ ss, false, KotlinAccess::EXTERNAL, "override", "", "Array<Int>", "derivedFunction",
					{
						ParameterNode {
							ParameterNode::REFERENCE_IN,
							"Double",
							"aa"
						},
						ParameterNode {
							ParameterNode::REFERENCE_OUT,
							"Double",
							"bb"
						},
					}
				};
			}
		}
	}

	const char* result = ss.str();
	EXPECT_STREQ(result,
		"interface Monster\n"
		"{\n"
		"	fun baseFunction(baseParam : Int) : String\n"
		"}\n"
		"class Undyne : monster, guard\n"
		"{\n"
		"	fun func(attack : Int, defence : Float, sans : String) : Int\n"
		"	{\n"
		"	}\n"
		"	private fun funcPrivate(attack : Int, defence : Float, sans : String) : Int\n"
		"	{\n"
		"	}\n"
		"	protected fun baseFunction(baseParam : Int) : String\n"
		"	{\n"
		"	}\n"
		"	internal fun internalFunction(pickachu : Short, raichu : Int) : StringArray\n"
		"	{\n"
		"	}\n"
		"	external override fun derivedFunction(aa : Double, bb : Double) : Array<Int>\n"
		"}\n"
	);

	printf(result);
	return;
}

TEST(KotlinSourceStream, CommentKotlinSourceStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		CommentKotlinSourceStream clazzComment{ ss };
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
		CommentKotlinSourceStream methodComment{ ss };
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

TEST(KotlinSourceStream, CallKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	CallKotlinSourceScopedStream methodA(ss,
		"",
		{ "Library", "A" },
		"functionA",
		{
			ParameterNode{ParameterNode::REFERENCE_IN, "int", "aa"},
			ParameterNode{ParameterNode::REFERENCE_OUT, "float", "bb"},
			ParameterNode{ParameterNode::VALUE, "double", "cc"}
		}
	);

	CallKotlinSourceScopedStream methodb(ss,
		"Int",
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
			"Library.A.functionA(aa, bb, cc)\n"
			"Library.B.functionB(aa, bb) as Int\n"
		);
		printf(result);
		ss.clear();
	}
	return;
}

TEST(KotlinSourceStream, PropertyKotlinSourceScopedStream)
{
	using namespace LibraryInterfaceGenerator::Implementation;
	SourceStream ss;

	{
		PropertyKotlinSourceScopedStream prop(ss, KotlinAccess::PUBLIC, "", "Int", "sans", true);
		{
			auto getter = prop.createGetter();
			ss << "return _sans\n";
		}
	}

	{
		PropertyKotlinSourceScopedStream prop(ss, KotlinAccess::PRIVATE, "", "Float", "papyrus", false);
		{
			auto getter = prop.createGetter();
			ss << "return _papyrus\n";
		}
		{
			auto getter = prop.createSetter();
			ss << "_papyrus = value\n";
		}
	}

	{
		PropertyKotlinSourceScopedStream prop(ss, KotlinAccess::PROTECTED, "override", "Float", "papyrus2", false);
		{
			auto getter = prop.createGetter();
			ss << "return _papyrus\n";
		}
		{
			auto getter = prop.createSetter();
			ss << "_papyrus = value\n";
		}
	}

	{
		const char* result = ss.str();
		EXPECT_STREQ(result,
			"val sans : Int\n"
			"	get()\n"
			"	{\n"
			"		return _sans\n"
			"	}\n"
			"\n"
			"private var papyrus : Float\n"
			"	get()\n"
			"	{\n"
			"		return _papyrus\n"
			"	}\n"
			"	set(value)\n"
			"	{\n"
			"		_papyrus = value\n"
			"	}\n"
			"\n"
			"protected override var papyrus2 : Float\n"
			"	get()\n"
			"	{\n"
			"		return _papyrus\n"
			"	}\n"
			"	set(value)\n"
			"	{\n"
			"		_papyrus = value\n"
			"	}\n"
			"\n"
		);
		printf(result);
		ss.clear();
	}
	return;
}