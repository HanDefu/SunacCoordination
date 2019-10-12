#ifndef BASE64_DECODER_H
#define BASE64_DECODER_H
#include <string>
//extern "C"
//{
#include "cdecode.h"
//}
class Base64Decoder
{
public:
	Base64Decoder();
	std::string decode(const std::string &code);
	int decode(const char *inputBuffer, int count, char *decodedBuffer);
private:
	base64_decodestate state;
};
#endif