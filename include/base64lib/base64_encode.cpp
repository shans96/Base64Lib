#include "base64_encode.h"

std::string encode_to_base64(const char *source_bytes,
	size_t source_bytes_size)
{
	std::vector<unsigned char> padded_bytes;
	padded_bytes.reserve(source_bytes_size);
	for (int i = 0; i < source_bytes_size; i++)
	{
		padded_bytes.push_back(source_bytes[i]);
	}
	pad_type pad_result = pad(&padded_bytes);

	std::string encoded_bytes;
	encoded_bytes.reserve((size_t)(padded_bytes.size() / 3) * 4);
	// The loop requires that the vector size be perfectly divisible by 3.
	for (size_t i = 0; i < padded_bytes.size(); i += 3)
	{
		unsigned char first_byte = padded_bytes[i];
		unsigned char second_byte = padded_bytes[i + 1];
		unsigned char last_byte = padded_bytes[i + 2];

		unsigned char first_hexad = first_byte >> 2; // A hexad is a group of six bits.

		unsigned char second_chunk_first_two = (first_byte & 0b00000011) << 4;
		unsigned char second_chunk_last_four = (second_byte & 0b11110000) >> 4;
		unsigned char second_hexad = second_chunk_first_two ^ second_chunk_last_four;

		unsigned char third_chunk_first_four = (second_byte & 0b00001111) << 2;
		unsigned char third_chunk_last_two = last_byte >> 6;
		unsigned char third_hexad = third_chunk_first_four ^ third_chunk_last_two;

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
}
