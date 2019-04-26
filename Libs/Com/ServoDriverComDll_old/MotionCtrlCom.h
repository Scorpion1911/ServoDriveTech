#pragma once
#include "Basetype_def.h"
#include "RingNetInterface.h"
#include "RnDriverPlot.h"
#include "RnMotionAxiMapping.h"

#ifndef RING_NET_MOTION_RESOURCE

#define RING_NET_MOTION_RESOURCE
typedef struct
{
	short flag;
	short len;
	short checksum;
	short data[32];
} TPci;

typedef struct
{
	short dataLen;
	short data[32];

	short resultLen;
	short result[32];
	short resultIndex;
}TCommand;


#define MAX_DEBUG_CMD_SIZE (256)
#define MAX_DEBUG_CMD_DATA_SIZE (252)

typedef struct  
{
	uint16 cmd;
	uint16 sLen;
	uint16 param;
	uint16 checksum;
	uint16 pData[MAX_DEBUG_CMD_DATA_SIZE];
}StUSER_PROTOCOL_DEBUG_CMD;

#endif

class CMotionCtrlCom
{
public:
	CMotionCtrlCom();
	~CMotionCtrlCom();
protected:
	CRingNetInterface * m_pDriver;
	CRnDriverPlot*		m_pPlot;
	CRnMotionAxiMapping*	m_pMapping;
	enum ENUM_PCI_STS
	{
		PCI_IDLE = 0,
		PCI_PC_BUSY	= 1,
		PCI_PC_FINISH = 2,
		PCI_DSP_BUSY = 3,
		PCI_DSP_FINISH = 4
	};


public:
	short Initial(CRingNetInterface* pDriver);
	short PostPci(TPci* gPci, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
	short SendPci(TPci* gPci, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);

	short PostPcie(TPci* gPci, Uint8 core_index, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
	short SendPcie(TPci* gPci, Uint8 core_index, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);

	//////////////////////////////////////////////////////////////////////////
	short StartPlot(int16 wave_num, Uint16 *wave_byte_size_list, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
	short StopPlot(Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
//	long GetWaveBufferSize(Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
//	short GetWaveData(double** data, uint32& number, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);
	short GetWaveBufferData(char* data, uint32& number, Uint8 station_id = 0xf0, Uint8 ch_id = RN_DSP_CH_ID);

	//////////////////////////////////////////////////////////////////////////

	virtual short GetComProtocolType(){ return m_pDriver->GetComProtocolType(); };
protected:
	short CheckUserProtocol(StUSER_PROTOCOL_DEBUG_CMD* cmd);
	short CalcUserProtocol(StUSER_PROTOCOL_DEBUG_CMD* cmd);
public:
	short SendUserDebugCmd(StUSER_PROTOCOL_DEBUG_CMD* cmd, Uint8 station_id);

};

