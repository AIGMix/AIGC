#pragma once
#include "aigc/net/TCPClientHelper.h"

namespace aigc
{
	class HttpResponseHelper
	{
	public:
		HttpResponseHelper(TCPClientHelperPtr client)
		{
			this->client = client;
			this->status = 0;
			this->recvContentSize = 0;
			this->contentLength = 0;
			this->isHeaderFinish = false;
			this->isHeaderfirstLine = true;
			this->isChunkedResponse = false;
			this->lastChunkEnd = false;
			this->expectedChunkSize = 0;
			this->isContentFinish = false;
			this->buffer = StringHelper::Malloc(1024);
		}

		/**
		 * @brief ��ȡ��Ӧ
		 */
		bool GetResponse()
		{
			while (true)
			{
				if (this->Recieve() <= 0)
					return false;
				if (!this->ParseHeader())
					return false;
				if (isHeaderFinish)
					break;
			}

			recvContentSize = datas.size();
			return true;
		}

		/**
		 * @brief ��ȡ����
		 * @param buffer ����
		 * @param bufferSize �����С
		 * @return ��ȡ�Ĵ�С
		 */
		int Read(char* buffer, int bufferSize)
		{
			if (bufferSize <= 0 || buffer == NULL)
				return -1;

			while (true)
			{
				ParseContent();
				if ((int)contents.size() > bufferSize)
					break;
				if (isContentFinish)
					break;

				int size = this->Recieve();
				if (size <= 0)
					return -1;
				recvContentSize += size;
			}

			if (contents.size() <= 0)
				return 0;

			int copySize = contents.size();
			if (copySize > bufferSize)
				copySize = bufferSize;

			copy(contents.begin(), contents.begin() + copySize, buffer);
			contents.erase(contents.begin(), contents.begin() + copySize);
			return copySize;
		}

	private:
		/**
		 * @brief ��������
		 */
		int Recieve()
		{
			int size = 0;
			char* cbuffer = this->buffer.get();
			if ((size = client->Receive(cbuffer, 1024)) <= 0)
			{
				message = client->GetLastErrorMessage(&status);
				return size;
			}

			datas.insert(datas.end(), cbuffer, cbuffer + size);
			return size;
		}

		/**
		 * @brief ����ͷ
		 * @return false��ʾ�쳣
		 */
		bool ParseHeader()
		{
			static char crlf[] = { '\r', '\n' };
			while (true)
			{
				const auto iter = std::search(datas.begin(), datas.end(), std::begin(crlf), std::end(crlf));
				if (iter == datas.end())
					break;

				//��ȡ���в���ȡdata
				const std::string line(datas.begin(), iter);
				datas.erase(datas.begin(), iter + 2);

				//���б�ʾheader�Ľ�β
				if (line.empty())
				{
					isHeaderFinish = true;
					//����cookie
					for (int i = 0; i < (int)headers.size(); i++)
					{
						if (headers[i].find("Set-Cookie:") == std::string::npos)
							continue;
						std::string cookie = StringHelper::SubString(headers[i], "Set-Cookie:");
						cookies.push_back(StringHelper::Trim(cookie));
					}
					return true;
				}

				//��һ�У����� "Http/1.1 202 OK"
				if (isHeaderfirstLine)
				{
					isHeaderfirstLine = false;
					strings array = StringHelper::Split(line, ' ');
					if (array.size() >= 2)
						status = std::stoi(array[1]);
					continue;
				}

				//��ȡÿһ�еĲ���
				headers.push_back(line);
				const auto pos = line.find(':');
				if (pos == std::string::npos)
					continue;

				std::string name = line.substr(0, pos);
				std::string value = StringHelper::Trim(line.substr(pos + 1), " ");

				//��ȡ���ݳ���
				if (name == "Content-Length")
					contentLength = std::stoul(value);
				//���ݳ��Ȳ���ȷ��
				else if (name == "Transfer-Encoding")
				{
					if (value == "chunked")
						isChunkedResponse = true;
					else
					{
						message = "Unsupported transfer encoding: " + value;
						return false;
					}
				}
			}
			return true;
		}

		void ParseContent()
		{
			if (!isChunkedResponse)
			{
				if (contentLength <= recvContentSize)
					isContentFinish = true;
				if (datas.size() <= 0)
					return;
				contents.insert(contents.end(), datas.begin(), datas.end());
				datas.clear();
				return;
			}

			/**
			 * Chunked����ʹ�����ɸ�Chunk�������ɣ���һ����������Ϊ0��chunk��ʾ������
			 * ÿ��Chunk��Ϊͷ�������������֣�ͷ������ָ����һ�����ĵ��ַ�������ʮ�����Ƶ����֣���������λ��һ�㲻д����
			 * ���Ĳ��־���ָ�����ȵ�ʵ�����ݣ�������֮���ûس�����(CRLF)������
			 * �����һ������Ϊ0��Chunk�е������ǳ�Ϊfooter�����ݣ���һЩ���ӵ�Header��Ϣ��ͨ������ֱ�Ӻ��ԣ�
			 *
			 * ��ʽ��
			 * [chunk size][\r\n][chunk data][\r\n][chunk size][\r\n][chunk data][\r\n][chunk size = 0][\r\n][\r\n]
			 */
			if (datas.size() <= 0)
				return;

			while (true)
			{
				if (expectedChunkSize > 0)
				{
					//��ȡ��д��Ĵ�С
					int size = expectedChunkSize;
					if (size > (int)datas.size())
						size = datas.size();
					contents.insert(contents.end(), datas.begin(), datas.begin() + static_cast<ptrdiff_t>(size));

					//�ü�
					datas.erase(datas.begin(), datas.begin() + static_cast<ptrdiff_t>(size));
					expectedChunkSize -= size;

					if (expectedChunkSize == 0)
						lastChunkEnd = true;
					if (datas.empty())
						break;
				}
				else
				{
					//ȥ����һ���Ӵ������\r\n
					if (lastChunkEnd)
					{
						if (datas.size() < 2)
							break;
						lastChunkEnd = false;
						datas.erase(datas.begin(), datas.begin() + 2);
					}

					//���Ҵ��Ӵ���\r\n
					char crlf[] = { '\r', '\n' };
					const auto pos = std::search(datas.begin(), datas.end(), std::begin(crlf), std::end(crlf));
					if (pos == datas.end())
						break;

					const std::string size(datas.begin(), pos);
					datas.erase(datas.begin(), pos + 2);

					expectedChunkSize = std::stoul(size, nullptr, 16);
					//�������Ϊ0��ʾ�Ѿ��������
					if (expectedChunkSize == 0)
					{
						isContentFinish = true;
						return;
					}
				}
			}
			return;
		}


	public:
		int  status;				//״̬��
		std::string message;		//������Ϣ
		strings headers;			//ͷ����
		strings cookies;			
		int contentLength;			//���ݳ���

	private:
		TCPClientHelperPtr client;

		std::shared_ptr<char> buffer;	//���ջ���
		std::vector<int8_t> datas;		//��ʱ����
		std::vector<int8_t> contents;	//��������
		int recvContentSize;			//���յ������ݳ���

		bool isHeaderfirstLine;			//�Ƿ�ͷ����һ��
		bool isHeaderFinish;			//�Ƿ�ͷ���������
		bool isContentFinish;			//�Ƿ����ݽ������

		bool isChunkedResponse;			//�Ƿ�Ϊchunk����
		int  expectedChunkSize;			
		bool lastChunkEnd;
	};

	typedef std::shared_ptr<HttpResponseHelper> HttpResponseHelperPtr;

}