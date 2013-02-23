/*************************************************
    File name: UDP 多功能通信类
    Author: yj	Version: V2	Date:2/23/2013
    Explanation: 此类在原V1版本的基础上进行扩展
                 以此提高传输速度
    Others: 无
    History: 修改历史记录（包括Date,Author,Modification）
*************************************************/
#include <winsock2.h>
#include "UdpTransEx.h"
#include <assert.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define UCTrace         printf


/*************************************************
  Function: 初始化网络套接字
  Return: 成功返回TRUE, 失败返回FALSE
*************************************************/
BOOL InitSocket()
{
    static bool   init  = false;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    if(init)
    {
        return TRUE;
    }

    wVersionRequested = MAKEWORD( 2, 2 );

    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return FALSE;
    }

    if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
            /* Tell the user that we could not find a usable */
            /* WinSock DLL.                                  */
            WSACleanup( );
            return FALSE; 
    }

    init = true;
    /* The WinSock DLL is acceptable. Proceed. */
    return TRUE;
}

/*************************************************
  Function: 类构造函数
*************************************************/
UdpTransEx::UdpTransEx(void)
{
    InitSocket(); // 初始化网络套接字
    InitUdpTrans(); // 初始化局域网接口
    
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);	
    m_seq = 0;
    m_ack_seq = 0;
    m_mask = 0;
    m_ack_mask = 0;
    memset(m_addrs, 0, sizeof(m_addrs));
}

UdpTransEx::~UdpTransEx(void)
{
}

/*************************************************
  Function: 初始化通信类
  Explanation: 主要获取本地连接的ip地址
  Return: 成功返回TRUE, 失败返回FALSE
*************************************************/
BOOL UdpTransEx::InitUdpTrans()
{
    SOCKET		sock	= socket(AF_INET, SOCK_DGRAM, 0);	
    sockaddr_in	addr    = {0};
    int			len;

    TransferSockaddr(&addr, 0x01010101, 80);
    len = connect(sock, (sockaddr *)&addr, sizeof(addr));
    memset(&addr, 0, sizeof(addr));
    len = sizeof(addr);
    getsockname(sock, (sockaddr *)&addr, &len);
    closesocket(sock);

    m_addrs[addr_lan] = addr;

    return TRUE;
}

/*************************************************
  Function: 绑定套按字
  Explanation: 在所有的操作之前需要先调用这个函数
  Input: addr 要绑定本地的IP    
         port 要绑定的端口，如果为0由系统随机生成
  Return: 绑定成功返回TRUE, 失败返回FASE
*************************************************/
BOOL UdpTransEx::Bind( UINT addr, USHORT port )
{
    sockaddr_in             addrin        = {0};
    sockaddr_in             localin       = {0};
    u_long			        fionbio		  = 1;
    int				        len			  = sizeof(localin);

    TransferSockaddr(&addrin, addr, port);
    if(0 != port)
    {
        // 设置地址重用
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&addrin, sizeof(sockaddr_in));
    }

    // 执行绑定操作
    if(::bind(m_socket, (sockaddr *)&addrin, sizeof(sockaddr_in)))
    {
        // 绑定失败, 随机生成的端口
        sockaddr_in		null	= {0};

        null = addrin;
        null.sin_addr.s_addr = 0x01010101;
        if( ::connect(m_socket, (sockaddr *)&null, sizeof(null)) < 0)
        {
            return FALSE;
        }
    }

    // 获取本地套接字信息
    getsockname(m_socket, (sockaddr *)&localin, &len);
    m_addrs[addr_lan].sin_port = localin.sin_port;
    // 设置套按字为异步模式
    ioctlsocket(m_socket, FIONBIO, &fionbio);
    return TRUE;
}

