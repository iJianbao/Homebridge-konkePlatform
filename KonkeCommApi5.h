/********************************************************************
 版权所有 (C), 2012-2017, 杭州控客信息技术有限公司
 ********************************************************************
 文 件 名   : KonkeCommApi.h
 版 本 号   : 初稿
 作    者   :
 生成日期   : 2015年8月5日
 最近修改   :
 功能描述   :
 函数列表   :
 修改历史   :
 日    期   : 2015年8月5日
 修改内容   : 创建文件
 ********************************************************************/
#ifndef _KONKE_COMM_API_H
#define _KONKE_COMM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#define  SSID_LEN_MAX   32
#define  KEY_LEN_MAX    32
#define  IP_LEN_MAX     16
#define  TIMEZONE_LEN_MAX 16
#define  MAC_LEN_MAX 18
#define  PASSWORLD_LEN_MAX 16
#define  DEVICE_STAT_LEN_MAX 32
#define  DEVICE_TYPE_LEN_MAX 16
#define  PLUG_NAME_LEN_MAX 16

#define  FAIL       0
#define  SUCCESS    1

#define  VOICE_URL "http://voicetest.ikonke.com:8180/VoiceServlet/voiceServiceServlet?filepath="

#define MAIN_CONT_TIMER_ID "1071"
#define MAIN_CONT_DELAY_ID "1072"

#define SUB_NODE_1_TIMER_ID "1073"
#define SUB_NODE_2_TIMER_ID "1074"
#define SUB_NODE_3_TIMER_ID "1075"
#define SUB_NODE_4_TIMER_ID "1076"

typedef void (*KonkeCallBackFun)(char *pcStr);
typedef void* (ThreadPoint)(void * pPoint);

typedef struct CONFIG_INFO
{
    char  szSSID[SSID_LEN_MAX];    
    char  szKey[KEY_LEN_MAX];     
    char  szGateway[IP_LEN_MAX];
    char  szTimezone[TIMEZONE_LEN_MAX];
    char  szBSSID[MAC_LEN_MAX];
    KonkeCallBackFun pCallBack;
}CONFIG_INFO_S;

#define TIMER_INFO_COMM_LEN_MAX 8
#define TIME_LEN_MAX 20
#define REPEAT_LEN_MAX 32
typedef struct DEVICE_TIMER_INFO
{
    char  szTimerId[TIMER_INFO_COMM_LEN_MAX];           //定时器id（每个定时器id不同 每个插座定时器的id范围是1－1022  1023是延时id）
    char  szTimerEnable[TIMER_INFO_COMM_LEN_MAX];       //定时器开关 （y－代表开  n－代表关）
    char  szOpenTime[TIME_LEN_MAX];                     //定时器开时间（格式为：yyyy-MM-dd-HH:mm:ss）
    char  szOpenEnable[TIMER_INFO_COMM_LEN_MAX];        //定时器开操作的使能（y－代表开  n－代表关）
    char  szCloseTime[TIME_LEN_MAX];                    //定时器关时间（格式为：yyyy-MM-dd-HH:mm:ss
    char  szCloseEnable[TIMER_INFO_COMM_LEN_MAX];       //定时器关操作的使能（y－代表开  n－代表关）
    char  szRepeat[REPEAT_LEN_MAX];                     //定时器重复 1:周日 2:周一 3:周二 4:周三 5:周四 6:周五 7：周六  0:代表不重复。型如（1,3,4,7）代表周日  周二  周三 周六 重复
}DEVICE_TIMER_INFO_S;//定时参数结构

#define MAX_DEVICE_TIMER_NUM 64
typedef struct DEVICE_TIMER_INFO_LIST
{
    int nTimerNum;                                                  //每个设备的定时器个数（k2定时器最多为8组  mini k最多为32组）
    DEVICE_TIMER_INFO_S stDeviceTimerInfo[MAX_DEVICE_TIMER_NUM];    //每个定时器的具体参数结构
    
}DEVICE_TIMER_INFO_LIST_S;//定时list

