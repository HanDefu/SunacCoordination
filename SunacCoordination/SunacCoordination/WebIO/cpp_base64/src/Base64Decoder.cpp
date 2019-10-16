#include "stdafx.h"
#include "Base64Decoder.h"
Base64Decoder::Base64Decoder() { base64_init_decodestate(&state); }
std::string Base64Decoder::decode(const std::string &code)
{
	char *buffer = new char[code.length()];
	int numberOfCharacters = base64_decode_block(code.c_str(), (int)code.length(), buffer, &state);
	buffer[numberOfCharacters] = '\0';
	std::string result(buffer);
	delete[] buffer;
	return result;
}
int Base64Decoder::decode(const char *inputBuffer, int count, char *decodedBuffer)
{
	return base64_decode_block(inputBuffer, count, decodedBuffer, &state);
}