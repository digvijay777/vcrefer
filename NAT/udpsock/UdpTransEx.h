#pragma once

// 传输以包为单位，包头大小
#pragma pack(1)
typedef struct _transfer_head
{
    unsigned short		version;	// 版本号
    unsigned short		head_length;	// 头部长度
    unsigned short		data_length;	// 数据长度
    unsigned short		cmd;		// 命令类型, 0表示向服务器请求信息

    unsigned int		dest_ip;		// 目标IP
    unsigned short		dest_port;	// 目标端口
    unsigned int		src_ip;		// 源IP
    unsigned short		src_port;	// 源端口

    unsigned int		seq;		// 收到的字节序
    unsigned int		ack_seq;	// 回应的字节序
    // 以上是V1版本的结数据结构
    unsigned int        mask;       // 传送给对方的掩码
    unsigned int        ack_mask;   // 回应对方的掩码
} transfer_head;
#pragma pack()

#define MAX_BUFFER              1024000     // 最大缓存
#define MTU_UDP_MAX				(548)
#define MTU_MAX					(MTU_UDP_MAX + 28)
#define MTU_TIME_OUT			(15)

// 命令类型定义
enum cmd_type{
    cmd_check = 0x0,			// 向中转服务器请求自身的标识
    cmd_connect = 0x1,		// 连接远程的主机
    cmd_hole = 0x2,			// 向远程主机发出打洞请求
    cmd_hold_ack = 0x3,		// 打洞成功后回应的请求 
    cmd_lan = 0x4,			// 发出内网直连请求
    cmd_lan_ack = 0x5,		// 回应内网直连请求

    cmd_data = 0x10,		    // 向对方传送数据
    cmd_data_ack = 0x11,			// 向对方回应收到数据

    cmd_heartbeat = 0x100,	// 发起心跳
    cmd_exit = -1    		// 退出
};

// 当关连接状态
enum connect_sate {
    state_none = 0,			    // 初始状态
    state_connecting,			// 连接中状态
    state_accepting,			// 监听中状态
    state_holing,				// 互联或打洞状态
    state_ok					// 正常通信状态
};

// 地址类型
enum address_type{
    addr_transfer = 0x0,
    addr_lan = 0x1, 
    addr_wan = 0x2,
    addr_dest = 0x3
};

class UdpTransEx
{
public:
    UdpTransEx(void);
    ~UdpTransEx(void);

    typedef struct _pack
    {
        char            *data;
        int             len;
        int             cmd;
        sockaddr_in     *src_addr;
        sockaddr_in     *dest_addr;
        sockaddr_in     *trans_addr;
        UINT            *seq;
        UINT            *ack_seq;
        UINT            *mask;
        UINT            *ack_mask;
    } PACK;

public:
    BOOL        InitUdpTrans();
    BOOL	    Bind(UINT addr, USHORT port);
    BOOL	    SetTransfer(const char* addr, USHORT port);

private:
    BOOL	    WaitForRead(int nsec, int nusec = 0);
    BOOL        RawSend(PACK* pack);
    BOOL        RawRecv(PACK* pack);

public:
    static BOOL TransferSockaddr(sockaddr_in* sockaddr, const char* addr, USHORT port);
    static BOOL	TransferSockaddr(sockaddr_in* sockaddr, UINT addr, USHORT port);

private:
    SOCKET              m_socket;
    sockaddr_in         m_addrs[4];
    ULONG               m_seq;
    ULONG               m_ack_seq;
    ULONG               m_mask;
    ULONG               m_ack_mask;
};
