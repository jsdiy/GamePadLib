//	ゲームパッド
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/04	初版

/*	ピンアサイン
			ESP32-C3
SPI-SS		GPIO_NUM_10
SPI-MOSI	GPIO_NUM_7
SPI-MISO	GPIO_NUM_2	グラフィックLCDでは未使用
SPI-SCK		GPIO_NUM_6
LCD-DC		GPIO_NUM_4
LCD-RESET	GPIO_NUM_NC	ENピンへ接続

			ESP32-C3	*…BootStrap関連ピン
			[Antenna side]
			3V3		IO-0	AxisY(AnalogInput)
LCD-RESET	EN		IO-1	AxisX(AnalogInput)
LCD-DC		IO-4	IO-2*	SPI-MISO
			IO-5	IO-3	ButtonABC(AnalogInput)
SPI-SCK		IO-6	IO-19	USB_D+
SPI-MOSI	IO-7	IO-18	USB_D-
			IO-8*	TXD
ButtonP		IO-9*	RXD
			GND		IO-10*	SPI-SS
			[USB-conn side]
*/

#include <Arduino.h>
#include "LcdST77xx.hpp"
#include "GamePad.hpp"

//オブジェクト
LcdST77xx	lcd;
GamePad	gpad;

//色
static	Color	ColorWhite(0xFF, 0xFF, 0xFF);
static	Color	ColorBlack(0x00, 0x00, 0x00);
static	Color	ColorRed(0xFF, 0x00, 0x00);
static	Color	ColorGreen(0x00, 0xFF, 0x00);
static	Color	ColorBlue(0x00, 0x00, 0xFF);
static	Color	ColorYellow(0xFF, 0xFF, 0x00);

//定数
static	constexpr	gpio_num_t
	PinLcdCS	= GPIO_NUM_10,	PinLcdDC	= GPIO_NUM_4,
	PinAxisX	= GPIO_NUM_1,	PinAxisY	= GPIO_NUM_0,	//アナログ入力
	PinButtonABC	= GPIO_NUM_3,	//アナログ読み取りボタン
	PinButtonP		= GPIO_NUM_9;	//デジタル読み取りボタン

static	constexpr	auto
	SLongPress	= "LongPress",
	SHolding	= "Holding",
	SEmpty		= "";

//変数・関数・ほか
using	Key = Joystick::KeyCode;
static	const	char	*keyStateP, *keyStateA;
static	const	char	*keyStateB, *keyStateC;
static	void	LongPressP() { keyStateP = SLongPress; }
static	void	LongPressA() { keyStateA = SLongPress; }
static	void	ReleaseP() { keyStateP = SEmpty; }
static	void	ReleaseA() { keyStateA = SEmpty; }

void	setup()
{
	Serial.begin(115200);
	delay(1000);

	lcd.Initialize(PinLcdCS, PinLcdDC, 7735, 128, 160, 0, 0);
	lcd.RotateFlip(ERotFlip::Rot90);
	lcd.SetTextScale(2, 2);
	lcd.ClearScreen(ColorGreen);
	lcd.DrawString(0, 0, "Test");
	delay(1000);

	gpad.Initialize(PinAxisX, PinAxisY, true, true, 3, 3, PinButtonP, PinButtonABC);	//引数はサムスティックの取り付け方向による
	gpad.SetLongHoldThresholdTime(2000);	//2秒押したら長押しと判定する実験
	ReleaseP();	ReleaseA();	//初期値:""
	keyStateB = SEmpty;	keyStateC = SEmpty;	//初期値:""
	gpad.OnLongPressCb().Add(Key::P, LongPressP);	gpad.OnReleaseCb().Add(Key::P, ReleaseP);	//コールバックを登録
	gpad.OnLongPressCb().Add(Key::A, LongPressA);	gpad.OnReleaseCb().Add(Key::A, ReleaseA);	//コールバックを登録
	gpad.KeyEventTrigger(true);	//登録したコールバックが実行されるようにする
}

void	loop()
{
	bool isKeyUpdated = gpad.CheckKeyState();	//キー入力に変化があったか（内部でキー入力監視をしている）
	if (!isKeyUpdated) { return; }

	//以下、同期処理としてキー押下状態を取得している
	auto stickL = gpad.IsKeyHolding(Key::Left) ? 'L' : ' ';
	auto stickR = gpad.IsKeyHolding(Key::Right) ? 'R' : ' ';
	auto stickU = gpad.IsKeyHolding(Key::Up) ? 'U' : ' ';
	auto stickD = gpad.IsKeyHolding(Key::Down) ? 'D' : ' ';
	auto adcAxisX = gpad.AdcAxisX();	//参考としてADC値を取得
	auto adcAxisY = gpad.AdcAxisY();	//参考としてADC値を取得

	auto btnP = gpad.IsKeyHolding(Key::P) ? 'P' : ' ';
	auto btnA = gpad.IsKeyHolding(Key::A) ? 'A' : ' ';
	auto btnB = gpad.IsKeyHolding(Key::B) ? 'B' : ' ';
	auto btnC = gpad.IsKeyHolding(Key::C) ? 'C' : ' ';
	auto adcRvd = gpad.RvdAdcVal();	//参考としてADC値を取得

	char keyState[] = { stickL, stickR, stickU, stickD, btnP, btnA, btnB, btnC, '\0' };

	if (gpad.IsKeyHolding(Key::B)) { keyStateB = SHolding; }
	if (gpad.IsKeyHolding(Key::C)) { keyStateC = SHolding; }
	if (gpad.IsKeyFree(Key::B)) { keyStateB = SEmpty; }
	if (gpad.IsKeyFree(Key::C)) { keyStateC = SEmpty; }

	uint8_t r = 0;
	lcd.Printf(0, lcd.YFromRow(r++), "%s", keyState);
	lcd.Printf(0, lcd.YFromRow(r++), "X:%4d Y:%4d", adcAxisX, adcAxisY);
	lcd.Printf(0, lcd.YFromRow(r++), "ABC:%4d", adcRvd);
	lcd.Printf(0, lcd.YFromRow(r++), "P:%-9s", keyStateP);	//非同期処理で状態が代入されている
	lcd.Printf(0, lcd.YFromRow(r++), "A:%-9s", keyStateA);	//非同期処理で状態が代入されている
	lcd.Printf(0, lcd.YFromRow(r++), "B:%-9s", keyStateB);
	lcd.Printf(0, lcd.YFromRow(r++), "C:%-9s", keyStateC);
}

/*	使用例
<platformio.ini>
lib_extra_dirs =
	./lib/InputLib
	./lib/GamePadLib

<main.cpp>
#include <Arduino.h>
#include "GamePad.hpp"

GamePad	gpad;

void	setup()
{
	gpad.Initialize(適切な設定値);
}

void	loop()
{
	if (gpad.CheckKeyState())
	{
		//キー入力に応じた様々な処理
	}
}
*/
