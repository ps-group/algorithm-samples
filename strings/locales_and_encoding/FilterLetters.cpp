#include "stdafx.h"
#include "FilterLetters.h"
#include <iostream>

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#endif

using namespace std;

// Функция фильтрует слова средствами библиотеки языка C.
//
// Работает неправильно на UNIX-системах, где для однойбайтных строк используется UTF-8.
// В результате перебор `char` в UNIX будет работать некорректно, т.к. в UTF-8 берётся
// `char` может быть лишь частью юникодного символа.
string FilterLetters_c_windows(const string &input)
{
    string output;
    output.reserve(input.size());
    for (char ch : input)
    {
        bool isAscii = (ch >= 0 && ch < 128);
        if (isAscii && isalpha(ch))
        {
            output += ch;
        }
    }
    return output;
}

// Функция фильтрует слова средствами библиотеки языка C.
//
// Работает корректно на UNIX-системах,
// потому что внутри функции мы конвертируем строку в wchar_t, в котором один элемент
// типа `wchar_t` почти всегда равен одному юникодному символу.
string FilterLetters_c_unix(const string &input)
{
    // Если выделить wstring того же размера, этого хватит,
    // т.к. длина 16-битной или 32-битной строки меньше или равна длине 8-битной строки,
    // хотя размер одного элемента и количество байт памяти больше или равно
    // аналогичным величинам в 8-битной строке.
    wstring inputW;
    inputW.resize(input.size(), 0);
    // Получаем указатель массив памяти строки.
    wchar_t *destination = &inputW[0];

    // Преобразуем строку `char` в строку `wchar_t`
    size_t encodedLength = mbstowcs(destination, input.c_str(), inputW.size());
    if (encodedLength == size_t(-1))
    {
        cerr << "Input string has broken encoding!" << endl;
        return string();
    }
    inputW = inputW.substr(0, encodedLength);

    wstring outputW;
    outputW.reserve(inputW.size());
    for (wchar_t ch : inputW)
    {
        if (iswalpha(ch))
        {
            outputW += ch;
        }
    }

    string output;
    output.resize(input.size());
    // Обратное преобразование строки `wchar_t` в строку `char`
    // Код возврата можно и не проверять, т.к. размер input больше или равен размеру output.
    encodedLength = wcstombs(&output[0], outputW.c_str(), output.size());
    output = output.substr(0, encodedLength);

    return output;
}

std::string FilterLetters_cxx11_utf8(const std::string &input)
{
    std::wstring_convert<codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring inputW = converter.from_bytes(input);

    wstring outputW;
    outputW.reserve(inputW.size());
    for (wchar_t ch : inputW)
    {
        if (iswalpha(ch))
        {
            outputW += ch;
        }
    }

    return converter.to_bytes(outputW);
}

#if defined(_WIN32)
std::string FilterLetters_winapi(const std::string &input)
{
    int estimatedLength = MultiByteToWideChar(CP_OEMCP, 0, input.c_str(), input.size(), NULL, 0);
    std::wstring inputW(estimatedLength, 0);
    MultiByteToWideChar(CP_OEMCP, 0, input.c_str(), input.size(), &inputW[0], estimatedLength);

    wstring outputW;
    outputW.reserve(inputW.size());
    for (wchar_t ch : inputW)
    {
        if (iswalpha(ch))
        {
            outputW += ch;
        }
    }

    estimatedLength = WideCharToMultiByte(CP_OEMCP, 0, outputW.c_str(), (int)outputW.size(), nullptr, 0, nullptr, nullptr);
    std::string output(estimatedLength, 0);
    WideCharToMultiByte(CP_OEMCP, 0, outputW.c_str(), (int)outputW.size(), &output[0], estimatedLength, NULL, NULL);

    return output;
}
#endif


