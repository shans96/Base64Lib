#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static const char *b64_encoding_table =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace base64lib
{
	std::string encode_to_base64(const char *source_bytes,
		size_t source_bytes_size);
}
