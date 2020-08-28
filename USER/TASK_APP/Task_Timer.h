

#ifndef TASK_TIMER_H
#define TASK_TIMER_H


extern OS_EVENT *timerQueue;				//��ʱ������
extern void Task_Timer(void *pdata);


extern void CopyBuffer(void *SouPointer,void *DesPointer,uint16 Length);

void Clear_Buffer(void *pointer,uint16 length);

void DoBatUsingGetFilter(void);

void Battery(void);

void zt_msg_send_real_time_pkg(void);

extern void Boot_HWDT_Feed(void);

extern void HWDT_Feed(void);

void System_State_LED(void);

//SWDģʽ���س�ʼ��������ϵͳ��������Ҫ����˺�������Ȼ�޷�ʹ��SWDģʽ���ء�
void SWD_Download_init(void);


//���й��ڻ����˵����ݶ�����ڸýṹ��
typedef struct{
    char		sendbuf[1000];		//���ͻ�����
    u16     counter;
    u16		icmd_len;			//�����
    int     trans_vals[6];      //���ڸ����������ݵ�����
    //���������״̬
    u8 nTimeForNoLinuxHeartIn ; //û������������
}ZT_INFO_Power_Board_TYPE;

void ack_with_Power_Board_Uart(uint8 com,char *buf, uint16 len);

void uart2_Power_Board_send(char *sp, uint16 len);

void Send_P0wer_Board_Master_State(FunctionalState Newstate);

#endif
