#include "InfixExpressionCalculator.h"
#include <cctype>

double CInfixExpressionCalculator::ParseExpr(const std::string &expr)
{
    boost::string_ref ref(expr);
    double value = ParseExprSum(ref);
    SkipSpaces(ref);
    if (!ref.empty())
    {
        value = std::numeric_limits<double>::quiet_NaN();
    }
    return value;
}

double CInfixExpressionCalculator::ParseDouble(boost::string_ref &ref)
{
    SkipSpaces(ref);

    double value = 0;
    bool parsedAny = false;
    while (!ref.empty() && std::isdigit(ref[0]))
    {
        parsedAny = true;
        const int digit = ref[0] - '0';
        value = value * 10.0 + double(digit);
        ref.remove_prefix(1);
    }
    if (!parsedAny)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (ref.empty() || (ref[0] != '.'))
    {
        return value;
    }
    ref.remove_prefix(1);
    double factor = 1.0;
    while (!ref.empty() && std::isdigit(ref[0]))
    {
        const int digit = ref[0] - '0';
        factor *= 0.1;
        value += factor * double(digit);
        ref.remove_prefix(1);
    }

    return value;
}

double CInfixExpressionCalculator::ParseExprMul(boost::string_ref &ref)
{
    double value = ParseDouble(ref);
    while (true)
    {
        SkipSpaces(ref);
        if (!ref.empty() && ref[0] == '*')
        {
            ref.remove_prefix(1);
            value *= ParseDouble(ref);
        }
        else if (!ref.empty() && ref[0] == '/')
        {
            ref.remove_prefix(1);
            value /= ParseDouble(ref);
        }
        else
        {
            break;
        }
    }

    return value;
}

double CInfixExpressionCalculator::ParseExprSum(boost::string_ref &ref)
{
    double value = ParseExprMul(ref);
    while (true)
    {
        SkipSpaces(ref);

        if (!ref.empty() && ref[0] == '+')
        {
            ref.remove_prefix(1);
            value += ParseExprMul(ref);
        }
        else if (!ref.empty() && ref[0] == '-')
        {
            ref.remove_prefix(1);
            value -= ParseExprMul(ref);
        }
        else
        {
            break;
        }
    }

    return value;
}

void CInfixExpressionCalculator::SkipSpaces(boost::string_ref &ref)
{
    size_t i = 0;
    while (i < ref.size() && std::isspace(ref[i]))
        ++i;
    ref.remove_prefix(i);
}
