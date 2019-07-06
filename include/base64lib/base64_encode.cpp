#include "base64_encode.h"
#include <stdexcept>

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
		unsigned char first_hexad = first_byte >> 2; 

		unsigned char second_hexad_first_bits = (first_byte & 0b00000011) << 4;
		unsigned char second_hexad_last_bits = (second_byte & 0b11110000) >> 4;
		unsigned char second_hexad = second_hexad_first_bits ^ second_hexad_last_bits;

		unsigned char third_hexad_first_bits = (second_byte & 0b00001111) << 2;
		unsigned char third_hexad_last_bits = last_byte >> 6;
		unsigned char third_hexad = third_hexad_first_bits ^ third_hexad_last_bits;

		unsigned char last_hexad = last_byte & 0b0111111;

		encoded_bytes += b64_encoding_table[first_hexad];
		encoded_bytes += b64_encoding_table[second_hexad];
		encoded_bytes += b64_encoding_table[third_hexad];
		encoded_bytes += b64_encoding_table[last_hexad];
	}

	if (pad_result == padded_once)
	{
		encoded_bytes.back() = '=';
	}
	else if (pad_result == padded_twice)
	{
		encoded_bytes.back() = '=';
		encoded_bytes.at(encoded_bytes.length() - 2) = '=';
	}
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
