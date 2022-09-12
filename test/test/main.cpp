#include <gtest/gtest.h>

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	//::testing::GTEST_FLAG(filter) = "Verification*";
	//::testing::GTEST_FLAG(filter) = "ModalityWorklist*";
	//::testing::GTEST_FLAG(filter) = "MPPS*";
	//::testing::GTEST_FLAG(filter) = "Storage*";

	return RUN_ALL_TESTS();
}