//////////////////////////////////////////////////////////////////////////////////////////
//	summary				:	Communicaiton cmd layer Define		 						//
//	file				:	ST_GTSD_Cmd.h												//
//	Description			:	use for cmd define											//
//	lib					:	none														//
//																						//
//======================================================================================//
//		programmer:		|	date:		|	Corporation:	|		copyright(C):		//
//--------------------------------------------------------------------------------------//
//		wang.bin(1420)  |	2016/1/20	|	googoltech		|		2016 - 2019			//
//--------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef			__GTSD_ST_CMD__
#define			__GTSD_ST_CMD__
//////////////////////////////////////////////////////////////////////////

#include "AbsCom.h"
#include "ServoDriverComDll.h"
const int32						GTSD_DSP_WRITE			= 0;					  //dsp???????§Õ
const int32						GTSD_DSP_READ			= 1;

// pcdebug ????dsp??fpga???????fpga??????????dsp????????fpga?????
const int32						DSPA_COMADDR			= 0x0400;				 //????short???
const int32						DSPB_COMADDR			= 0x8400;

const int32						FPGA_DSPA_BASEADDR		= 0;
const int32						FPGA_DSPB_BASEADDR		= 0x8000;				//short???

const int32						COM_AXIS_MAX			= 240;					//???????????4

const int32						GTSD_DSP_A				= 0;
const int32						GTSD_DSP_B				= 1;
const int32						GTSD_DSP_C				= 2;

const int32						PLOTWAVE_DSPA_NUM_ADDR	= 0x0006;
const int32						PLOTWAVE_DSPB_NUM_ADDR	= 0x8006;

const int32						PLOTWAVE_DSPA_DATA_ADDR = 0x0600;//0x0800//
const int32						PLOTWAVE_DSPB_DATA_ADDR = 0x8600;//0x8800//

const int32						FPGA_VERSION = 0x0000;

//???????????FPGA???

//??dsp??????????
const int32						FPGA_RN_ENC_START_OFST	= 0x100;
const int32						FPGA_RN_ENC_END_OFST	= 0x1FF;

const int32						FPGA_RN_AIN_START_OFST	= 0x300;
const int32						FPGA_RN_AIN_END_OFST	= 0x3FF;

const int32						FPGA_RN_IO_START_OFST	= 0x400;
const int32						FPGA_RN_IO_END_OFST		= 0x4FF;

const int32						FPGA_RN_REF_START_OFST	= 0x500;
const int32						FPGA_RN_REF_END_OFST	= 0x5FF;

const int32						FPGA_RN_UART_START_OFST = 0x800;
const int32						FPGA_RN_UART_END_OFST	= 0x8FF;

//??dsp?????????

const int32						FPGA_RN_CTL_START_OFST	= 0x000;
const int32						FPGA_RN_CTL_END_OFST	= 0x0FF;

const int32						FPGA_RN_TRIG_START_OFST = 0x200;
const int32						FPGA_RN_TRIG_END_OFST	= 0x2FF;

const int32						FPGA_RN_RMT_START_OFST	= 0x600;
const int32						FPGA_RN_RMT_END_OFST	= 0x6FF;

const int32						FPGA_RN_SYS_START_OFST	= 0x700;
const int32						FPGA_RN_SYS_END_OFST	= 0x7FF;

const int32						FPGA_RN_RAM_START_OFST	= 0x1000;
const int32						FPGA_RN_RAM_END_OFST	= 0x1FFF;

const int32						FPGA_RN_ILINK_START_OFST = 0x2000;
const int32						FPGA_RN_ILINK_END_OFST	 = 0x23FF;

const int32						FPGA_RN_USER_START_OFST = 0xC000;
const int32						FPGA_RN_USER_END_OFST	= 0xFEFF;

const int32						FPGA_RN_APP_START_OFST	= 0xFF00;
const int32						FPGA_RN_APP_END_OFST	= 0xFFFF;

