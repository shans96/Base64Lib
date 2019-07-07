#include "catch.hpp"
#include "../include/base64lib/base64_encode.h"

SCENARIO("Valid data is provided to encode", "[encoding]")
{
	GIVEN("The encode function is provided some data")
	{
		WHEN("This data requires two extra bytes of padding")
		{
			THEN("The resulting Base64 string will have two equal signs at the end")
			{
				REQUIRE(base64lib::encode_to_base64("A", 1) == std::string("QQ=="));
			}
		}
	}

	GIVEN("The encode function is provided some data")
	{
		WHEN("This data requires one extra byte of padding")
		{
			THEN("The resulting Base64 string will have one equal sign at the end")
			{
				REQUIRE(base64lib::encode_to_base64("AB", 2) == std::string("QUI="));
			}
		}
	}

	GIVEN("The encode function is provided some data")
	{
		WHEN("This data requires no extra padding")
		{
			THEN("The resulting Base64 string will have no equal signs at the end")
			{
				REQUIRE(base64lib::encode_to_base64("ABC", 3) == std::string("QUJD"));
			}
		}
	}
}
