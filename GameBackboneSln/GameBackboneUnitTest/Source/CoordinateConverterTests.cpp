#include "stdafx.h"

#include <GameBackbone/Navigation/CoordinateConverter.h>
#include <GameBackbone/Navigation/NavigationTools.h>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

using namespace GB;

// Contains all of the tests for CoordinateConverter
BOOST_AUTO_TEST_SUITE(CoordinateConverter_Tests)

BOOST_AUTO_TEST_SUITE(CoordinateConverter_CTRs)

// Tests the default constructor to ensure there are no intrinsic memory leaks.
BOOST_AUTO_TEST_CASE(CoordinateConverter_Default_CTR){
	CoordinateConverter* TestConverter = new CoordinateConverter();
	
	delete TestConverter;
}

BOOST_AUTO_TEST_SUITE_END() //End CoordinateConverter_CTRs

BOOST_AUTO_TEST_SUITE(CoordinateConverter_Offsets)

// Tests the functions with a negative offset
BOOST_AUTO_TEST_CASE(CoordinateConverter_NegativeOffset) {
	CoordinateConverter TestConverter(50, sf::Vector2f{-1500, -1500});

	sf::Vector2i ngOrigin = {0, 0};
	sf::Vector2f sfOrigin = TestConverter.convertCoordToWindow(ngOrigin);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfOrigin.x, -1475.f, 0.001);
	BOOST_CHECK_CLOSE(sfOrigin.y, -1475.f, 0.001);
	sf::Vector2i originalOrigin = TestConverter.convertCoordToNavGrid(sfOrigin);
	// Ensure that the original coordinate is the same as the navigation grid coordinate
	BOOST_CHECK(originalOrigin == ngOrigin);

	sf::Vector2i ngPoint = {43, 12};
	sf::Vector2f sfPoint = TestConverter.convertCoordToWindow(ngPoint);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfPoint.x, 675.f, 0.001);
	BOOST_CHECK_CLOSE(sfPoint.y, -875.f, 0.001);
	sf::Vector2i originalPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensure that the original coordinate is the same as the navigation grid coordinate
	BOOST_CHECK(originalPoint == ngPoint);
}

// Tests the functions with a positive offset
BOOST_AUTO_TEST_CASE(CoordinateConverter_PositiveOffset) {
	CoordinateConverter TestConverter(50, sf::Vector2f{1500, 1500});

	sf::Vector2i ngOrigin = {0, 0};
	sf::Vector2f sfOrigin = TestConverter.convertCoordToWindow(ngOrigin);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfOrigin.x, 1525.f, 0.001);
	BOOST_CHECK_CLOSE(sfOrigin.y, 1525.f, 0.001);
	sf::Vector2i originalOrigin = TestConverter.convertCoordToNavGrid(sfOrigin);
	BOOST_CHECK(originalOrigin == ngOrigin);

	sf::Vector2i ngPoint = {43, 12};
	sf::Vector2f sfPoint = TestConverter.convertCoordToWindow(ngPoint);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfPoint.x, 3675.f, 0.001);
	BOOST_CHECK_CLOSE(sfPoint.y, 2125.f, 0.001);
	sf::Vector2i originalPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensure that the original coordinate is the same as the navigation grid coordinate
	BOOST_CHECK(originalPoint == ngPoint);
}

// Tests the functions with no offset
BOOST_AUTO_TEST_CASE(CoordinateConverter_ZeroOffset) {
	CoordinateConverter TestConverter(50, sf::Vector2f{0, 0});

	sf::Vector2i ngOrigin = {0, 0};
	sf::Vector2f sfOrigin = TestConverter.convertCoordToWindow(ngOrigin);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfOrigin.x, 25.f, 0.001);
	BOOST_CHECK_CLOSE(sfOrigin.y, 25.f, 0.001);
	sf::Vector2i originalOrigin = TestConverter.convertCoordToNavGrid(sfOrigin);
	// Ensure that the original coordinate is the same as the navigation grid coordinate
	BOOST_CHECK(originalOrigin == ngOrigin);

	sf::Vector2i ngPoint = {43, 12};
	sf::Vector2f sfPoint = TestConverter.convertCoordToWindow(ngPoint);
	// Ensure that the sf coordinate is correct
	BOOST_CHECK_CLOSE(sfPoint.x, 2175.f, 0.001);
	BOOST_CHECK_CLOSE(sfPoint.y, 625.f, 0.001);
	sf::Vector2i originalPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensure that the original coordinate is the same as the navigation grid coordinate
	BOOST_CHECK(originalPoint == ngPoint);
}

