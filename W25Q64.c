#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	MySPI_Init();
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);//��id��ָ��
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//���ص����豸��id��
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//���ص����豸�ĸ߰�λ
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//���ص����豸�ĵͰ�λ
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)//����һ����������Ϳ���ִ��һ��д���ݵĲ���
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)//�ȴ���æ����
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timeout = 100000;
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)//����һ����ַ���������飬�����������Ϳ����ڸõ�ַ��д��һ��������������
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address); //����24λ��ַ
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}                               //ѭ���������������д��
	MySPI_Stop();
	
	W25Q64_WaitBusy();      //�ȴ�д�����
}

void W25Q64_SectorErase(uint32_t Address) //��������������
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);// ���Ͳ�����������
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);         //����������ַ
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)//��ȡ���ݣ���ȡû��ҳ������
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);      //���Ͷ�ȡ�ĵ�ַ
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//�û����õ�����
	}
	MySPI_Stop();
}
