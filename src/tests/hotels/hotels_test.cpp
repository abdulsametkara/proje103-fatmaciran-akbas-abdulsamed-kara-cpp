//#define ENABLE_HOTELS_TEST  // Uncomment this line to enable the Hotels tests

#include "gtest/gtest.h"
#include "../../hotels/header/hotels.h"  // Adjust this include path based on your project structure
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, char** argv) {
#ifdef ENABLE_TRY_TEST
	::testing::InitGoogleTest(&argc, argv);
	::testing::GTEST_FLAG(color) = "no";
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}