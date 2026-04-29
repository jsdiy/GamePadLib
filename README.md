# GamePadLib
InputLibを応用したゲームパッドライブラリ

## 概要
拙作InputLibを応用したゲームパッドライブラリです。同じく拙作のJoystickライブラリを拡張した形です。
シンプルな自作ゲームパッドデバイスを手軽にESP32シリーズで使用することができます。
アナログレバー（X軸/Y軸/プッシュスイッチ）、他3ボタン（A,B,Cボタン）をサポートします。

## 特徴
- アナログレバーを十字ボタンのように扱うことができます。もちろんADC値を取得することもできます。
- レバー／ボタンの押下状態を同期／非同期に監視することができます。
- レバー／ボタンの押下状態に応じたコールバックを登録し、非同期でキー操作に対応することができます。
- 複数ボタンの同時押しを検知することができます。
- 3ボタン(A,B,C)の押下状態を1つのGPIOで読み取ることができます。

## 配線
```
			ESP32-C3	*…BootStrap関連ピン
			[Antenna side]
			3V3		IO-0	AxisY(AnalogInput)
LCD-RESET	EN		IO-1	AxisX(AnalogInput)
LCD-DC		IO-4	IO-2*	SPI-MISO
			IO-5	IO-3	ButtonABC(AnalogInput)
SPI-SCK		IO-6	USB_D+
SPI-MOSI	IO-7	USB_D-
			IO-8*	TXD
ButtonP		IO-9*	RXD
			GND		IO-10*	SPI-SS
```
```
					+-- 3V3
GPIO(AnalogInput) --+-- AxisX（サムスティックの軸1）
					+-- GND

					+-- 3V3
GPIO(AnalogInput) --+-- AxisY（サムスティックの軸2）
					+-- GND

GPIO(DigitalInput) ---- ButtonP --- GND（サムスティックに内蔵のプッシュボタン）

					+--<2.2kR>--- 3V3
GPIO(AnalogInput) --+--<2.2kR>--- ButtonA --- GND（タクトスイッチ）
					+--<4.7kR>--- ButtonB --- GND（タクトスイッチ）
					+--< 10kR>--- ButtonC --- GND（タクトスイッチ）
```

## フォルダ構成
```
<lib>
  +--<GamePadLib>
  +--<InputLib>	GamePadLibを使用するのに必須
  +--<GLcdLib>	付属のmain.cppを動かす場合に必要
  +--<SpiDma>	付属のmain.cppを動かす場合に必要
```
[InputLibを参照](https://github.com/jsdiy/InputLib),
[GLcdLib/SpiDmaを参照](https://github.com/jsdiy/GLcdLib)

## platformio.ini
```
lib_extra_dirs =
  ./lib/InputLib	必須
  ./lib/GamePadLib	必須
  ./lib/GLcdLib		付属のmain.cppを動かす場合に必要
```

## main.cpp 最小コード例
```
#include <Arduino.h>
#include "GamePad.hpp"

GamePad	gpad;
using	Key = Joystick::KeyCode;

void	setup()
{
	gpad.Initialize(適切な設定値);
}

void	loop()
{
	if (gpad.CheckKeyState())
	{
		//キー入力に応じた様々な処理
		if (gpad.IsKeyHolding(Key::A)) { Aボタンが押されている場合の処理 }
	}
}
```

## 動作確認環境
- PlatformIO + ArduinoFramework
- ESP32-C3
- サムスティック：アルプスアルパイン製 RKJXV1224005（相当品）
- タクトスイッチ

![動作確認環境](https://github.com/jsdiy/GamePadLib/blob/main/sample/IMG_20260429_000808.jpg)
