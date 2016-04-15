#pragma once
#include <boost/utility/string_ref.hpp>

// Совершает разбор выражения в инфиксной (естественной) нотации.
// Возвращает результат вычисления выражения.
class CInfixExpressionCalculator
{
public:
    CInfixExpressionCalculator() = delete;

    // Выполняет разбор выражений вида "7 / 2 + 12 - 3 * 4 + 17 - 2 * 7"
    // Вычисляет и возвращает результат.
    static double ParseExpr(const std::string &expr);

private:
    static double ParseDouble(boost::string_ref &ref);
    static double ParseExprMul(boost::string_ref &ref);
    static double ParseExprSum(boost::string_ref &ref);
    static void SkipSpaces(boost::string_ref &ref);
};
