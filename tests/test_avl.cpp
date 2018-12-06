//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "AVL"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "../binary_search_tree/avl.h";

//Name your test cases for what they test
BOOST_AUTO_TEST_CASE(insert)
{
	AVL obj;
	BOOST_CHECK_EQUAL("false", "SomeValue");
}