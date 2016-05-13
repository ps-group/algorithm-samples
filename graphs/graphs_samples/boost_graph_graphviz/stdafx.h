#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#pragma warning (disable: 4512)

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

#define BOOST_TEST_INCLUDED
#define BOOST_TEST_NO_MAIN
#pragma warning (disable: 4702)
#pragma warning (disable: 4535)
#include <boost/test/included/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>
