#pragma once

/*
 В этом примере показаны 4 реализации функции, выделяющей буквы из входной строки.
 Мы хотим, чтобы функция умела определять не только буквы английского алфавита, но и кириллицу.
 Есть разные споособы реализации этой цели, с разной степенью кроссплатформенности.
*/

std::string FilterLetters_c_windows(const std::string &input);
std::string FilterLetters_c_unix(const std::string &input);
std::string FilterLetters_cxx11_utf8(const std::string &input);

#if defined(_WIN32)
std::string FilterLetters_winapi(const std::string &input);
#endif
