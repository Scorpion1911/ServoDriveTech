#include "stdafx.h"
#include "RnMotionAxiMapping.h"
#include "RingNetDeviceDef.h"


CRnMotionAxiMapping::CRnMotionAxiMapping()
{
}


CRnMotionAxiMapping::~CRnMotionAxiMapping()
{
}

short CRnMotionAxiMapping::InitialMapping(CRingNetDriver* driver)
{
    return RTN_IMPOSSIBLE_ERR;

	if (driver == NULL)
		return RTN_NULL_POINT;
	m_pDriver = driver;
	m_axi_num = 0;
	m_dsp_num = 0;
	Uint16 axisNum = 0;
	for (Uint8 i = 0; i < driver->m_device_num; i++)
	{
		if ((driver->m_pRnDeviceOnline[i]->m_staion_type & 0xF000) == 0x9000) //dsp process
		{
			m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
			m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
			m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
			m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
			m_axi_num++;

			m_dsp_num++;
		}

		switch (driver->m_pRnDeviceOnline[i]->m_staion_type)
		{
        case TB_MARVIE42_8401:
			//////////////////////////////////////////////////////////////////////////
			m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
			m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
			m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
			m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
			m_axi_num++;

			m_dsp_num++;
			break;
		default:
			break;
		}
	}
	return RTN_SUCCESS;
}

Uint16  CRnMotionAxiMapping::ConvertDspToStationId(int16 dsp_id)//half divide
{	
	
	uint16 index_check = dsp_id;
	while (index_check < m_axi_num)
	{
		if (m_axi_inf[index_check].m_dsp_id == dsp_id)
		{
			return (m_axi_inf[index_check].m_station_id << 8) | m_axi_inf[index_check].m_station_ch;
		}
		if (m_axi_inf[index_check].m_dsp_id > dsp_id)
		{
			return 0xFFFF;
		}
		index_check = index_check + dsp_id - m_axi_inf[index_check].m_dsp_id;
	}
	return 0xFFFF;
	
}
