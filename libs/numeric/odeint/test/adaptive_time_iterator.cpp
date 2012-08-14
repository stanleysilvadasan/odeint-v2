/*
 * adaptive_time_iterator.cpp
 *
 *  Created on: Aug 6, 2012
 *      Author: Karsten Ahnert
 */

#define BOOST_TEST_MODULE odeint_adaptive_time_iterator

#include <iterator>
#include <algorithm>
#include <vector>

#include <boost/numeric/odeint/config.hpp>
#include <boost/array.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <boost/numeric/odeint/iterator/adaptive_time_iterator.hpp>
#include "dummy_steppers.hpp"

namespace mpl = boost::mpl;
using namespace boost::numeric::odeint;

struct dummy_system { };

typedef dummy_stepper::state_type state_type;
typedef dummy_stepper::value_type value_type;
typedef dummy_stepper::time_type time_type;
typedef std::vector< std::pair< state_type , time_type > > result_vector;

BOOST_AUTO_TEST_SUITE( adaptive_time_iterator_test )

typedef mpl::vector<
    dummy_controlled_stepper
    , dummy_dense_output_stepper
    > dummy_steppers;


BOOST_AUTO_TEST_CASE_TEMPLATE( transitivity1 , Stepper , dummy_steppers )
{
    typedef adaptive_time_iterator< Stepper , dummy_system > stepper_iterator;

    state_type x = {{ 1.0 }};
    stepper_iterator first1( Stepper() , dummy_system() , x , 1.5 , 0.1 , true );
    stepper_iterator last1( Stepper() , dummy_system() , x , 1.0 , 0.1 , false );
    stepper_iterator last2( Stepper() , dummy_system() , x , 1.0 , 0.1 , false );

    BOOST_CHECK( first1 == last1 );
    BOOST_CHECK( first1 == last2 );
    BOOST_CHECK( last1 == last2 );
}

// this test would fail
BOOST_AUTO_TEST_CASE_TEMPLATE( transitivity2 , Stepper , dummy_steppers )
{
    typedef adaptive_time_iterator< Stepper , dummy_system > stepper_iterator;
    // state_type x = {{ 1.0 }};
    // stepper_iterator first1( dummy_stepper() , dummy_system() , x , 1.5 , 0.1 , true );
    // stepper_iterator first2( dummy_stepper() , dummy_system() , x , 2.0 , 0.1 , false );
    // stepper_iterator last( dummy_stepper() , dummy_system() , x , 1.0 , 0.1 , false );
}


BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm , Stepper , dummy_steppers )
{
    typedef adaptive_time_iterator< Stepper , dummy_system > stepper_iterator;
    state_type x = {{ 1.0 }};
    result_vector res;
    stepper_iterator first( Stepper() , dummy_system() , x , 0.0 , 0.1 , true );
    stepper_iterator last( Stepper() , dummy_system() , x , 0.35 , 0.1 , false );

    std::copy( first , last , std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[0].second , 0.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].second , 0.1 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].second , 0.2 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].second , 0.3 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm_with_factory , Stepper , dummy_steppers )
{
    state_type x = {{ 1.0 }};
    result_vector res;
    std::copy( make_adaptive_time_iterator_begin( Stepper() , dummy_system() , x , 0.0 , 0.1 ) ,
               make_adaptive_time_iterator_end( Stepper() , dummy_system() , x , 0.35 , 0.1 ) ,
               std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[0].second , 0.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].second , 0.1 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].second , 0.2 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].second , 0.3 , 1.0e-13 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( copy_algorithm_with_range_factory , Stepper , dummy_steppers )
{
    state_type x = {{ 1.0 }};
    result_vector res;
    boost::range::copy( make_adaptive_time_range( Stepper() , dummy_system() , x , 0.0 , 0.35 , 0.1 ) ,
                        std::back_insert_iterator< result_vector >( res ) );

    BOOST_CHECK_EQUAL( res.size() , size_t( 4 ) );
    BOOST_CHECK_CLOSE( res[0].first[0] , 1.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[0].second , 0.0 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].first[0] , 1.25 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[1].second , 0.1 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].first[0] , 1.5 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[2].second , 0.2 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].first[0] , 1.75 , 1.0e-13 );
    BOOST_CHECK_CLOSE( res[3].second , 0.3 , 1.0e-13 );
}




BOOST_AUTO_TEST_SUITE_END()