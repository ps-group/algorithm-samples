#include "stdafx.h"
#include "InfixExpressionCalculator.h"


namespace
{
const double EPSILON = std::numeric_limits<double>::epsilon();
}


BOOST_AUTO_TEST_SUITE(CInfixExpressionCalculator_class)
    BOOST_AUTO_TEST_CASE(accounts_operator_precedence)
    {
        double value = CInfixExpressionCalculator::ParseExpr("10.2 + 1.1445 * 13");
        BOOST_CHECK_CLOSE_FRACTION(value, 25.0785, EPSILON);
    }

    BOOST_AUTO_TEST_CASE(can_skip_extra_spaces)
    {
        double value = CInfixExpressionCalculator::ParseExpr("    10.2 +    1.1445 * 13    ");
        BOOST_CHECK_CLOSE_FRACTION(value, 25.0785, EPSILON);
    }

    BOOST_AUTO_TEST_CASE(accepts_floating_point)
    {
        double value = CInfixExpressionCalculator::ParseExpr("10.2 + 124.01 + 97.0");
        BOOST_CHECK_CLOSE_FRACTION(value, 231.21, EPSILON);
    }

    BOOST_AUTO_TEST_CASE(accounts_left_association_rules)
    {
        double value = CInfixExpressionCalculator::ParseExpr("10.2 / 2.0 / 2 / 2 / 2.000");
        BOOST_CHECK_CLOSE_FRACTION(value, 0.6375, EPSILON);
    }
BOOST_AUTO_TEST_SUITE_END()
