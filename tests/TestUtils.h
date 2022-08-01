#include <GameManager.h>

#include <gtest/gtest.h>

#define EXPECT_THROW_CUSTOM(OP, EXCEP) \
	do { \
		try \
		{ \
			OP; \
			FAIL() << "Expected EXCEP"; \
		} \
		catch (const EXCEP& e) \
		{ \
		} \
		catch (...) \
		{ \
			FAIL() << "Expected EXCEP"; \
		} \
	} while (0)
