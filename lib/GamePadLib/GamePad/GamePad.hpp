//	ゲームパッド（アナログピン読み取り複数ボタンに対応）
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/04	初版

#pragma	once

#include <Arduino.h>
#include "AnalogInputRvd.hpp"
#include "HwSwitchRvd.hpp"
#include "Joystick.hpp"

class	GamePad	: public Joystick
{
protected:
	AnalogInputRvd	anInput;
	HwSwitchRvd	btnAa, btnBa, btnCa;
	void	ButtonConfig(gpio_num_t dpinP, gpio_num_t apinBtnABC);	//基底クラスの同名関数を隠蔽
	void	UpdateState()	override;

public:
	GamePad() {}
	void	Initialize(gpio_num_t axisX, gpio_num_t axisY, bool invertX, bool invertY, int8_t newtralRangeH, int8_t newtralRangeL,
				gpio_num_t dpinBtnP, gpio_num_t apinBtnABC);	//基底クラスの同名関数を隠蔽

//debug
public:
	int16_t	RvdAdcVal() const { return anInput.AdcVal; }
	std::string	ToName(AnalogInputRvd::ESwitch label) const { return AnalogInputRvd::ToName(label); }
	std::string	ToName(HwSwitch::State state) const { return HwSwitchRvd::ToName(state); }
};
