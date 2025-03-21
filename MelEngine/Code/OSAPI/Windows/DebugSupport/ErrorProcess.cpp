
#include "ErrorProcess.h"
#include<cassert>

ME::ErrorProcess* ME::ErrorProcess::GetInstance()
{
	static ErrorProcess e;
	return &e;
}



void ME::ErrorProcess::StartErroeProcess(const std::wstring& text, const bool assertFlag)
{
	UINT button = MB_OK;
	UINT icon = MB_ICONSTOP;
	MessageBox
	(
		errowMessageHWND,
		text.c_str(),
		L"エラーが発生しました!",
		button | icon
	);
	if (assertFlag)assert(0);

}
