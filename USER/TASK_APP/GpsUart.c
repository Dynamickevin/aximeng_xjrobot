/**************************************************************
  COPYRIGHT (C), BOLIGTHED Co., Ltd.  
 *Filename:  		GpsUart.c
 ***************************************************************/
#include <includes.h>
#include "GpsUart.h"

StateGPS gGPS;


/*

char* box_str_get_line_rt_src(char* _out,char* strIn,int _outMaxCnt)
{
    char* endPos = box_str_find_char_pos( strIn , '\r');
    int dataLen = 0;
    if ( endPos == NULL )
    {
        endPos = box_str_find_char_pos( strIn , '\n');
    }

    if ( endPos == NULL )
    {
        dataLen = box_str_len(strIn);
    }
    else
	{
        dataLen = endPos - strIn;
    }
    if ( dataLen > _outMaxCnt )
    {
        dataLen = _outMaxCnt;
    }
    CopyBuffer(strIn,_out,dataLen);
    _out[dataLen] = 0;
    return (strIn+dataLen);
}

*/

/************************************************* 
*Return:		
*DESCRIPTION: 提取出GPS时间信息
* $GNRMC,015835.00,A,3945.11111,N,11608.87560,E,0.199,,181117,,,A*62 
*************************************************/
bool GpsParaTimeDateForGNRMC(char* strIn)
{
    int i;
	int hour;
    strIn = box_str_find_char_pos(strIn,',');
    //
    if ( strIn == NULL )
    {
        return false;
    }
    strIn++;
    for ( i=0 ; i<6 ; i++ )
    {
        if ( (strIn[i]<='9') && (strIn[i]>='0') )
		{
		
		}
        else
		{
            return false;
        }
    }
	hour = (strIn[0]-'0')*10 + (strIn[1]-'0');
	gGPS.gpsTime_hour = (hour + 8) % 24;					//格林尼治时间转化为北京时间，24小时制
    gGPS.gpsTime_min  = (strIn[2]-'0')*10 + (strIn[3]-'0');
    gGPS.gpsTime_sec  = (strIn[4]-'0')*10 + (strIn[5]-'0');
    //
    for ( i=0 ; i<8 ; i++ )
    {
        strIn = box_str_find_char_pos(strIn,',');
        if ( strIn == NULL )
        {
            return false;
        }
        strIn++;
    }
    for ( i=0 ; i<6 ; i++ )
    {
        if ( (strIn[i]<='9') && (strIn[i]>='0') ){}
        else
		{
            return false;
        }
    }
    gGPS.gpsTime_mday = (strIn[0]-'0')*10 + (strIn[1]-'0');
    gGPS.gpsTime_mon  = (strIn[2]-'0')*10 + (strIn[3]-'0') - 1;
    gGPS.gpsTime_year  = (strIn[4]-'0')*10 + (strIn[5]-'0') + 2000-1900;
    //gGPS.gpsTimeSec = mktime(&gGPS.gpsTime) + 8*3600;

    //printf("Get Time %d-%d-%d %d:%d:%d\r\n", 
    //    gGPS.gpsTime.tm_year,
    //    gGPS.gpsTime.tm_mon,
    //    gGPS.gpsTime.tm_mday,
    //    gGPS.gpsTime.tm_hour,
    //    gGPS.gpsTime.tm_min ,
    //    gGPS.gpsTime.tm_sec 
    //    );
    
		/*GetTimeSecondUs(&i,NULL);//zs 20181205 del
    i -= gGPS.gpsTimeSec ;
    if ( (i<-30) || (i>30) )
    {
        SetTime_Second( gGPS.gpsTimeSec );
    }*/
     	 
	//memcpy( &gGPS.gpsTime , GetSysTime() , sizeof(gGPS.gpsTime) );  //zs
    return true;
}


