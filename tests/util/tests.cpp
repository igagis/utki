#include "../../src/utki/debug.hpp"
#include "../../src/utki/types.hpp"
#include "../../src/utki/util.hpp"
#include "../../src/utki/Buffer.hpp"

#include "tests.hpp"



using namespace utki;


namespace TestSerialization{
void Run(){
	//16 bit
	for(std::uint32_t i = 0; i <= std::uint16_t(-1); ++i){
		std::array<std::uint8_t, sizeof(std::uint16_t)> buf;
		utki::util::Serialize16LE(std::uint16_t(i), buf.begin());

		ASSERT_ALWAYS(buf[0] == std::uint8_t(i & 0xff))
		ASSERT_ALWAYS(buf[1] == std::uint8_t((i >> 8) & 0xff))

		std::uint16_t res = utki::util::Deserialize16LE(buf.begin());
		ASSERT_ALWAYS(res == std::uint16_t(i))
//		TRACE(<< "TestSerialization(): i16 = " << i << std::endl)
	}

	//32 bit
	for(std::uint64_t i = 0; i <= std::uint32_t(-1); i += 1317){//increment by 1317, because if increment by 1 it takes too long to run the test
		std::array<std::uint8_t, sizeof(std::uint32_t)> buf;
		utki::util::Serialize32LE(std::uint32_t(i), buf.begin());

		ASSERT_ALWAYS(buf[0] == std::uint8_t(i & 0xff))
		ASSERT_ALWAYS(buf[1] == std::uint8_t((i >> 8) & 0xff))
		ASSERT_ALWAYS(buf[2] == std::uint8_t((i >> 16) & 0xff))
		ASSERT_ALWAYS(buf[3] == std::uint8_t((i >> 24) & 0xff))

		std::uint32_t res = utki::util::Deserialize32LE(buf.begin());
		ASSERT_ALWAYS(res == std::uint32_t(i))
//		TRACE(<< "TestSerialization(): i32 = " << i << std::endl)
	}
}
}//~namespace



namespace TestScopeExit{
void Run(){
	bool flag = false;
	{
		utki::util::ScopeExit se([&flag](){
			flag = true;
		});
	}
	ASSERT_ALWAYS(flag)
}
}