typedef struct DEVICE_INFO
{
    char  szDeviceMacAddr[MAC_LEN_MAX];                             //设备的mac地址（设备的唯一标识）
    char  szDeviceIpAddr[IP_LEN_MAX];                               //设备的ip地址（在局域网分配的IP地址）
    char  szDevicePasswd[PASSWORLD_LEN_MAX];                        //设备的密码（设备自身隐含的密码）
    char  szDeviceStat[DEVICE_STAT_LEN_MAX];                        //设备继电器状态（open：开  close：关）
    char  szDeviceType[DEVICE_TYPE_LEN_MAX];                        //设备类型 k2  mini_b  mini_w  分别代表 二代，mini pro，mini k
    char  szPlug1[PLUG_NAME_LEN_MAX];                               //插件1 名字 ir_module(红外) rf_module（射频） tp_module（环境）  rt_module（人体）
    char  szPlug2[PLUG_NAME_LEN_MAX];                               //插件2 名字 无插件为空
}DEVICE_INFO_S;//设备信息

#define MAX_DEVICE_ELEMENT 64
typedef struct DEVICE_INFO_LIST
{
    int nDeviceNum;                                                 //设备个数（当前局域网中的设备个数）
    DEVICE_INFO_S stDeviceInfo[MAX_DEVICE_ELEMENT];                 //每个设备的具体参数结构
    
}DEVICE_INFO_LIST_S;//设备list

#define TP_MODULE_COMM_LEN_MAX 8
typedef struct TP_MODULE_INFO
{
    char szTemper[TP_MODULE_COMM_LEN_MAX];               //温度
    char szHumi[TP_MODULE_COMM_LEN_MAX];                 //湿度
    char szLight[TP_MODULE_COMM_LEN_MAX];                //光照
    
}TP_MODULE_INFO_S;//tp_module 模块
    
typedef struct DIDCOVERY_PARA
{
    KonkeCallBackFun pCallBack;                         //回调函数
    char szBroadcastAddr[IP_LEN_MAX];                   //广播地址(形如192.168.100.255)
    
}DIDCOVERY_PARA_S; //开起发现线程传递的参数
    
#define KLIGHT_COMM_LEN_MAX 8
/*
    r 	颜色R分量。范围0~255。
    g	颜色G分量。范围0~255。
    b	颜色B分量。范围0~255。
    w	颜色W分量。范围0~100。
    param	对应模式的参数。日光，彩光模式下为色盘偏移角度（0-2pai），流光模式下为速度（10、30、50、70、90），总共5档。
    Smooth	变色时是否平滑过渡。彩光模式为0，其他模式为1.
*/
typedef struct KLIGHT_PARA
{
    char szRed[KLIGHT_COMM_LEN_MAX];
    char szGreen[KLIGHT_COMM_LEN_MAX];
    char szBlue[KLIGHT_COMM_LEN_MAX];
    char szW[KLIGHT_COMM_LEN_MAX];
    char szMode[KLIGHT_COMM_LEN_MAX];   //模式。1 – 日光，2 – 彩光，3 – 睡眠，4 – 音乐， 5 – 流光。
    char szParam[KLIGHT_COMM_LEN_MAX];
    char szSmooth[KLIGHT_COMM_LEN_MAX];
}KLIGHT_PARA_S;
    
/* Klight 模式 */
typedef enum
{
    KLIGHT_MODE_SUN         = 1,            /* 日光模式 */
    KLIGHT_MODE_COLOR       = 2,            /* 彩光模式 */
    KLIGHT_MODE_SLEEP       = 3,            /* 睡眠模式 */
    KLIGHT_MODE_MUSIC       = 4,            /* 音乐模式 */
    KLIGHT_MODE_FLOW        = 5             /* 流光模式 */
}KLIGHT_MODE_E;

    
#define MAX_KLIGHT_INFO_ELEMENT 4
typedef struct KLIGHT_INFO_SET
{
    KLIGHT_MODE_E enKlightCurMode;                                  //当前模式
    char szKlightStat[KLIGHT_COMM_LEN_MAX];                         //当前彩灯的开关状态open/close
    KLIGHT_PARA_S szKlightInfo[MAX_KLIGHT_INFO_ELEMENT];            //数组包含每个模式的参数 [0] －－日光参数  [1]-－彩光参数  [2]-－－睡眠参数  [3]-－流光参数
    
}KLIGHT_INFO_SET_S;//灯泡参数集合
    
