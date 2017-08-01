
#include "FlowSerialArduinoSerial.h"
#include "Arduino.h"

namespace FlowSerial{

	SerialSocket::SerialSocket(uint8_t* iflowRegister, size_t iregisterLength, uint32_t buadRate)
		:BaseSocket(iflowRegister, iregisterLength)
	{
		Serial.begin(buadRate);
	}

	bool SerialSocket::update(){
		return update(0);
	}

	bool SerialSocket::update(uint16_t timeoutMs){
		// Default is to return zero meaning nothing has been received yet.
		bool ret = false;
		uint16_t currentTime = millis();
		if(timeoutMs == 0){
			int readNow;
			while((readNow = Serial.available()) != 0){
				Serial.readBytes(inputData, readNow);
				ret |= handleData(inputData, readNow);
			}	
		}
		else{
			do{
				int readNow;
				while((readNow = Serial.available()) != 0){
					Serial.readBytes(inputData, readNow);
					ret |= handleData(inputData, readNow);
				}
			}while(ret = false && ((millis() & 0xFFFF) - currentTime) < timeoutMs);
		}
		return ret;
	}

	void SerialSocket::writeToInterface(const uint8_t data[], size_t arraySize){
		Serial.write(data, arraySize);
	}

	void SerialSocket::read(uint8_t startAddress, uint8_t returnData[], size_t size){
		uint8_t trials = 0;
		do{
			clearReturnedData();
			sendReadRequest(startAddress, size);
			if(trials++ > 10){
				errorMask |= READERROR;
				return;
			}
		}while(update(500) == false);
		getReturnedData(returnData, size);
	}
}
