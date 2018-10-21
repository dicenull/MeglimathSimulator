#include "RatioClient.h"

int RC::movedByway(int way) {
	if (way == 8) {
		return -1;
	}
	if (way <= 3) {
		return way + 4;
	}
	else {
		return way - 4;
	}

	//switch (way) {
	//case 0:
	//	return 4;
	//case 1:
	//	return 5;
	//case 2:
	//	return 6;
	//case 3:
	//	return 7;
	//case 4:
	//	return 0;
	//case 5:
	//	return 1;
	//case 6:
	//	return 2;
	//case 7:
	//	return 3;

	//}
}

RatioClient::RatioClient(TeamType type) : Client(type) {
	_is_ready = true;
}

RatioClient::~RatioClient() {

}