typedef struct AIRCLEANER_PARA
{
    int nPower;                 //电源0:关闭，1:开启
    int nFulizi;                //负离子0:关闭，1:开启
    int nWind;                  //1、2、3、4 默认为1
    int nRunTime;               //加湿器运行时间，单位小时
    int nTimer;                 //定时关机时间，单位小时，0代表此功能关闭。设置范围为（1-12） 
    int nAirVol;                //净化空气量，单位为立方米
    int nAutoMode;              //自动模式（负离子＋风速的一个组合）0:关闭 1：开启
    int nSleepMode;             //睡眠 0:关闭，1:开启
    int nAirQuality;            //0、1、2、3 一次为： 优，良，轻度污染，重度污染
    int nAirIndex;              //室内空气指数
}AIRCLEANER_PARA_S;
    
extern DEVICE_INFO_LIST_S g_stDeviceInfoList;                              //设备信息列表（全局变量，当前局域网中的设备列表）
extern DEVICE_TIMER_INFO_LIST_S g_stDeviceTimerInfoList;                   //设备定时器列表（全局变量，每个设备的定时器列表）
extern KLIGHT_INFO_SET_S g_stKlightInfoSet;                                //彩灯各个模式的参数

/********************************************************************
 函数名: CreatThread
 功能描述: 创建线程
 输入参数:  pthread_t *pnThreadId,ThreadPoint pFunction,void* pParam
 输出参数:
 返回值:  1:成功  0失败
 其它说明:
 日期: 2015年05月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int CreatThread(pthread_t *pnThreadId,ThreadPoint pFunction,void* pParam);


/********************************************************************
 函数名: StartDiscovery
 功能描述: 开启发现的线程
 输入参数:  DIDCOVERY_PARA_S pstDiscoveryPara;
 输出参数:
 返回值:
 其它说明:pCallBack回调函数
 发现时需要开线程拉起 StartDiscovery 成功之后会回调pCallBack 自动回收资源
 日期: 2015年11月09日
 作者: liwentao
 修改历史:
 ********************************************************************/
void *StartDiscovery(void *pstDiscoveryPara);

/********************************************************************
 函数名: StopDiscovery
 功能描述: 停止发现
 输入参数: KonkeCallBackFun pCallBack
 输出参数:
 返回值:  1:成功  0失败
 其它说明:
 日期: 2015年11月09日
 作者: liwentao
 修改历史:
 ********************************************************************/
int StopDiscovery(KonkeCallBackFun pCallBack);
    
/********************************************************************
 函数名: GetDevList
 功能描述: 获取设备列表
 输入参数:
 输出参数:char szOut[]
 返回值:  1:成功  0失败
 其它说明:
 日期: 2017年11月14日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetDevList(char szOut[]);

/********************************************************************
 函数名: OpenDeviceRelay
 功能描述: 开启设备继电器
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  0:成功  -1失败
 其它说明:
 日期: 2015年10月14日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *OperateDeviceRelay(char *pcDeviceMacAddr, char *pcCmd);

/********************************************************************
 函数名: CheckDeviceRelay
 功能描述: 获取设备继电器状态
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  NULL:失败     open/close  状态
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *CheckDeviceRelay(char *pcDeviceMacAddr);

/********************************************************************
 函数名: CheckDeviceTimer
 功能描述: 获取设备定时列表
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  0:失败     1:success 查看全局变量 g_stDeviceTimerInfoList
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
int CheckDeviceTimer(char *pcDeviceMacAddr);

/********************************************************************
 函数名: AddDeviceTimer
 功能描述: 设置设备定时器
 输入参数:  pcDeviceMacAddr  mac地址  DEVICE_TIMER_INFO_S *pstDeviceTimerInfo定时器参数
 输出参数:
 返回值:  0:失败     1:success
 其它说明:timerId 填写0即可内部程序会自动分配  k2pro  共8组  mini 32组
 日期: 2015年11月03日
 作者: liwentao
 修改历史:
 ********************************************************************/
