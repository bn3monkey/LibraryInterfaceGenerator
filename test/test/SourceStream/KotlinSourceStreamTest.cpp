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