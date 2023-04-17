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
        "\t\t\tinstance ? : synchronized(this) { instance ? : Undyne().also{instance = it} }\n"
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
		ClassKotlinSourceScopedStream classStream{ ss, "Undyne", {"monster", "guard"} };
		{
			{
				MethodKotlinSourceScopedStream func{ ss, MethodKotlinSourceScopedStream::Access::PUBLIC, "", "", "Int", "func",
					{
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Int",
							"attack"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Float",
							"defence"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_OUT,
							"String",
							"sans"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream func{ ss, MethodKotlinSourceScopedStream::Access::PRIVATE, "", "", "Int", "funcPrivate",
					{
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Int",
							"attack"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Float",
							"defence"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_OUT,
							"String",
							"sans"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream func{ ss, MethodKotlinSourceScopedStream::Access::PROTECTED, "", "", "String", "baseFunction",
					{
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::VALUE,
							"Int",
							"baseParam"
						}
					}
				};
			}

			{
				MethodKotlinSourceScopedStream internalFunction{ ss, MethodKotlinSourceScopedStream::Access::INTERNAL, "", "", "StringArray", "internalFunction",
					{
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::VALUE,
							"Short",
							"pickachu"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Int",
							"raichu"
						},
					}
				};
			}

			{
				MethodKotlinSourceScopedStream derivedFunction{ ss, MethodKotlinSourceScopedStream::Access::EXTERNAL, "override", "", "Array<Int>", "derivedFunction",
					{
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_IN,
							"Double",
							"aa"
						},
						MethodKotlinSourceScopedStream::Parameter {
							MethodKotlinSourceScopedStream::Parameter::REFERENCE_OUT,
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