int AddDeviceTimer(char *pcDeviceMacAddr, DEVICE_TIMER_INFO_S *pstDeviceTimerInfo);

/********************************************************************
 函数名: AddDeviceDelay
 功能描述: 设置设备定时器
 输入参数:  pcDeviceMacAddr＝mac地址 pcDeviceType ＝ "k2pro"/"mini_b"/"mini_w"/"k2"
 delayPara = open/close#absolute_time#relative_time
 例如 延时5min后开 －－－> open#当前时间加上5分钟#5  open#yyyy-MM-dd-HH:mm:ss#5
 例如 延时30min后关 －－－> close#当前时间加上30分钟#30  close#yyyy-MM-dd-HH:mm:ss#30
 输出参数:
 返回值:  0:失败     1:success
 其它说明:timerId 填写 1023  延时只有一组
 
 日期: 2015年11月03日
 作者: liwentao
 修改历史:
 ********************************************************************/
int AddDeviceDelay(char *pcDeviceMacAddr,char *pcDeviceType, char *delayPara);

/********************************************************************
 函数名: DeleteDeviceTimer
 功能描述: 删除设置设备定时器
 输入参数:  pcDeviceMacAddr  mac地址  char *pcTimerId
 输出参数:
 返回值:  0:失败     1:success
 其它说明:
 日期: 2015年11月04日
 作者: liwentao
 修改历史:
 ********************************************************************/
int DeleteDeviceTimer(char *pcDeviceMacAddr, char *pcTimerId);

/********************************************************************
 函数名: GetTpModuleData
 功能描述: 获取环境插件的数据
 输入参数:  pcDeviceMacAddr  mac地址  TP_MODULE_INFO_S *pstTpInfo环境参数
 输出参数:
 返回值:  0:失败     1:success
 其它说明:
 日期: 2015年11月04日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetTpModuleData(char *pcDeviceMacAddr, TP_MODULE_INFO_S *pstTpInfo);

/********************************************************************
 函数名: OperateDeviceUsb
 功能描述: 操作设备USB状态
 输入参数:  pcDeviceMacAddr  mac地址  char *pcCmd "open"/"close"
 输出参数:
 返回值:  "open"/"close"/NULL
 其它说明:
 日期: 2015年10月14日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *OperateDeviceUsb(char *pcDeviceMacAddr, char *pcCmd);

/********************************************************************
 函数名: CheckDeviceRelay
 功能描述: 获取设备USB状态
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  NULL:失败     open/close  状态
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *CheckDeviceUsb(char *pcDeviceMacAddr);

/********************************************************************
 函数名: OperateDeviceLight
 功能描述: 操作设备小夜灯开关
 输入参数:  pcDeviceMacAddr  mac地址  char *pcCmd "open"/"close"
 输出参数:
 返回值:  "open"/"close"/NULL
 其它说明:
 日期: 2015年10月14日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *OperateDeviceLight(char *pcDeviceMacAddr, char *pcCmd);

/********************************************************************
 函数名: CheckDeviceRelay
 功能描述: 获取设备小夜灯状态
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  NULL:失败     open/close  状态
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *CheckDeviceLight(char *pcDeviceMacAddr);

/********************************************************************
 函数名: CheckDevicePower
 功能描述: 获取设备实时功率
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数: char szPower[]  表示实时功率  单位 kw
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
int CheckDevicePower(char *pcDeviceMacAddr, char szPower[]);
    
/********************************************************************
 函数名: CheckKlightStat
 功能描述: 获取klight状态
 输入参数:  pcDeviceMacAddr  mac地址
 输出参数:
 返回值:  0:失败     1:成功
 其它说明: 获取的状态保存在全局变量中  KLIGHT_INFO_SET_S g_stKlightInfoSet;

 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
int CheckKlightStat(char *pcDeviceMacAddr);
    
/********************************************************************
 函数名: SetKlightStat
 功能描述: 设置klight状态
 输入参数:  char *pcDeviceMacAddr KLIGHT_PARA_S *pstKlightInfo
 输出参数:
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
int SetKlightStat(char *pcDeviceMacAddr, KLIGHT_PARA_S *pstKlightInfo);
    
/********************************************************************
 函数名: OperateKlightStat
 功能描述: 开关灯泡
 输入参数:  char *pcDeviceMacAddr char *pcCmd ＝ "open"/"close"
 输出参数:
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2015年11月02日
 作者: liwentao
 修改历史:
 ********************************************************************/
