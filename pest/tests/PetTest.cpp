#include <Pet.hpp>

#include <streambuf>

#define BOOST_TEST_MODULE Pet
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE ( GenerateTraceLine  )
{
    stringbuf buffer;
}

BOOST_AUTO_TEST_CASE ( PetMain )
{
    BOOST_CHECK_EQUAL(2+2,4);
}