//////////////////////////////////////////////////////////////////////////
//uart ?????????
//pcdebug ????? ?????????????
const int32						FPGA_DSPA_UART_PCDEBUG_BASEADDR = 0x0380;
const int32						FPGA_DSPB_UART_PCDEBUG_BASEADDR = 0x8380;

//????????
const int32						FPGA_DSPA_UART_RNNET_BASEADDR	= 0x0800;
const int32						FPGA_DSPB_UART_RNNET_BASEADDR	= 0x8800;
const int32						FPGA_DSPC_UART_RNNET_BASEADDR	= 0x4800;

//????????????????????dsp??????????¡ê?????????,??????byte????????????????????????¦Ë
const int32						FPGA_UART_CONFIG_W		= 0x00; //?? / §Õ ??????????
const int32						FPGA_UART_CONFIG_R		= 0x00;
//§Õ?
//D8 - D10:????????¦Ë????
//3??b100 : 5bit;
//3??b101: 6bit;
//3??b110: 7bit
//3??b111 : 8bit;
//D11:??¦Ë????
//1??b0??1bit;
//2??b1??2bit;
//D12 - D13:???§µ??????
//2??b01????§µ?ï”
//2??b10 : ?§µ?ï”
//????????§µ?ï”
//D15 - D14 : ????

//???
//D14 - D14:????
//D15??????????
//1????????
//0????????

const int32						FPGA_UART_TX_STATE_R = 0x02; //????FIFO???????
//D9 - D0?????TX FIFO????
//D10??????TX FIFO§Õ?????¦Ë
//?1???????FIFO§Õ????
//?0???¦Ä????
//D11 : ????TX FIFO????¦Ë
//	  ?1???????FIFO????
//	  ?0???????FIFO????
//D12 : TX FIFO??????§Õ????
//D13 - D15????????

const int32						FPGA_UART_BAUDRATE_WR	= 0x04; //??????????

const int32						FPGA_UART_STARTEND_W	= 0x06;

const int32						FPGA_UART_RX_STATE_R	= 0x06;

const int32						FPGA_UART_SEND_FIFO_W	= 0x08; //???????FIFO
//??¨°???FIFO§Õ??????????????
//D7 - D0:?§Õ?????????????????¦Ë????????????§¹¦Ë??
//???? ????????¦Ë???Ú…?3??b100????????????¦Ë???5¦Ë??D4 - D0????D7 - D5??0??
//D15 - D8 : ????????0??

const int32						FPGA_UART_RECEIVE_FIFO_R = 0x08; //???????FIFO?????
//?????????16¦Ë???????????8¦Ë??§¹??
//????????¦Ë????????????§¹¦Ë??
//???? ????????¦Ë???Ú…?3??b100????8¦Ë?????????5¦Ë??§¹??????¦Ë??????


const double					FPGA_CLOCK = 125000000.0;

const int32						PLOTWAVE_GET_DATA_MAX_ONCE = 480;

//////////////////////////////////////////////////////////////////////////
//eeprom ?????????
//pcdebug ????? ?????????????
const int32						FPGA_EEPROM_PCDEBUG_BASEADDR	= 0x0800; //short??? byte???0x1000-0x10ff

//????????
const int32						FPGA_EEPROM_RNNET_BASEADDR		= 0x1000; //byte???

