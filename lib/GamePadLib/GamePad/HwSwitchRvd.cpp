//	ハードウェアスイッチ	抵抗分圧回路(Resistive voltage deivider)対応
//	push_switch, toggle_switch, slide_switch, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include "HwSwitchRvd.hpp"

//初期化
void	HwSwitchRvd::Initialize()
{
	HwSwitch::Initialize(GPIO_NUM_NC);
	SetReadSource(false);
}

//状態を更新する
//・currentStateを更新する。
//・戻り値は現在のピンの値。
int8_t	HwSwitchRvd::UpdateState()
{
	int8_t currentPinVal = this->Read();
	currentState = GetNextState(currentPinVal);
	return currentPinVal;
}

//debug
std::string	HwSwitchRvd::ToName(HwSwitch::State state)
{
	switch (state)
	{
	case	HwSwitch::State::Press:	return "Press";
	case	HwSwitch::State::Holding:	return "Holding";
	case	HwSwitch::State::LongPress:	return "LongPress";
	case	HwSwitch::State::LongHolding:	return "LongHolding";
	case	HwSwitch::State::Release:	return "Release";
	case	HwSwitch::State::Free:	return "Free";
	default:	break;
	}
	return "";
}
