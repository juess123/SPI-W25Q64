# SPI-W25Q64

使用SPI写入W25Q64的全部必文件。其中W25Q64_Ins.h是w25q64的所需的命令文件。

在mian主函数中使用这些函数即可看到现象
  uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04}; 
  uint8_t ArrayRead[4];
  
  W25Q64_SectorErase(0x000000);擦除命令
	W25Q64_PageProgram(0x000000, ArrayWrite, 4); 在0x000000这个地址写入ArrayWrite数组的4个数据
	
	W25Q64_ReadData(0x000000, ArrayRead, 4);在0x000000这个地址读取ArrayWrite数组的4个数据
	
	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);显示写入的数据
	
	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);显示读取的数据

 记得添加OLED头文件