//byte???????????????pcdebu?????????????????pcdebug?????????short???????????????¦Ë
const int32						FPGA_EEPROM_CTL					= 0x00;   //????????
//D0:  eeprom ??§Õ????¦Ë
//	   1'b0 :§Õeeprom??????
//	   1'b1 :??eeprom??????
//D1 - D3: eeprom§à?????????6???????3'b000;
//D4: eeprom??§Õ??????¦Ë
//	  1'b1?????eeprom??§Õ??ï”
//	  1'b0: ??????
//D5: eeprom§à?????¦Ë¦Ë
//	  1'b1????D4¦Ë?1??eeprom§à?????¦Ë
//	  1'b0: ????¦Ë??
//D6 - D15????????
const int32						FPGA_EEPROM_WRITE_ADDR			= 0x02;  //§Õ????????
//D0 - D7??§Õeeprom??????
//D8 - D15????????
const int32						FPGA_EEPROM_WRITE_DAT			= 0x04;  //§Õ????????
//D0 - D7??§Õeeprom?????????????
//D8 - D15????????
const int32						FPGA_EEPROM_READ_ADDR			= 0x06;  //??????????
//D0 - D7????eeprom??????
//D8 - D15????????
const int32						FPGA_EEPROM_READ_DAT			= 0x0a;  //??????????
//D0 - D7????eeprom?????????????
//D8 - D15????????
const int32						FPGA_EEPROM_STATUS				= 0x08;  //???????
//D0: ??§Õ???????¦Ë??????§Õ???????????1'b0:???????1'b1 : ??§Õ???????
//D1 : ??§Õ?????? 1'b0:??????1'b1 : ??§Õ????

//////////////////////////////////////////////////////////////////////////
//???????????
static int16 GetCmdIDAndAxisNum(short cmdID, short motorNum);

//////////////////////////////////////////////////////////////////////////

