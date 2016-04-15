#pragma once

#include "Operation.h"

// Совершает разбор выражения в обратной польской нотации (Reverse Polish Notation).
// Возвращает массив операций, которые надо совершить.
class CRPNExpressionTokenizer
{
public:
    CRPNExpressionTokenizer();

    bool Parse(const std::string &expression);
    OperationsList GetResult()const;

private:
    bool Advance(const std::string &expression, size_t &position);
    bool AdvanceNormalState(const std::string &expression, size_t &position);

    OperationsList m_operations;
    std::string m_currentNumber;
};
