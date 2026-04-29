//	アナログ入力	抵抗分圧回路(Resistive voltage deivider)対応
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/04	初版

#pragma	once

#include <Arduino.h>
#include "AnalogInput.hpp"

class AnalogInputRvd	: public AnalogInput
{
public:
	enum	class	ESwitch	: uint8_t
	{
		None	= 0,
		A	= 0x01,
		B	= 0x02,
		C	= 0x04,
	};

protected:
	static	constexpr	ulong	AdcTimeMillis = 5;	//アナログ入力の読み取り間隔。5未満は誤判定しやすくなる（気がする）
	static	constexpr	uint8_t	MatchCountMax = 3;	//判定精度。何連続で同じ判定だったらそのボタンが押されたとするか
	//ボタンの押下判定に AdcTimeMillis * MatchCountMax(ms) かかることになる

protected:
	AnalogInput	anSwitch;
	ulong	adcTime = AdcTimeMillis, prevTime = 0;
	uint8_t	matchCount = 0;
	ESwitch	stableVal = ESwitch::None, prevVal = ESwitch::None;
	ESwitch	AdcValToState(int16_t val) const;

public:
	AnalogInputRvd() {}
	void	Initialize(gpio_num_t analogPin);	//基底クラスの同名関数を隠蔽
	ESwitch	Read();	//基底クラスの同名関数を隠蔽

//debug
public:
	int16_t	AdcVal;
	static	std::string	ToName(ESwitch label);
};

inline AnalogInputRvd::ESwitch	operator | (AnalogInputRvd::ESwitch lhs, AnalogInputRvd::ESwitch rhs)
{
	return static_cast<AnalogInputRvd::ESwitch>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline AnalogInputRvd::ESwitch	operator & (AnalogInputRvd::ESwitch lhs, AnalogInputRvd::ESwitch rhs)
{
	return static_cast<AnalogInputRvd::ESwitch>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}