BOOST_AUTO_TEST_SUITE_END() // end CoordinateConverter_Offsets

BOOST_AUTO_TEST_SUITE(CoordinateConverter_Locations)

// Checks that the edge cases act as we expect
BOOST_AUTO_TEST_CASE(CoordinateConverter_BottomRightEdge) {
	CoordinateConverter TestConverter(50, sf::Vector2f{0, 0});

	sf::Vector2f sfPoint = {49.999f, 49.999f};
	sf::Vector2i ngPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensures that the navigation grid point is contained in the first square
	BOOST_CHECK(ngPoint.x == 0 && ngPoint.y == 0);

	sfPoint = {50, 50};
	ngPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensures that the navigation grid point is contained in the second square
	BOOST_CHECK(ngPoint.x == 1 && ngPoint.y == 1);
}

// Checks that our general case works as we expect
BOOST_AUTO_TEST_CASE(CoordinateConverter_MidPoint) {
	CoordinateConverter TestConverter(50, sf::Vector2f{0, 0});

	sf::Vector2f sfPoint = {25, 25};
	sf::Vector2i ngPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensures that the navigation grid point is contained in the first square
	BOOST_CHECK(ngPoint.x == 0 && ngPoint.y == 0);

	sfPoint = {75, 75};
	ngPoint = TestConverter.convertCoordToNavGrid(sfPoint);
	// Ensures that the navigation grid point is contained in the second square
	BOOST_CHECK(ngPoint.x == 1 && ngPoint.y == 1);
}

BOOST_AUTO_TEST_SUITE_END() // end CoordinateConverter_Locations


BOOST_AUTO_TEST_SUITE(CoordinateConverter_Paths)

// ensure accuracy when converting from NavGrid paths to window paths
BOOST_AUTO_TEST_CASE(CoordinateConverter_NavGrid_Path_To_Window_Path) {
	// init values
	const float SQUARE_WIDTH = 6;
	const sf::Vector2f ORIGIN_POINT = {0,0};
	CoordinateConverter converter(SQUARE_WIDTH, ORIGIN_POINT);
	NavGridCoordinatePath navGridPath = { {0,0}, {0,1}, {1,1} };
	WindowCoordinatePath handConvertedWindowPath = { {3,3}, {3, 9}, {9,9} };

	// convert path
	WindowCoordinatePath convertedPath = converter.convertPathToWindow(navGridPath);

	// ensure that the generated path matches the hand converted path
	for (unsigned int  i = 0; i < handConvertedWindowPath.size(); i++) {
		BOOST_CHECK(handConvertedWindowPath.front().x == convertedPath.front().x
			&& handConvertedWindowPath.front().y == convertedPath.front().y);
		handConvertedWindowPath.pop_front();
		convertedPath.pop_front();
	}
}

// ensure accuracy when converting from window paths to NavGrid paths  
BOOST_AUTO_TEST_CASE(CoordinateConverter_Window_Path_To_NavGrid_Path) {
	// init values
	const float SQUARE_WIDTH = 6;
	const sf::Vector2f ORIGIN_POINT = { 0,0 };
	CoordinateConverter converter(SQUARE_WIDTH, ORIGIN_POINT);
	NavGridCoordinatePath handConvertedNavGridPath = { { 0,0 },{ 0,1 },{ 1,1 } };
	WindowCoordinatePath windowPath = { { 3,3 },{ 3, 9 },{ 9,9 } };

	// convert path
	NavGridCoordinatePath convertedPath = converter.convertPathToNavGrid(windowPath);

	// ensure that the generated path matches the hand converted path
	for (unsigned int i = 0; i < windowPath.size(); i++) {
		BOOST_CHECK(handConvertedNavGridPath.front().x == convertedPath.front().x
			&& handConvertedNavGridPath.front().y == convertedPath.front().y);
		handConvertedNavGridPath.pop_front();
		convertedPath.pop_front();
	}
}

BOOST_AUTO_TEST_SUITE_END() // end CoordinateConverter_Paths

// Keep at end of file
BOOST_AUTO_TEST_SUITE_END()