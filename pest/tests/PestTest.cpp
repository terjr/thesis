#include <Pest.hpp>

#include <streambuf>

#define BOOST_TEST_MODULE Pest
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE ( GenerateTraceLine  )
{
    stringbuf buffer;
}

BOOST_AUTO_TEST_CASE ( PestMain )
{
    BOOST_CHECK_EQUAL(2+2,4);
}
