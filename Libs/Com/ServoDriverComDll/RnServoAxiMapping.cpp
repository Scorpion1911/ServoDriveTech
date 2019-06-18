#include "stdafx.h"
#include "RnServoAxiMapping.h"
#include "RingNetDeviceDef.h"


CRnServoAxiMapping::CRnServoAxiMapping()
{
	m_axi_num = 0;
	m_dsp_num = 0;
	m_pDriver = NULL;
//+++++++++++++++++++++2019.05.28modification+++++++++++++++++++
    m_map_type = 1;
//--------------------------------------------------------------
//	m_map_type = 0;
//==============================================================
	m_station_id = 0xF0;
	for (int i = 0; i < RN_MAX_SERVO_AXI_NUM;i++)
	{
		m_axi_inf[i].m_station_id = RN_INVAVLID_STATION_ID;//
	}
}


CRnServoAxiMapping::~CRnServoAxiMapping()
{
}

short CRnServoAxiMapping::InitialMapping(CRingNetDriver* driver)
{
	if (driver == NULL)
		return RTN_NULL_POINT;
	m_pDriver = driver;
	m_axi_num = 0;
	m_dsp_num = 0;
	Uint16 axisNum = 0;
//	for (Uint8 i = 0; i < driver->m_device_num; i++)
//	{
//        if(TB_GTSD13 == (driver->m_pRnDeviceOnline[i]->m_staion_type))
//        {
//            continue;
//        }
//		if (0x8000 == ((driver->m_pRnDeviceOnline[i]->m_staion_type) & 0xF000))
//		{
//			axisNum =( ((driver->m_pRnDeviceOnline[i]->m_staion_type)&0x0F00)>>8);
//			for (int j = 0; j < axisNum; j++)
//			{
//				m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
//				m_axi_inf[m_axi_num].m_axi_id_in_dsp = (j%2);
//				m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
//				if ((0==j) || (1==j))
//				{
//					m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
//				}
//				else if ((2 == j) || (3 == j))
//				{
//					m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
//				}
//				else if ((4 == j) || (5 == j))
//				{
//					m_axi_inf[m_axi_num].m_station_ch = RN_EXT_CH_ID;
//				}
//				else
//				{
//					m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
//				}
//				m_axi_num++;
//				if (0 == (j % 2))
//				{
//					m_dsp_num++;
//				}
//			}
//		}
//	}
    for (Uint8 i = 0; i < driver->m_device_num;i++)
    {
        if(driver->m_pRnDeviceOnline[i] == NULL)
        {
            return RTN_OBJECT_UNCREATED;
        }
        m_station_inf[driver->m_pRnDeviceOnline[i]->m_station_id] = m_axi_num;
        switch (driver->m_pRnDeviceOnline[i]->m_staion_type)
        {
        case TB_GTSD13_8100 :
            break;
        case TB_GTD10_8140 :
        case TB_GTHD11_8130 :
        case TB_GTSD15_8120 :
        case TB_MARVIE11_8110 :
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;
            m_dsp_num++;
            break;
        case TB_GTD20_8210 :
        case TB_MARVIE21_8200 :
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            break;
        case TB_MARVIE41_8400:
        case TB_MARVIE42_8401:
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            //////////////////////////////////////////////////////////////////////////
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            break;
    case TB_MARVIE61_8600 :
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            //////////////////////////////////////////////////////////////////////////
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            //////////////////////////////////////////////////////////////////////////
            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_EXT_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 0;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
            m_axi_inf[m_axi_num].m_station_ch = RN_EXT_CH_ID;
            m_axi_inf[m_axi_num].m_axi_id_in_dsp = 1;
            m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
            m_axi_num++;

            m_dsp_num++;
            //////////////////////////////////////////////////////////////////////////
            break;
        default:
            if (0x8000 == ((driver->m_pRnDeviceOnline[i]->m_staion_type) & 0xF000))
            {
                axisNum =( ((driver->m_pRnDeviceOnline[i]->m_staion_type)&0x0F00)>>8);
                for (int j = 0; j < axisNum; j++)
                {
                    m_axi_inf[m_axi_num].m_station_id = driver->m_pRnDeviceOnline[i]->m_station_id;
                    m_axi_inf[m_axi_num].m_axi_id_in_dsp = (j%2);
                    m_axi_inf[m_axi_num].m_dsp_id = m_dsp_num;
                    if ((0==j) || (1==j))
                    {
                        m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
                    }
                    else if ((2 == j) || (3 == j))
                    {
                        m_axi_inf[m_axi_num].m_station_ch = RN_PCI_CH_ID;
                    }
                    else if ((4 == j) || (5 == j))
                    {
                        m_axi_inf[m_axi_num].m_station_ch = RN_EXT_CH_ID;
                    }
                    else
                    {
                        m_axi_inf[m_axi_num].m_station_ch = RN_DSP_CH_ID;
                    }
                    m_axi_num++;
                    if (1 == (j % 2))
                    {
                        m_dsp_num++;
                    }
                }
                if(axisNum & 0x1 == 1)
                {
                    m_dsp_num++;
                }
            }
            break;
        }
    }
	return RTN_SUCCESS;
}
Uint16 CRnServoAxiMapping::ConvertAxiToStationId(int16 axi_id)
{
	if (0 == m_map_type)
	{
		if (axi_id < m_axi_num)
		{
			return (m_axi_inf[axi_id].m_station_id << 8) | m_axi_inf[axi_id].m_station_ch;
		}
	} 
	else if(1 == m_map_type)
	{
        axi_id = m_station_inf[m_station_id] + axi_id;
        if (axi_id < m_axi_num)
        {
            return (m_axi_inf[axi_id].m_station_id << 8) | m_axi_inf[axi_id].m_station_ch;
        }

//		Uint8 station_ch = RN_DSP_CH_ID;
//        if (0x8000 == ((m_pDriver->m_pRnDevice[m_station_id]->m_staion_type) & 0xF000))
//        {
//			Uint16 axisNum = (((m_pDriver->m_pRnDevice[m_station_id]->m_staion_type) & 0x0F00) >> 8);
//			if (axi_id<axisNum)
//			{
//				if ((0 == axi_id) || (1 == axi_id))
//				{
//					station_ch = RN_DSP_CH_ID;
//				}
//				else if ((2 == axi_id) || (3 == axi_id))
//				{
//					station_ch = RN_PCI_CH_ID;
//				}
//				else if ((4 == axi_id) || (5 == axi_id))
//				{
//					station_ch = RN_EXT_CH_ID;
//				}
//				else
//				{
//					station_ch = RN_DSP_CH_ID;
//				}
//			}
//		}

// 		switch (m_pDriver->m_pRnDevice[m_station_id]->m_staion_type)
// 		{
// 		case TB_MARVIE11:
// 			station_ch = RN_DSP_CH_ID;
// 			break;
// 		case TB_MARVIE21:
// 			station_ch = RN_DSP_CH_ID;
// 			break;
// 		case TB_MARVIE41:
// 		case TB_MARVIE42:
// 			if ((0 == axi_id) || (1 == axi_id))
// 			{
// 				station_ch = RN_DSP_CH_ID;
// 			} 
// 			else if((2 == axi_id) || (3 == axi_id))
// 			{
// 				station_ch = RN_PCI_CH_ID;
// 			}
// 			else
// 			{
// 				station_ch = RN_DSP_CH_ID;
// 			}
// 			break;
// 		case TB_MARVIE61:
// 			if ((0 == axi_id) || (1 == axi_id))
// 			{
// 				station_ch = RN_DSP_CH_ID;
// 			}
// 			else if ((2 == axi_id) || (3 == axi_id))
// 			{
// 				station_ch = RN_PCI_CH_ID;
// 			}
// 			else if ((4 == axi_id) || (5 == axi_id))
// 			{
// 				station_ch = RN_EXT_CH_ID;
// 			}
// 			else
// 			{
// 				station_ch = RN_DSP_CH_ID;
// 			}
// 			//////////////////////////////////////////////////////////////////////////
// 			break;
// 		default:
// 			break;
// 		}
//		return ((m_station_id << 8) | station_ch);
	}
	else
	{
		return 0xFEFF;//0xFFFF modify by huang.lz in 2018/2/5
	}
	return 0xFEFF;//0xFFFF modify by huang.lz in 2018/2/5
}