int16 GTSD_CMD_ST_OPEN(void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress, int16 comType = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_CLOSE(int16 comType = GTSD_COM_TYPE_NET);


int16 GTSD_CMD_ST_SetServoOn(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetServoOff(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetServoState(int16 axis, SERVO_STATE* serv, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetServoTaskMode(int16 axis, int16 mode, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetServoTaskMode(int16 axis, SERVO_MODE* mode, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Write16BitByAdr(int16 axis, int16 ofst, int16 value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Read16BitByAdr(int16 axis, int16 ofst, int16* value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Write32BitByAdr(int16 axis, int16 ofst, int32 value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Read32BitByAdr(int16 axis, int16 ofst, int32* value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Write64BitByAdr(int16 axis, int16 ofst, int64 value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Read64BitByAdr(int16 axis, int16 ofst, int64* value, void* ptr, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetIdRef(int16 axis, double id_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetIdRef(int16 axis, ID_STATE* id_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetIqRef(int16 axis, double iq_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetIqRef(int16 axis, IQ_STATE* iq_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetSpdRef(int16 axis, double spd_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetSpdRef(int16 axis, SPD_STATE* spd_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetUdRef(int16 axis, double ud_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetUdRef(int16 axis, UD_STATE* ud_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetUqRef(int16 axis, double uq_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetUqRef(int16 axis, UQ_STATE* uq_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetUaRef(int16 axis, double ua_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetUaRef(int16 axis, UA_STATE* ua_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetUbRef(int16 axis, double ub_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetUbRef(int16 axis, UB_STATE* ub_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetUcRef(int16 axis, double uc_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetUcRef(int16 axis, UC_STATE* uc_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetPosAdjRef(int16 axis, double PosAdj_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetPosAdjRef(int16 axis, POS_ADJ_STATE* pos_adj_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_SetPosRef(int16 axis, int32 Pos_ref, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

/*
int16 GTSD_CMD_ST_SetFndCurr(int16 axis, int16 fnd, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetFndCurr(int16 axis, int16* fnd, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetFnqCurr(int16 axis, int16 fnq, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetFnqCurr(int16 axis, int16* fnq, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetTidCurr(int16 axis, int32 tid, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetTidCurr(int16 axis, int32* tid, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetTiqCurr(int16 axis, int32 tiq, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetTiqCurr(int16 axis, int32* tiq, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetFnVel(int16 axis, int16 fnvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetFnVel(int16 axis, int16* fnvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetTiVel(int16 axis, double tivel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetTiVel(int16 axis, double* tivel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetAbsLimVel(int16 axis, int16 abslimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetAbsLimVel(int16 axis, int16* abslimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetPosLimVel(int16 axis, int16 poslimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetPosLimVel(int16 axis, int16* poslimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetNegLimVel(int16 axis, int16 neglimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetNegLimVel(int16 axis, int16* neglimvel, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetFnPos(int16 axis, double fnpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetFnPos(int16 axis, double* fnpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetAbsLimPos(int16 axis, int16 abslimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetAbsLimPos(int16 axis, int16* abslimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetPosLimPos(int16 axis, int16 poslimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetPosLimPos(int16 axis, int16* poslimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetNegLimPos(int16 axis, int16 neglimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetNegLimPos(int16 axis, int16* neglimpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetKgaFfdPos(int16 axis, int16 kgaffdpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetKgaFfdPos(int16 axis, int16* kgaffdpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_SetKgvFfdPos(int16 axis, int16 kgvffdpos, int16 com_type = GTSD_COM_TYPE_NET);
int16 GTSD_CMD_ST_GetKgvFfdPos(int16 axis, int16* kgvffdpos, int16 com_type = GTSD_COM_TYPE_NET);
*/
int16 GTSD_CMD_ST_Set16bitFPGAByAddr(int16 dsp_number, int16 com_addr, int16 value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Get16bitFPGAByAddr(int16 dsp_number, int16 com_addr, int16* pvalue, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Set32bitFPGAByAddr(int16 dsp_number, int16 com_addr, int32 value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Get32bitFPGAByAddr(int16 dsp_number, int16 com_addr, int32* pvalue, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_SetWaveBuf(int16 dsp_number, WAVE_BUF_PRM wave, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_GetWaveBuf(int16 dsp_number, tWaveBufCmd* ctrlword, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_GetWaveData(int16 dsp_number, int16* read_num, int16** data, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_ClearFpgaFifo(int16 dsp_number, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Write16BitByAdr(int16 axis, int16 ofst, int16 value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Read16BitByAdr(int16 axis, int16 ofst, int16* value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Write32BitByAdr(int16 axis, int16 ofst, int32 value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Read32BitByAdr(int16 axis, int16 ofst, int32* value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Write64BitByAdr(int16 axis, int16 ofst, int64 value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_Fram_Read64BitByAdr(int16 axis, int16 ofst, int64* value, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_FlashWrite(int16 axis, INTEL_HEX_FRAME* packet, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_FlashRead(int16 axis, INTEL_HEX_FRAME* packet_w, INTEL_HEX_FRAME* packet_r, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_FlashErase(int16 axis, int16 blockNum, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_InterruptSwitch(int16 axis, int16 int_switch, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_ProcessorGeneralFunc(int16 axis, GENERALFUNCTION* gefunc, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ResetSystem(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_CheckResetFinish(int16 axis, bool& flag_finish, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_ReadProcessorVersion(int16 axis, Uint16& ver, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ReadFirmwareVersion(int16 axis, Uint16& ver, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_ClrAlarm(int16 axis,int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

//////////////////////////////////////////////////////////////////////////
int16 GTSD_CMD_ST_ReadLogAlarmCode(int16 axis, Uint32* alarmCode, Uint16& lenth, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ReadLogAlarmTimes(int16 axis, Uint16* alarmTimes, Uint16& lenth, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
//////////////////////////////////////////////////////////////////////////

//uart boot
int16 GTSD_CMD_ST_OpenSerialPort(int16 axis, int32 baudRate, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_CloseSerialPort(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ReadSerialPort(int16 axis, Uint8 *buf, int32 length, int32 *length_read, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_WriteSerialPort(int16 axis, Uint8 *buf, int32 length, int32 *length_written, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);


//?????????
int16 GTSD_CMD_ST_ScanRnTopology(int16 com_type = GTSD_COM_TYPE_RNNET, int16 stationId = 0xff);

//EEPROM ??§Õ,????
int16 GTSD_CMD_ST_ReadEEPROM(int16 axis, int32& ofst, int8* value, int16& num, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_WriteEEPROM(int16 axis, int32& ofst, int8* value, int16& num, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ClearEEPROM(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);

int16 GTSD_CMD_ST_ResetFPGA(int16 axis, int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);
int16 GTSD_CMD_ST_ConfigEEPROM(int16 com_type = GTSD_COM_TYPE_NET, int16 stationId = 0xf0);


#endif
