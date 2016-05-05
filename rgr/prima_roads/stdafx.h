#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#ifdef _WIN32
#pragma warning (disable: 4512)
#endif

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

#define BOOST_TEST_INCLUDED
#ifdef _WIN32
#pragma warning (disable: 4702)
#pragma warning (disable: 4535)
#endif
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>
