

#ifndef TASK_TIMER_H
#define TASK_TIMER_H


extern OS_EVENT *timerQueue;				//定时器邮箱
extern void Task_Timer(void *pdata);


extern void CopyBuffer(void *SouPointer,void *DesPointer,uint16 Length);

void Clear_Buffer(void *pointer,uint16 length);

void DoBatUsingGetFilter(void);

void Battery(void);

void zt_msg_send_real_time_pkg(void);

extern void Boot_HWDT_Feed(void);

extern void HWDT_Feed(void);

void System_State_LED(void);

//SWD模式下载初始化函数，系统启动后，需要加入此函数，不然无法使用SWD模式下载。
void SWD_Download_init(void);


//所有关于机器人的数据都存放在该结构体
typedef struct{
    char		sendbuf[1000];		//发送缓冲区
    u16     counter;
    u16		icmd_len;			//命令长度
    int     trans_vals[6];      //用于辅助命令数据的生成
    //机器人相关状态
    u8 nTimeForNoLinuxHeartIn ; //没有心跳包接收
}ZT_INFO_Power_Board_TYPE;

void ack_with_Power_Board_Uart(uint8 com,char *buf, uint16 len);

void uart2_Power_Board_send(char *sp, uint16 len);

void Send_P0wer_Board_Master_State(FunctionalState Newstate);

#endif
