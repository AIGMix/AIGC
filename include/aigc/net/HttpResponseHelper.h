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
		 * @brief 获取响应
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
		 * @brief 读取内容
		 * @param buffer 缓存
		 * @param bufferSize 缓存大小
		 * @return 读取的大小
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
		 * @brief 接收数据
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
		 * @brief 解析头
		 * @return false表示异常
		 */
		bool ParseHeader()
		{
			static char crlf[] = { '\r', '\n' };
			while (true)
			{
				const auto iter = std::search(datas.begin(), datas.end(), std::begin(crlf), std::end(crlf));
				if (iter == datas.end())
					break;

				//获取此行并截取data
				const std::string line(datas.begin(), iter);
				datas.erase(datas.begin(), iter + 2);

				//空行表示header的结尾
				if (line.empty())
				{
					isHeaderFinish = true;
					//设置cookie
					for (int i = 0; i < (int)headers.size(); i++)
					{
						if (headers[i].find("Set-Cookie:") == std::string::npos)
							continue;
						std::string cookie = StringHelper::SubString(headers[i], "Set-Cookie:");
						cookies.push_back(StringHelper::Trim(cookie));
					}
					return true;
				}

				//第一行，例子 "Http/1.1 202 OK"
				if (isHeaderfirstLine)
				{
					isHeaderfirstLine = false;
					strings array = StringHelper::Split(line, ' ');
					if (array.size() >= 2)
						status = std::stoi(array[1]);
					continue;
				}

				//获取每一行的参数
				headers.push_back(line);
				const auto pos = line.find(':');
				if (pos == std::string::npos)
					continue;

				std::string name = line.substr(0, pos);
				std::string value = StringHelper::Trim(line.substr(pos + 1), " ");

				//获取内容长度
				if (name == "Content-Length")
					contentLength = std::stoul(value);
				//内容长度不能确定
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
			 * Chunked编码使用若干个Chunk串连而成，由一个标明长度为0的chunk标示结束。
			 * 每个Chunk分为头部和正文两部分，头部内容指定下一段正文的字符总数（十六进制的数字）和数量单位（一般不写），
			 * 正文部分就是指定长度的实际内容，两部分之间用回车换行(CRLF)隔开。
			 * 在最后一个长度为0的Chunk中的内容是称为footer的内容，是一些附加的Header信息（通常可以直接忽略）
			 *
			 * 格式：
			 * [chunk size][\r\n][chunk data][\r\n][chunk size][\r\n][chunk data][\r\n][chunk size = 0][\r\n][\r\n]
			 */
			if (datas.size() <= 0)
				return;

			while (true)
			{
				if (expectedChunkSize > 0)
				{
					//获取待写入的大小
					int size = expectedChunkSize;
					if (size > (int)datas.size())
						size = datas.size();
					contents.insert(contents.end(), datas.begin(), datas.begin() + static_cast<ptrdiff_t>(size));

					//裁剪
					datas.erase(datas.begin(), datas.begin() + static_cast<ptrdiff_t>(size));
					expectedChunkSize -= size;

					if (expectedChunkSize == 0)
						lastChunkEnd = true;
					if (datas.empty())
						break;
				}
				else
				{
					//去除上一个子串后面的\r\n
					if (lastChunkEnd)
					{
						if (datas.size() < 2)
							break;
						lastChunkEnd = false;
						datas.erase(datas.begin(), datas.begin() + 2);
					}

					//查找此子串的\r\n
					char crlf[] = { '\r', '\n' };
					const auto pos = std::search(datas.begin(), datas.end(), std::begin(crlf), std::end(crlf));
					if (pos == datas.end())
						break;

					const std::string size(datas.begin(), pos);
					datas.erase(datas.begin(), pos + 2);

					expectedChunkSize = std::stoul(size, nullptr, 16);
					//如果长度为0表示已经接收完毕
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
		int  status;				//状态码
		std::string message;		//错误信息
		strings headers;			//头数据
		strings cookies;			
		int contentLength;			//数据长度

	private:
		TCPClientHelperPtr client;

		std::shared_ptr<char> buffer;	//接收缓存
		std::vector<int8_t> datas;		//临时数据
		std::vector<int8_t> contents;	//内容数据
		int recvContentSize;			//接收到的数据长度

		bool isHeaderfirstLine;			//是否头部第一行
		bool isHeaderFinish;			//是否头部接收完成
		bool isContentFinish;			//是否内容接收完成

		bool isChunkedResponse;			//是否为chunk编码
		int  expectedChunkSize;			
		bool lastChunkEnd;
	};

	typedef std::shared_ptr<HttpResponseHelper> HttpResponseHelperPtr;

}