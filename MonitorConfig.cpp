/*
MonitorConfig.c文件监控系统配置参数设定函数的实现
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include<unistd.h>
#include "Drv.h"
#include"lldrv.h"
#include "MonitorConfig.h"

extern BOOL  globalMonitorInitFlag;/*703监测系统初始化标志*/

/************读取monitor配置信息*************/
int  getMonitorConfigValue( float * p_getOIL1_LMT,			/*滑油压力低I值报警值**************/
                            float * p_getOIL2_LMT,			/*滑油压力低II值报警值*************/
                            float * p_getSPD1_LMT,			/*超速I值报警值********************/
                            float * p_getSPD2_LMT			/*超速II值报警值*******************/
                                                                        )
{
        FILE	*monitorConfigFilePtr = NULL;
        float 	getOIL1_LMT,getOIL2_LMT,getSPD1_LMT,getSPD2_LMT;

        MonitorInit();

        monitorConfigFilePtr=fopen("/home/data.txt","r");//fopen函数返回一个指针
        if(monitorConfigFilePtr==NULL)//文件打开失败，直接默认值
        {
                perror("can not open the file to write!\n");
                getOIL1_LMT=0.07;
                getOIL2_LMT=0.06;
                getSPD1_LMT=11000;
                getSPD2_LMT=13000;
                return 0;
        }
        else//文件打开成功，输入四个阈值
        {
                fscanf(monitorConfigFilePtr,"%f,%f,%f,%f",
                        &getOIL1_LMT,&getOIL2_LMT,&getSPD1_LMT,&getSPD2_LMT);//从文件monitorConfigFilePtr读取四个变量值
						//判断是否合理，不合理就按照默认值
                if(getOIL1_LMT>5||getOIL1_LMT<0.01)
                        getOIL1_LMT=0.07;
                if(getOIL2_LMT>5||getOIL2_LMT<0.01)
                        getOIL2_LMT=0.06;
                if(getSPD1_LMT>30000||getSPD1_LMT<300)
                        getSPD1_LMT=11000;
                if(getSPD2_LMT>30000||getSPD2_LMT<300)
                        getSPD2_LMT=13000;
                fclose(monitorConfigFilePtr);
        }
		//将阈值存到指针变量
        *p_getOIL1_LMT=getOIL1_LMT;
        *p_getOIL2_LMT=getOIL2_LMT;
        *p_getSPD1_LMT=getSPD1_LMT;
        *p_getSPD2_LMT=getSPD2_LMT;
        return 1;
}

/*
monitor帮助信息
*/
void monitorHelp(void)
{
    MonitorInit();
    char    usr_command[25];
    float	GET_OIL1_LMT ;
    float	GET_OIL2_LMT ;
    float	GET_SPD1_LMT ;
    float	GET_SPD2_LMT ;
    char getMonitorConfigValue_command[]="getMonitorConfigValue";//读取监测配置数据
    char setMonitorConfigFile_command[]="setMonitorConfigFile";  //设置监测配置数据
    char monitorStart_command[]="monitorStart";                  //开始监测
    char readMonitorConfigFile_command[]="readMonitorConfigFile";//读取监测配置文件

    printf("******************************************************************\n\r");
    printf("\nIf you do not want to chang the value,please type :\nmonitorStart\n\r");
    printf("***********************************************************\n\r");
    printf("If you want to set the value, use the function \n\rsetMonitorConfigFile then run monitorStart\n\r");
    printf("If you want to check the current value, please type:\n\rreadMonitorConfigFile\n\r");
    printf("******************************************************************\n\r");
    printf("example:\n\rsetMonitorConfigFile\n\rmonitorStart\n\r");
    printf("******************************************************************\n\r");
    printf("Please do as the example!\n\r");
    while(1)
    {
        gets(usr_command);

        if(strcmp(usr_command,getMonitorConfigValue_command)==0)
        {
               getMonitorConfigValue(&GET_OIL1_LMT,&GET_OIL2_LMT,&GET_SPD1_LMT,&GET_SPD2_LMT );
               setMonitorConfigFile();
        }
        else if(strcmp(usr_command,setMonitorConfigFile_command)==0)
        {
              setMonitorConfigFile();
        }
       else if(strcmp(usr_command,monitorStart_command)==0)
        {
              return;
        }
       else if(strcmp(usr_command,readMonitorConfigFile_command)==0)
        {
              readMonitorConfigFile();
        }
        fflush(stdin);
        memset(usr_command,0,sizeof(usr_command));
    }
}

/*
*配置monitor参数
*/
void setMonitorConfigFile(void)
{
        float	oil1Lmt,oil2Lmt,spd1Lmt,spd2Lmt;
        char 	str[200];
        FILE	*monitorConfigFilePtr = NULL;

        MonitorInit();

        printf("\nplease input OIL1_LIMT(example 0.07):");
        scanf("%f",&oil1Lmt);//键盘输入相应阈值
        printf("\nplease input OIL2_LIMT(example 0.06):");
        scanf("%f",&oil2Lmt);
        printf("\nplease input SPD1_LIMT(example 11000):");
        scanf("%f",&spd1Lmt);
        printf("\nplease input SPD2_LIMT(example 13000):");
        scanf("%f",&spd2Lmt);

        sprintf(str,"%f,%f,%f,%f",oil1Lmt,oil2Lmt,spd1Lmt,spd2Lmt);//四个阈值打印到str

        monitorConfigFilePtr=fopen("/home/data.txt","w");
        if(monitorConfigFilePtr==NULL)
        {
                perror("can not open the file to write!\n");
        }
        else//打开文件成功
        {
                fprintf(monitorConfigFilePtr,"%s",str);//str中的数据输出到文件
                fclose(monitorConfigFilePtr);
                printf("\n\n\rthe current value yout set to monitorConfigDataFile.txt is:\n");
                readMonitorConfigFile();
                printf("\nif the value is not right,then do setMonitorConfigFile again!\n");
        }
}

/*
*获得当前的全局控制变量临界值的值
*/
void readMonitorConfigFile(void)
{
        float getVal[4];

         MonitorInit();
        getMonitorConfigValue(&getVal[0],&getVal[1],&getVal[2],&getVal[3]);

        printf( "\ncurrent value:\n OIL1_LMT = %f\n OIL2_LMT = %f\n SPD1_LMT = %f\n SPD2_LMT = %f\n",getVal[0],getVal[1],getVal[2],getVal[3] );
}
/*
*监测系统初始化
*/
//void monitorInit(void)
//{
//        if (globalMonitorInitFlag==false)
//        {
////                docInit();
////                correctTime();
////                isa2vmeHwInit();
//                globalMonitorInitFlag=true;
//        }
//}
