#ifndef MONITORFUNCTION_H
#define MONITORFUNCTION_H

extern void relaySet(int chan,unsigned short  val);/*继电器操作，chan继电器通道，val通道设定置*/
extern void m35Data(unsigned short* dataBuf);/*读取两个AD转换值,dataBuf返回数组，大小32个数据*/
extern int compare(const void *arg1, const void *arg2);/*arg1和arg2指向的两个数大小比较规则，第一个比第二个大返回1,反之返回0*/
extern void timerHandler(void);/*定时器，定时释放信号量*/
extern void tStop10s(void);/*保护动作以单脉冲形式输出10秒，上位系统来的"保护投入"无效（接点接通）时立即无条件解除脉冲输出*/
/*extern void tPause2m(void);*//*监测系统暂停2分钟*/
extern void getTime(char *tchar);/*tchar返回系统时间*/
extern void formatStrLength(char *str, int num);/*给字符串str添加空格到长度为num*/
void SWaction();

#endif // MONITORFUNCTION_H
