#include "stdafx.h"
#include "BoostGraph.h"
#include <sstream>
#include <boost/algorithm/string.hpp>


using std::vector;
using std::string;
using std::stringstream;


namespace
{

const char C_INPUT_TEXT_1[] = R"***(
4 5
2 3 5
1 4 5
1 2 6
1 3 1
3 4 3
)***";

const char C_OUTPUT_1[] = R"***(
9
1 3
2 3
3 4
)***";

// Используем split для игнорирования пробелов и переносов строк при проверке.
vector<string> SplitWords(string const& text)
{
	std::string trimmed = boost::trim_copy(text);

	vector<string> words;
	boost::split(words, trimmed, boost::is_space(), boost::token_compress_on);
	return words;
}

void RunTextInputTestcase(std::string const& inputText, std::string const& expectedOutput)
{
	CBoostGraph graph;
	stringstream input(inputText);
	stringstream output;
	BOOST_CHECK(graph.ReadText(input));
    graph.RunPrima();
    graph.PrintResults(output);
	vector<string> words = SplitWords(output.str());
    vector<string> expectedWords = SplitWords(expectedOutput);
    BOOST_CHECK(words == expectedWords);
}

}

BOOST_AUTO_TEST_SUITE(Graph_with_Bellmand_Ford_algorithm)
	BOOST_AUTO_TEST_CASE(can_load_text_format)
	{
		RunTextInputTestcase(C_INPUT_TEXT_1, C_OUTPUT_1);
    }
BOOST_AUTO_TEST_SUITE_END()
