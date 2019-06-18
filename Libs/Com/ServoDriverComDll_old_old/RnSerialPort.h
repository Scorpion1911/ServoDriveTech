#pragma once

#include "Basetype_def.h"
#include "RnServoAxiMapping.h"
#include "RingNetInterface.h"
// 
// struct StChunk
// {
// 	struct StChunk *next;
// 	Uint32 addr;
// 	vector<Uint8> data;
// 	StChunk() : next(NULL), addr(0) {}
// };
// 
// struct StImage
// {
// 	StChunk *first_chunk, *last_chunk;
// 	Uint32 len;
// 	Uint32 max_addr;
// 	Uint32 range_start, range_len;
// 	Uint32 alignment;
// 	StImage(Uint32 range_start, Uint32 range_len, Uint32 alignment)
// 		: first_chunk(NULL), last_chunk(NULL), len(0), max_addr(0),
// 		range_start(range_start), range_len(range_len), alignment(alignment) {}
// 	~StImage();
// };
// 
// class CRnSerialPort
// {
// public:
// 	CRnSerialPort();
// 	~CRnSerialPort();
// private:
// //	const int16 IHEX_ADDR_ERROR = 5;
// 
// 	enum Action {
// 		AC_NONE = 0,
// 		AC_PROGRAM,
// 		AC_LOAD,
// 		AC_WRITE_KEY,
// 		AC_ERASE,
// 		AC_ERASE_USER,
// 	} action;
// protected:
// 	CRnServoAxiMapping* m_pMapping;
// 	CRingNetInterface* m_pDriver;
// protected:
// protected:
// 	const int16						GTSD_DSP_WRITE = 0;					  //dsp����Ķ�д	
// 	const int16						GTSD_DSP_READ = 1;
// 
// 	// pcdebug ��ַΪdsp��fpga֮��ͨ�ŵ�fpga��ַ����ͬ��dsp��Ӧ��ͬ��fpga��ַ��
// 	const Uint16					DSPA_COMADDR = 0x0400;				 //��ַΪshort��ַ
// 	const Uint16					DSPB_COMADDR = 0x8400;
// 
// 	const Uint16					FPGA_DSPA_BASEADDR = 0;
// 	const Uint16					FPGA_DSPB_BASEADDR = 0x8000;				//short��ַ
// 
// 	const int16						COM_AXIS_MAX = 240;					//��������Ϊ4	
// 
// 	const int16						GTSD_DSP_A = 0;
// 	const int16						GTSD_DSP_B = 1;
// 	const int16						GTSD_DSP_C = 2;
// 
// 	const Uint16						PLOTWAVE_DSPA_NUM_ADDR = 0x0006;
// 	const Uint16						PLOTWAVE_DSPB_NUM_ADDR = 0x8006;
// 
// 	const Uint16						PLOTWAVE_DSPA_DATA_ADDR = 0x0600;//0x0800//
// 	const Uint16						PLOTWAVE_DSPB_DATA_ADDR = 0x8600;//0x8800//
// 
// 	const Uint16						FPGA_VERSION = 0x0000;
// 
// 	//�Ȼ����ܷ��ʵ�FPGA��ַ
// 	//////////////////////////////////////////////////////////////////////////
// 	//uart ����ַ����
// 	//pcdebug ��ַ�� �Ȼ����ҵĵ�ַ��ͬ
// 	const Uint16					FPGA_DSPA_UART_PCDEBUG_BASEADDR = 0x0380;
// 	const Uint16					FPGA_DSPB_UART_PCDEBUG_BASEADDR = 0x8380;
// 
// 	//�Ȼ�����ַ
// 	const Uint16					FPGA_DSPA_UART_RNNET_BASEADDR = 0x0800;
// 	const Uint16					FPGA_DSPB_UART_RNNET_BASEADDR = 0x8800;
// 
// 	//�����ַ��ƫ�Ƶ�ַ������dsp��Ӧ��ƫ��һ�£�����ַ��ͬ,ƫ�ƶ���byte��ַ������ʹ�õ�ʱ����Ҫ����һλ
// 	const int16						FPGA_UART_CONFIG_W = 0x00; //�� / д ����������
// 	const int16						FPGA_UART_CONFIG_R = 0x00;
// 	//дʱ
// 	//D8 - D10:��������λ��
// 	//3��b100 : 5bit;
// 	//3��b101: 6bit;
// 	//3��b110: 7bit
// 	//3��b111 : 8bit;
// 	//D11:ֹͣλ����
// 	//1��b0��1bit;
// 	//2��b1��2bit;
// 	//D12 - D13:��żУ������
// 	//2��b01����У�飻
// 	//2��b10 : żУ�飻
// 	//��������У�飻
// 	//D15 - D14 : ����
// 
// 	//��ʱ
// 	//D14 - D14:����
// 	//D15��ģ��ʹ��״̬
// 	//1��ģ��ʹ��
// 	//0��ģ��ر�
// 
// 	const int16						FPGA_UART_TX_STATE_R = 0x02; //����FIFO״̬�Ĵ���
// 	//D9 - D0����ǰTX FIFO����
// 	//D10������TX FIFOд����־λ
// 	//Ϊ1��ʾ����FIFOд����
// 	//Ϊ0��ʾδ����
// 	//D11 : ����TX FIFO�ձ�־λ
// 	//	  Ϊ1��ʾ����FIFOΪ�գ�
// 	//	  Ϊ0��ʾ����FIFO�ǿգ�
// 	//D12 : TX FIFO������д���
// 	//D13 - D15��������
// 
// 	const int16						FPGA_UART_BAUDRATE_WR = 0x04; //����������
// 
// 	const int16						FPGA_UART_STARTEND_W = 0x06;
// 
// 	const int16						FPGA_UART_RX_STATE_R = 0x06;
// 
// 	const int16						FPGA_UART_SEND_FIFO_W = 0x08; //���ڷ���FIFO
// 	//Ӧ�ò���FIFOд��׼�����͵�����
// 	//D7 - D0:Ϊд������ݣ���������λ�����ã�ȷ����Чλ��
// 	//���� ��������λ���趨Ϊ3��b100������������λ��Ϊ5λ��D4 - D0����D7 - D5��0��
// 	//D15 - D8 : ��������0��
// 
// 	const int16						FPGA_UART_RECEIVE_FIFO_R = 0x08; //���ڽ���FIFO�Ĵ���
// 	//���ڽ��յĵ�16λ�������ݣ���8λ��Ч��
// 	//��������λ�����ã�ȷ����Чλ��
// 	//���� ��������λ���趨Ϊ3��b100����8λ�������ݸ�5λ��Ч������λ������
// 
// 
// 	const double					FPGA_CLOCK = 125000000.0;
// 
// public:
// 	short Initial(CRnServoAxiMapping* pMapping, CRingNetInterface* pDriver);
// public:
// 	short open(int16 axis, int32 baudRate);
// 	short close(int16 axis);
// 	short read(int16 axis, Uint8 *buf, int32 length, int32 *length_read);
// 	short write(int16 axis, Uint8 *buf, int32 length, int32 *length_written);
// 
// 	short downloadBootStream(int16 axis, string& inputKey, int16 cmd, string& filename, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress);
// 	short send_ldr_img(int16 axis, string& inputKey, string& filename, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress);
// 	int16 parse_key(string& inputKey, Uint8 key[16]);
// 	StImage* read_img(string& filename, Uint32 range_start, Uint32 range_len, Uint32 alignment);
// 	int32 pad(int32 len, int32 align);
// 	short do_autobaud(int16 axis, Uint8 c, Uint8 *buf, Uint32 len);
// 	short do_ldr_autobaud(int16 axis);
// 	short send_erase_cmd(int16 axis, void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress);
// 
// private:
// 	bool status;
// 	int32 baudrate;
// };
// 
// 
// int hex_read_callback(void *ctxt, const Uint8 *buf, Uint32 addr, Uint32 len);

