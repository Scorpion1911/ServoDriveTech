//////////////////////////////////////////////////////////////////////////////////////////
//	summary				:	Firmware download 		 									//
//	file				:	FirmwareDl.h												//
//	Description			:	use for Firmware download									//
//	lib					:	none														//
//																						//
//======================================================================================//
//		programmer:		|	date:		|	Corporation:	|		copyright(C):		//
//--------------------------------------------------------------------------------------//
//		wang.bin(1420)  |	2016/1/20	|	googoltech		|		2016 - 2019			//
//--------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GTSD_FIRMWARE_DL_
#define	_GTSD_FIRMWARE_DL_

#include "Basetype_def.h"

class CFirmwareDL
{
public:
	CFirmwareDL();
	~CFirmwareDL();

public:	
	static const int32		BUF_LEN			= 200;
	static const int32		FF_LEN			= 200;
	static const int16		DELAY_TIMES		= 100;

	static const int32		CMD_READ = 0;
	static const int32		CMD_WRITE = 1;
	static const int32		CMD_PRET_ON = 2;
	static const int32		CMD_PRET_OFF = 3;
	static const int32		CMD_ERASE = 4;
	static const int32		CMD_SECTOR_ERASE = 5;

	static const int32		FPGA_ALTERA = 0;
	static const int32		FPGA_XILINX = 1;

	static const int32		SECTOR_SIZE = 1 << 16;// 2 ^ 16;// 64Kbyte
	static const int16		MAX_OP_WORD_NUM = 100;

	//pcdebug偏移地址，基地址是0
	static const Uint16		REMOTE_FPGA_CTL = 0x37F;
	static const Uint16		REMOTE_FPGA_DATA_START = 0x300;
	static const Uint16		REMOTE_FPGA_DATA_END = 0x37C;
	static const Uint16		REMOTE_FPGA_FILE_START = 0x374;

	//等环网偏移地址
	static const Uint16		RN_REMOTE_FPGA_CTL = (0x7F*2);
	static const Uint16		RN_REMOTE_FPGA_DATA_START = (0x00);
	static const Uint16		RN_REMOTE_FPGA_DATA_END = (0x7C * 2);
	static const Uint16		RN_REMOTE_FPGA_FILE_START = (0x74 * 2);

	Uint32 m_byte_write;
	Uint32 m_addr_ofst;
	uint16 sector_erase;
	static const Uint16						DSPA_COMADDR = 0x0400;				 //地址为short地址
	static const Uint16						DSPB_COMADDR = 0x8400;

	static const Uint16						FPGA_DSPA_BASEADDR = 0;
	static const Uint16						FPGA_DSPB_BASEADDR = 0x8000;				//short地址

	static const Uint16						FPGA_RN_RMT_START_OFST = 0x600;
	static const Uint16						FPGA_RN_RMT_END_OFST = 0x6FF;
	static const Uint16						FPGA_RN_COMM_FUNC_CODE = 0xFF0E;

	int16 ResetVar();
	int16 SetRemoteUpdataStartbit(int16 com_type, int16 stationId);
	int16 CheckRemoteUpdataState(int16 com_type, int16 stationId, int32 delaytimes = DELAY_TIMES);
	int16 GetFPGAInfo(int16 com_type, int16 stationId, int16& FPGAType,uint32& flash_ofst_addr, uint16& sector_erase);
	int16 SetRemoteUpdataEnableBit(int16 com_type, int16 stationId);
	int16 SetRemoteUpdataReadRequest(int16 com_type, Uint32 flash_addr, Uint16 iLength, int16 stationId);
	int16 ProtectOff(int16 com_type, int16 stationId );
	int16 ProtectOn(int16 com_type, int16 stationId);
  	int16 EraseData(int16 com_type, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress, int16 stationId);
	int16 EraseFPGAData(int16 com_type, uint32 byte_num, uint32 offset_addr, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress, int16 stationId);
	int16 GetFPGAByteNum(string pFileName, uint32& byte_num);
	int16 EraseSectorData(int16 com_type, uint32 byte_address, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress, int16 stationId);
	int16 GetFpgaFlashData(int16 com_type, Uint32 flash_addr, int16 *Getbuf, Uint16 iLength, int16 stationId);
	int16 SendFpgaFlashData(int16 com_type, Uint32 flash_addr, int16 *Sendbuf, Uint16 iLength, int16 stationId);
	int32 CheckFFNumber(short* buffer, int lenth);

    int16 WriteFPGAFileToFlash(int16 com_type, string pFileName, void(*tpfUpdataProgressPt)(void*,int16*),void* ptrv,int16& progress,int16 stationId);
	int16 ReadFPGADataToFile(int16 com_type, string FileName, Uint32 Bytelenth, int16 stationId);
};
extern CFirmwareDL* g_firmwareDl;
#endif
