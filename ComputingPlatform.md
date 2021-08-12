
## 资源调度及应用接口方式

每个需要接入智能计算平台的计算资源，需要驻留一个client软件模块，client软件模块一方面通过以太网和资源调度主控节点进行通信，接受主控节点的控制，另一方面，调用计算资源接口函数API，实现功能组件的加载、停止、检测、参数下发等功能。

* client软件由总体提供。

* 计算资源接口函数API由功能厂商实现。

### 计算资源函数接口API定义

#### 加载计算核运行镜像接口
```
int32_t loadCoreImage(int32_t coreNum, char* pImage, int32_t imageSize)
```

#### 卸载计算核运行镜像接口
```
int32_t unLoadCoreImage(int32_t coreNum)
```

#### 部署信息下发接口

```
int32_t userDeployInfoNotify(Pb__DeployInfo *deployInfo)

相关数据接口定义:

typedef struct _Pb__DeployInfo Pb__DeployInfo;
typedef struct _Pb__ComInfo Pb__ComInfo;

struct  _Pb__DeployInfo
{
  ProtobufCMessage base;
  uint32_t jobid;
  char *app_name;
  int32_t chip_id;
  int32_t core_id;
  size_t n_neighbors;
  Pb__ComInfo **neighbors;
};

struct  _Pb__ComInfo
{
  ProtobufCMessage base;
  char *box;
  char *board;
  int32_t chip_id;
  int32_t core_id;
  char *alg_name;
  char *com_name;
};
```

#### 运行状态查询接口

```
int32_t userGetInstructionStat(int32_t coreNum,  Pb__InstructionStat* pStat)

typedef struct _Pb__InstructionStat Pb__InstructionStat;

struct  _Pb__InstructionStat
{
  ProtobufCMessage base;
  int32_t stepid;
  int32_t taskid;
  int64_t inputcounter; // 输出接收的数据包个数
  int64_t outputcounter; // 输出发送的数据包个数
  int32_t core_status; // core运行状态, _ResetSatus=0, _RunningSatus=1
};

```

#### 组件运行参数/数据下发接口

```

int32_t userParamDataInject(char* type, int32_t coreNum, char* data, int32_t len)

type = "Subscriber-Information" 时,下发订阅者多播链信息，data = [订阅主题名称, 多播链ID]...

在应用编排时，发布了数据发布服务的组件，才会收到此信息；组件发送数据时，将以多播链ID而不是芯片ID作为目的地址。

在应用编排时，发布了订阅请求的组件，会从消息接收接口中收到订阅的数据信息。

```

#### 组件运行参数/数据获取接口

```

int32_t userParamDataFetch(char* type, int32_t coreNum, char* data, int32_t maxlen)

```

## 业务数据交互方式

* 对于FPGA计算资源，总体提供提供FPGA通信IPCORE。

* 对于CPU计算资源，总体提供CPU消息通信库。

### FPGA通信IPCORE 接口

```
    参考RapidIO HELLO包结构定义

    数据位宽暂定128位，第一拍输入参数（目标ID等信息），后面再输入数据。

```

### CPU消息通信库接口定义

```
int32_t infiniMsgSend(uint32_t destid, char* data, int32_t len)

int32_t infiniMsgRecv(char* buff, int32_t bufflen) //阻塞监听接收

```