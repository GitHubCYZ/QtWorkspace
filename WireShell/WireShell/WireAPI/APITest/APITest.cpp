//
// ����ʾ���Ӵ���
//

#include "stdafx.h"
#include <windows.h>
#include "../WireAPI.h"

int RunProject(int argc, _TCHAR* argv[])
{
	char * ip = "192.168.0.143";      // ����(localhost)/��������WireTest����IP��ַ
	char * prj_name = "DIN30";        // ���������߲�Ʒ���/��������
	TestInfo info;
	memset(&info,0,sizeof(TestInfo));

	if(argc == 3)
	{
		ip = argv[1];
		prj_name = argv[2];
	}

	// ���ݲ���
	strcpy(info.pri_name,prj_name);   // ������
	strcpy(info.spareparts,"SP001");  // ����
	strcpy(info.batch,"20190815");    // ����
	info.counter = 100;               // ��ˮ  

	if(wire_Open(ip) == WIRE_NO_ERROR)  // ���ϲ��Ի�
	{
		if(wire_LoadProject(prj_name))  // ���ع��̻�������
		{
			int r = wire_RunProject(&info);  // ���е�ǰ���̻�������
			if(r == 1)                      // ����ͨ��
				printf("Run %s PASSED\n",prj_name);
			else if(r == 2)                 // ���б�ȡ��
				printf("Project %s Cancelled\n",prj_name);
			else                            // ����ʧ��  
				printf("Run %s FAILED\n",prj_name);

			// ��ӡ������Ϣ
			printf("Operator:%s\n",info.the_operator);
			printf("DateTime:%s\n",info.date_time);
			printf("TestResult:%d\n",info.operat_flag);
			// ...
		}
		else
		{
			printf("Load %s FAILED\n",prj_name);
		}

		wire_Close();
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	for(int i=0;i<3;i++)
	{
		printf("--->Try %d...",i+1);
		RunProject(argc,argv);

		Sleep(1000);
	}

	return 0;
}