int OperateKlightStat(char *pcDeviceMacAddr, char *pcCmd);
    

/***********************konke 智能门铃功能定义**************************
 小k智能门铃由一个发射器(无线节点)和一个接收器(门铃)组成，功能有语音传送、定时闹钟、
 延时闹钟、静音灯功能
 内置36首系统自带和弦音乐,并且可以自行录制铃音（自定义铃音，最多10首）
 1个接收器(门铃)可以与4个发射器(无线节点)配对使用

 以下 接收器－－简称“门铃” 发射器---一下简称“无线节点”或者“节点”
 ********************************************************************/
    
/********************************************************************
 函数名: GetDoorbellNodeId(char *pcDeviceMacAddr, char szOut[], int nLen)
 
 功能描述:获取当前门铃已经配置的无线节点ID
 
 输入参数:  char *pcDeviceMacAddr  nLen不小于64
 输出参数:  char szOut ---- 1.若一个无线节点都没 szOut＝“no_node”  
           2.若有节点，一直回复4个，若节点数据为ffffffff，则代表没有这个节点
           形如ID#ID#ID#ID ---- ffffffff#80ca1599#ffffffff#80ca1588
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetDoorbellNodeId(char *pcDeviceMacAddr, char szOut[], int nLen);

/********************************************************************
 函数名: PlayDoorbellMusic(char *pcDeviceMacAddr, char *pcMusicId)
 
 功能描述:播放门铃存在的铃音
 
 输入参数:  char *pcDeviceMacAddr ---mac地址  char *pcMusicId---铃音id
           系统铃音只需要传输1-36（总共36首）
           自定义的铃音传id－－自己产生一方面可以保存到手机本地、可以从门铃中获取
           具体如何产生，参看GenerateMusicID
 输出参数:
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int PlayDoorbellMusic(char *pcDeviceMacAddr, char *pcMusicId);

/********************************************************************
 函数名: DeleteDoorbellMusic(char *pcDeviceMacAddr, char *pcMusicId)
 功能描述:删除门铃存在的铃音（系统铃音（也就是id为1-36）不能删除）
         只能删除自定义的铃音
 输入参数:  char *pcDeviceMacAddr ---mac地址  char *pcMusicId---铃音id
 输出参数:
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int DeleteDoorbellMusic(char *pcDeviceMacAddr, char *pcMusicId);

 /********************************************************************
 函数名: GenerateMusicID(char *pcDeviceMacAddr, char szOut[], int nLen)
 功能描述:生成铃音的ID --- 这个接口是为你录制铃音做准备的 （也就是自定义了铃音保存到了门铃中）
 －－－－－－－－－  注意：产生id之后 怎么用  下面说明很重要－－－－－－－－－－－－－－－
 －－－－铃音录制功能
 1、首先保证你手机预先录制好音频文件  格式务必为amr
 2、音频文件的大小需要你控制好，不能太小录制就如微信语音一样  小了不要发送  大了自动限制  
    可参看“小k智能”App 不能大于100k 
 3、手机端录制好后，需要通过http上传到我们服务器(GET的方式)地址参看宏定义VOICE_URL
 4、手机端上传成功后（自己做判断），调用通知门铃下载铃音（函数 NotifyDoorbellDownloadMusic 
    这里面有个参数要注意pcFunMode是来区分是铃音录制还是即时传呼的   这个里是铃音录制 record）
 5、手机端check门铃下载铃音的进度（GetMusicDownloadProcess）
 6、ok－－完事

建议：完成之后最好删除手机本地录制的音频

 －－－－即时播放功能
 1、首先保证你手机预先录制好音频文件  格式务必为amr
 2、音频文件的大小需要你控制好，不能太小录制就如微信语音一样  小了不要发送  大了自动限制  
    可参看“小k智能”App 不能大于100k 
 3、手机端录制好后，需要通过http上传到我们服务器(GET的方式)地址参看宏定义VOICE_URL
 4、手机端上传成功后（自己做判断），调用通知门铃下载铃音（函数 NotifyDoorbellDownloadMusic 
    这里面有个参数要注意pcFunMode是来区分是铃音录制还是即时传呼的   这个里是即时传输 paging)
 5、手机端check门铃下载铃音的进度（GetMusicDownloadProcess）
 6、ok－－完事

 建议：完成之后最好删除手机本地录制的音频
 
 输入参数:  char *pcDeviceMacAddr ---mac地址  nLen 不小于 64
 输出参数:  char szOut ---- 铃音id (musicid)
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GenerateMusicID(char *pcDeviceMacAddr, char szOut[], int nLen);

/********************************************************************
 函数名: NotifyDoorbellDownloadMusic(char *pcDeviceMacAddr, char szOut[], int nLen)
 功能描述:通知门铃下载手机上传的自定义铃音 
 输入参数:  char *pcDeviceMacAddr char *pcFunMode --record 铃音录制 -- paging 实时传呼
           pcFileUrl --- 宏定义VOICE_URL ＋ musicid   nLen--不小于8
 输出参数: char szOut[] －－－已经自定义铃音的数量（最多支持10首）
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int NotifyDoorbellDownloadMusic(char *pcDeviceMacAddr, char *pcFunMode, char *pcFileUrl, char szOut[], int nLen);

 /********************************************************************
 函数名: GetMusicDownloadProcess(char *pcDeviceMacAddr)
 功能描述:获取门铃自定义铃音的下载进度
 输入参数:  char *pcDeviceMacAddr nLen--不小于8
 输出参数: char szOut[] ＝ 0 - 99 - ok 传输进度
          最后ok代表下载完成
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetMusicDownloadProcess(char *pcDeviceMacAddr, char szOut[], int nLen);

/********************************************************************
 函数名: GetCustomDoorbellMusicID(char *pcDeviceMacAddr, char szOut[], int nLen)
 功能描述:获取自定义铃音的ID-----这个接口的前提是你已经录制了铃音（也就是自定义了铃音保存到了门铃中）
 输入参数:  char *pcDeviceMacAddr ---mac地址  nLen 不小于 256
 输出参数:  char szOut ---- 1.若一个自定义的铃音都没 szOut＝“no_musicid”
           2.若有自定义的铃音，szOut＝“musicid1#musicid2....”
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int GetCustomDoorbellMusicID(char *pcDeviceMacAddr, char szOut[], int nLen);

/********************************************************************
 函数名: LearnWirelessNode(char *pcDeviceMacAddr)
 功能描述:学习无线开关-----学习无线开关分为两部分 1、开始学习－调用此接口，收到成功的响应后
 2、延时2s开启定时查询 学习的情况调用
 输入参数:  char *pcDeviceMacAddr ---mac地址 
 输出参数:  
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月12日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int LearnWirelessNode(char *pcDeviceMacAddr);

 /********************************************************************
 函数名: LearnWirelessNode(char *pcDeviceMacAddr)
 功能描述:学习无线开关-----学习无线开关分为两部分 1、开始学习－调用此接口，收到成功的响应后
         2、延时2s-3s 开启定时查询 学习的情况调用CheckLearnWirelessNodeStat
 输入参数:  char *pcDeviceMacAddr ---mac地址 
 输出参数:  
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月13日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int LearnWirelessNode(char *pcDeviceMacAddr);

  /********************************************************************
 函数名: CheckLearnWirelessNodeStat(char *pcDeviceMacAddr, char szOut[], int nLen)
 功能描述:check学习无线开关的情况-----不断check直至    失败/成功／已学习
         如果得到返回值为学习中，还需要继续check
         建议起一个定时器进行查询，查询间隔2s-3s

 输入参数:  char *pcDeviceMacAddr ---mac地址  int nLen 不小于32
 输出参数:   szOut = "learn" 表示还在学习中。。。
           szOut = "nodeId" 表示学习成功无线节点的ID
           szOut = "failed" 表示学习失败
           szOut = "exist"  表示此节点已经学习过（这个节点已经存在）
 返回值: 0:失败     1:成功 
 其它说明:
 日期: 2016年09月13日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int CheckLearnWirelessNodeStat(char *pcDeviceMacAddr, char szOut[], int nLen);

 /********************************************************************
 函数名: ExitLearnWirelessNode(char *pcDeviceMacAddr)
 功能描述:退出学习无线开关-----进入学习之后，想退出不想学习，需要调用次接口
 输入参数:  char *pcDeviceMacAddr ---mac地址 
 输出参数:  
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月13日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int ExitLearnWirelessNode(char *pcDeviceMacAddr);

  /********************************************************************
 函数名: DeleteWirelessNode(char *pcDeviceMacAddr)
 功能描述:删除无线节点-----删除已经配置好的无线节点
 输入参数:  char *pcDeviceMacAddr ---mac地址 
           char *pcWirelessNodeId ---无线节点id
 输出参数:  
 返回值: 0:失败     1:成功
 其它说明:
 日期: 2016年09月13日
 作者: liwentao
 修改历史:
 ********************************************************************/
 int DeleteWirelessNode(char *pcDeviceMacAddr, char *pcWirelessNodeId);
  
