解决任务：
1，构建模拟采集图形直观显示。
2，烧录口添加二极管保护。

********************************************
注意点：进入低功耗后，串口的连接耗电，导致不能唤醒，即使断电再上电也不可以唤醒。解决方法：串口线拔掉。
********************************************
CAN添加过滤ID方法:
1，接收规则总数：GAFLCFG = 0x02;
2，接收规则数组总数：CAN_RX_RULE_NUM
3，接收规则数据内容：g_rxrule_table[CAN_RX_RULE_NUM][6]
********************************************
数据闪存操作函数：
void EEPROM_Manipulate(
		eel_u08 EEPROM_CMD,	
		eel_u08 EEPROM_ID,	
		eel_u08 * EEPROM_WRbuf,//写缓存	
		eel_u08 * EEPROM_RDbuf	//读缓存
		)
#define EEPROM_WR		0//只写
#define EEPROM_RD		1//只读
#define EEPROM_WR_RD		2//写+读
#define EEPROM_ID_5		5//操作id=5，type_E[6]，操作6个字节	
volatile  uint8_t  	EEPROM_WR_buf[8];
volatile  uint8_t  	EEPROM_RD_buf[8];
********************************************
100us延时程序函数(353个计数，一个计数0.283us )
********************************************







