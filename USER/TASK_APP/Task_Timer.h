

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


#endif
