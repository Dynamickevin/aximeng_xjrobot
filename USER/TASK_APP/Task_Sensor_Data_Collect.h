#ifndef TASK_SENSOR_DATA_COLLECT_H
#define TASK_SENSOR_DATA_COLLECT_H


//extern uint16 OSVersion(void);

extern OS_EVENT *Semaphore1;
extern OS_EVENT	*Mbox1;


extern void Task_Sensor_Data_Collect(void *pdata);

extern OS_EVENT *Sensor_Data_Collect_Queue;		//



#endif
