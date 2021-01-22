#pragma once
#pragma once

#include <map>

#include "aigc/net/TCPClientHelper.h"
#include "aigc/net/HttpResponseHelper.h"

namespace aigc
{
	class HttpRequestHelper
	{
	public:

		/**
		 * @param url ����
		 */
		HttpRequestHelper(const std::string& url,
			const std::string& method = "GET",
			const std::map<std::string, std::string>& paras = {},
			const strings& headers = {})
		{
			this->url = url;
			this->status = 0;
			this->method = method;
			this->paras = paras;
			this->headers = headers;
			this->urlInfo = NetHelper::GetUrlInfo(url);
			this->client = std::make_shared<TCPClientHelper>();
		}

		/**
		 * @brief ��������
		 */
		bool SendRequest()
		{
			if (!client->Connect(urlInfo.domain, atoi(urlInfo.port.c_str())))
			{
				message = client->GetLastErrorMessage(&status);
				return false;
			}

			std::string body;
			for (auto parameter : paras)
				body += "?" + NetHelper::UrlEncode(parameter.first) + "=" + NetHelper::UrlEncode(parameter.second);
			if (body.size() > 0)
				body[0] = '&';

			//��װ����
			std::string headerData = method + " " + urlInfo.path +
									" HTTP/1.1\r\n" +
									StringHelper::Join(headers, "\r\n") +
									"Host: " + urlInfo.domain + "\r\n" +
									"Content-Length: " + std::to_string(body.size()) + "\r\n\r\n";
			std::vector<int8_t> requestData = StringHelper::StringsToUint8({ headerData, body });

			//������������
			auto remaining = requestData.size();
			auto sendData = requestData.data();
			while (remaining > 0)
			{
				const auto size = client->Send((char*)sendData, remaining);
				if (size <= 0)
				{
					message = client->GetLastErrorMessage(&status);
					return false;
				}
				remaining -= size;
				sendData += size;
			}
			return true;
		}

		/**
		 * @brief ��ȡ��Ӧ��
		 */
		HttpResponseHelperPtr GetResponse()
		{
			HttpResponseHelperPtr response = std::make_shared<HttpResponseHelper>(client);
			if (response->GetResponse())
				return response;

			status = response->status;
			message = response->message;
			return NULL;
		}

	public:
		strings headers;							//ͷ
		std::string method;							//GET\POST
		std::map<std::string, std::string> paras;	//����

		std::string url;			//����
		NetHelper::UrlInfo urlInfo; //������Ϣ
		int status;					//������
		std::string message;		//������Ϣ

	public:
		TCPClientHelperPtr client;
	};

	typedef std::shared_ptr<HttpRequestHelper> HttpRequestHelperPtr;

}