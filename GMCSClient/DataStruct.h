#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QDateTime>

struct HostOnlineInfo
{
    QString strHostIP;  //����IP
    QString strHostId;  //����ID
    int iState;         //����״̬ 0:���� 1:���� 2:�쳣
    int iScanCnt;       //ɨ�����
};


#endif // DATASTRUCT_H
