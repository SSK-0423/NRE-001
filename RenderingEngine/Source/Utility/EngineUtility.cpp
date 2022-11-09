#include "EngineUtility.h"
#include <locale>

namespace NamelessEngine::Utility
{
	const int EXTENSION_DOT_POINT = 3;

	std::string ReplaceString(std::string str, std::string target, std::string replacement) {
		// �u���������������w�肳��Ă���ꍇ
		if (!target.empty()) {
			std::string::size_type pos = 0;
			// �S�u��
			while ((pos = str.find(target, pos)) != std::string::npos) {
				// �u��
				str.replace(pos, target.length(), replacement);
				pos += replacement.length();
			}
		}

		return str;
	}

	std::string GetExtension(std::string fileName)
	{
		// �g���q���܂܂�Ă��Ȃ���Ή������Ȃ�
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return ""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	std::wstring GetExtension(std::wstring fileName)
	{
		// �g���q���܂܂�Ă��Ȃ���Ή������Ȃ�
		size_t endPos = fileName.length() - 1;
		if (fileName[endPos - EXTENSION_DOT_POINT] != '.') { return L""; }

		return fileName.substr(fileName.length() - EXTENSION_DOT_POINT);
	}

	errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize) {
		size_t convertedCount;

		// ���{��Ή�
		// ���ꂪ�Ȃ���src�ɓ��{�ꂪ�܂܂�Ă���ꍇ�ɕϊ������s����
		setlocale(LC_ALL, "Japanese");

		errno_t err = mbstowcs_s(&convertedCount, dst, dstSize, src, _TRUNCATE);
		return err;
	}
}