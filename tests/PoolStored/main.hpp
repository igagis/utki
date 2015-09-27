#pragma once

#include "../../src/utki/debug.hpp"

#include "tests.hpp"


inline void TestTingPoolStored(){
	BasicPoolStoredTest::Run();
	
	TRACE_ALWAYS(<< "[PASSED]: PoolStored test" << std::endl)
}
