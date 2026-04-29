//	ハードウェアスイッチ	抵抗分圧回路(Resistive voltage deivider)対応
//	push_switch, toggle_switch, slide_switch, ...
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/04	初版

#pragma	once

#include <Arduino.h>
#include "HwSwitch.hpp"

class HwSwitchRvd	: public HwSwitch
{
protected:
	bool	isSwOn;

public:
	void	Initialize();	//基底クラスの同名関数を隠蔽
	void	SetReadSource(bool isSwOn) { this->isSwOn = isSwOn; }
	int8_t	UpdateState();	//基底クラスの同名関数を隠蔽
	int8_t	Read() { return isSwOn ? SwOn : SwOff; }	//基底クラスの同名関数を隠蔽

//debug
public:
	static	std::string	ToName(HwSwitch::State state);
};
