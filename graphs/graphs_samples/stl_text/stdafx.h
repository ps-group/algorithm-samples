#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#define BOOST_TEST_INCLUDED
#pragma warning (disable: 4702)
#pragma warning (disable: 4535)
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>
