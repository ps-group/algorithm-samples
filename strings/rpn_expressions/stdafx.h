#ifndef STDAFX_H
#define STDAFX_H

#ifdef _WIN32
#include <SDKDDKVer.h>
#include <tchar.h>
#endif

#if defined(__cplusplus)

#include <cctype>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <limits>

#if defined(_MSC_VER)
  #pragma warning (disable: 4512)
#endif // _MSC_VER

#define BOOST_TEST_DYN_LINK

#if defined(_MSC_VER)
  #pragma warning (disable: 4702)
  #pragma warning (disable: 4535)
#endif // _MSC_VER
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/replace.hpp>

#endif // __cplusplus


#endif // STDAFX_H
