// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� REMOTETHREAD_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ 
// REMOTETHREAD_API ������Ϊ�ǴӴ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef REMOTETHREAD_EXPORTS
#define REMOTETHREAD_API __declspec(dllexport)
#else
#define REMOTETHREAD_API __declspec(dllimport)
#endif

// �����Ǵ� RemoteThread.dll ������
class REMOTETHREAD_API CRemoteThread {
public:
	CRemoteThread(void);
	// TODO: �ڴ�������ķ�����
};

extern REMOTETHREAD_API int nRemoteThread;

REMOTETHREAD_API int fnRemoteThread(void);
