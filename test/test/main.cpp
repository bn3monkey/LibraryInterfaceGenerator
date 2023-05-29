#include <gtest/gtest.h>

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	//::testing::GTEST_FLAG(filter) = "CXXSourceStream*";
	//::testing::GTEST_FLAG(filter) = "KotlinSourceStream*";
	// 
	::testing::GTEST_FLAG(filter) = "NativeSourceDirectory*";
	//::testing::GTEST_FLAG(filter) = "NativeInterface*";
	//::testing::GTEST_FLAG(filter) = "SourceDirectory*";
	//::testing::GTEST_FLAG(filter) = "Wrapper*";

	return RUN_ALL_TESTS();
}