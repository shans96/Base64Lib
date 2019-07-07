#include "base64_encode.h"

std::string encode_to_base64(const char *source_bytes,
	size_t source_bytes_size)
{
	std::vector<unsigned char> padded_bytes(source_bytes, source_bytes + source_bytes_size);
	pad_type pad_result = pad(&padded_bytes);

	std::string encoded_bytes;
	encoded_bytes.reserve((padded_bytes.size() / 3) * 4);
	// The loop requires that the vector size be perfectly divisible by 3.
	for (size_t i = 0; i < padded_bytes.size(); i += 3)
	{
		unsigned char first_byte = padded_bytes[i];
		unsigned char second_byte = padded_bytes[i + 1];
		unsigned char last_byte = padded_bytes[i + 2];

		// A hexad is a group of six bits.

		encoded_bytes += b64_encoding_table[first_byte >> 2];
		encoded_bytes += b64_encoding_table[calculate_second_hexad(first_byte, second_byte)];
		encoded_bytes += b64_encoding_table[calculate_third_hexad(second_byte, last_byte)];
		encoded_bytes += b64_encoding_table[last_byte & 63];
	}

	overwrite_end_bytes(&encoded_bytes, pad_result);
	return encoded_bytes;
}

pad_type pad(std::vector<unsigned char> *vector)
{
	if (vector->size() % 3 == 0)
	{
		return not_padded;
	}
	else if ((vector->size() + 1) % 3 == 0)
	{
		vector->push_back('\0');
		return padded_once;
	}
	else if ((vector->size() + 2) % 3 == 0)
	{
		vector->push_back('\0');
		vector->push_back('\0');
		return padded_twice;
	}
	throw std::runtime_error("Error occurred while encoding: vector has invalid size");
}

void overwrite_end_bytes(std::string *string,
	pad_type pad_result)
{
	if (pad_result == padded_once)
	{
		string->back() = '=';
	}
	else if (pad_result == padded_twice)
	{
		string->back() = '=';
		string->at(string->length() - 2) = '=';
	}
}

unsigned char calculate_second_hexad(unsigned char first_byte, 
	unsigned char second_byte)
{
	unsigned char first_two_bits = (first_byte & 3) << 4;
	unsigned char last_four_bits = (second_byte & 240) >> 4;
	return first_two_bits ^ last_four_bits;
}

unsigned char calculate_third_hexad(unsigned char first_byte,
	unsigned char second_byte)
{
	unsigned char first_four_bits = (first_byte & 15) << 2;
	unsigned char last_two_bits = second_byte >> 6;
	return first_four_bits ^ last_two_bits;
}