/********************************************************************
 *
 *以下是加湿器相关功能
 * 1.开关控制
 * 2.雾量大小调节（其中雾量的值为1至100，代表雾量大小）
 * 3.开关、雾量获取
 * 4.恒湿设置（恒湿的值范围：10到90）
 * 5.获取温湿度
 * 6.定时(设置和获取)
 ********************************************************************/
/********************************************************************
 函数名: OperateHumiSwitch
 功能描述: 开关加湿器
 输入参数:  char *pcDeviceMacAddr char *pcCmd ＝ "open"/"close"
 输出参数:
 返回值:  NULL:失败     open/close  状态
 其它说明:
 日期: 2017年01月05日
 作者: liwentao
 修改历史:
 ********************************************************************/
char *OperateHumiSwitch(char *pcDeviceMacAddr, char *pcCmd);
    
/********************************************************************
 函数名: GetHumiStat
 功能描述: 获取加湿器状态、雾量大小  恒湿功能状态、恒湿大小
 输入参数:  char *pcDeviceMacAddr
 输出参数:  char szHumiStat[] ：open/close/lack/clean代表加湿器的开、关、缺水、清洁，四个状态是互斥的
          char szFogVol[]：雾量大小值为1至100
          char szWetStat[]:open/close代表恒湿更能的开关
          char szWetVol[]:恒湿大小值为10到90
          nSzLen 不小于8
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月05日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetHumiStat(char *pcDeviceMacAddr, char szHumiStat[], char szFogVol[], char szWetStat[], char szWetVol[], int nSzLen);
    
/********************************************************************
 函数名: AdjustFogVol
 功能描述: 雾量大小调节
 输入参数:  char *pcDeviceMacAddr int nVol 值为1至100
 输出参数:
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月05日
 作者: liwentao
 修改历史:
 ********************************************************************/
