#pragma once

// �����԰�Ϊ��λ����ͷ��С
#pragma pack(1)
typedef struct _transfer_head
{
    unsigned short		version;	// �汾��
    unsigned short		head_length;	// ͷ������
    unsigned short		data_length;	// ���ݳ���
    unsigned short		cmd;		// ��������, 0��ʾ�������������Ϣ

    unsigned int		dest_ip;		// Ŀ��IP
    unsigned short		dest_port;	// Ŀ��˿�
    unsigned int		src_ip;		// ԴIP
    unsigned short		src_port;	// Դ�˿�

    unsigned int		seq;		// �յ����ֽ���
    unsigned int		ack_seq;	// ��Ӧ���ֽ���
    // ������V1�汾�Ľ����ݽṹ
    unsigned int        mask;       // ���͸��Է�������
    unsigned int        ack_mask;   // ��Ӧ�Է�������
} transfer_head;
#pragma pack()

#define MAX_BUFFER              1024000     // ��󻺴�
#define MTU_UDP_MAX				(548)
#define MTU_MAX					(MTU_UDP_MAX + 28)
#define MTU_TIME_OUT			(15)

// �������Ͷ���
enum cmd_type{
    cmd_check = 0x0,			// ����ת��������������ı�ʶ
    cmd_connect = 0x1,		// ����Զ�̵�����
    cmd_hole = 0x2,			// ��Զ����������������
    cmd_hold_ack = 0x3,		// �򶴳ɹ����Ӧ������ 
    cmd_lan = 0x4,			// ��������ֱ������
    cmd_lan_ack = 0x5,		// ��Ӧ����ֱ������

    cmd_data = 0x10,		    // ��Է���������
    cmd_data_ack = 0x11,			// ��Է���Ӧ�յ�����

    cmd_heartbeat = 0x100,	// ��������
    cmd_exit = -1    		// �˳�
};

// ��������״̬
enum connect_sate {
    state_none = 0,			    // ��ʼ״̬
    state_connecting,			// ������״̬
    state_accepting,			// ������״̬
    state_holing,				// �������״̬
    state_ok					// ����ͨ��״̬
};

// ��ַ����
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
