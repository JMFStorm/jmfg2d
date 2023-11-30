#pragma once

#include "types.h"

void assert_true(bool assertion, const char* assertion_title, const char* file, const char* func, int line);

#define ASSERT_TRUE(assertion, assertion_title) assert_true(assertion, assertion_title, __FILE__, __func__, __LINE__)