/************************************************* 
*Return:		
*DESCRIPTION: 	提取出GPS位置信息
//$GNGGA,015835.00,3945.11111,N,11608.87560,E,1,08,1.03, 65.2 ,M,-9.2,M,,*6D
*************************************************/
bool GpsParaPosDateFromGNGGA(char* strIn)
{
    int i;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL ) 
	{ 
		return false; 
	} 
	strIn++;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL )
	{ 
		return false; 
	}
	strIn++;
    //纬度 1111.11111
    if( strIn[4] != '.' ) 
	{ 
		return false; 
	}
    for ( i=0 ; i<4 ; i++ )
    {
        if ( (strIn[i]<='9') && (strIn[i]>='0') )
		{
			
		}
        else
		{
            return false;
        }
    }
	CopyBuffer(strIn,gGPS.lat_str,12);
	gGPS.lat_str[12] = 0;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL ) 
	{ 
		return false; 
	}
	strIn++;
    gGPS.lat_n_s = *strIn; 
	strIn++;
    if( *strIn != ',' ) 
	{ 
		return false;
	}
	strIn++;

    // 经度，11111.11111
    if( strIn[5] != '.' )
	{ 
		return false; 
	}
    for ( i=0 ; i<5 ; i++ )
    {
        if ( (strIn[i]<='9') && (strIn[i]>='0') )
		{

		}
        else
		{
            return false;
        }
    }
	CopyBuffer(strIn,gGPS.lng_str,13);
	gGPS.lng_str[13] = 0;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL )
	{ 
		return false; 
	} 
	strIn++;
    gGPS.lon_w_e = *strIn; 
	strIn++;
    if( *strIn != ',' ) 
	{
		return false; 
	}
	strIn++;  
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL ) 
	{
		return false; 
	} 
	strIn++;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL )
	{ 
		return false; 
	} 
	strIn++;
    strIn = box_str_find_char_pos(strIn,',');
    if ( strIn == NULL ) 
	{ 
		return false; 
	} 
	strIn++;

    if ( (strIn[0]<='9') && (strIn[0]>='0') )
	{

	}
    else
	{
        //printf("%s",strIn);
        return false;
    }
	CopyBuffer(strIn,gGPS.height_str,6);
	gGPS.height_str[6] = 0;
	
    gGPS.f_height = atof(gGPS.height_str);
    gGPS.f_lng    = (gGPS.lng_str[0]-'0')*100 + (gGPS.lng_str[1]-'0')*10 + (gGPS.lng_str[2]-'0') 
        + (atof(gGPS.lng_str+3)/60);
    gGPS.f_lat    =  (gGPS.lat_str[0]-'0')*10 + (gGPS.lat_str[1]-'0') 
        + (atof(gGPS.lat_str+2)/60);

    if ( gGPS.lat_n_s != 'N' )
    {
        gGPS.f_lat = -gGPS.f_lat;
    }
    if ( gGPS.lon_w_e != 'E' )
    {
        gGPS.f_lng = -gGPS.f_lng;
    }
    
   // printf("Get POS %s %f %f %f\r\n",  gGPS.strGNGGA ,
   //     gGPS.f_lng ,
   //     gGPS.f_lat ,
   //     gGPS.f_height
   //     );
    return true;
}

/************************************************* 
*Return:		
*DESCRIPTION: GPS
*************************************************/
/*
void GPS_recv_data_CB(UartDriverType* pUart)//zs 20181205 del
{
    char* find_end;
    int len_cur_line;
    char* str = (char*)(pUart->recvBuf);
    pUart->recvBuf[pUart->recvDataCnt] = 0;

    if (gGPS.getDataState == 1)  //鏌ユ壘 $ 宸茬粡鍏跺畠绗﹀彿鏁版嵁
    {
        find_end = box_str_get_line_rt_src(gGPS.strGNRMC+gGPS.getDataCnt,str,(sizeof(gGPS.strGNRMC)-2-gGPS.getDataCnt) );
        if ( (*find_end=='\r') || (*find_end=='\n') )
        {
            //printf("%s\r\n",gGPS.strGNRMC);
            gGPS.bCurGetTime = GpsParaTimeDateForGNRMC(gGPS.strGNRMC);
        }
        else{
            gGPS.getDataState = 0;
            return;
        }
    }
    else if (gGPS.getDataState == 2){
        find_end = box_str_get_line_rt_src(gGPS.strGNGGA+gGPS.getDataCnt,str,(sizeof(gGPS.strGNGGA)-2-gGPS.getDataCnt) );
        if ( (*find_end=='\r') || (*find_end=='\n') )
        {
            //printf("%s\r\n",gGPS.strGNGGA);
            gGPS.bCurGetPos = GpsParaPosDateFromGNGGA(gGPS.strGNGGA);
        }
        else{
            gGPS.getDataState = 0;
            return;
        }
    }
    gGPS.getDataState = 0;

    while( *str ){
        str = box_str_find_char_pos( str , '$' );
        if ( str == NULL )
        {
            return;
        }
        str++;
        if ( 0 == box_str_cmp_left( str , "GNRMC" ) )
        {
            find_end = box_str_get_line_rt_src(gGPS.strGNRMC,str,(sizeof(gGPS.strGNRMC)-2) );
            if ( (*find_end=='\r') || (*find_end=='\n') )
            {
                //printf("%s\r\n",gGPS.strGNRMC);
                gGPS.bCurGetTime = GpsParaTimeDateForGNRMC(gGPS.strGNRMC);
            }
            else if( (find_end-str) < (sizeof(gGPS.strGNRMC)-2) ){
                gGPS.getDataState = 1;
                gGPS.getDataCnt   = find_end-str;
                return;
            }
            else{
                return;
            }
            str = find_end;
        }
        else if ( 0 == box_str_cmp_left( str , "GNGGA" ) )
        {
            find_end = box_str_get_line_rt_src(gGPS.strGNGGA,str,(sizeof(gGPS.strGNGGA)-2) );
            if ( (*find_end=='\r') || (*find_end=='\n') )
            {
                //printf("%s\r\n",gGPS.strGNGGA);
                gGPS.bCurGetPos =  GpsParaPosDateFromGNGGA(gGPS.strGNGGA);
            }
            else if( (find_end-str) < (sizeof(gGPS.strGNGGA)-2) ){
                gGPS.getDataState = 2;
                gGPS.getDataCnt   = find_end-str;
                return;
            }
            else{
                return;
            }
            str = find_end;
        }
    }
}
*/
