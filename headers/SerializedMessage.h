#pragma once
#include<iostream>
#include<sstream>
#include<string>
using namespace std;

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

struct SerializedMessage
{
	void save(ostream &);
	void load(string);

	friend class boost::serialization::access;

	float paddleY{0};
	float paddle2Y{0};
	float ballX{0};
	float ballY{0};
	bool playerActive{false};
	int pointsClient{0};
	int pointsServer{0};

	template<class Archive>
	void serialize(Archive& , const unsigned int);

};