/*************************************************
  Function: 设置中转服务器信息
  Explanation: 紧接着Bind之后，在其它操作之前必须要
               设置中转服务器地址，并且要为有效地址
               否则以后的所有通信都将不正常
  Input: addr IP或域名
         port 端口
  Return: 成功连接中转服务器返回TRUE, 否则返回FALSE
*************************************************/
BOOL UdpTransEx::SetTransfer( const char* addr, USHORT port )
{
    PACK        pack;

    if(FALSE == TransferSockaddr(&m_addrs[addr_transfer], addr, port))
    {
        return FALSE;
    }

    // 尝试3次，每次一秒连接中转服务器
    for(int i = 0; i < 3; i++)
    {
        memset(&pack, 0, sizeof(pack));
        RawSend(&pack);
        if(FALSE == WaitForRead(1))
        {
            continue;
        }

        // 接收数据
        memset(&pack, 0, sizeof(pack));
        pack.dest_addr = &m_addrs[addr_wan];
        return RawRecv(&pack);
    }

    return FALSE;
}

/*************************************************
  Function: 初始化sockaddr
  Input: addr 以ip地址或域名的方式
         port 地址的端口
  Output: sockaddr 初始化的对像
  Return: 无效的addr参数时返回FALSE, 否则返回TRUE
*************************************************/
BOOL UdpTransEx::TransferSockaddr( sockaddr_in* sockaddr, const char* addr, USHORT port )
{
    struct hostent*		host	= gethostbyname(addr);

    if(NULL == host)
    {
        return FALSE;
    }

    return TransferSockaddr(sockaddr, *( (UINT *)host->h_addr_list[0] ), port);
}

/*************************************************
  Function: 初始化sockaddr
  Input: addr 以无符号整数表示的地址
         port 地址的端口
  Output: sockaddr 初始化的对像
*************************************************/
BOOL UdpTransEx::TransferSockaddr( sockaddr_in* sockaddr, UINT addr, USHORT port )
{
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr.s_addr = addr;

    return TRUE;
}

/*************************************************
  Function: 原始发送函数
  Input: pack 要发送的所有信息
  Return: 发成功返回TURE,失败返回FALSE
  Other: 在普通的局域网环境下，我建议将UDP的数据控制在1472字节以下为好
         鉴于Internet上的标准MTU值为576字节,
         所以我建议在进行Internet的UDP编程时,
         最好将UDP的数据长度控件在548字节(576-8-20)以内.
*************************************************/
BOOL UdpTransEx::RawSend( PACK* pack )
{
    char                 buff[MTU_MAX]      = {0};
    transfer_head*		th				    = (transfer_head *)buff;

    // 数据的长锯不能超过一个MTU在大小
    if( pack->len > (MTU_UDP_MAX - sizeof(transfer_head)) )
    {
        assert(false);
        return FALSE;
    }

    // 设置中转服务器
    if(NULL == pack->trans_addr)
    {
        pack->trans_addr = &m_addrs[addr_transfer];
    }

    if(NULL == pack->src_addr)
    {
        pack->src_addr = &m_addrs[addr_wan];
    }

    if(NULL == pack->dest_addr)
    {
        pack->dest_addr = &m_addrs[addr_dest];
    }

    // 打印调式命令
    if(0 != pack->cmd)
    {
        UCTrace("\r[debug] RawSend cmd: %d, seq: %d, ack: %d, len: %d %s:%d\n>", 
            pack->cmd, m_seq, m_ack_seq, pack->len,
            inet_ntoa(pack->trans_addr->sin_addr), 
            (int)ntohs(pack->trans_addr->sin_port));
    }

    // 构建包头
    th->version = htons(0x2);
    th->head_length = htons(sizeof(transfer_head));
    th->data_length = htons(pack->len);
    th->cmd = htons(pack->cmd);

    th->dest_ip = pack->dest_addr->sin_addr.s_addr;
    th->dest_port = pack->dest_addr->sin_port;
    th->src_ip = pack->src_addr->sin_addr.s_addr;
    th->src_port = pack->src_addr->sin_port;

    th->seq = htonl(m_seq);
    th->ack_seq = htonl(m_ack_seq);
    th->mask = htonl(m_mask);
    th->ack_mask = htonl(m_mask);

    if(NULL != pack->data)
    {
        memcpy(buff + sizeof(transfer_head), pack->data, pack->len);
    }

    // 调用系统的发送函数
    if( ::sendto(m_socket, buff, pack->len + sizeof(transfer_head), 0,
        (sockaddr *)pack->trans_addr, sizeof(sockaddr_in)) <= 0)
    {
        return FALSE;
    }

    return TRUE;
}

