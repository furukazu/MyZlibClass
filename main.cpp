// ConsoleApplication3.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "LocalZlib.h"
#include "main.h"


int main()
{
	auto inBuf = CreateBuffer("The Quick Brown Foxes Jumped over Lazy Dogs.  The Quick Brown Fox Jumps over the Lazy Dog.  The Quick Brown Foxes Jumped over the Lazy Dog.");

	LocalZlib zlib;
	auto outBuf = zlib.Deflate(inBuf);
	auto reBuf = zlib.Inflate(outBuf);

	auto resultString = std::string(std::begin(*reBuf),std::end(*reBuf));

	return 0;
}

