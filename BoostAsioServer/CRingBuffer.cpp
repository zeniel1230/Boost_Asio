#include "CRingBuffer.h"

CRingBuffer::CRingBuffer(int _size, int _tempSize)
{
	m_buffer = nullptr;
	m_readPoint = nullptr;
	m_parsingPoint = nullptr;
	m_writePoint = nullptr;
	m_bufferStartPoint = nullptr;
	m_bufferMaxSize = 0;
	m_tempBufferSize = 0;
	m_dataInBuffer = 0;

	m_bufferMaxSize = _size;
	m_tempBufferSize = _tempSize;

	m_buffer = new char[_size];
	if (m_buffer == nullptr) return;

	m_bufferStartPoint = m_buffer + m_tempBufferSize;
	m_bufferEndPoint = m_buffer + m_bufferMaxSize;
	m_writePoint = m_readPoint = m_bufferStartPoint;
	m_parsingPoint = m_readPoint = m_bufferStartPoint;
}

CRingBuffer::~CRingBuffer()
{
	SAFE_DELETE_ARRAY(m_buffer);
}

void CRingBuffer::Reset()
{
	m_bufferStartPoint = m_readPoint = m_parsingPoint = m_writePoint;
	m_dataInBuffer = 0;
}

void CRingBuffer::Write(int _size)
{
	m_writePoint += _size;

	if (m_writePoint >= (m_bufferEndPoint))
	{
		m_writePoint = m_bufferStartPoint;
	}
}

void CRingBuffer::Read(int _size)
{
	//������ ���������� �д� ���������� ������
	int remainedSize = (int)(m_bufferEndPoint - m_readPoint);

	//������ ���������� �д� �������� ����� 1�ۿ� ���� ��� -> TempBuffer������ ������
	if (remainedSize == 1)
	{
		char* tempBuffer = m_bufferStartPoint - 1;
		*tempBuffer = *m_readPoint;
		m_readPoint = tempBuffer;
		//���� readPoint�� ��Ŷ �����ŭ �����ش�.
		m_readPoint += _size;

		return;
	}

	//������ ���������� �д� ������ ���� -> ������ ù�������� �д� ������ �Űܾ��Ѵ�.
	if (remainedSize == 0)
	{
		m_readPoint = m_bufferStartPoint;
	}

	//��Ŷ ����� �����ִ� ũ�⺸�� ũ��. -> ���� ©�ȴ�.
	if (_size > remainedSize)
	{
		//�� ������ �����Ѵ�.
		m_readPoint = (m_bufferStartPoint - remainedSize);
	}

	m_readPoint += _size;

	if (m_readPoint == m_bufferEndPoint)
	{
		m_readPoint = m_bufferStartPoint;
	}

	return;
}

char* CRingBuffer::CanParsing()
{
	m_dataInBuffer = GetDataInBuffer();

	//������ �ȿ� �����Ͱ� 2�̻� �ִ°�? (��Ŷ ������ ���� �� �ִ� �ּ����� ũ�� = 2)
	if (m_dataInBuffer >= 2)
	{
		//������ �ȿ� PacketSize�� �о�� �� z�����Ƿ� �� �̻�ŭ �����ִ� �����Ͱ� �����Ѵٸ� Parsing();
		if (m_dataInBuffer >= GetPacketSize())
		{
			return Parsing();
		}

		//�����Ͱ� 2�̻������� ��Ŷ �����ŭ ����� ���� ���� ���
		//return nullptr;
	}

	//�����Ͱ� ���ų� 1���ͼ� ũ������ �� �� ���� ���
	return nullptr;
}

char* CRingBuffer::Parsing()
{
	//������ ���������� �д� ���������� ������
	int remainedSize = (int)(m_bufferEndPoint - m_parsingPoint);
	//�Ľ��� �����ʹ� �д� �������� �о�´�.
	char* parsingData = m_parsingPoint;

	//������ ���������� �д� �������� ����� 1�ۿ� ���� ��� -> TempBuffer������ ������
	if (remainedSize == 1)
	{
		char* tempBuffer = m_bufferStartPoint - 1;
		*tempBuffer = *m_parsingPoint;
		parsingData = m_parsingPoint = tempBuffer;
		//���� parsingPoint�� ��Ŷ �����ŭ �����ش�.
		m_parsingPoint += GetPacketSize();

		return parsingData;
	}

	//������ ���������� �д� ������ ���� -> ������ ù�������� �д� ������ �Űܾ��Ѵ�.
	if (remainedSize == 0)
	{
		m_parsingPoint = m_bufferStartPoint;
	}

	unsigned short packetSize = GetPacketSize();

	//��Ŷ ����� �����ִ� ũ�⺸�� ũ��. -> ���� ©�ȴ�.
	if (packetSize > remainedSize)
	{
		//�� ������ �����Ѵ�.
		memcpy(m_bufferStartPoint - remainedSize, m_parsingPoint, remainedSize);
		parsingData = m_parsingPoint = (m_bufferStartPoint - remainedSize);
	}

	m_parsingPoint += packetSize;

	if (m_parsingPoint == m_bufferEndPoint)
	{
		m_parsingPoint = m_bufferStartPoint;
	}

	return parsingData;
}

unsigned short CRingBuffer::GetWriteableSize()
{
	unsigned short size;

	//����� ���
	if (m_readPoint < m_writePoint)
	{
		size = (unsigned short)(m_bufferEndPoint - m_writePoint);
	}
	else if (m_readPoint == m_writePoint)
	{
		//readPoint�� writePoint�� ������
		//�����Ͱ� �ִ� ��� == �ѹ��� ���Ƽ� ������
		if (this->GetDataInBuffer() > 0)
		{
			size = 0;
		}
		//readPoint�� writePoint�� ������
		//�����Ͱ� ���� ��� == ringBuffer�� �����͸� �޴� �����̰ų� ó���Ұ� ����(����X)
		else
		{
			size = (unsigned short)(m_bufferEndPoint - m_writePoint);
		}
	}
	else
	{
		size = (unsigned short)(m_readPoint - m_writePoint);
	}

	return size;
}

unsigned short CRingBuffer::GetDataInBuffer()
{
	unsigned short size;

	if (m_parsingPoint > m_writePoint)
	{
		size = (unsigned short)(m_bufferEndPoint - m_parsingPoint + (m_writePoint - m_bufferStartPoint));
	}
	else
	{
		size = (unsigned short)(m_writePoint - m_parsingPoint);
	}

	return size;
}

unsigned short CRingBuffer::GetPacketSize()
{
	return *(unsigned short*)(m_parsingPoint + 2);
}
