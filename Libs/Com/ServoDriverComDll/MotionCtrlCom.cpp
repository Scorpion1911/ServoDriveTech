#include "stdafx.h"
#include "MotionCtrlCom.h"



CMotionCtrlCom::CMotionCtrlCom()
{
}


CMotionCtrlCom::~CMotionCtrlCom()
{
}

short CMotionCtrlCom::Initial(CRingNetInterface* pDriver)
{
	if (pDriver == NULL)
	{
		return RTN_NULL_POINT;
	}

	m_pDriver = pDriver;
	return RTN_SUCCESS;
}

short CMotionCtrlCom::PostPci(TPci* gPci, Uint8 station_id)
{
	const int MAX_RETYR = 1;
	short rtn, i;
	if (m_pDriver == NULL)
	{
		return RTN_OBJECT_UNCREATED;
	}
	if (gPci == NULL)
	{
		return RTN_NULL_POINT;
	}

	// ����У���
	gPci->checksum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		gPci->checksum += gPci->data[i];
	}
	// ��������д���־
	gPci->flag = PCI_PC_FINISH;

	Uint16 byte_num = sizeof(TPci);
	Uint16 dword_num = (byte_num + 3) >> 2;
	int16 pData[250];
	memcpy(pData, gPci, byte_num);

	rtn = m_pDriver->RnNetComHandler(RN_PACKET_PDU_WR_ASK, RN_USER_PROTOCOL_MOTION, (int16*)pData, dword_num << 1, station_id, RN_DSP_CH_ID, 0, RN_ADDR_SAME);
	return rtn;
}
short CMotionCtrlCom::SendPci(TPci* gPci, Uint8 station_id)
{
	const int MAX_RETYR = 1;
	short rtn, i;

	// ����У���
	gPci->checksum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		gPci->checksum += gPci->data[i];
	}

	// ��������д���־
	gPci->flag = PCI_PC_FINISH;

	Uint16 byte_num = (gPci->len + 3) << 1;
	Uint16 dword_num = (byte_num + 3) >> 2;

	int16 pData[250];
	memcpy(pData, gPci, byte_num);
	rtn = m_pDriver->RnNetComHandler(RN_PACKET_PDU_WR_ASK, RN_USER_PROTOCOL_MOTION, (int16*)pData, dword_num << 1, station_id, RN_DSP_CH_ID, 1, RN_ADDR_SAME);
	/////////////////////////////////////////////////
	memcpy(gPci, pData, 4);
	if (PCI_DSP_FINISH != gPci->flag)
	{
//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("PCI_DSP_FINISH != gPci.flag !\n")));
		return CMD_ERROR_READ_BLOCK;
	}
	if ((gPci->len < 0) || (gPci->len > 32))
	{
//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("( gPci.len < 0 )||( gPci.len > 32 ) !\n")));
		return CMD_ERROR_READ_LEN;
	}
	//////////////////////////////////////
	byte_num = (gPci->len + 3) << 1;
	memcpy(gPci,pData, byte_num);
	////////////////
	short sum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		sum += gPci->data[i];
	}

	// ���У���
	if (sum == gPci->checksum)
	{
		//return 0;//
//		pRingNet->m_device_list[pRingNet->m_dsp_tag_id].m_dsp_cmd_index = 0x7F & (pRingNet->m_device_list[pRingNet->m_dsp_tag_id].m_dsp_cmd_index + 1);
		return gPci->data[0];
	}
	else
	{
//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("checksum err !\n")));
		return CMD_ERROR_READ_CHECKSUM;
	}
	return RTN_SUCCESS;
}

short CMotionCtrlCom::PostPcie(TPci* gPci, Uint8 core_index, Uint8 station_id)
{
	const int MAX_RETYR = 1;
	short rtn, i;
	if (m_pDriver == NULL)
	{
		return RTN_OBJECT_UNCREATED;
	}
	if (gPci == NULL)
	{
		return RTN_NULL_POINT;
	}

	// ����У���
	gPci->checksum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		gPci->checksum += gPci->data[i];
	}
	// ��������д���־
	switch (core_index)
	{
	case 1: 
		gPci->flag = 0x5a;
		break;
	case 2:
		gPci->flag = 0xa5;
		break;
	default:
		break;
	}
	gPci->flag = PCI_PC_FINISH;

	Uint16 byte_num = sizeof(TPci);
	Uint16 dword_num = (byte_num + 3) >> 2;
	int16 pData[250];
	memcpy(pData, gPci, byte_num);

	rtn = m_pDriver->RnNetComHandler(RN_PACKET_MAIL_WR_ASK, RN_USER_PROTOCOL_MOTION, (int16*)pData, dword_num << 1, station_id, RN_DSP_CH_ID, 0, RN_ADDR_SAME);
	return rtn;
}
short CMotionCtrlCom::SendPcie(TPci* gPci, Uint8 core_index, Uint8 station_id)
{
	const int MAX_RETYR = 1;
	short rtn, i;

	// ����У���
	gPci->checksum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		gPci->checksum += gPci->data[i];
	}

	// ��������д���־
	gPci->flag = PCI_PC_FINISH;

	Uint16 byte_num = (gPci->len + 3) << 1;
	Uint16 dword_num = (byte_num + 3) >> 2;

	int16 pData[250];
	memcpy(pData, gPci, byte_num);
	rtn = m_pDriver->RnNetComHandler(RN_PACKET_MAIL_WR_ASK, RN_USER_PROTOCOL_MOTION, (int16*)pData, dword_num << 1, station_id, RN_DSP_CH_ID, 1, RN_ADDR_SAME);
	/////////////////////////////////////////////////
	memcpy(gPci, pData, 4);
	if (PCI_DSP_FINISH != gPci->flag)
	{
		//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("PCI_DSP_FINISH != gPci.flag !\n")));
		return CMD_ERROR_READ_BLOCK;
	}
	if ((gPci->len < 0) || (gPci->len > 32))
	{
		//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("( gPci.len < 0 )||( gPci.len > 32 ) !\n")));
		return CMD_ERROR_READ_LEN;
	}
	//////////////////////////////////////
	byte_num = (gPci->len + 3) << 1;
	memcpy(gPci, pData, byte_num);
	////////////////
	short sum = gPci->len;
	for (i = 0; i < gPci->len; ++i)
	{
		sum += gPci->data[i];
	}

	// ���У���
	if (sum == gPci->checksum)
	{
		//return 0;//
		//		pRingNet->m_device_list[pRingNet->m_dsp_tag_id].m_dsp_cmd_index = 0x7F & (pRingNet->m_device_list[pRingNet->m_dsp_tag_id].m_dsp_cmd_index + 1);
		return gPci->data[0];
	}
	else
	{
		//		RETAILMSG(_GOOGOLTECH_RING_NET_DEBUG_, (TEXT("checksum err !\n")));
		return CMD_ERROR_READ_CHECKSUM;
	}
	return RTN_SUCCESS;
}