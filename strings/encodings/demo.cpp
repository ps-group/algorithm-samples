#include "stdafx.h"
#include "FilterLetters.h"
#include <iostream>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#endif

std::vector<int> MakePrefixFunction(const std::string &prefix);

int main()
{
#if defined(_WIN32)
    // Для Windows устанавливаем кодировку EOM (DOS) для всех функций стандартной библиотеки C.
    // Под Linux строка ".OCP" принята не будет.
    setlocale(LC_ALL, ".OCP");
#endif

    std::string input;
    std::getline(std::cin, input);

    std::cout << "1st way: '" << FilterLetters_c_windows(input) << "'" << std::endl;
    std::cout << "2nd way: '" << FilterLetters_c_unix(input) << "'" << std::endl;

    try
    {
        std::cout << "3rd way: '" << FilterLetters_cxx11_utf8(input) << "'" << std::endl;
    }
    catch (std::exception const&)
    {
        std::cerr << "3rd way failed - input is not valid UTF-8" << std::endl;
    }

#if defined(_WIN32)
    std::cout << "4th way: '" << FilterLetters_winapi(input) << "'" << std::endl;
#endif

    return 0;
}

