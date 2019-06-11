#ifndef __IDEVICESETTINGS_H_
#define __IDEVICESETTINGS_H_

#include <stdint.h>

#pragma pack(push, 2)

/* SetBaudrate in CAN_SETTINGS */
enum
{
	AUTO,
	USE_TQ
};

/* Baudrate in CAN_SETTINGS/CANFD_SETTINGS */
enum CANBaudrate
{
	BPS20,
	BPS33,
	BPS50,
	BPS62,
	BPS83,
	BPS100,
	BPS125,
	BPS250,
	BPS500,
	BPS800,
	BPS1000,
	BPS666,
	BPS2000,
	BPS4000,
	CAN_BPS5000,
	CAN_BPS6667,
	CAN_BPS8000,
	CAN_BPS10000,
};

/* Mode in CAN_SETTINGS */
enum
{
	NORMAL = 0,
	DISABLE = 1,
	LOOPBACK = 2,
	LISTEN_ONLY = 3,
	LISTEN_ALL = 7
};

typedef struct
{
	uint8_t Mode;
	uint8_t SetBaudrate;
	uint8_t Baudrate;
	uint8_t transceiver_mode;
	uint8_t TqSeg1;
	uint8_t TqSeg2;
	uint8_t TqProp;
	uint8_t TqSync;
	uint16_t BRP;
	uint8_t auto_baud;
	uint8_t innerFrameDelay25us;
} CAN_SETTINGS;
#define CAN_SETTINGS_SIZE 12
static_assert(sizeof(CAN_SETTINGS) == CAN_SETTINGS_SIZE, "CAN_SETTINGS is the wrong size!");

/* FDMode in CANFD_SETTINGS */
enum
{
	NO_CANFD,
	CANFD_ENABLED,
	CANFD_BRS_ENABLED,
	CANFD_ENABLED_ISO,
	CANFD_BRS_ENABLED_ISO
};

typedef struct _CANFD_SETTINGS
{
	uint8_t FDMode; /* mode, secondary baudrate for canfd */
	uint8_t FDBaudrate;
	uint8_t FDTqSeg1;
	uint8_t FDTqSeg2;
	uint8_t FDTqProp;
	uint8_t FDTqSync;
	uint16_t FDBRP;
	uint8_t FDTDC;
	uint8_t reserved;
} CANFD_SETTINGS;
#define CANFD_SETTINGS_SIZE 10

typedef struct ETHERNET_SETTINGS_t
{
	uint8_t duplex; /* 0 = half, 1 = full */
	uint8_t link_speed;
	uint8_t auto_neg;
	uint8_t led_mode;
	uint8_t rsvd[4];
} ETHERNET_SETTINGS;
#define ETHERNET_SETTINGS_SIZE 8

typedef struct OP_ETH_GENERAL_SETTINGS_t
{
	uint8_t ucInterfaceType;
	uint8_t reserved0[3];
	uint16_t tapPair0;
	uint16_t tapPair1;
	uint16_t tapPair2;
	uint16_t tapPair3;
	uint16_t tapPair4;
	uint16_t tapPair5;
	union {
		struct
		{
			unsigned bTapEnSwitch : 1;
			unsigned bTapEnPtp : 1;
			unsigned bEnReportLinkQuality : 1;
		} flags;
		unsigned uFlags;
	};
} OP_ETH_GENERAL_SETTINGS;
#define OP_ETH_GENERAL_SETTINGS_SIZE 20
static_assert(sizeof(OP_ETH_GENERAL_SETTINGS) == OP_ETH_GENERAL_SETTINGS_SIZE, "OP_ETH_GENERAL_SETTINGS is the wrong size!");

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // nameless struct/union
#endif
typedef struct OP_ETH_SETTINGS_t
{
	uint8_t ucConfigMode;
	unsigned char preemption_en;
	union {
		struct {
			// Reuse the mac_addr for switch mode if required!
			unsigned char mac_addr1[6];// Original Addr for spoofing
			unsigned char mac_addr2[6];// Target Addr for spoofing
			unsigned short mac_spoofing_en : 1;
			unsigned short mac_spoofing_isDstOrSrc : 1;
			unsigned short reserved : 14;
		};
		unsigned char reserved0[14];
	};
} OP_ETH_SETTINGS;
#define OP_ETH_SETTINGS_SIZE 16
static_assert(sizeof(OP_ETH_SETTINGS) == OP_ETH_SETTINGS_SIZE, "OP_ETH_SETTINGS is the wrong size!");
#ifdef _MSC_VER
#pragma warning(pop)
#endif

typedef struct
{
	uint8_t MasterEnable;
	uint8_t SlaveEnable;
	uint8_t MasterNetwork;
	uint8_t SlaveNetwork;
} TIMESYNC_ICSHARDWARE_SETTINGS;
#define TIMESYNC_ICSHARDWARE_SETTINGS_SIZE 4

