#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static const char *b64_encoding_table =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

enum pad_type
{
	not_padded,
	padded_once,
	padded_twice
};

std::string encode_to_base64(const char *source_bytes,
	size_t source_bytes_size);
pad_type pad(std::vector<unsigned char> *vector);
void overwrite_end_bytes(std::string *string,
	pad_type pad_type);
unsigned char calculate_second_hexad(unsigned char first_byte,
	unsigned char second_byte);
unsigned char calculate_third_hexad(unsigned char first_byte,
	unsigned char second_byte);