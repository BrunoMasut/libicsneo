#include "icsneo/communication/packet/canpacket.h"

using namespace icsneo;

std::shared_ptr<CANMessage> HardwareCANPacket::DecodeToMessage(const std::vector<uint8_t>& bytestream) {
	const HardwareCANPacket* data = (const HardwareCANPacket*)bytestream.data();

	auto msg = std::make_shared<CANMessage>();

	// Arb ID
	if(data->header.IDE) { // Extended 29-bit ID
		msg->arbid = (data->header.SID & 0x7ff) << 18;
		msg->arbid |= (data->eid.EID & 0xfff) << 6;
		msg->arbid |= (data->dlc.EID2 & 0x3f);
		msg->isExtended = true;
	} else { // Standard 11-bit ID
		msg->arbid = data->header.SID;
	}

	// DLC
	uint8_t length = data->dlc.DLC;
	msg->dlcOnWire = length; // This will hold the real DLC on wire 0x0 - 0xF
	if(data->header.EDL && data->timestamp.IsExtended) { // CAN FD
		msg->isCANFD = true;
		msg->baudrateSwitch = data->header.BRS; // CAN FD Baudrate Switch
		if(length > 8) {
			switch(length) { // CAN FD Length Decoding
				case 0x9:
					length = 12;
					break;
				case 0xa:
					length = 16;
					break;
				case 0xb:
					length = 20;
					break;
				case 0xc:
					length = 24;
					break;
				case 0xd:
					length = 32;
					break;
				case 0xe:
					length = 48;
					break;
				case 0xf:
					length = 64;
					break;
				default:
					return nullptr;
			}
		}
	} else if(length > 8) { // This is a standard CAN frame with a length of more than 8
		// Yes, this is possible. On the wire, the length field is a nibble, and we do want to return an accurate value
		// We don't want to overread our buffer, though, so make sure we cap the length
		length = 8;
	}
	
	// Data
	// The first 8 bytes are always in the standard place
	if((data->dlc.RTR && data->header.IDE) || (!data->header.IDE && data->header.SRR)) { // Remote Request Frame
		msg->data.resize(length); // This data will be all zeros, but the length will be set
		msg->isRemote = true;
	} else {
		msg->data.reserve(length);
		msg->data.insert(msg->data.end(), data->data, data->data + (length > 8 ? 8 : length));
		if(length > 8) { // If there are more than 8 bytes, they come at the end of the message
			// Messages with extra data are formatted as message, then uint16_t netid, then uint16_t length, then extra data
			const auto extraDataStart = bytestream.begin() + sizeof(HardwareCANPacket) + 2 + 2;
			msg->data.insert(msg->data.end(), extraDataStart, extraDataStart + (length - 8));
		}
	}

	return msg;
}

bool HardwareCANPacket::EncodeFromMessage(const CANMessage& message, std::vector<uint8_t>& result) {
	if(message.isCANFD && message.isRemote)
		return false; // RTR frames can not be used with CAN FD

	const size_t dataSize = message.data.size();
	if(dataSize > 64 || (dataSize > 8 && !message.isCANFD))
		return false; // Too much data for the protocol

	uint8_t lengthNibble = uint8_t(message.data.size());
	if(lengthNibble > 8) {
		switch(lengthNibble) {
			case 12:
				lengthNibble = 0x9;
				break;
			case 16:
				lengthNibble = 0xA;
				break;
			case 20:
				lengthNibble = 0xB;
				break;
			case 24:
				lengthNibble = 0xC;
				break;
			case 32:
				lengthNibble = 0xD;
				break;
			case 48:
				lengthNibble = 0xE;
				break;
			case 64:
				lengthNibble = 0xF;
				break;
			default:
				return false; // CAN FD frame may have had an incorrect byte count
		}
	}

	// Pre-allocate as much memory as we will possibly need for speed
	result.reserve(17 + dataSize);

	result.push_back(0 /* byte count here later */ << 4 | (uint8_t(message.network.getNetID()) & 0xF));

	// Two bytes for Description ID, big endian
	result.insert(result.end(), { uint8_t(message.description >> 8), uint8_t(message.description) });

	// Next 2-4 bytes are ArbID
	if(message.isExtended) {
		if(message.arbid >= 0x20000000) // Extended messages use 29-bit arb IDs
			return false;

		result.insert(result.end(), {
			(uint8_t)(message.arbid >> 21),
			(uint8_t)(((((message.arbid & 0x001C0000) >> 13) & 0xFF) + (((message.arbid & 0x00030000) >> 16) & 0xFF)) | 8),
			(uint8_t)(message.arbid >> 8),
			(uint8_t)message.arbid
		});
	} else {
		if(message.arbid >= 0x800) // Standard messages use 11-bit arb IDs
			return false;

		result.insert(result.end(), {
			(uint8_t)(message.arbid >> 3),
			(uint8_t)((message.arbid & 0x7) << 5)
		});
	}

	// Status and DLC bits
	if(message.isCANFD) {
		result.push_back(0x0F); // FD Frame
		uint8_t fdStatusByte = lengthNibble;
		if(message.baudrateSwitch)
			fdStatusByte |= 0x80; // BRS status bit
		result.push_back(fdStatusByte);
	} else {
		// TODO Support high voltage wakeup, bitwise-or in 0x8 here to enable
		uint8_t statusNibble = message.isRemote ? 0x4 : 0x0;
		result.push_back((statusNibble << 4) | lengthNibble);
	}

	// Now finally the payload
	result.insert(result.end(), message.data.begin(), message.data.end());
	result.push_back(0);

	// Fill in the length byte from earlier
	result[0] |= result.size() << 4;

	return true;
}