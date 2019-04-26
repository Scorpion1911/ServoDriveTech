#include "stdafx.h"
#include "PlotWaveSrc.h"
#include "BaseReturn_def.h"

#define FRAME_HEAD (0x5555AAAA)
#define FRAME_HEAD_SIZE (4)
#define FRAME_HEAD_NUM (256)
#define MAX_READ_NUM (256)

CPlotWaveSrc::CPlotWaveSrc()
{
	m_frame_head_num = FRAME_HEAD_NUM;															// Frame head spacing number
	m_wave_size = 0;
	m_frame_head_cnt = 0;
	m_pBuffer = NULL;
	m_buffer_max = 0;
	m_buffer_wr_addra = 0;
	m_buffer_rd_addra = 0;
	m_max_read_byte_num = MAX_READ_NUM;
}

CPlotWaveSrc::~CPlotWaveSrc()
{

}

//////////////////////////////////////////////////////////////////////////
short CPlotWaveSrc::Initial(char* buffer, const unsigned long buffer_size, unsigned short wave_size)
{
	if (buffer == NULL)
	{
		return RTN_NULL_POINT;
	}
	m_pBuffer = buffer;
	m_wave_size = wave_size;
	m_buffer_max = buffer_size;
	return RTN_SUCCESS;
}
short CPlotWaveSrc::GetWaveData(char* pData, unsigned short* pByte_num)
{
	unsigned long 	i;
	unsigned long buffer_cnt;
	
	buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra;
	if (m_buffer_wr_addra > m_buffer_rd_addra)
	{
		buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra;
	}
	else
	{
		buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra + m_buffer_max;
	}

	if (buffer_cnt < m_max_read_byte_num)
	{
		*pByte_num = 0;
		return RTN_SUCCESS;
	}

	if (m_buffer_rd_addra + m_max_read_byte_num <= m_buffer_max) //not over loop
	{
		memcpy(pData, m_pBuffer + m_buffer_rd_addra, m_buffer_max);
	}
	else
	{
		i = m_buffer_max - m_buffer_rd_addra;
		memcpy(pData, m_pBuffer + m_buffer_rd_addra, i);
		memcpy(pData + i, 0, m_max_read_byte_num - i);
	}
	m_buffer_rd_addra = (m_buffer_rd_addra + m_max_read_byte_num) % m_buffer_max;
	*pByte_num = m_max_read_byte_num;

	return	RTN_SUCCESS;
}
short CPlotWaveSrc::AddWaveData(char* pData)
{
	unsigned long 	i;
	unsigned long buffer_cnt;

	buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra;
	if (m_buffer_wr_addra > m_buffer_rd_addra)
	{
		buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra;
	}
	else
	{
		buffer_cnt = m_buffer_wr_addra - m_buffer_rd_addra + m_buffer_max;
	}


	if (buffer_cnt + m_wave_size + FRAME_HEAD_SIZE >= m_buffer_max) //overflow
	{
		return RTN_MALLOC_FAIL;
	}

	if (m_buffer_wr_addra + m_wave_size < m_buffer_max) //not over loop
	{
		memcpy(m_pBuffer + m_buffer_wr_addra, pData, m_wave_size);
		m_buffer_wr_addra = m_buffer_wr_addra + m_wave_size;
	}
	else
	{
		i = m_buffer_max - m_buffer_wr_addra - 1;
		memcpy(m_pBuffer + m_buffer_wr_addra, pData, i);
		memcpy(0, pData + i, m_wave_size-i);
		m_buffer_wr_addra = m_wave_size - i;
	}
	m_frame_head_cnt++;
	//////////////////////////////////////////////////////////////////////////
	if (m_frame_head_cnt >= m_frame_head_num)
	{
		if (m_buffer_wr_addra + FRAME_HEAD_SIZE < m_buffer_max) //not over loop
		{
			memcpy(m_pBuffer + m_buffer_wr_addra, pData, m_wave_size);
			m_buffer_wr_addra = m_buffer_wr_addra + FRAME_HEAD_SIZE;
		}
		else
		{
			i = m_buffer_max - m_buffer_wr_addra - 1;
			memcpy(m_pBuffer + m_buffer_wr_addra, pData, i);
			memcpy(0, pData + i, FRAME_HEAD_SIZE - i);
			m_buffer_wr_addra = FRAME_HEAD_SIZE - i;
		}
		m_frame_head_cnt = 0;
	}
	return	RTN_SUCCESS;
}