#pragma once
#include "CServer.h"

class CSpinLock
{
private:
    bool m_locked = false;

public:
    void Lock()
    {
        while (m_locked); // �ٸ� ������ lock�� ȹ���ߴٸ� ���ѷ���.

        m_locked = true;
    }
    void UnLock()
    {
        m_locked = false;
    }
};
