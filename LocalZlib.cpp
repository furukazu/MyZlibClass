#include "LocalZlib.h"

#include <algorithm>

spBuffer CreateBuffer(const std::string & text) {
	auto ret = spBuffer(new buffer());
	std::copy(std::begin(text),std::end(text), std::back_inserter(*ret));
	return ret;
}

spBuffer CreateBuffer(const byte * source, std::size_t size) {
	auto ret = spBuffer(new buffer());
	std::copy(source, source + size, std::back_inserter(*ret));
	return ret;
}

spBuffer AppendBuffer(spBuffer dst, const byte * source, std::size_t size) {
	std::copy(source,source+size,std::back_inserter(*dst));
	return dst;
}

LocalZlib::LocalZlib()
{
}


LocalZlib::~LocalZlib()
{
}

spBuffer LocalZlib::Deflate(spBuffer input) {

	result = spBuffer(new buffer());
	result->resize(input->size()*2);

	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	::deflateInit(&z, Z_DEFAULT_COMPRESSION);

	z.avail_in = input->size();
	z.next_in = input->data();

	z.avail_out = result->size();
	z.next_out = result->data();

	::deflate(&z, Z_FINISH);

	const auto size = z.total_out;

	::deflateEnd(&z);

	result->resize(size);

	return result;
}

spBuffer LocalZlib::Inflate(spBuffer input) {

	const std::size_t siz = 0x080;
	decltype(z.total_out) completed = 0;

	result = spBuffer(new buffer());
	byte tmp[siz];

	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	::inflateInit(&z);


	z.avail_in = input->size();
	z.next_in = input->data();

	z.avail_out = siz;
	z.next_out = tmp;

retry:
	auto res = ::inflate(&z, Z_NO_FLUSH);

	if (res == Z_STREAM_END) {
		const auto progress = z.total_out - completed;
		completed += progress;
		AppendBuffer(result, tmp, progress);
		goto finish;
	}

	if (res == Z_OK) {
		goto retry;
	}

	if (res == Z_BUF_ERROR) {
		const auto progress = siz - z.avail_out;
		completed += progress;
		AppendBuffer(result, tmp, progress);
		z.avail_out = siz;
		z.next_out = tmp;
		goto retry;
	}

	return result;

finish:
	::inflateEnd(&z);


	return result;
}
