//////////////////////////////////////////////////////////////////////////////////////////
//	summary				:	Communicaiton cmd layer Define		 						//
//	file				:	ServoDriverComDll.h											//
//	Description			:	use for cmd define											//
//	lib					:	none														//
//																						//
//======================================================================================//
//		programmer:		|	date:		|	Corporation:	|		copyright(C):		//
//--------------------------------------------------------------------------------------//
//		wang.bin(1420)  |	2016/1/20	|	googoltech		|		2016 - 2019			//
//--------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef			__GTSD_SERVO_MOTION_COM_DLL__
#define			__GTSD_SERVO_MOTION_COM_DLL__	



// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SERVODRIVERCOMDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SERVODRIVERCOMDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SERVODRIVERCOMDLL_EXPORTS
#define SERVODRIVERCOMDLL_API __declspec(dllexport)
#else
#define SERVODRIVERCOMDLL_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
//#include "Basetype_def.h"
//#include "BaseReturn_def.h"
//#include "StRingNetRegDef.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
SERVODRIVERCOMDLL_API short GT_PC_RN_Open(void(*tpfUpdataProgressPt)(void*, short*), void* ptrv);
SERVODRIVERCOMDLL_API short GT_PC_RN_Close();

SERVODRIVERCOMDLL_API short RN_Initial_PCResp(void* object);
SERVODRIVERCOMDLL_API short RN_Delete_PCResp();
SERVODRIVERCOMDLL_API short GT_RN_PostPci(void* gPci, unsigned char station_id = 0xf0, unsigned char ch_id = 2);//(TPci* gPci : ch_id = 2 : RN_DSP_CH_ID)
SERVODRIVERCOMDLL_API short GT_RN_SendPci(void* gPci, unsigned char station_id = 0xf0, unsigned char ch_id = 2);//(TPci* gPci : ch_id = 2 : RN_DSP_CH_ID)
SERVODRIVERCOMDLL_API short GT_RN_PostPcie(void* gPci, unsigned char core_index, unsigned char station_id = 0xf0, unsigned char ch_id = 2);//(TPci* gPci : ch_id = 2 : RN_DSP_CH_ID)
SERVODRIVERCOMDLL_API short GT_RN_SendPcie(void* gPci, unsigned char core_index, unsigned char station_id = 0xf0, unsigned char ch_id = 2);//(TPci* gPci : ch_id = 2 : RN_DSP_CH_ID)
SERVODRIVERCOMDLL_API short RN_ComHandler(unsigned char mode, unsigned char cmd, unsigned short byte_addr, short* pData, unsigned short word_num, unsigned char des_id, unsigned char des_c, 
	unsigned char needReq = true, unsigned char addr_mod = 1, unsigned char wait_level = 0, unsigned char dsitance = 0xf0);
SERVODRIVERCOMDLL_API short RN_GetCns(void* pCns, unsigned char dsitance = 0xf0);//(COMMON_NET_STATUS* pCns : ch_id = 2 : RN_DSP_CH_ID)
SERVODRIVERCOMDLL_API short RN_FpgaUpate(char* filePath, void(*tpfUpdataProgressPt)(void*, short*), void* ptrv, short stationId = 0xf0);
SERVODRIVERCOMDLL_API short RN_CpldUpate(char* filePath, void(*tpfUpdataProgressPt)(void*, short*), void* ptrv, short stationId /*= 0xf0*/);
SERVODRIVERCOMDLL_API short RN_EepromWrite(unsigned short byte_addr, unsigned char* byte_data, unsigned short byte_num, short stationId = 0xf0);
SERVODRIVERCOMDLL_API short RN_EepromRead(unsigned short byte_addr, unsigned char* byte_data, unsigned short byte_num, short stationId /*= 0xf0*/);
SERVODRIVERCOMDLL_API short RN_EepromWriteExt(unsigned short byte_addr, unsigned char* byte_data, unsigned short byte_num, short stationId = 0xf0);
SERVODRIVERCOMDLL_API short RN_EepromReadExt(unsigned short byte_addr, unsigned char* byte_data, unsigned short byte_num, short stationId /*= 0xf0*/);
//////////////////////////////////////////////////////////////////////////
SERVODRIVERCOMDLL_API short RingNetWrFloder(void* src_file, void* des_file, unsigned char station_id);
SERVODRIVERCOMDLL_API short RingNetWrFile(void* src_file, unsigned long src_dwCreationDisposition, void* des_file, unsigned long des_dwCreationDisposition, unsigned char station_id);

//SERVODRIVERCOMDLL_API short RingNetWrFloder(LPCTSTR src_file, LPCTSTR des_file, unsigned char station_id);
//SERVODRIVERCOMDLL_API short RingNetWrFile(LPCTSTR src_file, DWORD src_dwCreationDisposition, LPCTSTR des_file, DWORD des_dwCreationDisposition, unsigned char station_id);
SERVODRIVERCOMDLL_API short RN_PcComDebug();
//////////////////////////////////////////////////////////////////////////

SERVODRIVERCOMDLL_API short RN_MotionStartPlot(short wave_num, unsigned short *wave_byte_size_list, unsigned char station_id = 0xf0, unsigned char ch_id = 2);
SERVODRIVERCOMDLL_API short RN_MotionStopPlot(unsigned char station_id = 0xf0, unsigned char ch_id = 2);//WAVE_BUF_PRM* pWaveCfg
//SERVODRIVERCOMDLL_API short RN_MotionGetWaveData(double** data, unsigned long& number, unsigned char station_id = 0xf0, unsigned char ch_id = 2);
SERVODRIVERCOMDLL_API short RN_MotionGetWaveBufferData(char* data, unsigned long& number, unsigned char station_id = 0xf0, unsigned char ch_id = 2);
//SERVODRIVERCOMDLL_API long RN_GetWaveBufferSize(unsigned char station_id = 0xf0, unsigned char ch_id = 2);

#endif