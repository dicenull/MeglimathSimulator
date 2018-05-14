# pragma once
# include <Siv3D.hpp>

namespace asc
{
	using namespace s3d;

	/// <summary>
	/// 文字列を送受信可能な TCPClient, TCPServer
	/// </summary>
	/// <remarks>
	/// 送受信は UTF-8 で行います。
	/// </remarks>
	template<class Socket>
	class TCPString : public Socket
	{
	public:

		/// <summary>
		/// 1 文字読み込みます。
		/// </summary>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字は正しく扱えません。
		/// </remarks>
		bool readChar(char32& to)
		{
			std::string buffer;

			buffer.resize(1);

			if (!lookahead(buffer[0]))
				return false;

			skip(sizeof(std::string::value_type));
			to = Unicode::FromUTF8(std::move(buffer))[0];
			
			return true;
		}

		/// <summary>
		/// 文字数を指定して文字列を読み込みます。
		/// </summary>
		/// <param name="length">
		/// 読み込む文字数
		/// </param>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字は正しく扱えません。
		/// </remarks>
		bool readString(size_t length, String& to)
		{
			std::string buffer;

			buffer.resize(length);

			if (!lookahead(&buffer[0], buffer.size()))
				return false;

			skip(sizeof(std::string::value_type) * buffer.size());
			to = Unicode::FromUTF8(std::move(buffer));

			return true;
		}

		/// <summary>
		/// 1 行読み込みます。
		/// </summary>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字も扱えます。
		/// </remarks>
		/// <remarks>
		/// 改行コードは LF を使用します。
		/// </remarks>
		bool readLine(String& to)
		{
			return readUntil('\n', to);
		}

		/// <summary>
		/// 指定した文字が来るまで読み込みます。
		/// </summary>
		/// <param name="end">
		/// 指定する文字
		/// </param>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字も扱えます。
		/// </remarks>
		bool readUntil(char end, String& to)
		{
			std::string buffer;

			buffer.resize(available());

			if (!lookahead(&buffer[0], buffer.size()))
				return false;

			const auto pos = buffer.find(end);

			if (pos == buffer.npos)
				return false;

			buffer.resize(pos + 1);

			skip(sizeof(std::string::value_type) * buffer.size());
			to = Unicode::FromUTF8(std::move(buffer));

			return true;
		}

		/// <summary>
		/// 指定した文字列が来るまで読み込みます。
		/// </summary>
		/// <param name="end">
		/// 指定する文字列
		/// </param>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字も扱えます。
		/// </remarks>
		bool readUntil(const String& end, String& to)
		{
			std::string buffer;

			buffer.resize(available());

			if (!lookahead(&buffer[0], buffer.size()))
				return false;

			const auto str = end.toUTF8();
			const auto pos = buffer.find(str);

			if (pos == buffer.npos)
				return false;

			buffer.resize(pos + str.size());

			skip(sizeof(std::string::value_type) * buffer.size());
			to = Unicode::FromUTF8(std::move(buffer));

			return true;
		}

		/// <summary>
		/// 文字を可能な限り読み込みます。
		/// </summary>
		/// <param name="to">
		/// 読み込み先
		/// </param>
		/// <returns>
		/// 読み込みに成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字も扱えます。
		/// </remarks>
		bool readAll(String& to)
		{
			std::string buffer;

			buffer.resize(available());

			if (!lookahead(&buffer[0], buffer.size()))
				return false;

			skip(sizeof(std::string::value_type) * buffer.size());
			to = Unicode::FromUTF8(std::move(buffer));

			return true;
		}

		/// <summary>
		/// 文字列を送信します。
		/// </summary>
		/// <param name="data">
		/// 送信する文字列
		/// </param>
		/// <returns>
		/// 送信に成功すれば true
		/// </returns>
		/// <remarks>
		/// 日本語などの 1 バイトではない文字も扱えます。
		/// </remarks>
		bool sendString(const String& data);

		bool sendString(const String& data, const Optional<SessionID>& id);
	};

	template<>
	bool TCPString<TCPServer>::sendString(const String& data, const Optional<SessionID>& id)
	{
		const auto str = data.toUTF8();

		return send(str.data(), str.length(), id);
	}

	template<>
	bool TCPString<TCPClient>::sendString(const String& data)
	{
		const auto str = data.toUTF8();

		return send(str.data(), str.length());
	}

	using TCPStringClient = TCPString<TCPClient>;
	using TCPStringServer = TCPString<TCPServer>;
}