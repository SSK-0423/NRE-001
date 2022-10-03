#include "EngineUtility.h"
#include <locale>

namespace NamelessEngine::Utility
{
	const int EXTENSION_DOT_POINT = 3;

	std::string ReplaceString(std::string str, std::string target, std::string replacement) {
		// 置換したい文字が指定されている場合
		if (!target.empty()) {
			std::string::size_type pos = 0;
			// 全置換
			while ((pos = str.find(target, pos)) != std::string::npos) {
				// 置換
				str.replace(pos, target.length(), replacement);
				pos += replacement.length();
			}
		}

		return str;
	}

	std::string GetExtension(std::string fileName)
	{
		// 拡張子が含まれていなければ何もしない
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return ""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	std::wstring GetExtension(std::wstring fileName)
	{
		// 拡張子が含まれていなければ何もしない
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return L""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize) {
		size_t convertedCount;

		// 日本語対応
		// これがないとsrcに日本語が含まれている場合に変換が失敗する
		setlocale(LC_ALL, "Japanese");

		errno_t err = mbstowcs_s(&convertedCount, dst, dstSize, src, _TRUNCATE);
		return err;
	}
}