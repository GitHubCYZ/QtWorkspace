#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>


struct QUEUE_MSG
{
    int iMsgType;       //��Ϣ����
    QString strInfaceIP;//�ӿڻ�IP
    QString strDesIP;   //Ŀ������IP
    QString strHostId;  //Ŀ����������
    QString strMsg;     //������Ϣ
    QDateTime dateTime; //�������ʱ��
};

struct HostOnlineInfo
{
    QString strHostIP;  //����IP
    QString strHostId;  //����ID
    int iState;         //����״̬ 0:���� 1:���� 2:�쳣
    int iScanCnt;       //ɨ�����
};

#endif // DATASTRUCT_H
