//////////////////////////////////////////////////////////////////////////////////////////
//	summary				:	Communicaiton cmd layer Define		 						//
//	file				:	ServoDriverComDll.cpp										//
//	Description			:	use for cmd define											//
//	lib					:	none														//
//																						//
//======================================================================================//
//		programmer:		|	date:		|	Corporation:	|		copyright(C):		//
//-------------------------------------------------------------------------------------//
//		wang.bin(1420)  |	2016/1/20	|	googoltech		|		2016 - 2019			//
//--------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <conio.h>
#include "ST_GTSD_Cmd.h"
#include "BaseReturn_def.h"
#include "PlotWave.h"
#include "ParseHex.h"
#include "FirmwareDl.h"
#include "hex2ldr.h"
#include "serialport.h"
#include "eepromOpt.h"
#include "ServoDriverComDll.h"
#include "RnDriverPlot.h"
#include "RingNetInterface.h"
#include "ServoDriverCom.h"
#include "Eeprom.h"

//Ϊ�˻�ȡ�������ӵ�״̬�����ʣ���Ϊ�Ȼ�����֧�ְ��ף�����ֻ֧��ǧ�ף�����ǰ��ף�
#include <winsock2.h>
#include <ws2tcpip.h>

wstring NetCardName = L"";
wstring NetCardNum = L"";

extern HANDLE GTSD_Event;

CRnDriverPlot* g_RnDrivePlot = NULL;
CServoDriverCom* g_RnServoCom = NULL;
CRingNetInterface* g_RnInterface = NULL;
extern CAbsCom* g_AbsCom;
extern CEeprom* g_Eeprom;



int16 TryLock(void)
{
	if (WaitForSingleObject(GTSD_Event, 1000) == WAIT_OBJECT_0)
	{
		return Rt_Success;
	}

	return Net_Rt_Lock_Err;
}

