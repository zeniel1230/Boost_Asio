#pragma once
#include "CServer.h"

#define	SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = NULL;} }

class CRingBuffer
{
private:
	//���� ������
	int			m_bufferMaxSize;
	//�ӽ� ���� ���� ������
	int			m_tempBufferSize;
	//���� �ּ�
	char*		m_buffer;
	//�д� ����
	char*		m_readPoint;
	//�Ľ� ����
	char*		m_parsingPoint;
	//���� ����
	char*		m_writePoint;
	//������ ���� ����
	char*		m_bufferStartPoint;
	//������ �� ����
	char*		m_bufferEndPoint;
	//���� �� ������ ũ��
	int			m_dataInBuffer;

public:
	CRingBuffer(int _size, int _tempSize);
	~CRingBuffer();

	//�ʱ�ȭ
	void Reset();
	//_size��ŭ ����,�б�
	void Write(int _size);
	void Read(int _size);
	//�Ľ� �������� ���� üũ
	char* CanParsing();
	//�Ľ�
	char* Parsing();

	//�� �� �ִ�, ���� �� �ִ� ������ ũ�� ���
	unsigned short GetWriteableSize();
	//������ �ȿ� �ִ� �������� �� ũ��
	unsigned short GetDataInBuffer();
	//��Ŷ ������ 
	unsigned short GetPacketSize();

	char* GetWritePoint() { return m_writePoint; }
	char* GetReadPoint() { return m_readPoint; }
	int GetBufferSize() { return m_bufferMaxSize; }
};

