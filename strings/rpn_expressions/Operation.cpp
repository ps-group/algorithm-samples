#include "stdafx.h"
#include "Operation.h"
#include <cassert>

// Оператор сравнения учитывает возможное накопление погрешности
// в числах с плавающей точкой.
bool SOperation::operator ==(const SOperation &other) const
{
    if (op != other.op)
        return false;

    return fabs(value - other.value) < std::numeric_limits<double>::epsilon();
}

SOperation MakeBinaryOperation(Operation op)
{
    assert(op != Operation::Push);
    SOperation result;
    result.op = op;
    return result;
}

SOperation MakePushOperation(double value)
{
    SOperation result;
    result.op = Operation::Push;
    result.value = value;
    return result;
}

double ExecuteOperations(const OperationsList &list)
{
    // При обращении за пределы массива будет выброшено исключение std::out_of_range,
    // потому что мы используем метод .at(i) вместо оператора [i].
    try
    {
        // Стек значений хранит значения до того момента, пока к ним не применят операцию.
        std::vector<double> stack;
        for (const SOperation &op : list)
        {
            size_t topIndex = stack.size() - 1;
            switch (op.op)
            {
            case Operation::Add:
                stack.at(topIndex - 1) += stack.at(topIndex);
                stack.pop_back();
                break;
            case Operation::Divide:
                stack.at(topIndex - 1) /= stack.at(topIndex);
                stack.pop_back();
                break;
            case Operation::Multiply:
                stack.at(topIndex - 1) *= stack.at(topIndex);
                stack.pop_back();
                break;
            case Operation::Substract:
                stack.at(topIndex - 1) -= stack.at(topIndex);
                stack.pop_back();
                break;
            case Operation::Push:
                stack.push_back(op.value);
                break;
            }
        }
        // Последнее оставшееся на стеке число и есть результат вычислений.
        return stack.at(0);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return std::numeric_limits<double>::quiet_NaN();
    }
}