int16 Unlock(int16 error)
{
	SetEvent(GTSD_Event);

	return error;
}
//////////////////////////////////////////////////////////////////////////
int16 GTSD_Convert_axi(int16& axis)
{
	int16 station_id = g_RnServoCom->ConvertAxiToStationId(axis);
//	int16 station_id = 0xF0;// (axis >> 1) + 1;
	switch (station_id & 0xFF)
	{
	case RN_DSP_CH_ID: axis = (axis & 0x1);
		break;
	case RN_PCI_CH_ID: axis = (axis & 0x1) + 2;
		break;
	case RN_EXT_CH_ID: axis = (axis & 0x1) + 4;
		break;
	default:
		axis = 0;
		station_id = (int16)0xF200;
		break;
	} 
	return (station_id>>8) & 0xFF;
};
//////////////////////////////////////////////////////////////////////////
//���̱߳���
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Open(void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16 com_type)
{
	int16 progress;
	void* ptr = ptrv;
	//�ٷֱȽ���
	progress = 0;
	if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);
	////////////////////////if use the ring net way//////////////////////////////////////////////////
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		short rtn;
		if (Net_Rt_Lock_Err == TryLock())
		{
			return Net_Rt_Lock_Err;
		}
		if (g_RnInterface == NULL)
		{
			g_RnInterface = new CRingNetInterface;
			if (g_RnInterface == NULL)
			{
				return Unlock(RTN_MALLOC_FAIL);
			}
			rtn = g_RnInterface->RnNetCom_Open(tpfUpdataProgressPt, ptrv, progress);
			if (rtn != RTN_SUCCESS)
			{
				return Unlock(rtn);
			}
		}
		progress = 100;
		if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);
		if (g_RnServoCom == NULL)
		{
			g_RnServoCom = new CServoDriverCom;
			if (g_RnServoCom == NULL)
			{
				return Unlock(RTN_MALLOC_FAIL);
			}
			rtn = g_RnServoCom->Initial(g_RnInterface);
			if (rtn != RTN_SUCCESS)
			{
				return Unlock(rtn);
			}
		}
		if (NULL == g_Eeprom)
		{
			g_Eeprom = new CEeprom;
			if (g_Eeprom == NULL)
			{
				return Unlock(RTN_MALLOC_FAIL);
			}
			rtn = g_Eeprom->Initial(g_RnInterface);
			if (rtn != RTN_SUCCESS)
			{
				return Unlock(rtn);
			}
			g_RnServoCom->InitialEeprom(g_Eeprom);
		}
		if (g_AbsCom == NULL)
		{
			g_AbsCom = new CAbsCom;
			if (g_AbsCom == NULL)
			{
				return RTN_MALLOC_FAIL;
			}
			g_AbsCom->m_pRnNetCom = g_RnInterface;
		}

		if (g_hex == NULL)
		{
			g_hex = new Hex(':');
			if (g_hex == NULL)
			{
				return RTN_MALLOC_FAIL;
			}

		}
		if (g_firmwareDl == NULL)
		{
			g_firmwareDl = new CFirmwareDL;
			if (g_firmwareDl == NULL)
			{
				return RTN_MALLOC_FAIL;
			}
		}
		if (g_hex2ldr == NULL)
		{
			g_hex2ldr = new CHex2ldr;
			if (g_hex2ldr == NULL)
			{
				return RTN_MALLOC_FAIL;
			}
		}
		if (g_serial == NULL)
		{
			g_serial = new SerialCtl;
			if (g_serial == NULL)
			{
				return RTN_MALLOC_FAIL;
			}
		}
		if (g_eeprom == NULL)
		{
			g_eeprom = new eepromOpt;
			if (g_eeprom == NULL)
			{
				return RTN_MALLOC_FAIL;
			}
		}

		return Unlock(RTN_SUCCESS);
	}
	////////////////////////if use the ring net way end//////////////////////////////////////////////////

	if (g_plotWave == NULL && com_type != GTSD_COM_TYPE_RNNET)
	{
		g_plotWave = new CPlotWave(com_type);
		if (g_plotWave == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
	}
	if (g_RnDrivePlot == NULL && com_type == GTSD_COM_TYPE_RNNET)
	{
		g_RnDrivePlot = new CRnDriverPlot;
		if (g_RnDrivePlot == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
		Uint16 dsp_list[2] = { 0xF001, 0xF002 };
		short rtn = g_RnDrivePlot->PW_CreateDspWave(2, dsp_list);
		if (rtn != RTN_SUCCESS)
		{
			return rtn;
		}
	}
	if (g_hex == NULL)
	{
		g_hex = new Hex(':');
		if (g_hex == NULL)
		{
			return RTN_MALLOC_FAIL;
		}

	}
	if (g_firmwareDl == NULL)
	{
		g_firmwareDl = new CFirmwareDL;
		if (g_firmwareDl == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
	}
	if (g_hex2ldr == NULL)
	{
		g_hex2ldr = new CHex2ldr;
		if (g_hex2ldr == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
	}
	if (g_serial == NULL)
	{
		g_serial = new SerialCtl;
		if (g_serial == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
	}
	if (g_eeprom == NULL)
	{
		g_eeprom = new eepromOpt;
		if (g_eeprom == NULL)
		{
			return RTN_MALLOC_FAIL;
		}
	}
	//�ٷֱȽ���
	progress = 5;
	if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_OPEN(tpfUpdataProgressPt, ptr,progress, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Close(int16 com_type)
{
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (Net_Rt_Lock_Err == TryLock())
		{
			return Net_Rt_Lock_Err;
		}
		
		if (g_RnServoCom)
		{
			delete g_RnServoCom;
			g_RnServoCom = NULL;
 		}
		if (g_RnInterface)
		{
			delete g_RnInterface;
			g_RnInterface = NULL;
		}
		if (g_AbsCom != NULL)
		{
			g_AbsCom->m_pRnNetCom = NULL;
		}

		if (g_plotWave != NULL)
		{
			delete g_plotWave;
			g_plotWave = NULL;
		}
		if (g_hex != NULL)
		{
			delete g_hex;
			g_hex = NULL;
		}
		if (g_firmwareDl != NULL)
		{
			delete g_firmwareDl;
			g_firmwareDl = NULL;
		}
		if (g_hex2ldr != NULL)
		{
			delete g_hex2ldr;
			g_hex2ldr = NULL;
		}
		if (g_serial != NULL)
		{
			delete g_serial;
			g_serial = NULL;
		}
		if (g_eeprom != NULL)
		{
			delete g_eeprom;
			g_eeprom = NULL;
		}
		if (g_RnDrivePlot)
		{
			delete g_RnDrivePlot;
			g_RnDrivePlot = NULL;
		}

		if (g_AbsCom)
		{
			delete g_AbsCom;
			g_AbsCom = NULL;
		}

		if (g_Eeprom)
		{
			delete g_Eeprom;
			g_Eeprom = NULL;
		}

		return Unlock(RTN_SUCCESS);
	}
	//////////////////////////////////////////////////////////////////////////
	if (g_plotWave != NULL)
	{
		delete g_plotWave;
		g_plotWave = NULL;
	}	
	if (g_hex != NULL)
	{
		delete g_hex;
		g_hex = NULL;
	}
	if (g_firmwareDl != NULL)
	{
		delete g_firmwareDl;
		g_firmwareDl = NULL;
	}
	if (g_hex2ldr != NULL)
	{
		delete g_hex2ldr;
		g_hex2ldr = NULL;
	}
	if (g_serial != NULL)
	{
		delete g_serial;
		g_serial = NULL;
	}
	if (g_eeprom != NULL)
	{
		delete g_eeprom;
		g_eeprom = NULL;
	}
	if (g_RnDrivePlot)
	{
		delete g_RnDrivePlot;
		g_RnDrivePlot = NULL;
	}
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_CLOSE(com_type));
}

////////////   add by luo.mj 20180329                //////////////////////////////////////////////

SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetStationIdList(vector<int16>& stationIdList, int16 com_type/* = GTSD_COM_TYPE_NET*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		g_RnServoCom->GetStationIdList(stationIdList);
		return RTN_SUCCESS;
	}

	return RTN_PARAM_ERR;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetStationAxisNum(int16* axisNum, int16 com_type/* = GTSD_COM_TYPE_NET*/, int16 stationId/* = 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
    if (0xF0 != stationId)
    {
      short rtn;
      rtn = g_RnServoCom->SetStationId(stationId);
      if (rtn != 0)
      {
        return rtn;
      }
//			g_RnServoCom->GetStationAxisNum(axisNum);
    }
    g_RnServoCom->GetStationAxisNum(axisNum);
		return RTN_SUCCESS;
	}
	return RTN_PARAM_ERR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetServoOn(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetServoOn(axis);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetServoOn(axis, com_type,stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetServoOff(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetServoOff(axis);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetServoOff(axis, com_type,stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetServoState(int16 axis, SERVO_STATE* serv, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetServoState(axis, serv);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetServoState(axis, serv, com_type,stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetServoTaskMode(int16 axis, int16 mode, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetServoTaskMode(axis, mode);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetServoTaskMode(axis, mode, com_type,stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetServoTaskMode(int16 axis, SERVO_MODE* mode, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetServoTaskMode(axis, mode);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetServoTaskMode(axis, mode, com_type,stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Write16BitByAdr(int16 axis, int16 ofst, int16 value, void* ptr,int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Write16BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Write16BitByAdr(axis, ofst, value, ptr, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Read16BitByAdr(int16 axis, int16 ofst, int16* value, void* ptr, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Read16BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Read16BitByAdr(axis, ofst, value, ptr, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Write32BitByAdr(int16 axis, int16 ofst, int32 value, void* ptr, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Write32BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Write32BitByAdr(axis, ofst, value, ptr, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Read32BitByAdr(int16 axis, int16 ofst, int32* value, void* ptr, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Read32BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Read32BitByAdr(axis, ofst, value, ptr, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Write64BitByAdr(int16 axis, int16 ofst, int64 value, void* ptr, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Write64BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Write64BitByAdr(axis, ofst, value, ptr, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Read64BitByAdr(int16 axis, int16 ofst, int64* value, void* ptr, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Read64BitByAdr(axis, ofst, value, ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Read64BitByAdr(axis, ofst, value, ptr,com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetIdRef(int16 axis, double id_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetIdRef(axis, id_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetIdRef(axis, id_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetIdRef(int16 axis, ID_STATE* id_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetIdRef(axis, id_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetIdRef(axis, id_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetIqRef(int16 axis, double iq_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetIqRef(axis, iq_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetIqRef(axis, iq_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetIqRef(int16 axis, IQ_STATE* iq_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetIqRef(axis, iq_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetIqRef(axis, iq_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetSpdRef(int16 axis, double spd_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetSpdRef(axis, spd_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetSpdRef(axis, spd_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetSpdRef(int16 axis, SPD_STATE* spd_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetSpdRef(axis, spd_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetSpdRef(axis, spd_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetUdRef(int16 axis, double ud_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetUdRef(axis, ud_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetUdRef(axis, ud_ref, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetUdRef(int16 axis, UD_STATE* ud_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetUdRef(axis, ud_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetUdRef(axis, ud_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetUqRef(int16 axis, double uq_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetUqRef(axis, uq_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetUqRef(axis, uq_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetUqRef(int16 axis, UQ_STATE* uq_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetUqRef(axis, uq_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetUqRef(axis, uq_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetUaRef(int16 axis, double ua_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetUaRef(axis, ua_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetUaRef(axis, ua_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetUaRef(int16 axis, UA_STATE* ua_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetUaRef(axis, ua_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetUaRef(axis, ua_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetUbRef(int16 axis, double ub_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetUbRef(axis, ub_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetUbRef(axis, ub_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetUbRef(int16 axis, UB_STATE* ub_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetUbRef(axis, ub_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetUbRef(axis, ub_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetUcRef(int16 axis, double uc_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetUcRef(axis, uc_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetUcRef(axis, uc_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetUcRef(int16 axis, UC_STATE* uc_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetUcRef(axis, uc_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetUcRef(axis, uc_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetPosAdjRef(int16 axis, double PosAdj_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetPosAdjRef(axis, PosAdj_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetPosAdjRef(axis, PosAdj_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetPosAdjRef(int16 axis, POS_ADJ_STATE* pos_adj_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetPosAdjRef(axis, pos_adj_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_GetPosAdjRef(axis, pos_adj_ref, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetPosRef(int16 axis, int32 Pos_ref, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetPosRef(axis, Pos_ref);
	}
	//////////////////////////////////////////////////////////////////////////
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_SetPosRef(axis, Pos_ref, com_type, stationId));
}
/*
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetFndCurr(int16 axis, int16 fnd, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetFndCurr(axis, fnd, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetFndCurr(int16 axis, int16* fnd, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetFndCurr(axis, fnd, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetFnqCurr(int16 axis, int16 fnq, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetFnqCurr(axis, fnq, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetFnqCurr(int16 axis, int16* fnq, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetFnqCurr(axis, fnq, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetTidCurr(int16 axis, int32 tid, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetTidCurr(axis, tid, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetTidCurr(int16 axis, int32* tid, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetTidCurr(axis, tid, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetTiqCurr(int16 axis, int32 tiq, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetTiqCurr(axis, tiq, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetTiqCurr(int16 axis, int32* tiq, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetTiqCurr(axis, tiq, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetFnVel(int16 axis, int16 fnvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetFnVel(axis, fnvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetFnVel(int16 axis, int16* fnvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetFnVel(axis, fnvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetTiVel(int16 axis, double tivel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetTiVel(axis, tivel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetTiVel(int16 axis, double* tivel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetTiVel(axis, tivel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetAbsLimVel(int16 axis, int16 abslimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetAbsLimVel(axis, abslimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetAbsLimVel(int16 axis, int16* abslimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetAbsLimVel(axis, abslimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetPosLimVel(int16 axis, int16 poslimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetPosLimVel(axis, poslimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetPosLimVel(int16 axis, int16* poslimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetPosLimVel(axis, poslimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetNegLimVel(int16 axis, int16 neglimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetNegLimVel(axis, neglimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetNegLimVel(int16 axis, int16* neglimvel, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetNegLimVel(axis, neglimvel, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetFnPos(int16 axis, double fnpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetFnPos(axis, fnpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetFnPos(int16 axis, double* fnpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetFnPos(axis, fnpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetAbsLimPos(int16 axis, int16 abslimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetAbsLimPos(axis, abslimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetAbsLimPos(int16 axis, int16* abslimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetAbsLimPos(axis, abslimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetPosLimPos(int16 axis, int16 poslimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetPosLimPos(axis, poslimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetPosLimPos(int16 axis, int16* poslimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetPosLimPos(axis, poslimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetNegLimPos(int16 axis, int16 neglimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetNegLimPos(axis, neglimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetNegLimPos(int16 axis, int16* neglimpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetNegLimPos(axis, neglimpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetKgaFfdPos(int16 axis, int16 kgaffdpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetKgaFfdPos(axis, kgaffdpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetKgaFfdPos(int16 axis, int16* kgaffdpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetKgaFfdPos(axis, kgaffdpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetKgvFfdPos(int16 axis, int16 kgvffdpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_SetKgvFfdPos(axis, kgvffdpos, com_type));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetKgvFfdPos(int16 axis, int16* kgvffdpos, int16 com_type)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_GetKgvFfdPos(axis, kgvffdpos, com_type));
}
*/
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Set16bitFPGAByAddr(int16 dsp_number, int16 com_addr, int16 value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Set16bitFPGAByAddr(dsp_number, com_addr, value);
	}
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_Set16bitFPGAByAddr(dsp_number, com_addr, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Get16bitFPGAByAddr(int16 dsp_number, int16 com_addr, int16* pvalue, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Get16bitFPGAByAddr(dsp_number, com_addr, pvalue);
	}
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_Get16bitFPGAByAddr(dsp_number, com_addr, pvalue, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Set32bitFPGAByAddr(int16 dsp_number, int16 com_addr, int32 value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Set32bitFPGAByAddr(dsp_number, com_addr, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_Set32bitFPGAByAddr(dsp_number, com_addr, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Get32bitFPGAByAddr(int16 dsp_number, int16 com_addr, int32* pvalue, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Get32bitFPGAByAddr(dsp_number, com_addr, pvalue);
	}
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
		if (com_type == GTSD_COM_TYPE_RNNET)
	{	
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_Get32bitFPGAByAddr(dsp_number, com_addr, pvalue, com_type, stationId));
}


SERVODRIVERCOMDLL_API int16 GTSD_CMD_SetWaveBuf(int16 dsp_number, WAVE_BUF_PRM wave, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_SetWaveBuf(dsp_number, wave);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{	
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
//		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_SetWaveBuf(dsp_number, wave, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetWaveBuf(int16 dsp_number, tWaveBufCmd* ctrlword, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetWaveBuf(dsp_number, ctrlword);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
//		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_GetWaveBuf(dsp_number, ctrlword, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetWaveData(int16 dsp_number, int16* read_num, int16** data, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_GetWaveData(dsp_number, read_num, data);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		int16 axis = dsp_number * 2;
		stationId = GTSD_Convert_axi(axis);
		dsp_number = 0;
	}
	return Unlock(GTSD_CMD_ST_GetWaveData(dsp_number, read_num, data, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_ClearFpgaFifo(int16 dsp_number, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ClearFpgaFifo(dsp_number);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	return Unlock(GTSD_CMD_ST_ClearFpgaFifo(dsp_number, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Write16BitByAdr(int16 axis, int16 ofst, int16 value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Write16BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Write16BitByAdr(axis, ofst, value, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Read16BitByAdr(int16 axis, int16 ofst, int16* value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Read16BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Read16BitByAdr(axis, ofst, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Write32BitByAdr(int16 axis, int16 ofst, int32 value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Write32BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Write32BitByAdr(axis, ofst, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Read32BitByAdr(int16 axis, int16 ofst, int32* value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Read32BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Read32BitByAdr(axis, ofst, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Write64BitByAdr(int16 axis, int16 ofst, int64 value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Write64BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Write64BitByAdr(axis, ofst, value, com_type, stationId));
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_Fram_Read64BitByAdr(int16 axis, int16 ofst, int64* value, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_Fram_Read64BitByAdr(axis, ofst, value);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_Fram_Read64BitByAdr(axis, ofst, value, com_type, stationId));
}

int16 GTSD_CMD_FlashWrite(int16 axis, INTEL_HEX_FRAME* packet, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_FlashWrite(axis, packet);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_FlashWrite(axis, packet, com_type, stationId));
}
int16 GTSD_CMD_FlashRead(int16 axis, INTEL_HEX_FRAME* packet_w, INTEL_HEX_FRAME* packet_r, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_FlashRead(axis, packet_w, packet_r);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_FlashRead(axis, packet_w, packet_r, com_type, stationId));
}
int16 GTSD_CMD_FlashErase(int16 axis, int16 blockNum, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_FlashErase(axis, blockNum);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_FlashErase(axis, blockNum, com_type, stationId));
}
int16 GTSD_CMD_InterruptSwitch(int16 axis, int16 int_switch, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_InterruptSwitch(axis, int_switch);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if(com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_InterruptSwitch(axis, int_switch, com_type, stationId));
}
//////   ?????????????????????           ///////////////////
SERVODRIVERCOMDLL_API int16 GTSD_CMD_ProcessorFlashHandler(int16 axis, wstring& filePath, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	int16 progress;
	Uint64 byteLenth = 0;
	int16 block = 0;
    void* ptr = ptrv;
	if (g_hex == NULL)
	{
		return -1;
	}
	//�ٷֱȽ���
	progress = 0;
	(*tpfUpdataProgressPt)(ptr,&progress);

	Uint16 pre_mode = GTSD_CMD_FroceCheckMode(CRingNetInterface::COM_DSP_CHEKC_FORCE_OFF);
	//��λ����
	g_hex->ResetVar();

	int16 rtn;
	//���ж�
	rtn = GTSD_CMD_InterruptSwitch(axis, 0, com_type, stationId);
	if (rtn!=0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}

	rtn = g_hex->ParseHex(ws2s(filePath));
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}
	//�ٷֱȽ���
	progress = 10;
	(*tpfUpdataProgressPt)(ptr, &progress);

	//������Ҫ�������ٿ�block,�Ƚ�16bit���ݸ���ת��Ϊbit����
	//flash 2M byte 4kbyte Ϊ1��block 
	byteLenth = g_hex->dataLenth * 2;
	block = (int16)(byteLenth / 4096.0);
	block += 1;

	rtn = GTSD_CMD_FlashErase(axis, block, com_type, stationId);
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}
	//�ٷֱȽ���
	progress = 30;
	(*tpfUpdataProgressPt)(ptr, &progress);

	rtn = g_hex->WriteFlash(axis, &(g_hex->m_hex_frame_write),tpfUpdataProgressPt,ptr,com_type,stationId);
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}

	//�ٷֱȽ���
	progress = 50;
	(*tpfUpdataProgressPt)(ptr, &progress);


	rtn = g_hex->ReadFlash(axis, &(g_hex->m_hex_frame_write),tpfUpdataProgressPt, ptr, com_type,stationId);
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}

	//�ٷֱȽ���
	progress = 90;
	(*tpfUpdataProgressPt)(ptr, &progress);

	rtn = g_hex->CompareFlash(&(g_hex->m_hex_frame_write), &(g_hex->m_hex_frame_read));
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}

	//���ж�
	rtn = GTSD_CMD_InterruptSwitch(axis, 1, com_type, stationId);
	if (rtn != 0)
	{
		GTSD_CMD_FroceCheckMode(pre_mode);
		return rtn;
	}
	//�ٷֱȽ���
	progress = 100;
	(*tpfUpdataProgressPt)(ptr, &progress);

	GTSD_CMD_FroceCheckMode(pre_mode);

	return RTN_SUCCESS;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_StartPlot(int16& axis, WAVE_BUF_PRM& wave, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_StartPlot(axis, wave);
	}
// 	if (com_type == GTSD_COM_TYPE_RNNET)
// 	{
// 		if (g_RnDrivePlot == NULL)
// 		{
// 			return RTN_OBJECT_UNCREATED;
// 		}
// 		Uint16 *wave_size = new Uint16[wave.cmd.bit.NUM];
// 		if (wave_size == NULL)
// 		{
// 			return RTN_MALLOC_FAIL;
// 		}
// 		for (int i = 0; i < wave.cmd.bit.NUM; i++)
// 		{
// 			wave_size[i] = wave.inf[i].bytes;
// 		}
// 		short rtn = g_RnDrivePlot->PW_StartDspPlot(0xf000 + (axis>>1) + 1 , wave.cmd.bit.NUM, wave_size);
// 		delete wave_size;
// 		if (rtn != RTN_SUCCESS)
// 			return rtn;
// 		rtn = GTSD_CMD_SetWaveBuf(axis>>1, wave, com_type, stationId);
// 		return rtn;
// 	}

	if (g_plotWave == NULL)
	{
		return -2;
	}

	//���Ƿ���
	if ((axis < 0) || (axis >= CPlotWave::pw_MaxAxis))
	{
		return -1;
	}
	int16 axis_t;
	axis_t = (axis >> 1);
	g_plotWave->PW_StartDspPlot(axis_t, wave, com_type, stationId);
// 	switch (axis_t)
// 	{
// 	case 0:
// 	{
// 		g_plotWave->PW_StartDspAPlot(wave,com_type,stationId);
// 		break;
// 	}
// 	case 1:
// 	{
// 		g_plotWave->PW_StartDspBPlot(wave, com_type, stationId);
// 		break;
// 	}
// 	default:
// 		break;
// 	}
	//����������־
	g_plotWave->pw_RunFlag[axis_t] = true;

	return 0;
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_StopPlot(int16& axis, WAVE_BUF_PRM& wave, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_StopPlot(axis, wave);
	}
// 	if (com_type == GTSD_COM_TYPE_RNNET)
// 	{
// 		if (g_RnDrivePlot == NULL)
// 		{
// 			return RTN_OBJECT_UNCREATED;
// 		}
// 		//ֹͣdspA��FPGA��FIFO��д����
// 		wave.cmd.bit.ENP = 0;
// 		short rtn = GTSD_CMD_SetWaveBuf(axis>>1, wave, com_type, stationId);
// 		if (rtn)
// 			return rtn;
// 		return g_RnDrivePlot->PW_StopDspPlot(0xf000 + (axis >> 1) + 1);
// 	}
	if (g_plotWave == NULL)
	{
		return -2;
	}
	//���Ƿ���
	if ((axis < 0) || (axis >= CPlotWave::pw_MaxAxis))
	{
		return -1;
	}
	int16 axis_t;
	axis_t = (axis >> 1);
	g_plotWave->PW_StopDspPlot(axis_t, wave, com_type, stationId);
// 	switch (axis_t)
// 	{
// 	case 0:
// 	{
// 		g_plotWave->PW_StopDspAPlot(wave,com_type,stationId);
// 		break;
// 	}
// 	case 1:
// 	{
// 		g_plotWave->PW_StopDspBPlot(wave,com_type,stationId);
// 		break;
// 	}
// 	default:
// 		break;
// 	}
	//����������־Ϊfalse
	g_plotWave->pw_RunFlag[axis_t] = false;

	return 0;
}
SERVODRIVERCOMDLL_API int16 GTSD_CMD_PcGetWaveData(int16& axis, double** data, int32& number, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_PcGetWaveData(axis, data, number);
	}
// 	if (com_type == GTSD_COM_TYPE_RNNET)
// 	{
// 		Uint32 read_number;
// 		short rtn = g_RnDrivePlot->PW_PcGetDspWaveData(0xf000 + (axis >> 1) + 1, data, read_number);
// 		number = read_number;
// 		return rtn;
// 	}

	if (g_plotWave == NULL)
	{
		return -2;
	}
	//���Ƿ���
	if ((axis < 0) || (axis >= CPlotWave::pw_MaxAxis))
	{
		return -1;
	}
	int16 axis_t;
	axis_t = (axis >> 1);
	g_plotWave->PW_PcGetDspWaveData(axis_t, data, number, com_type);
// 	switch (axis_t)
// 	{
// 	case 0:
// 	{
// 		g_plotWave->PW_PcGetDspAWaveData(data, number,com_type);
// 		break;
// 	}
// 	case 1:
// 	{
// 		g_plotWave->PW_PcGetDspBWaveData(data, number,com_type);
// 		break;
// 	}
// 	default:
// 		break;
// 	}
	return 0;
}
SERVODRIVERCOMDLL_API bool GTSD_CMD_CheckPlotState(int16& axis, int16 stationId /*= 0xf0*/)
{
	//check plotwave state
	if (g_plotWave == NULL)
	{
		return false;
	}
	//���Ƿ���
	if ((axis < 0) || (axis >= CPlotWave::pw_MaxAxis))
	{
		return false;
	}
	int16 axis_t;
	axis_t = (axis >> 1);

	bool isStart = false;
	isStart = g_plotWave->pw_RunFlag[axis_t];

	return isStart;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_FirmwareFlashHandler(int16 axis, wstring& filePath, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	int16 progress;
    void* ptr = ptrv;
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (g_firmwareDl == NULL)
	{
    return Unlock(-1);//add by chenchao ����ʱ���ͷ��ź���������������ʹ��
	}

	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	//�ٷֱȽ���
	progress = 0;
	if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);

	int16 rtn = 0;
	//��λ����
	g_firmwareDl->ResetVar();

	//�ٷֱȽ���
	progress = 5;
	if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);

	rtn = g_firmwareDl->WriteFPGAFileToFlash(com_type, ws2s(filePath), tpfUpdataProgressPt, ptr, progress,stationId);

	if (rtn != 0)
	{
    return Unlock(-2);
	}

	//�ٷֱȽ���
	progress = 100;
	if (tpfUpdataProgressPt) (*tpfUpdataProgressPt)(ptr, &progress);


	/*int16 index1,index2;
	index1 = filePath.find_last_of('\\',filePath.length());
	index2 = filePath.find_last_of('.', filePath.length());
	string tmp;
	string tmp1;
	tmp = filePath.substr(index1, index2-index1);
	tmp = tmp + "_read.rpd";
	tmp1 = filePath.substr(0, index1);
	filePath = tmp1 + tmp;
	rtn = g_firmwareDl->ReadFPGADataToFile(com_type,filePath, g_firmwareDl->m_byte_write);
	if (rtn != 0)
	{
	return -2;
	}*/

	return Unlock(rtn);
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ProcessorGeneralFunc(int16 axis, GENERALFUNCTION* gefunc, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ProcessorGeneralFunc(axis, gefunc);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ProcessorGeneralFunc(axis, gefunc, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ResetSystem(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ResetSystem(axis);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ResetSystem(axis, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_CheckResetFinish(int16 axis, bool& flag_finish, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_CheckResetFinish(axis, flag_finish);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_CheckResetFinish(axis, flag_finish, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadProcessorVersion(int16 axis, Uint16& ver, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ReadProcessorVersion(axis, ver);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ReadProcessorVersion(axis, ver, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadFirmwareVersion(int16 axis, Uint16& ver, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
  if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;

  if (com_type == GTSD_COM_TYPE_RNNET)
  {
	  if (0xF0 != stationId)
	  {
		  short rtn;
		  rtn = g_RnServoCom->SetStationId(stationId);
		  if (rtn != 0)
		  {
			  return rtn;
		  }
	  }
	  return g_RnServoCom->GTSD_CMD_ReadFirmwareVersion(axis, ver);
  }

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ReadFirmwareVersion(axis, ver, com_type, stationId));
}
//add bu luo.mj
SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadFpgaVersion(int16 axis, VERSION* ver, int16 com_type, int16 stationId)
{
  if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL)
    return RTN_OBJECT_UNCREATED;

  if (com_type == GTSD_COM_TYPE_RNNET)
  {
	  if (0xF0 != stationId)
	  {
		  short rtn;
		  rtn = g_RnServoCom->SetStationId(stationId);
		  if (rtn != 0)
		  {
			  return rtn;
		  }
	  }

	  return g_RnServoCom->GTSD_CMD_ReadFPGAVersion(axis, ver);
  }

  if (Net_Rt_Lock_Err == TryLock())
  {
    return Net_Rt_Lock_Err;
  }
  if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
  return Unlock(CMD_ERROR_UNKNOWN);//pcdebug return error
}


SERVODRIVERCOMDLL_API int16 GTSD_CMD_ClrAlarm(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ClrAlarm(axis);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ClrAlarm(axis, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_Hex2Ldr(wstring& HexFile, wstring& LdrFile, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (g_hex2ldr == NULL)
	{
		return -1;
	}
	//�����ַ�̶�����Ϊһ�㲻��䶯��1000����Ϊǰ���м��ܵĲ���
	string addr = "10000000";
	g_hex2ldr->hex2ldr_execute(ws2s(HexFile), ws2s(LdrFile), addr);
	
	return 0;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_OpenSerialPort(int16 axis, int32 baudRate, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_OpenSerialPort(axis, baudRate, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_CloseSerialPort(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_CloseSerialPort(axis, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadSerialPort(int16 axis, Uint8 *buf, int32 length, int32 *length_read, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ReadSerialPort(axis, buf, length, length_read, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_WriteSerialPort(int16 axis, Uint8 *buf, int32 length, int32 *length_written, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_WriteSerialPort(axis, buf, length, length_written, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ProcessorUartBootHandler(int16 axis, wstring& filePath, int32 baudRate, int16 cmd, string& inputKey, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	int16 progress;

	void* ptr = ptrv;
	if (g_serial == NULL)
	{
		return RTN_OBJECT_UNCREATED;// -1;
	}
	//�ٷֱȽ���
	progress = 0;
	(*tpfUpdataProgressPt)(ptr, &progress);

	int16 rtn;
	//�򿪴��ڣ����ݲ�����
	rtn = GTSD_CMD_OpenSerialPort(axis, baudRate, com_type, stationId);
	if (rtn != 0)
	{
		return rtn;// -1;
	}
	//�ٷֱȽ���
	progress = 10;
	(*tpfUpdataProgressPt)(ptr, &progress);

	bool rtnb = g_serial->downloadBootStream(axis, inputKey, cmd, ws2s(filePath), tpfUpdataProgressPt, ptr, progress, com_type,stationId);

	if (rtnb  == false)
	{
		return RTN_UPDATE_WAIT_OVERTIME;// -2;
	}
	
	//�ٷֱȽ���
	progress = 100;
	(*tpfUpdataProgressPt)(ptr, &progress);

	rtn = GTSD_CMD_CloseSerialPort(axis, com_type, stationId);
	if (rtn != 0)
	{
		return RTN_MALLOC_FAIL;// -1;
	}
	return 0;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadLogAlarmCode(int16 axis, Uint32* alarmCode, Uint16& lenth, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ReadLogAlarmCode(axis, alarmCode, lenth);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ReadLogAlarmCode(axis, alarmCode, lenth, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadLogAlarmTimes(int16 axis, Uint16* alarmTimes, Uint16& lenth, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;
	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return g_RnServoCom->GTSD_CMD_ReadLogAlarmTimes(axis, alarmTimes, lenth);
	}

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	if (com_type == GTSD_COM_TYPE_RNNET)  stationId = GTSD_Convert_axi(axis);
	return Unlock(GTSD_CMD_ST_ReadLogAlarmTimes(axis, alarmTimes, lenth, com_type, stationId));
}

string ws2s(const wstring& ws)
{
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	size_t retnum;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	setlocale(LC_ALL, "chs");
	wcstombs_s(&retnum, _Dest, _Dsize, _Source, _Dsize);
	std::string result = _Dest;
	delete[] _Dest;
	return result;
}
wstring s2ws(const string& s)
{
	const char* _Source = s.c_str();
	size_t retnum;
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	setlocale(LC_ALL, "chs");
	mbstowcs_s(&retnum, _Dest, _Dsize, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[] _Dest;
	return result;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_GetNetCardMsg(void)
{
  Uint32 dwSize = 0;
  Uint32 dwRetVal = 0;
  string tmp = "";
  string tmpid = "";
  Uint32 i, j;

  if ((NetCardName == L"")||(NetCardNum == L""))
  {
    return 0;
  }

  /* variables used for GetIfTable and GetIfEntry */
  MIB_IFTABLE *pIfTable;
  MIB_IFROW *pIfRow;

  // Allocate memory for our pointers.
  pIfTable = (MIB_IFTABLE *)MALLOC(sizeof(MIB_IFTABLE));
  if (pIfTable == NULL)
  {
    printf("Error allocating memory needed to call GetIfTable\n");
    return 1;
  }
  // Make an initial call to GetIfTable to get the
  // necessary size into dwSize
  dwSize = sizeof(MIB_IFTABLE);
  if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
  {
    FREE(pIfTable);
    pIfTable = (MIB_IFTABLE *)MALLOC(dwSize);
    if (pIfTable == NULL)
    {
      printf("Error allocating memory needed to call GetIfTable\n");
      return 1;
    }
  }
  // Make a second call to GetIfTable to get the actual
  // data we want.
  if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR)
  {
    printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
    for (i = 0; i < pIfTable->dwNumEntries; i++)
    {
      pIfRow = (MIB_IFROW *)& pIfTable->table[i];
      printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
      printf("\tInterfaceName[%d]:\t %ws", i, pIfRow->wszName);
      printf("\n");
      printf("\tDescription[%d]:\t ", i);
      for (j = 0; j < pIfRow->dwDescrLen; j++)
        printf("%c", pIfRow->bDescr[j]);
      printf("\n");
      printf("\tType[%d]:\t ", i);
      switch (pIfRow->dwType) {
      case IF_TYPE_OTHER:
        printf("Other\n");
        break;
      case IF_TYPE_ETHERNET_CSMACD:
        printf("Ethernet\n");
        break;
      case IF_TYPE_ISO88025_TOKENRING:
        printf("Token Ring\n");
        break;
      case IF_TYPE_PPP:
        printf("PPP\n");
        break;
      case IF_TYPE_SOFTWARE_LOOPBACK:
        printf("Software Lookback\n");
        break;
      case IF_TYPE_ATM:
        printf("ATM\n");
        break;
      case IF_TYPE_IEEE80211:
        printf("IEEE 802.11 Wireless\n");
        break;
      case IF_TYPE_TUNNEL:
        printf("Tunnel type encapsulation\n");
        break;
      case IF_TYPE_IEEE1394:
        printf("IEEE 1394 Firewire\n");
        break;
      default:
        printf("Unknown type %ld\n", pIfRow->dwType);
        break;
      }
      printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
      printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);

      //�ж����������Ƿ�һ�£�һ�����ж��ٶȡ�
      tmp = ((const char*)pIfRow->bDescr);
      tmpid = ws2s(pIfRow->wszName);
      //�ҵ�����λ��
      int16 pos_start;
      int16 pos_end;
      string tmpValue;
      string brace_start = "{";
      string brace_end = "}";
      pos_start = tmpid.find(brace_start.c_str());
      pos_end = tmpid.find(brace_end.c_str());
      //û�ҵ�����
      if ((pos_start == -1) || (pos_end == -1))
      {
        return 1;
      }
      else
      {
        tmpValue = tmpid.substr(pos_start + 1, pos_end - pos_start - 1);
      }

      if ((ws2s(NetCardName) == tmp) || (tmpValue == ws2s(NetCardNum)))
      {
        if (pIfRow->dwSpeed == 1000000000)
        {
          return Rt_Success;
        }
        else
        {
          return 2;
        }
      }

      printf("\tPhysical Addr:\t ");
      if (pIfRow->dwPhysAddrLen == 0)
        printf("\n");
      for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
        if (j == (pIfRow->dwPhysAddrLen - 1))
          printf("%.2X\n", (int)pIfRow->bPhysAddr[j]);
        else
          printf("%.2X-", (int)pIfRow->bPhysAddr[j]);
      }
      printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
      printf("\tOper Status[%d]:\t ", i);
      switch (pIfRow->dwOperStatus) {
      case IF_OPER_STATUS_NON_OPERATIONAL:
        printf("Non Operational\n");
        break;
      case IF_OPER_STATUS_UNREACHABLE:
        printf("Unreachable\n");
        break;
      case IF_OPER_STATUS_DISCONNECTED:
        printf("Disconnected\n");
        break;
      case IF_OPER_STATUS_CONNECTING:
        printf("Connecting\n");
        break;
      case IF_OPER_STATUS_CONNECTED:
        printf("Connected\n");
        break;
      case IF_OPER_STATUS_OPERATIONAL:
        printf("Operational\n");
        break;
      default:
        printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
        break;
      }
      printf("\n");
    }
  }
  else
  {
    printf("GetIfTable failed with error: \n", dwRetVal);
    if (pIfTable != NULL)
    {
      FREE(pIfTable);
      pIfTable = NULL;
    }
    return 1;
    // Here you can use FormatMessage to find out why
    // it failed.
  }
  if (pIfTable != NULL)
  {
    FREE(pIfTable);
    pIfTable = NULL;
  }
  return 3;

//  Uint32 dwSize = 0;
//  Uint32 dwRetVal = 0;
//  string tmp ="";
//  string tmpid = "";
//  Uint32 i, j;

//  //���������Ϣ������̨
//  AllocConsole();

//  if (NetCardName == L"")
//  {
//    return 1;
//  }

//  /* variables used for GetIfTable and GetIfEntry */
//  MIB_IFTABLE *pIfTable;
//  MIB_IFROW *pIfRow;

//  // Allocate memory for our pointers.
//  pIfTable = (MIB_IFTABLE *)MALLOC(sizeof(MIB_IFTABLE));
//  if (pIfTable == NULL)
//  {
//    _cprintf_s("Error allocating memory needed to call GetIfTable\n");
//    FreeConsole();
//    return 1;
//  }
//  // Make an initial call to GetIfTable to get the
//  // necessary size into dwSize
//  dwSize = sizeof(MIB_IFTABLE);
//  if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
//  {
//    FREE(pIfTable);
//    pIfTable = (MIB_IFTABLE *)MALLOC(dwSize);
//    if (pIfTable == NULL)
//    {
//      _cprintf_s("Error allocating memory needed to call GetIfTable\n");
//      FreeConsole();
//      return 1;
//    }
//  }
//  // Make a second call to GetIfTable to get the actual
//  // data we want.
//  if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR)
//  {
//    _cprintf_s("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
//    for (i = 0; i < pIfTable->dwNumEntries; i++)
//    {
//      pIfRow = (MIB_IFROW *)& pIfTable->table[i];
//      _cprintf_s("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
//      _cprintf_s("\tInterfaceName[%d]:\t %ws", i, pIfRow->wszName);
//      _cprintf_s("\n");
//      _cprintf_s("\tDescription[%d]:\t ", i);
//      for (j = 0; j < pIfRow->dwDescrLen; j++)
//        _cprintf_s("%c", pIfRow->bDescr[j]);
//      _cprintf_s("\n");
//      _cprintf_s("\tType[%d]:\t ", i);
//      switch (pIfRow->dwType) {
//      case IF_TYPE_OTHER:
//        _cprintf_s("Other\n");
//        break;
//      case IF_TYPE_ETHERNET_CSMACD:
//        _cprintf_s("Ethernet\n");
//        break;
//      case IF_TYPE_ISO88025_TOKENRING:
//        _cprintf_s("Token Ring\n");
//        break;
//      case IF_TYPE_PPP:
//        _cprintf_s("PPP\n");
//        break;
//      case IF_TYPE_SOFTWARE_LOOPBACK:
//        _cprintf_s("Software Lookback\n");
//        break;
//      case IF_TYPE_ATM:
//        _cprintf_s("ATM\n");
//        break;
//      case IF_TYPE_IEEE80211:
//        _cprintf_s("IEEE 802.11 Wireless\n");
//        break;
//      case IF_TYPE_TUNNEL:
//        _cprintf_s("Tunnel type encapsulation\n");
//        break;
//      case IF_TYPE_IEEE1394:
//        _cprintf_s("IEEE 1394 Firewire\n");
//        break;
//      default:
//        _cprintf_s("Unknown type %ld\n", pIfRow->dwType);
//        break;
//      }
//      _cprintf_s("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
//      _cprintf_s("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);

//      //�ж����������Ƿ�һ�£�һ�����ж��ٶȡ�
//      tmp = ((const char*)pIfRow->bDescr);
//      tmpid = ws2s(pIfRow->wszName);
//      //�ҵ�����λ��
//      int16 pos_start;
//      int16 pos_end;
//      string tmpValue;
//      string brace_start = "{";
//      string brace_end = "}";
//      pos_start = tmpid.find(brace_start.c_str());
//      pos_end = tmpid.find(brace_end.c_str());
//      //û�ҵ�����
//      if ((pos_start == -1) || (pos_end == -1))
//      {
//        return 1;
//      }
//      else
//      {
//        tmpValue = tmpid.substr(pos_start + 1, pos_end - pos_start - 1);
//      }

//      if ((ws2s(NetCardName) == tmp) || (tmpValue == ws2s(NetCardNum)))
//      {
//        if (pIfRow->dwSpeed == 1000000000)
//        {
//          FreeConsole();
//          return Rt_Success;
//        }
//        else
//        {
//          FreeConsole();
//          return 2;
//        }
//      }

//      _cprintf_s("\tPhysical Addr:\t ");
//      if (pIfRow->dwPhysAddrLen == 0)
//        _cprintf_s("\n");
//      for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
//        if (j == (pIfRow->dwPhysAddrLen - 1))
//          _cprintf_s("%.2X\n", (int)pIfRow->bPhysAddr[j]);
//        else
//          _cprintf_s("%.2X-", (int)pIfRow->bPhysAddr[j]);
//      }
//      _cprintf_s("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
//      _cprintf_s("\tOper Status[%d]:\t ", i);
//      switch (pIfRow->dwOperStatus) {
//      case IF_OPER_STATUS_NON_OPERATIONAL:
//        _cprintf_s("Non Operational\n");
//        break;
//      case IF_OPER_STATUS_UNREACHABLE:
//        _cprintf_s("Unreachable\n");
//        break;
//      case IF_OPER_STATUS_DISCONNECTED:
//        _cprintf_s("Disconnected\n");
//        break;
//      case IF_OPER_STATUS_CONNECTING:
//        _cprintf_s("Connecting\n");
//        break;
//      case IF_OPER_STATUS_CONNECTED:
//        _cprintf_s("Connected\n");
//        break;
//      case IF_OPER_STATUS_OPERATIONAL:
//        _cprintf_s("Operational\n");
//        break;
//      default:
//        _cprintf_s("Unknown status %ld\n", pIfRow->dwAdminStatus);
//        break;
//      }
//      _cprintf_s("\n");
//    }
//  }
//  else
//  {
//    _cprintf_s("GetIfTable failed with error: \n", dwRetVal);
//    if (pIfTable != NULL)
//    {
//      FREE(pIfTable);
//      pIfTable = NULL;
//    }
//    FreeConsole();
//    return 1;
//    // Here you can use FormatMessage to find out why
//    // it failed.
//  }
//  if (pIfTable != NULL)
//  {
//    FREE(pIfTable);
//    pIfTable = NULL;
//  }
//  FreeConsole();
//  return 3;
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ScanRnTopology(int16 com_type /*= GTSD_COM_TYPE_RNNET*/, int16 stationId /*= 0xff*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_ScanRnTopology(com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadEEPROM(int16 axis, Uint16 ofst, Uint8* value, Uint16 num, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}


	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}

		return Unlock(g_RnServoCom->GTSD_CMD_ReadEEPROM(axis, ofst, value, num));
	}

	return Unlock(RTN_PARAM_ERR);
// 	if (Net_Rt_Lock_Err == TryLock())
// 	{
// 		return Net_Rt_Lock_Err;
// 	}
// 	return Unlock(GTSD_CMD_ST_ReadEEPROM(axis,ofst,value,num,com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_WriteEEPROM(int16 axis, Uint16 ofst, Uint8* value, Uint16 num, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; 

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}


	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}

		return Unlock(g_RnServoCom->GTSD_CMD_WriteEEPROM(axis, ofst, value, num));
	}

	return Unlock(RTN_PARAM_ERR);
// 	if (Net_Rt_Lock_Err == TryLock())
// 	{
// 		return Net_Rt_Lock_Err;
// 	}
// 	return Unlock(GTSD_CMD_ST_WriteEEPROM(axis, ofst, value, num, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ReadEEPROMExt(int16 axis, Uint16 ofst, Uint8* value, Uint16 num, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}


	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return Unlock(g_RnServoCom->GTSD_CMD_ReadEEPROMExt(axis, ofst, value, num));
	}

	return Unlock(RTN_PARAM_ERR);;
	// 	if (Net_Rt_Lock_Err == TryLock())
	// 	{
	// 		return Net_Rt_Lock_Err;
	// 	}
	// 	return Unlock(GTSD_CMD_ST_ReadEEPROM(axis,ofst,value,num,com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_WriteEEPROMExt(int16 axis, Uint16 ofst, Uint8* value, Uint16 num, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED;

	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}


	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return Unlock(g_RnServoCom->GTSD_CMD_WriteEEPROMExt(axis, ofst, value, num));
	}

	return Unlock(RTN_PARAM_ERR);
	// 	if (Net_Rt_Lock_Err == TryLock())
	// 	{
	// 		return Net_Rt_Lock_Err;
	// 	}
	// 	return Unlock(GTSD_CMD_ST_WriteEEPROM(axis, ofst, value, num, com_type, stationId));
}


SERVODRIVERCOMDLL_API int16 GTSD_CMD_ClearEEPROM(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
//	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) return RTN_OBJECT_UNCREATED; if (com_type == GTSD_COM_TYPE_RNNET) return g_RnServoCom->GTSD_CMD_ClearEEPROM(axis);
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_ClearEEPROM(axis, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ResetFPGA(int16 axis, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_ResetFPGA(axis, com_type, stationId));
}

SERVODRIVERCOMDLL_API int16 GTSD_CMD_ConfigEEPROM(int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}
	return Unlock(GTSD_CMD_ST_ConfigEEPROM(com_type, stationId));
}

SERVODRIVERCOMDLL_API Uint16 GTSD_CMD_FroceCheckMode(Uint16 mode)
{
	if (g_RnServoCom == NULL) 
		return RTN_OBJECT_UNCREATED; 
	return g_RnServoCom->GTSD_CMD_FroceCheckMode(mode);
}


SERVODRIVERCOMDLL_API short GTSD_CMD_XmlWriteFile(int16 axis, char* pFileNameList[], int pFileTypeList[], int file_num,
	void(*tpfUpdataProgressPt)(void*, short*), void* ptrv, short& progress, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL)
		return Unlock(RTN_OBJECT_UNCREATED);

	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return Unlock(g_RnServoCom->GTSD_CMD_XmlWriteFile(axis, pFileNameList, pFileTypeList, file_num, tpfUpdataProgressPt, ptrv, progress));
	}

	return Unlock(RTN_PARAM_ERR);
}

SERVODRIVERCOMDLL_API short GTSD_CMD_XmlReadFile(int16 axis, char* pFileNameList[], int pFileTypeList[], int& file_num,
	void(*tpfUpdataProgressPt)(void*, short*), void* ptrv, short& progress, int16 com_type /*= GTSD_COM_TYPE_NET*/, int16 stationId /*= 0xf0*/)
{
	if (Net_Rt_Lock_Err == TryLock())
	{
		return Net_Rt_Lock_Err;
	}

	if (com_type == GTSD_COM_TYPE_RNNET && g_RnServoCom == NULL) 
		return Unlock(RTN_OBJECT_UNCREATED);

	if (com_type == GTSD_COM_TYPE_RNNET)
	{
		if (0xF0 != stationId)
		{
			short rtn;
			rtn = g_RnServoCom->SetStationId(stationId);
			if (rtn != 0)
			{
				return rtn;
			}
		}
		return Unlock(g_RnServoCom->GTSD_CMD_XmlReadFile(axis, pFileNameList, pFileTypeList, file_num, tpfUpdataProgressPt, ptrv, progress));
	}

	return Unlock(RTN_PARAM_ERR);

}