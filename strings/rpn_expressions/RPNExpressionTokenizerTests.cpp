#include "stdafx.h"
#include "RPNExpressionTokenizer.h"


namespace
{
bool AreEqual(OperationsList const& left, OperationsList const& right)
{
    return left == right;
}

const double EPSILON = std::numeric_limits<double>::epsilon();
}


BOOST_AUTO_TEST_SUITE(CRPNExpressionTokenizer_class)
    BOOST_AUTO_TEST_CASE(produces_tokens_sequence)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("10 12 13 +*"));
        BOOST_CHECK(AreEqual(tokenizer.GetResult(), {
                                 MakePushOperation(10),
                                 MakePushOperation(12),
                                 MakePushOperation(13),
                                 MakeBinaryOperation(Operation::Add),
                                 MakeBinaryOperation(Operation::Multiply)
                             }));
    }

    BOOST_AUTO_TEST_CASE(can_skip_spaces_between_operators)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("10 24 18 60912 +    *   -"));
        BOOST_CHECK(AreEqual(tokenizer.GetResult(), {
                                 MakePushOperation(10),
                                 MakePushOperation(24),
                                 MakePushOperation(18),
                                 MakePushOperation(60912),
                                 MakeBinaryOperation(Operation::Add),
                                 MakeBinaryOperation(Operation::Multiply),
                                 MakeBinaryOperation(Operation::Substract)
                             }));
    }

    BOOST_AUTO_TEST_CASE(accepts_floating_point)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("10.2 124.01 97. + +"));
        BOOST_CHECK(AreEqual(tokenizer.GetResult(), {
                                 MakePushOperation(10.2),
                                 MakePushOperation(124.01),
                                 MakePushOperation(97),
                                 MakeBinaryOperation(Operation::Add),
                                 MakeBinaryOperation(Operation::Add)
                             }));
    }

    BOOST_AUTO_TEST_CASE(produces_empty_if_input_empty)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("     "));
        BOOST_CHECK(AreEqual(tokenizer.GetResult(), OperationsList()));
    }

    BOOST_AUTO_TEST_CASE(simple_calculation_finishes_OK)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("  1   12 +"));
        double value = ExecuteOperations(tokenizer.GetResult());
        BOOST_CHECK_CLOSE_FRACTION(value, 13, EPSILON);
    }

    BOOST_AUTO_TEST_CASE(complex_calculation_finishes_OK)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("94 0.12 * 24 3 / 4 / -"));
        double value = ExecuteOperations(tokenizer.GetResult());
        BOOST_CHECK_CLOSE_FRACTION(value, 9.28, EPSILON);
    }

    BOOST_AUTO_TEST_CASE(trivial_calculation_finishes_OK)
    {
        CRPNExpressionTokenizer tokenizer;
        BOOST_CHECK(tokenizer.Parse("94 0 * 24 24 / -"));
        double value = ExecuteOperations(tokenizer.GetResult());
        BOOST_CHECK_CLOSE_FRACTION(value, -1, EPSILON);
    }
BOOST_AUTO_TEST_SUITE_END()