/*************************************************
  Function: 原始接收函数
  Output: pack 从中转服务器接收到的相关信息
  Return: 接收成功返回TRUE，接收失败返回FALSE
*************************************************/
BOOL UdpTransEx::RawRecv( PACK* pack )
{
    char				buff[MTU_MAX]	= {0};
    transfer_head*		th				= (transfer_head *)buff;
    int					ret;
    sockaddr_in			rv_addr			= {0};
    int					rv_addr_len		= sizeof(rv_addr);
    int					data_len;

    // 调用系统真实接收函数
    if( (ret = recvfrom(m_socket, buff, MTU_MAX, 0, 
        (sockaddr*)&rv_addr, &rv_addr_len)) < 0)
    {
        return FALSE;
    }

    // 验证版本
    if(0x2 != ntohs(th->version))
    {
        return FALSE;
    }

    // 验证数据长度
    data_len = ntohs(th->data_length) + ntohs(th->head_length);
    if(data_len > sizeof(buff))
    {
        return FALSE;
    }

    pack->cmd = ntohs(th->cmd);
    if(NULL != pack->src_addr)
    {
        ((sockaddr_in *)pack->src_addr)->sin_family = rv_addr.sin_family;
        ((sockaddr_in *)pack->src_addr)->sin_port = th->src_port;
        ((sockaddr_in *)pack->src_addr)->sin_addr.s_addr = th->src_ip;
    }

    if(NULL != pack->dest_addr)
    {
        ((sockaddr_in *)pack->dest_addr)->sin_family = rv_addr.sin_family;
        ((sockaddr_in *)pack->dest_addr)->sin_port = th->dest_port;
        ((sockaddr_in *)pack->dest_addr)->sin_addr.s_addr = th->dest_ip;
    }

    if(NULL != pack->data && pack->len >= ntohs(th->data_length))
    {
        pack->len = ntohs(th->data_length);

        memcpy(pack->data, buff + ntohs(th->head_length), pack->len);
    }

    if(NULL != pack->seq)
    {
        *(pack->seq) = ntohl(th->seq);
    }

    if(NULL != pack->ack_seq)
    {
        *(pack->ack_seq) = ntohl(th->ack_seq);
    }

    if(NULL != pack->mask)
    {
        *(pack->mask) = ntohl(th->mask);
    }

    if(NULL != pack->ack_mask)
    {
        *(pack->ack_mask) = ntohl(th->ack_mask);
    }

    if(0 != ntohs(th->cmd))
    {
        UCTrace("\r[debug] ------- cmd: %d, seq: %d, ack: %d, len: %d\n>",
            (int)ntohs(th->cmd), ntohl(th->seq),
            ntohl(th->ack_seq), (int)ntohs(th->data_length));
    }

    return TRUE;
}

/*************************************************
  Function: 阻塞式等待接收信息
  Input: nsec 超时的秒数
         nusec 超时的毫秒数
  Return: 有数据返回时返回TRUE, 没有数据返回时返回FALSE
*************************************************/
BOOL UdpTransEx::WaitForRead( int nsec, int nusec /*= 0*/ )
{
    fd_set			fd;
    struct timeval	LmtTime; 
    int				ret;

    FD_ZERO(&fd);
    FD_SET(m_socket, &fd);

    LmtTime.tv_sec = nsec;				//连接超时: 秒 
    LmtTime.tv_usec = nusec;			//毫秒
    ret = ::select(0, &fd, 0, 0, &LmtTime); 
    if(ret < 1)
    {
        return FALSE;
    }

    return TRUE;
}





