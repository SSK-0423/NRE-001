#include "EngineUtility.h"
#include <locale>
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

errno_t charToWchar(const char* src, wchar_t* dst, const size_t dstSize) {
	size_t convertedCount;
	
	// ���{��Ή�
	// ���ꂪ�Ȃ���src�ɓ��{�ꂪ�܂܂�Ă���ꍇ�ɕϊ������s����
	setlocale(LC_ALL, "Japanese");

	errno_t err = mbstowcs_s(&convertedCount, dst, dstSize, src, _TRUNCATE);
	return err;
}