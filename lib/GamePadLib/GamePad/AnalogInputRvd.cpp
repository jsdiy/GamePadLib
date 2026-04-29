//	アナログ入力	抵抗分圧回路(Resistive voltage deivider)対応
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include "AnalogInputRvd.hpp"

//初期化
void	AnalogInputRvd::Initialize(gpio_num_t analogPin)
{
	anSwitch.Initialize(analogPin);
	anSwitch.MilliVoltMode(true);	//ex) max:2800mV@ESP32-C3
}

//ピン状態を読み込む
AnalogInputRvd::ESwitch	AnalogInputRvd::Read()
{
	auto nowTime = millis();
	if (adcTime < nowTime - prevTime)
	{
		prevTime = nowTime;
		auto val = anSwitch.Read();	AdcVal = val;
		auto currentVal = AdcValToState(val);
		if (prevVal == currentVal) { matchCount++; } else { matchCount = 0; prevVal = currentVal; }
		if (MatchCountMax <= matchCount) { stableVal = currentVal; matchCount = 0; }	//N回連続で同じ判定なら安定値とする
	}
	return stableVal;
}

//押下状態を読み取る
/*	閾値の前提とする抵抗値:	Rpu=2.2kR, swA=2.2kR, B=4.7kR, C=10kR
状態		実測平均 (mV)
なし		2800
C			2715
B			2250
B + C		1965
A			1665
A + C		1495
A + B		1345
A + B + C	1235
*/
AnalogInputRvd::ESwitch	AnalogInputRvd::AdcValToState(int16_t val) const
{
	if		(2760 < val) { return ESwitch::None; }
	else if (2480 < val) { return ESwitch::C; }
	else if (2110 < val) { return ESwitch::B; }
	else if (1810 < val) { return ESwitch::B | ESwitch::C; }
	else if (1580 < val) { return ESwitch::A; }
	else if (1420 < val) { return ESwitch::A | ESwitch::C; }
	else if (1290 < val) { return ESwitch::A | ESwitch::B; }
	else { return ESwitch::A | ESwitch::B | ESwitch::C; }
}

//debug
std::string	AnalogInputRvd::ToName(AnalogInputRvd::ESwitch label)
{
	if		(label == ESwitch::None)	{ return "---"; }
	else if (label == ESwitch::A)		{ return "A  "; }
	else if (label == ESwitch::B)		{ return " B "; }
	else if	(label == ESwitch::C)		{ return "  C"; }
	else if (label == (ESwitch::A | ESwitch::B))	{ return "AB "; }
	else if (label == (ESwitch::A | ESwitch::C))	{ return "A C"; }
	else if (label == (ESwitch::B | ESwitch::C))	{ return " BC"; }
	else								{ return "ABC"; }
}
