#include <gtest/gtest.h>

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	//::testing::GTEST_FLAG(filter) = "CXXSourceStream*";
	::testing::GTEST_FLAG(filter) = "Wrapper*";
	//::testing::GTEST_FLAG(filter) = "NativeSourceDirectory*";
	//::testing::GTEST_FLAG(filter) = "NativeInterface*";

	return RUN_ALL_TESTS();
}