int AdjustFogVol(char *pcDeviceMacAddr, int nVol);
    
/********************************************************************
 函数名: SetConstandWet
 功能描述: 恒湿设置
 输入参数:  char *pcDeviceMacAddr char *pcCmd = open/close int nVol 范围：10到90
 输出参数:
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月05日
 作者: liwentao
 修改历史:
 ********************************************************************/
int SetConstandWet(char *pcDeviceMacAddr, char *pcCmd, int nVol);
    
/********************************************************************
 函数名: GetTempAndWet
 功能描述: 获取温湿度
 输入参数:  char *pcDeviceMacAddr int nSzLen 不小于8
 输出参数: char szTemp[], char szWetVol[]
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetTempAndWet(char *pcDeviceMacAddr, char szTemp[], char szWetVol[], int nSzLen);
    
/********************************************************************
 *
 *以下是净化器相关功能
 * 1.两个模式：自动和睡眠，功能点：风速，负离子，定时关机
 * 2.两个模式之间互斥
 * 3.两个模式之间的任何一个打开，再操作其他几个功能点模式就会退出，即关闭模式
 * 4.开自动模式要开负离子功能，关自动模式，负离子保持之前状态
 *
 ********************************************************************/
    
/********************************************************************
 函数名: GetAirCleanerStat
 功能描述: 获取净化器各个状态
 输入参数:  char *pcDeviceMacAddr int nSzLen 不小于8
 输出参数: AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数，方便下次操作传值
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年02月20日
 作者: liwentao
 修改历史:
 ********************************************************************/
