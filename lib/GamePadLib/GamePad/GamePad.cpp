//	ゲームパッド（アナログピン読み取り複数ボタンに対応）
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy

#include "GamePad.hpp"

/*	初期化
axisX, axisY:	X軸,Y軸のアナログ入力ピン
invertX, invertY:	X軸左右を反転する, Y軸上下を反転する
newtralRangeH, newtralRangeL: 軸のHigh側（右・上）,Low側（左・下）それぞれで中立（軸が傾いていない）とする範囲
dpinBtnP: デジタル入力ピン。サムスティックのプッシュボタンの読み取りを想定。不要ならGPIO_NUM_NC
apinBtn: アナログ入力ピン。A,B,Cボタンの読み取りを想定
*/
void	GamePad::Initialize(gpio_num_t axisX, gpio_num_t axisY, bool invertX, bool invertY, int8_t newtralRangeH, int8_t newtralRangeL,
			gpio_num_t dpinBtnP, gpio_num_t apinBtn)
{
	StickConfig(axisX, axisY, invertX, invertY, newtralRangeH, newtralRangeL);
	this->ButtonConfig(dpinBtnP, apinBtn);
	StartKeyMonitoring();
}

//ボタンを初期化／再設定する
void	GamePad::ButtonConfig(gpio_num_t dpinP, gpio_num_t apinBtn)
{
	//デジタル入力ボタンを初期化する
	if (dpinP != GPIO_NUM_NC) { btnP.Initialize(dpinP); }

	//アナログ入力ボタンを初期化する
	anInput.Initialize(apinBtn);
	btnAa.Initialize();	btnBa.Initialize();	btnCa.Initialize();

	//keyをキーにして*btnを辞書登録する
	buttons.clear();
	buttons.emplace(KeyCode::P, (dpinP != GPIO_NUM_NC) ? &btnP : nullptr);
	buttons.emplace(KeyCode::A, &btnAa);
	buttons.emplace(KeyCode::B, &btnBa);
	buttons.emplace(KeyCode::C, &btnCa);
}

//キー入力状態を更新する
//override
void	GamePad::UpdateState()
{
	//レバーの入力状態を更新する
	adcValX = axisX.UpdateState();
	adcValY = axisY.UpdateState();

	//アナログ入力ボタンの読み込み値をセットする
	using AnInSw = AnalogInputRvd::ESwitch;
	auto swVal = anInput.Read();
	btnAa.SetReadSource((swVal & AnInSw::A) == AnInSw::A);
	btnBa.SetReadSource((swVal & AnInSw::B) == AnInSw::B);
	btnCa.SetReadSource((swVal & AnInSw::C) == AnInSw::C);

	//アナログ入力ボタンの入力状態を更新する
	for (const auto& pair : buttons)
	{
		if (pair.second == &btnP)
		{
			pair.second->UpdateState();
		}
		else
		{
			auto btn = reinterpret_cast<HwSwitchRvd*>(pair.second);
			btn->UpdateState();
		}
	}
}
