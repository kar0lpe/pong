#include "SerializedMessage.h"

template<class Archive>

void SerializedMessage::serialize(Archive& archive, const unsigned int version)
{
	archive & paddleY;
	archive & paddle2Y;
	archive & ballX;
	archive & ballY;
	archive & playerActive;	
	archive & pointsClient;
	archive & pointsServer;		
}

void SerializedMessage::save(ostream &oss)
{
	boost::archive::binary_oarchive oa(oss);
	oa & *(this);
}

void SerializedMessage::load(string str_data)
{
	std::istringstream iss(str_data);
	boost::archive::binary_iarchive ia(iss);
	ia & *(this);
}
