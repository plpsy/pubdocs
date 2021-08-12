
## DEMO需要展示的功能

### DEMO拓扑

测试主机(安装GE卡1) <------------> 适配器工控机（安装GE卡2 + RIO卡1） <---------> 节点工控机（RIO卡2 + 429卡）

### 第一阶段

#### 场景1（RIO节点读取GE网）：

* GE卡1卡所在的主机，通过写操作，改写了GE卡1上的反射内存地址A，这时，适配器工控机上的GE卡2的反射内存地址A也被修改。

* 节点工控机读取GE反射内存A的数据时，向本机的RIO卡2的GE虚拟地址空间发起一个读操作，RIO卡2根据这个读操作，翻译成RIO读操作到适配器工控机的RIO卡1，RIO卡1发起总线读操作读取适配器上GE卡2反射内存A的数据。

* 最终，节点工控机成功读取适配器工控机上GE卡2反射内存地址A的数据。

#### 场景2（RIO节点写入GE网）：

* 节点工控机写GE反射内存A的数据时，向本机的RIO卡2的GE虚拟地址空间发起一个写操作，RIO卡2根据这个写操作，翻译成RIO写操作到适配器工控机的RIO卡1，RIO卡1发起总线写操作改写适配器上GE卡2反射内存A的数据。
  
* 适配器工控机上GE卡2广播写操作到GE网络。

### 第二阶段

#### 从适配器工控机上，直接配置节点工控机上429卡的寄存器，并可读取429卡上的数据信息


### RIO卡空间定义

#### PCIE BAR0 （配置空间）

###### GE空间起始地址，GE_HOST_OFFSET
字段长度: 8字节
字段偏移: 0x00
字段功能: 主机通过配置该字段，告知RIO卡，GE卡的反射内存空间起始地址，复位时默认为0。

###### 适配器RapidIO ID, BRIDGE_RIO_ID
字段长度: 2字节
字段偏移: 0x10
字段功能: 主机通过配置该字段，告知RIO卡，GE卡的反射内存空间起始地址，复位时默认为200。

###### RapidIO反射内存广播ID, MULTICAT_RIO_ID
字段长度: 2字节
字段偏移: 0x20
字段功能: 主机通过配置该字段，告知RIO卡，GE卡的反射内存空间起始地址，复位时默认为240。

###### 功能卡空间起始地址，USERCARD_HOST_OFFSET
字段长度: 8字节
字段偏移: 0x30
字段功能: 主机通过配置该字段，告知RIO卡，用户功能卡的反射内存空间起始地址，复位时默认为0。

###### 功能卡空间长度，USERCARD_SPACE_SIZE
字段长度: 4字节
字段偏移: 0x38
字段功能: 主机通过配置该字段，告知RIO卡，用户功能卡的反射内存空间起始地址，复位时默认为0。

###### RIO卡本地RapidIO ID，RIO_LOCAL_ID
字段长度: 2字节
字段偏移: 0x40
字段功能: 主机通过此字段获取RIO卡的RapidIO ID。


#### PCIE BAR1(0x00000000~0x3FFFFFFF) RapidIO反射网内存空间

* 对该空间的读操作，直接读取RIO卡上的板载内存同样的偏移(0x00000000 ~ 0x3FFFFFFF)

* 对该空间的写操作，一方面，会写RIO卡上的板载内存同样的偏移(0x00000000 ~ 0x3FFFFFFF)，另一方面，会将写操作通过NWrite发送到网络上去。NWrite的目标地址为一个广播地址，暂定240，对应地址范围(0x40000000 ~ 0x7FFFFFFF)

#### PCIE BAR2(0x00000000~0x10000000) GE虚拟反射内存空间

* 对该空间的读操作，RIO卡发起RapidIO NRead操作到RapidIO网络，NRead的目标地址为适配器RapidIO节点地址，暂定为200，对应地址范围为(0x80000000 ~ 0x90000000)

* 对该空间的写操作，RIO卡发起RapidIO NWrite操作到RapidIO网络，NWrite的目标地址为适配器RapidIO节点地址，暂定为200，对应地址范围为(0x80000000 ~ 0x90000000)

#### RIO卡响应RapidIO写操作Nwrite

* 对于地址范围(0x40000000 ~ 0x7FFFFFFF)，操作卡上板载内存范围(0x00000000 ~ 0x3FFFFFFF)

* 对于地址范围(0x80000000 ~ 0x8FFFFFFF)，若GE_HOST_OFFSET为0，则忽略，否则，写PCIE空间（GE_HOST_OFFSET ~ GE_HOST_OFFSET+0x0FFFFFFF)

* 对于地址范围(0xA0000000~0xAFFFFFFF)，若USERCARD_HOST_OFFSET为0，则忽略，否则，写PCIE空间（USERCARD_HOST_OFFSET ~ USERCARD_HOST_OFFSET+0x0FFFFFFF)

#### RIO卡响应RapidIO读操作NRead

* 对于地址范围(0x40000000 ~ 0x7FFFFFFF)，忽略

* 对于地址范围(0x80000000 ~ 0x8FFFFFFF)，若GE_HOST_OFFSET为0，则忽略，否则，读PCIE空间（GE_HOST_OFFSET ~ GE_HOST_OFFSET+0x0FFFFFFF)

* 对于地址范围(0xA0000000 ~ 0xAFFFFFFF)，若USERCARD_HOST_OFFSET为0，则忽略，否则，读PCIE空间（USERCARD_HOST_OFFSET ~ USERCARD_HOST_OFFSET+0x0FFFFFFF)
