
#ifndef _ARDUINOFLOWSOCKET_H_
#define _ARDUINOFLOWSOCKET_H_

#include <FlowSerial.hpp>

#define READERROR 0x01

namespace FlowSerial{
	/**
	 * @brief      FlowSerial implementation for use of the Arduino Serial
	 *             interface.
	 * @details    Since this is used for the Serial interface a FlowSerial
	 *             device can communicate to Arduino using various ways
	 *             including XBee shield, Ethernet shield, USB etc.
	 */
	class SerialSocket : public BaseSocket{
	public:
		/**
		 * @brief      Constructor for this class.
		 *
		 * @param      iflowRegister    Pointer to array that is being used to
		 *                              store received data.
		 * @param      iregisterLength  Length of array that was given.
		 */
		SerialSocket(uint8_t* iflowRegister, size_t iregisterLength);
		/**
		 * @brief      Reads from peer address. This has a timeout functionality
		 *             of 500 ms. It will try three time before throwing an
		 *             exception
		 *
		 * @param[in]  startAddress  The start address where to begin to read
		 *                           from other peer
		 * @param      returnData    Array will be filled with requested data.
		 * @param[in]  size          The required bytes that needs to be read
		 *                           starting from startAddress. Must not be
		 *                           more that the actual size of returnData
		 */
		void read(uint8_t startAddress, uint8_t returnData[], size_t size);
		/**
		 * @brief      Same as FlowSerial::SerialSocket::update(0);
		 *
		 * @return     true if a message is received.
		 */
		bool update();
		/**
		 * @brief      Checks input stream for available messages. Will block if
		 *             nothing is received. It is advised to use a thread for
		 *             this.
		 *
		 * @param[in]  timeoutMs  The timeout in milliseconds
		 *
		 * @return     True is message is received
		 */
		bool update(uint16_t timeoutMs);
		uint8_t getError(){return errorMask;}
		void clearError(){errorMask = 0;}
	private:
		virtual void writeToInterface(const uint8_t data[], size_t arraySize);
		uint8_t inputData[20];
		uint8_t errorMask = 0;
	};
}

#endif
