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
	MySPI_SwapByte(W25Q64_JEDEC_ID);//读id的指令
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//返回的是设备的id号
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//返回的是设备的高八位
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);//返回的是设备的低八位
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)//调用一次这个函数就可以执行一次写数据的操作
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)//等待繁忙结束
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

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)//给定一个地址，还有数组，还有数量。就可以在该地址下写入一定个数的数据了
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address); //发送24位地址
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}                               //循环，将数组的数据写入
	MySPI_Stop();
	
	W25Q64_WaitBusy();      //等待写入完成
}

void W25Q64_SectorErase(uint32_t Address) //擦除扇区的数据
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);// 发送擦除扇区命令
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);         //发送扇区地址
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)//读取数据，读取没有页的限制
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);      //发送读取的地址
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);//置换有用的数据
	}
	MySPI_Stop();
}