typedef struct _STextAPISettings
{
	uint32_t can1_tx_id;
	uint32_t can1_rx_id;
	union {
		struct sCAN1Options
		{
			unsigned bExtended : 1;
			unsigned : 15;
		};
		uint32_t DWord;
	} can1_options;
	uint32_t can2_tx_id;
	uint32_t can2_rx_id;
	union {
		struct sCAN2Options
		{
			unsigned bExtended : 1;
			unsigned : 15;
		};
		uint32_t DWord;
	} can2_options;

	uint32_t network_enables;

	uint32_t can3_tx_id;
	uint32_t can3_rx_id;
	union {
		struct sCAN3Options
		{
			unsigned bExtended : 1;
			unsigned : 15;
		};
		uint32_t DWord;
	} can3_options;

	uint32_t can4_tx_id;
	uint32_t can4_rx_id;
	union {
		struct sCAN4Options
		{
			unsigned bExtended : 1;
			unsigned : 15;
		};
		uint32_t DWord;
	} can4_options;

	uint32_t reserved[5];

} STextAPISettings;
#define STextAPISettings_SIZE 72

/* high_speed_auto_switch in SWCAN_SETTINGS */
enum
{
	SWCAN_AUTOSWITCH_DISABLED,
	SWCAN_AUTOSWITCH_NO_RESISTOR,
	SWCAN_AUTOSWITCH_WITH_RESISTOR,
	SWCAN_AUTOSWITCH_DISABLED_RESISTOR_ENABLED
};

typedef struct
{
	uint8_t Mode;
	uint8_t SetBaudrate;
	uint8_t Baudrate;
	uint8_t transceiver_mode;
	uint8_t TqSeg1;
	uint8_t TqSeg2;
	uint8_t TqProp;
	uint8_t TqSync;
	uint16_t BRP;
	uint16_t high_speed_auto_switch;
	uint8_t auto_baud;
	uint8_t RESERVED;
} SWCAN_SETTINGS;
#define SWCAN_SETTINGS_SIZE 14

/* Baudrate in LIN_SETTINGS / ISO9141_KEYWORD2000_SETTINGS / UART_SETTINGS */
enum
{
	BPS5000,
	BPS10400,
	BPS33333,
	BPS50000,
	BPS62500,
	BPS71429,
	BPS83333,
	BPS100000,
	BPS117647
};

/* MasterResistor in LIN_SETTINGS */
enum
{
	RESISTOR_ON,
	RESISTOR_OFF
};

/* Mode in LIN_SETTINGS */
enum
{
	SLEEP_MODE,
	SLOW_MODE,
	NORMAL_MODE,
	FAST_MODE
};

typedef struct _LIN_SETTINGS
{
	uint32_t Baudrate; /* New products since FIREVNETEP should rely on this only */
	uint16_t spbrg; /* Precompiled to be 40Mhz/Baudrate/16 - 1.  Only used in neoVI FIRE/FIREVNET(4dw) */
	uint8_t brgh; /* Must be zero */
	uint8_t numBitsDelay;
	uint8_t MasterResistor;
	uint8_t Mode;
} LIN_SETTINGS;
#define LIN_SETTINGS_SIZE 10

typedef struct
{
	uint16_t time_500us;
	uint16_t k;
	uint16_t l;
} ISO9141_KEYWORD2000__INIT_STEP;
#define ISO9141_KEYWORD2000__INIT_STEP_SIZE 6

typedef struct
{
	uint32_t Baudrate;
	uint16_t spbrg;
	uint16_t brgh;
	ISO9141_KEYWORD2000__INIT_STEP init_steps[16];
	uint8_t init_step_count;
	uint16_t p2_500us;
	uint16_t p3_500us;
	uint16_t p4_500us;
	uint16_t chksum_enabled;
} ISO9141_KEYWORD2000_SETTINGS;
#define ISO9141_KEYWORD2000_SETTINGS_SIZE 114
static_assert(sizeof(ISO9141_KEYWORD2000_SETTINGS) == ISO9141_KEYWORD2000_SETTINGS_SIZE, "ISO9141_KEYWORD2000_SETTINGS is the wrong size!");

typedef struct _UART_SETTINGS
{
	uint16_t Baudrate;
	uint16_t spbrg;
	uint16_t brgh;
	uint16_t parity;
	uint16_t stop_bits;
	uint8_t flow_control; /* 0- off, 1 - Simple CTS RTS */
	uint8_t reserved_1;
	union {
		uint32_t bOptions;
		struct {
			uint32_t invert_tx : 1;
			uint32_t invert_rx : 1;
			uint32_t half_duplex : 1;
			uint32_t reserved_bits : 13;
			uint32_t reserved_bits2 : 16;
		} sOptions;
	};
} UART_SETTINGS;
#define UART_SETTINGS_SIZE 16
static_assert(sizeof(UART_SETTINGS) == UART_SETTINGS_SIZE, "UART_SETTINGS is the wrong size!");

