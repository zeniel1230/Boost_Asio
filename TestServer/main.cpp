#include "CTestServer.h"

int main()
{
    CServerSetting setting;
    // session �� gameUser ����
    setting.SetSessionCount(10000);
    // Acceptor port ����
    // AddAddress�� ������ �����ϴ�.
    setting.AddAddress(30001);

    CTestServer testServer(&setting);
    testServer.Start();

    return 0;
}