#pragma once

#include <zlib.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>

typedef std::uint8_t byte;
typedef std::vector<byte> buffer;
typedef std::shared_ptr<buffer> spBuffer;

spBuffer CreateBuffer(const std::string & text);
spBuffer CreateBuffer(const byte * source,std::size_t size);
spBuffer AppendBuffer(spBuffer dst,const byte * source, std::size_t size);

class LocalZlib
{
public:
	LocalZlib();
	virtual ~LocalZlib();

public:
	spBuffer Deflate(spBuffer input);
	spBuffer Inflate(spBuffer input);

private:
	z_stream z;
	spBuffer result;
};