int GetAirCleanerStat(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
/********************************************************************
 函数名: OperateAirCleanerSwitch
 功能描述: 净化器的开关操作
 输入参数:  char *pcDeviceMacAddr  pstAirClennerPara->nPower 0:关闭  1:开启
 输出参数:  AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int OperateAirCleanerSwitch(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
/********************************************************************
 函数名: SetAirCleanerMode
 功能描述: 设置净化器的模式，自动或者睡眠
 输入参数:  char *pcDevMacAddr,
 AIRCLEANER_PARA_S *pstAirClennerPara－－－先前check得到的状态，模式仅操作
 nAutoMode或者nSleepMode互斥，只能一个为1（切记）。。。可以二者都关闭
 （重要：前提是净化器打开的时候－－npower＝1，才能操作模式...否则其他功能按钮不使能）
 输出参数:AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数，方便下次操作传值
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int SetAirCleanerMode(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
/********************************************************************
 函数名: SetAirCleanerFulizi
 功能描述: 负离子功能设置
 输入参数:  char *pcDevMacAddr,
 AIRCLEANER_PARA_S *pstAirClennerPara－－－先前check得到的状态，
 设置 nFulizi＝1或者0进行开启和关闭
 （重要：前提是净化器打开的时候－－npower＝1，才能操作模式...否则其他功能按钮不使能）
 输出参数:AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数，方便下次操作传值
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int SetAirCleanerFulizi(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
/********************************************************************
 函数名: OperateAirCleanerTimer
 功能描述: 操作净化器的定时（取消和设置）
 输入参数:  char *pcDevMacAddr,
 AIRCLEANER_PARA_S *pstAirClennerPara－－－先前check得到的状态，nTimer=0取消
 nTimer=xx设置定时（范围1-12单位小时）
 （重要：前提是净化器打开的时候－－npower＝1，才能操作模式...否则其他功能按钮不使能）
 输出参数:AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数，方便下次操作传值
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int OperateAirCleanerTimer(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
/********************************************************************
 函数名: SetAirCleanerWind
 功能描述: 设置净化器的风速
 输入参数:  char *pcDevMacAddr,
 AIRCLEANER_PARA_S *pstAirClennerPara－－－先前check得到的状态，nwind=1、2、3、4
 （重要：前提是净化器打开的时候－－npower＝1，才能操作模式...否则其他功能按钮不使能）
 输出参数:AIRCLEANER_PARA_S *pstAirClennerPara －－净化器具体参数，方便下次操作传值
 返回值:  0:失败     1:成功
 其它说明:
 日期: 2017年01月06日
 作者: liwentao
 修改历史:
 ********************************************************************/
int SetAirCleanerWind(char *pcDevMacAddr, AIRCLEANER_PARA_S *pstAirClennerPara);
    
#ifdef __cplusplus
}
#endif

#endif

