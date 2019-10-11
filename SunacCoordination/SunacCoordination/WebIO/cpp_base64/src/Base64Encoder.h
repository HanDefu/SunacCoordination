#ifndef BASE64_ENCODER_H
#define BASE64_ENCODER_H
#include <string>
//extern "C"
//{
#include "cencode.h"
//}

class Base64Encoder
{
public:
	Base64Encoder();
	std::string encode(const std::string &text);
	int encode(const char *inputBuffer, unsigned count, char *encodedBuffer);
	int encodeEnd(char *encodedBuffer);
private:
	base64_encodestate state;
};
#endif