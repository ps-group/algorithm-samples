#include "stdafx.h"
#include "RPNExpressionTokenizer.h"

CRPNExpressionTokenizer::CRPNExpressionTokenizer()
{
}

bool CRPNExpressionTokenizer::Parse(const std::string &expression)
{
    m_currentNumber.clear();
    m_operations.clear();

    size_t position = 0;
    while (position < expression.size())
    {
        if (!Advance(expression, position))
        {
            return false;
        }
    }
    return true;
}

OperationsList CRPNExpressionTokenizer::GetResult() const
{
    return m_operations;
}

bool CRPNExpressionTokenizer::Advance(const std::string &expression, size_t &position)
{
    // Состояние по-умолчанию.
    if (m_currentNumber.empty())
    {
        return AdvanceNormalState(expression, position);
    }

    char next = expression[position];
    // Состояние "сканируем число".
    if (std::isdigit(next) || next == '.')
    {
        m_currentNumber += next;
        ++position;
    }
    else
    {
        // Следует проверить strtod на ошибки, т.к. в строке может оказаться несколько точек.
        char *pEnd = nullptr;
        const double value = strtod(m_currentNumber.c_str(), &pEnd);
        if (pEnd == m_currentNumber.c_str())
        {
            std::cerr << "Invalid number " << m_currentNumber << std::endl;
            return false;
        }

        m_operations.push_back(MakePushOperation(value));
        m_currentNumber.clear();
    }
    return true;
}

bool CRPNExpressionTokenizer::AdvanceNormalState(const std::string &expression, size_t &position)
{
    // Ожидаем оператор или начало числа.
    char next = expression[position];
    if (next == '+')
    {
        m_operations.push_back(MakeBinaryOperation(Operation::Add));
    }
    else if (next == '-')
    {
        m_operations.push_back(MakeBinaryOperation(Operation::Substract));
    }
    else if (next == '*')
    {
        m_operations.push_back(MakeBinaryOperation(Operation::Multiply));
    }
    else if (next == '/')
    {
        m_operations.push_back(MakeBinaryOperation(Operation::Divide));
    }
    else if (std::isdigit(next))
    {
        m_currentNumber += next;
    }
    else if (!std::isblank(next)) // Пропуск пробелов.
    {
        std::cerr << "Unexpected character " << next << std::endl;
        return false;
    }
    ++position;
    return true;
}