Uint16 CRnServoAxiMapping::ConvertAxiToSubAxiId(int16 axi_id)
{
	if (0 == m_map_type)
	{
		if (axi_id < m_axi_num)
		{
			return m_axi_inf[axi_id].m_axi_id_in_dsp;
		}
	}
	else if (1 == m_map_type)
	{
        axi_id = m_station_inf[m_station_id] + axi_id;
        if (axi_id < m_axi_num)
        {
            return m_axi_inf[axi_id].m_axi_id_in_dsp;
        }
		
// 		switch (m_pDriver->m_pRnDevice[m_station_id]->m_staion_type)
// 		{
// 		case TB_MARVIE11:
// 			axis_num = 1;
// 			break;
// 		case TB_MARVIE21:
// 			axis_num = 2;
// 			break;
// 		case TB_MARVIE41:
// 		case TB_MARVIE42:
// 			axis_num = 4;
// 			break;
// 		case TB_MARVIE61:
// 			axis_num = 6;
// 			break;
// 		default:
// 			axis_num = 0;
// 			break;
// 		}
//		if (axi_id < axis_num)
//		{
//			return (axi_id%2);
//		}
	}
	else
	{
		return 0xFEFF;//0xFFFF modify by huang.lz in 2018/2/5
	}

	return 0xFFFF;
}

