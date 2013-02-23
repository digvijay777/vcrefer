/*************************************************
    File name: UDP �๦��ͨ����
    Author: yj	Version: V2	Date:2/23/2013
    Explanation: ������ԭV1�汾�Ļ����Ͻ�����չ
                 �Դ���ߴ����ٶ�
    Others: ��
    History: �޸���ʷ��¼������Date,Author,Modification��
*************************************************/
#include <winsock2.h>
#include "UdpTransEx.h"
#include <assert.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define UCTrace         printf


/*************************************************
  Function: ��ʼ�������׽���
  Return: �ɹ�����TRUE, ʧ�ܷ���FALSE
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
  Function: �๹�캯��
*************************************************/
UdpTransEx::UdpTransEx(void)
{
    InitSocket(); // ��ʼ�������׽���
    InitUdpTrans(); // ��ʼ���������ӿ�
    
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
  Function: ��ʼ��ͨ����
  Explanation: ��Ҫ��ȡ�������ӵ�ip��ַ
  Return: �ɹ�����TRUE, ʧ�ܷ���FALSE
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
  Function: ���װ���
  Explanation: �����еĲ���֮ǰ��Ҫ�ȵ����������
  Input: addr Ҫ�󶨱��ص�IP    
         port Ҫ�󶨵Ķ˿ڣ����Ϊ0��ϵͳ�������
  Return: �󶨳ɹ�����TRUE, ʧ�ܷ���FASE
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
        // ���õ�ַ����
        setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&addrin, sizeof(sockaddr_in));
    }

    // ִ�а󶨲���
    if(::bind(m_socket, (sockaddr *)&addrin, sizeof(sockaddr_in)))
    {
        // ��ʧ��, ������ɵĶ˿�
        sockaddr_in		null	= {0};

        null = addrin;
        null.sin_addr.s_addr = 0x01010101;
        if( ::connect(m_socket, (sockaddr *)&null, sizeof(null)) < 0)
        {
            return FALSE;
        }
    }

    // ��ȡ�����׽�����Ϣ
    getsockname(m_socket, (sockaddr *)&localin, &len);
    m_addrs[addr_lan].sin_port = localin.sin_port;
    // �����װ���Ϊ�첽ģʽ
    ioctlsocket(m_socket, FIONBIO, &fionbio);
    return TRUE;
}

/*************************************************
  Function: ������ת��������Ϣ
  Explanation: ������Bind֮������������֮ǰ����Ҫ
               ������ת��������ַ������ҪΪ��Ч��ַ
               �����Ժ������ͨ�Ŷ���������
  Input: addr IP������
         port �˿�
  Return: �ɹ�������ת����������TRUE, ���򷵻�FALSE
*************************************************/
BOOL UdpTransEx::SetTransfer( const char* addr, USHORT port )
{
    PACK        pack;

    if(FALSE == TransferSockaddr(&m_addrs[addr_transfer], addr, port))
    {
        return FALSE;
    }

    // ����3�Σ�ÿ��һ��������ת������
    for(int i = 0; i < 3; i++)
    {
        memset(&pack, 0, sizeof(pack));
        RawSend(&pack);
        if(FALSE == WaitForRead(1))
        {
            continue;
        }

        // ��������
        memset(&pack, 0, sizeof(pack));
        pack.dest_addr = &m_addrs[addr_wan];
        return RawRecv(&pack);
    }

    return FALSE;
}

/*************************************************
  Function: ��ʼ��sockaddr
  Input: addr ��ip��ַ�������ķ�ʽ
         port ��ַ�Ķ˿�
  Output: sockaddr ��ʼ���Ķ���
  Return: ��Ч��addr����ʱ����FALSE, ���򷵻�TRUE
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
  Function: ��ʼ��sockaddr
  Input: addr ���޷���������ʾ�ĵ�ַ
         port ��ַ�Ķ˿�
  Output: sockaddr ��ʼ���Ķ���
*************************************************/
BOOL UdpTransEx::TransferSockaddr( sockaddr_in* sockaddr, UINT addr, USHORT port )
{
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    sockaddr->sin_addr.s_addr = addr;

    return TRUE;
}

/*************************************************
  Function: ԭʼ���ͺ���
  Input: pack Ҫ���͵�������Ϣ
  Return: ���ɹ�����TURE,ʧ�ܷ���FALSE
  Other: ����ͨ�ľ����������£��ҽ��齫UDP�����ݿ�����1472�ֽ�����Ϊ��
         ����Internet�ϵı�׼MTUֵΪ576�ֽ�,
         �����ҽ����ڽ���Internet��UDP���ʱ,
         ��ý�UDP�����ݳ��ȿؼ���548�ֽ�(576-8-20)����.
*************************************************/
BOOL UdpTransEx::RawSend( PACK* pack )
{
    char                 buff[MTU_MAX]      = {0};
    transfer_head*		th				    = (transfer_head *)buff;

    // ���ݵĳ��ⲻ�ܳ���һ��MTU�ڴ�С
    if( pack->len > (MTU_UDP_MAX - sizeof(transfer_head)) )
    {
        assert(false);
        return FALSE;
    }

    // ������ת������
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

    // ��ӡ��ʽ����
    if(0 != pack->cmd)
    {
        UCTrace("\r[debug] RawSend cmd: %d, seq: %d, ack: %d, len: %d %s:%d\n>", 
            pack->cmd, m_seq, m_ack_seq, pack->len,
            inet_ntoa(pack->trans_addr->sin_addr), 
            (int)ntohs(pack->trans_addr->sin_port));
    }

    // ������ͷ
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

    // ����ϵͳ�ķ��ͺ���
    if( ::sendto(m_socket, buff, pack->len + sizeof(transfer_head), 0,
        (sockaddr *)pack->trans_addr, sizeof(sockaddr_in)) <= 0)
    {
        return FALSE;
    }

    return TRUE;
}

/*************************************************
  Function: ԭʼ���պ���
  Output: pack ����ת���������յ��������Ϣ
  Return: ���ճɹ�����TRUE������ʧ�ܷ���FALSE
*************************************************/
BOOL UdpTransEx::RawRecv( PACK* pack )
{
    char				buff[MTU_MAX]	= {0};
    transfer_head*		th				= (transfer_head *)buff;
    int					ret;
    sockaddr_in			rv_addr			= {0};
    int					rv_addr_len		= sizeof(rv_addr);
    int					data_len;

    // ����ϵͳ��ʵ���պ���
    if( (ret = recvfrom(m_socket, buff, MTU_MAX, 0, 
        (sockaddr*)&rv_addr, &rv_addr_len)) < 0)
    {
        return FALSE;
    }

    // ��֤�汾
    if(0x2 != ntohs(th->version))
    {
        return FALSE;
    }

    // ��֤���ݳ���
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
  Function: ����ʽ�ȴ�������Ϣ
  Input: nsec ��ʱ������
         nusec ��ʱ�ĺ�����
  Return: �����ݷ���ʱ����TRUE, û�����ݷ���ʱ����FALSE
*************************************************/
BOOL UdpTransEx::WaitForRead( int nsec, int nusec /*= 0*/ )
{
    fd_set			fd;
    struct timeval	LmtTime; 
    int				ret;

    FD_ZERO(&fd);
    FD_SET(m_socket, &fd);

    LmtTime.tv_sec = nsec;				//���ӳ�ʱ: �� 
    LmtTime.tv_usec = nusec;			//����
    ret = ::select(0, &fd, 0, 0, &LmtTime); 
    if(ret < 1)
    {
        return FALSE;
    }

    return TRUE;
}





