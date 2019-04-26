#pragma once
#include "ServoDriverComDef.h"

class CPlotWaveSrc
{
public:
	CPlotWaveSrc();
	~CPlotWaveSrc();
protected:
//	WAVE_BUF_PRM m_wave;
	unsigned short m_frame_head_num;															// Frame head spacing number
	unsigned short m_wave_size;
	unsigned short m_frame_head_cnt;
	char* m_pBuffer;
	unsigned long m_buffer_max;
	unsigned long m_buffer_wr_addra;
	unsigned long m_buffer_rd_addra;
	unsigned short m_max_read_byte_num;

public:
	short Initial(char* buffer, const unsigned long buffer_size, unsigned short wave_size);
	short GetWaveData(char* pData, unsigned short* pByte_num);
	short AddWaveData(char* pData);
};

