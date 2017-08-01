# FlowSerialArduinoSerial
Implementation of the FlowSerial library for the Arduino Serial interface. 
See https://github.com/overlord1123/FlowSerial for more details about the protocol.

When installed FlowSerial can be used to share a array with another party that also has FlowSerial installed.

# Install
Place the source code in a folder with the same name as the library. Then add this folder to the Arduino library folder. 
This folder can be found by opening the Arduino IDE and goto File->preferences->Sketchbook location.
In this folder a folder can be found called libraries. This is the Arduino library folder.

# Example code
```C++
#include <FlowSerialArduinoSerial.h>

static const int ledPin1 = 13;
static const int ledPin2 = 7;
static uint8_t flowReg[20];

// Here a FlowSerial socket is created.
static FlowSerial::SerialSocket test(flowReg, sizeof(flowReg));

static void toDoWhileWaiting();


void setup(){
	// The Serial interface needs to be setup.
	Serial.begin(115200);
	// Make two output pins
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	// Assign some numbers to the FlowSerial Array for testing.
	for (uint16_t i = 0; i < sizeof(flowReg) / sizeof(*flowReg); ++i)
	{
		flowReg[i] = i;
	}
	flowReg[2] = 20;
} 

void loop(){
	static uint8_t toBeWritten = 32;
	static unsigned long loopTimer = 0;
	static const long interval = 1000;
	//Just reset the loop timer
	loopTimer = millis();
	// Wait for interval milliseconds. Check FlowSerial while waiting.
	do{
		toDoWhileWaiting();
	}while(millis() - loopTimer < interval);
	//Step 3
	toBeWritten ^= 1;

	// Preform a write operation to FlowSerial peer. Writes toBeWritten into
	// address 0. The third arguments states it is a single byte.
	test.write(0, &toBeWritten, 1);
	//Step 4
	uint8_t returnedData;

	// Preform a read operation of address 0. Put data into returnedData. The
	// third arguments states it is a single byte and thus states that a single
	// byte is requested from peer.
	test.read(0, &returnedData, 1);

	// Check if the write and read byte correspond. If so then set led 2 on.
	if(returnedData == toBeWritten){
		digitalWrite(ledPin2, HIGH);
	}
	else{
		digitalWrite(ledPin2, LOW);
	}
	// Ignore any possible remaining data. If there is any data in buffer it may
	// be because the port is been overflowed.
	test.clearReturnedData();
}

static void toDoWhileWaiting(){
	//Step 1
	test.update();
	//Step 2
	if(flowReg[2] == 1){
		digitalWrite(ledPin1, HIGH);
	}
	else{
		digitalWrite(ledPin1, LOW);
	}
}
```
