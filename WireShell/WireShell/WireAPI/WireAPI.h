#ifdef WIREAPI_EXPORTS
#define WIREAPI_API __declspec(dllexport)
#else
#define WIREAPI_API __declspec(dllimport)
#endif

#define WIRE_NO_ERROR 0
#define WIRE_OPEN_FAIL -1

enum
{
	WIRE_FAILED = 0,       // ����ʧ��
	WIRE_PASSED = 1,       // ����ͨ��
	WIRE_CANCELLED = 2,    // ���Ա�ȡ��
	WIRE_BUSY = 3          // ����δִ��ϵͳæ
};

enum
{
	NG_OPEN = 1,       // ��· 
	NG_SHORT,          // ��·
	NG_DIODE_INVERSE   // �����ܷ���
};

//
// ���ݲ�������
//
typedef struct 
{
	char pri_name[64];     // ���������߲�Ʒ���
	char spareparts[64];   // ����
	char reference[64];    // Ʒ��
	char batch[64];        // ���� 
	int counter;           // ��ˮ
	char date_time[64];    // ʱ��
	char the_operator[64]; // ������
	char team[64];         // ����
	int operat_flag;       // �ź�   
	char label_no[64];     // ���ǩ��
	char error_coding[64]; // NG����
	char reverved[512];    // �����ռ�
} TestInfo;

/**
 �������ϲ��Ի�
 @ip : ���Ի�IP��ַ,Ĭ�ϲ���д����Ϊ����
 @return : ����0��ʾ���ӳɹ�,����1��ʾ����ʧ��
 */
WIREAPI_API int wire_Open(const char * ip = NULL);

/**
�鿴��ǰ�Ƿ�����Ի�ʱ����״̬
@return : 1 (����)  0 (�Ͽ�)
*/
WIREAPI_API int wire_IsOpened();

/**
 �ر�����
 @param ��
 */
WIREAPI_API void wire_Close();

/**
���ز��Թ���
@key_word : ��������/���߲�Ʒ���
@return : ����1��ʾ���سɹ�,����0��ʾ����ʧ��
*/
WIREAPI_API int wire_LoadProject(const char * key_word);

/**
 ���в��Թ���
 @test_info : ��������,���ڴ��ݽ�����Ϣ
 @return : ����1��ʾ����ͨ��,����0��ʾ����ʧ��,����2��ʾ����ȡ��
 */
WIREAPI_API int wire_RunProject(TestInfo * test_info); 


/**
 ��ȡ��ǰ�Ĳ��Ա���·��
 @buff : ·���ַ�����仺��
 @buff_len : �����ֽ���
 @return : ����0��ʾ��ȡ�ɹ�,����1��ʾ��ȡʧ��
 */
WIREAPI_API int wire_GetTestReport(char * buff,int buff_len); 