#pragma pack(pop)

#ifdef __cplusplus
#include "icsneo/communication/communication.h"
#include <iostream>

namespace icsneo {

class IDeviceSettings {
public:
	static constexpr uint16_t GS_VERSION = 5;
	static uint16_t CalculateGSChecksum(const std::vector<uint8_t>& settings);
	static CANBaudrate GetEnumValueForBaudrate(int64_t baudrate);
	static int64_t GetBaudrateValueForEnum(CANBaudrate enumValue);

	IDeviceSettings(std::shared_ptr<Communication> com, size_t size) : com(com), err(com->err), structSize(size) {}
	virtual ~IDeviceSettings() {}
	bool ok() { return !disabled && settingsLoaded; }
	
	bool refresh(bool ignoreChecksum = false); // Get from device

	// Send to device, if temporary device keeps settings in volatile RAM until power cycle, otherwise saved to EEPROM
	bool apply(bool temporary = false);
	bool applyDefaults(bool temporary = false);

	virtual int64_t getBaudrateFor(Network net) const;
	virtual bool setBaudrateFor(Network net, int64_t baudrate);

	virtual int64_t getFDBaudrateFor(Network net) const;
	virtual bool setFDBaudrateFor(Network net, int64_t baudrate);

	virtual const CAN_SETTINGS* getCANSettingsFor(Network net) const { (void)net; return nullptr; }
	CAN_SETTINGS* getMutableCANSettingsFor(Network net) {
		if(disabled || readonly)
			return nullptr;
		const uint8_t* offset = (const uint8_t*)getCANSettingsFor(net);
		if(offset == nullptr)
			return nullptr;
		return static_cast<CAN_SETTINGS*>((void*)(settings.data() + (offset - settingsInDeviceRAM.data())));
	}

	virtual const CANFD_SETTINGS* getCANFDSettingsFor(Network net) const { (void)net; return nullptr; }
	CANFD_SETTINGS* getMutableCANFDSettingsFor(Network net) {
		if(disabled || readonly)
			return nullptr;
		const uint8_t* offset = (const uint8_t*)getCANFDSettingsFor(net);
		if(offset == nullptr)
			return nullptr;
		return static_cast<CANFD_SETTINGS*>((void*)(settings.data() + (offset - settingsInDeviceRAM.data())));
	}

	virtual const CAN_SETTINGS* getLSFTCANSettingsFor(Network net) const { (void)net; return nullptr; }
	CAN_SETTINGS* getMutableLSFTCANSettingsFor(Network net) {
		if(disabled || readonly)
			return nullptr;
		const uint8_t* offset = (const uint8_t*)getLSFTCANSettingsFor(net);
		if(offset == nullptr)
			return nullptr;
		return static_cast<CAN_SETTINGS*>((void*)(settings.data() + (offset - settingsInDeviceRAM.data())));
	}

	virtual const SWCAN_SETTINGS* getSWCANSettingsFor(Network net) const { (void)net; return nullptr; }
	SWCAN_SETTINGS* getMutableSWCANSettingsFor(Network net) {
		if(disabled || readonly)
			return nullptr;
		const uint8_t* offset = (const uint8_t*)getSWCANSettingsFor(net);
		if(offset == nullptr)
			return nullptr;
		return static_cast<SWCAN_SETTINGS*>((void*)(settings.data() + (offset - settingsInDeviceRAM.data())));
	}

	const void* getRawStructurePointer() const { return settingsInDeviceRAM.data(); }
	void* getMutableRawStructurePointer() { return settings.data(); }
	template<typename T> const T* getStructurePointer() const { return static_cast<const T*>(getRawStructurePointer()); }
	template<typename T> T* getMutableStructurePointer() { return static_cast<T*>(getMutableRawStructurePointer()); }
	template<typename T> T getStructure() const { return *getStructurePointer<T>(); }
	template<typename T> bool applyStructure(const T& newStructure);

	const size_t& getSize() const { return structSize; }

	// if settings are disabled for this device. always false unless constructed null
	bool disabled = false;
	
	bool readonly = false;
	bool disableGSChecksumming = false;
protected:
	std::shared_ptr<Communication> com;
	device_errorhandler_t err;
	size_t structSize;

	// if we hold any local copies of the device settings
	bool settingsLoaded = false;

	std::vector<uint8_t> settings; // For writing settings to, calling apply() should copy over to device RAM (and EEPROM)
	std::vector<uint8_t> settingsInDeviceRAM; // For reading settings from

	// Parameter createInoperableSettings exists because it is serving as a warning that you probably don't want to do this
	typedef void* warn_t;
	IDeviceSettings(warn_t createInoperableSettings, std::shared_ptr<Communication> com)
		: disabled(true), readonly(true), err(com->err), structSize(0) { (void)createInoperableSettings; }
};

}

#endif // __cplusplus

#endif