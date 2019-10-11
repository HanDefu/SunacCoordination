#include "stdafx.h"
#include "Base64Encoder.h"
Base64Encoder::Base64Encoder() { base64_init_encodestate(&state); }
std::string Base64Encoder::encode(const std::string &text)
{
	char *buffer = new char[text.length() * 2];
	int numberOfBytesEncoded = base64_encode_block(text.c_str(), text.length(), buffer, &state);
	numberOfBytesEncoded += base64_encode_blockend(buffer + numberOfBytesEncoded, &state);
	buffer[numberOfBytesEncoded] = '\0';
	std::string result(buffer);
	delete[] buffer;
	base64_init_encodestate(&state);
	return result;
}
int Base64Encoder::encode(const char *inputBuffer, unsigned count, char *encodedBuffer)
{
	return base64_encode_block(inputBuffer, count, encodedBuffer, &state);
}
int Base64Encoder::encodeEnd(char *encodedBuffer)
{
	return base64_encode_blockend(encodedBuffer, &state);
}