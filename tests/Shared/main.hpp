#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestTingShared(){
	TestBasicTingShared::Run();

	TRACE_ALWAYS(<< "[PASSED]: Shared test" << std::endl)
}