Uint16 CRnServoAxiMapping::ConvertAxiToDspId(int16 axi_id)
{
	if (0 == m_map_type)
	{
		if (axi_id < m_axi_num)
		{
			return m_axi_inf[axi_id].m_dsp_id;
		}
	}
	else if (1 == m_map_type)
	{
        axi_id = m_station_inf[m_station_id] + axi_id;
        if (axi_id < m_axi_num)
        {
            return m_axi_inf[axi_id].m_dsp_id;
        }
//		Uint16 axis_num = 0;
//		if (0x8000 == ((m_pDriver->m_pRnDevice[m_station_id]->m_staion_type) & 0xF000))
//		{
//			axis_num = (((m_pDriver->m_pRnDevice[m_station_id]->m_staion_type) & 0x0F00) >> 8);
//		}
// 		switch (m_pDriver->m_pRnDevice[m_station_id]->m_staion_type)
// 		{
// 		case TB_MARVIE11:
// 			axis_num = 1;
// 			break;
// 		case TB_MARVIE21:
// 			axis_num = 2;
// 			break;
// 		case TB_MARVIE41:
// 		case TB_MARVIE42:
// 			axis_num = 4;
// 			break;
// 		case TB_MARVIE61:
// 			axis_num = 6;
// 			break;
// 		default:
// 			axis_num = 0;
// 			break;
// 		}
//		if (axi_id < axis_num)
//		{
//			return (axi_id / 2);
//		}
	}
	return 0xFFFF;
}

Uint16  CRnServoAxiMapping::ConvertDspToStationId(int16 dsp_id)
{
//	int16 axi_id = dsp_id << 1;
//	return ConvertAxiToStationId(axi_id);
    for(int i = 0; i<m_axi_num; i++)
    {
       if(m_axi_inf[i].m_dsp_id == dsp_id)
       {
           return (m_axi_inf[i].m_station_id << 8) | m_axi_inf[i].m_station_ch;
       }
    }
    return 0xFEFF;

// 	if (axi_id < m_axi_num)
// 	{
// 		return (m_axi_inf[axi_id].m_station_id << 8) | m_axi_inf[axi_id].m_station_ch;
// 	}
// 	return 0xFFFF;
}

short CRnServoAxiMapping::GetDspAxiStation(StDspAxiStationInf& inf, int16 axi_id)
{
	if (axi_id >= m_axi_num)
	{
		return RTN_PARAM_ERR;
	}
	inf = m_axi_inf[axi_id];
	return RTN_SUCCESS;
}


Uint16 CRnServoAxiMapping::ConvertFpgaByteAddrByDspId(Uint16 dsp_id, Uint16 fpga_byte_address)
{
//	int16 axi_id = dsp_id << 1;
	Uint16 base_address = 0;
//	if (axi_id < m_axi_num)
//	{
    Uint8 des_ch;
    switch (dsp_id) {   //该值表示在特定某个站点的某个DSP，而不是全部站点中DSP
    case 0:
        des_ch = RN_DSP_CH_ID;
        break;
    case 1:
        des_ch = RN_PCI_CH_ID;
        break;
    case 2:
        des_ch = RN_EXT_CH_ID;
        break;
    default:
        des_ch = RN_DSP_CH_ID;
        break;
    }
//		Uint8 des_ch = m_axi_inf[axi_id].m_station_ch;
    switch (des_ch)
    {
    case RN_DSP_CH_ID:
        base_address = 0x0000;
        break;
    case RN_PCI_CH_ID:
        base_address = 0x8000;
        break;
    case RN_EXT_CH_ID:
        base_address = 0xC000;
        break;
    default:
        base_address = 0x0000;
        break;
    }
//	}

	return base_address + fpga_byte_address;
}
