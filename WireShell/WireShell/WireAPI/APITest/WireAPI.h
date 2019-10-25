#ifdef WIREAPI_EXPORTS
#define WIREAPI_API __declspec(dllexport)
#else
#define WIREAPI_API __declspec(dllimport)
#endif

#define WIRE_NO_ERROR 0
#define WIRE_OPEN_FAIL -1

#define WIRE_PASSED 1
#define WIRE_FAILED 0

/**
 �������ϲ��Ի�
 @ip : ���Ի�IP��ַ,Ĭ�ϲ���д����Ϊ����
 @return : ����0��ʾ���ӳɹ�,����1��ʾ����ʧ��
 */
WIREAPI_API int wire_Open(const char * ip = NULL);


/**
 �ر�����
 @param ��
 */
WIREAPI_API void wire_Close();


/**
 ���в��Թ���
 @pri_name : ��������
 @return : ����1��ʾ����ͨ��,����0��ʾ����ʧ��
 */
WIREAPI_API int wire_RunProject(const char * pri_name); 


/**
 ��ȡ��ǰ�Ĳ��Ա���·��
 @buff : ·���ַ�����仺��
 @buff_len : �����ֽ���
 @return : ����0��ʾ��ȡ�ɹ�,����1��ʾ��ȡʧ��
 */
WIREAPI_API int wire_GetTestReport(char * buff,int buff_len); 