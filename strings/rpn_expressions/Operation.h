#pragma once

enum class Operation
{
    Add,
    Substract,
    Multiply,
    Divide,
    Push
};

struct SOperation
{
    Operation op;
    double value = 0;

    bool operator ==(const SOperation &other)const;
};

using OperationsList = std::vector<SOperation>;

SOperation MakeBinaryOperation(Operation op);
SOperation MakePushOperation(double value);
double ExecuteOperations(const OperationsList &list);
