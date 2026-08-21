[English](README.md) | 日本語

# TekuteruServo

TekuteruServoは、SG90と同じ感覚で扱えるシリアルサーボモーターです。

±21億°まで絶対角度を指定でき、リアルタイムの位置フィードバックにも対応しています。角度制御に加えて、速度を指定した連続回転にも対応しています。また、標準のServoライブラリに似た`attach()`や`write()`を採用しているため、簡単にプログラムできます。

ファームウェアは完全オープンソースで、Arduinoボード1枚で自由にカスタマイズできます(専用の書き込み機は不要)。

> **⚠ 互換性に関する重要な注意**
> 本ライブラリでは専用のシリアルプロトコルを使用しており、PWM方式のサーボライブラリとは異なります。そのため、標準的なPWMサーボには対応しておらず、Servo.hライブラリを使ってTekuteruServoを制御することもできません。

TekuteruServoのハードウェアは、次のページから購入できます。**[TekuteruServoを購入する](https://tekuteru.handcrafted.jp/items/121327019)**

TekuteruServoに関する不明点は[Gemini NotebookのAIチャット](https://notebooklm.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91)で質問できます。


## 目次
- [特徴](#特徴)
- [仕様](#仕様)
- [Pythonライブラリ(Raspberry Pi)](#pythonライブラリraspberry-pi)
- [配線ガイド](#配線ガイド)
- [インストール(Arduino IDE)](#インストールarduino-ide)
- [クラスメソッド](#クラスメソッド)
- [使用上の注意](#使用上の注意)
- [コード例](#コード例)
- [ファームウェアのカスタマイズ](#ファームウェアのカスタマイズ)
- [サポート&フィードバック](#サポートフィードバック)


## 特徴
* **高精度な多回転位置決め:** -2,147,483,647°〜+2,147,483,647°(±596万回転)を±1°の精度で制御。
* **デュアルモード動作:** 角度制御と連続回転(速度制御)のどちらにも対応。
* **速度調整:** 回転速度は1 deg/s単位で自由に設定可能。
* **リアルタイム位置フィードバック:** `read()`を呼べば、いつでも現在の角度を取得可能。
* **既存パーツとの互換性:** SG90と同じ配線、同程度のサイズ。
* **同じ感覚のAPI:** 標準Arduino Servoライブラリと同じ`attach()`や`write()`を使用可能。
* **幅広いボードに対応:** Arduino、ESP32、Raspberry Pi Picoなど、様々なマイコンで利用可能。
* **複数台に対応:** ライブラリ上の台数制限はなし。実際に使用できる台数は、I/Oピン数、RAM次第。
* **自由に再プログラム可能:** Arduinoボードをプログラマーとして使うことで、専用のツールを用意することなくサーボ本体のファームウェアを更新可能。


## 仕様
* **動作電圧:** 4.0 V〜8.4 V
* **ロジック電圧:** 3.3 V〜5 V
* **最大速度:** 930 deg/s(0.065 s/60°、155 rpm) **※8.4 V時**
* **ストールトルク:** 2.1 kgf·cm **※8.4 V時**
* **ストール電流:** 1.4 A **※8.4 V時**
* **通信速度:** デフォルト9600 baud(最大57600 baudまで変更可)
* **ギア素材:** ステンレス
* **寸法:** 31.8 x 12 x 30.1 mm
* **重量:** 13 g
* **ケーブル長:** 24 cm

### 性能表
| 供給電圧 | 最大速度(deg/s) | (rpm) | (s/60°) | ストールトルク |
| ---: | ---: | ---: | ---: | ---: |
| **5.0 V** | 650 deg/s | 108 rpm | 0.093 s/60° | 1.5 kgf·cm |
| **6.0 V** | 760 deg/s | 126 rpm | 0.079 s/60° | 1.7 kgf·cm |
| **7.4 V** | 840 deg/s | 140 rpm | 0.072 s/60° | 2.0 kgf·cm |
| **8.4 V** | 930 deg/s | 155 rpm | 0.065 s/60° | 2.1 kgf·cm |

最大速度は無負荷時の値です。実際の速度・トルクは、個体差、電源電圧、電源容量、負荷および温度によって変化します。


## Pythonライブラリ(Raspberry Pi)
Raspberry PiでTekuteruServoをPythonから制御したい場合は、専用のPythonライブラリを使用してください:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## 配線ガイド
TekuteruServoの配線は、標準的なPWMサーボとまったく同じです。

### Arduinoの5Vピンを使用する場合

| 線の色 | 役割 | 接続先 |
| :--- | :--- | :--- |
| 茶 | GND | Arduino GND |
| 赤 | VCC | Arduino 5V |
| 黄 | 信号線 | Arduino I/Oピン |

![配線図](images/wiring.png)

### 外部電源を使用する場合

複数台のサーボを動かす場合や大トルクを要する動作を行う場合は、Arduinoの5V端子ではなく外部電源から電源を供給してください。

| 線の色 | 役割 | 接続先 |
| :--- | :--- | :--- |
| 茶 | GND | 外部電源 GND および Arduino GND（※共通化） |
| 赤 | VCC | 外部電源 +V (4.0V〜8.4V) |
 黄 | 信号線 | Arduino I/Oピン |

![配線図(外部電源)](images/wiring_ext.png)

## インストール(Arduino IDE)
1. **Arduino IDE**を開きます。
2. **ライブラリマネージャ**を開きます。
3. 検索欄に「**TekuteruServo**」と入力します。
4. 最新バージョンを選び、**インストール**をクリックします。


## クラスメソッド

| メソッド | 概要 |
| :--- | :--- |
| [`attach(pin)`](#attachpin) | サーボを指定ピンに接続する |
| [`write(angle)`](#writeangle) | 最大速度で目標角度へ回転(ノンブロッキング) |
| [`write(angle, speed)`](#writeangle-speed) | 指定速度(deg/s)で目標角度へ回転 |
| [`write(angle, speed, wait)`](#writeangle-speed-wait) | 速度とブロッキング挙動を指定して回転 |
| [`writeRotation(speed)`](#writerotationspeed) | 指定速度(rpm)で連続回転 |
| [`read()`](#read) | 現在の角度を取得 |
| [`stop()`](#stop) | 即座に停止 |
| [`wait()`](#wait) | 動作完了までブロック |
| [`isMoving(&hasError)`](#ismovinghaserror) | 回転中かどうかを取得 |
| [`setHold(hold)`](#setholdhold) | 停止後のホールド挙動を設定 |
| [`setZero()`](#setzero) | 現在角度を0°として保存 |
| [`setSerialSpeed(baud)`](#setserialspeedbaud) | 通信速度を設定 |
| [`getFirmwareVersion()`](#getfirmwareversion) | ファームウェアバージョンを取得 |

---

### `attach(pin)`
```cpp
bool attach(uint8_t pin);
```
サーボを指定したピンに接続します。ボード上の任意のデジタルI/Oピンに接続可能です。
戻り値で、接続に成功したかどうかを確認できます。
- **戻り値**: サーボが正常に接続されていれば`true`、通信エラー時は`false`。

> **注:** 他のメソッドを使う前に、必ず`attach()`を呼び出してください。

---

### `write(angle)`
```cpp
void write(int32_t angle);
```
指定した角度まで、最大速度でサーボを回転させます(ノンブロッキング)。
電源投入直後は、現在位置が0°〜359°の範囲にマッピングされます。詳しくは[起動時の回転方向とキャリブレーション](#2-起動時の回転方向とキャリブレーション)を参照してください。
- **`angle`**: 範囲は`-2,147,483,647`〜`+2,147,483,647`。

---

### `write(angle, speed)`
```cpp
void write(int32_t angle, uint16_t speed);
```
指定した速度(単位: **deg/s**)で目標角度まで回転させます。

- **`speed`**: 回転速度(**deg/s**)。
  - **`0`**: 停止(`stop()`と同じ)
  - **`1`**: 最小速度(1 deg/s)
  - **上限値**: 供給電圧によって以下のように変わります:

| 供給電圧 | `speed`の上限 | 最大速度 |
| ---: | ---: | ---: |
| **5.0 V** | **650** | 650 deg/s |
| **6.0 V** | **760** | 760 deg/s |
| **7.4 V** | **840** | 840 deg/s |
| **8.4 V** | **930** | 930 deg/s |

**速度に関する補足:**
* **負荷がかかったとき:** 一時的に外部負荷で減速した場合は、可能な範囲でその後の速度を上げ、目標とする移動時間との差を補正します。
* **速度のばらつき:** 個体差により、実際の速度は指定値から最大±5%ほどずれることがあります。
* **低速時の滑らかさ:** 無負荷の低速回転では、動きが不規則・ぎこちなくなることがあります。

---

### `write(angle, speed, wait)`
```cpp
void write(int32_t angle, uint16_t speed, bool wait);
```
速度とブロッキング挙動を指定して、目標角度まで回転させます。

- **`angle`**: 目標位置(度)。
- **`speed`**: 回転速度(**deg/s**)。
- **`wait`**: `true`にすると、モーターが目標位置の±1°以内に到達するまでプログラムをブロックします。
- **通信エラー**: ブロックを抜け出します(`wait`が`true`のとき)。

---

### `writeRotation(speed)`
```cpp
void writeRotation(int16_t speed);
```
指定した速度(単位: **rpm**)でサーボを連続回転させます。次のコマンドを送るまで、回転はそのまま続きます。

**注:** 可動範囲は`-2,147,483,647°`〜`+2,147,483,647°`です。限界値に達するとモーターは停止します。

- **`speed`**: 回転速度(**rpm**)。
  - **`1`〜上限値**: 正転
  - **`-1`〜-上限値**: 逆転
  - **`0`**: モーター停止(`stop()`と同じ)。

最大速度は供給電圧によって以下のように変わります:

| 供給電圧 | `speed`の上限 | 最大速度 |
| ---: | ---: | ---: |
| **5.0 V** | **108** | 108 rpm |
| **6.0 V** | **126** | 126 rpm |
| **7.4 V** | **140** | 140 rpm |
| **8.4 V** | **155** | 155 rpm |

**速度に関する補足:**
* **負荷がかかったとき:** 一時的に外部負荷で減速した場合、そのまま設定速度を維持するだけで、遅れを取り戻すための加速はしません。
* **速度のばらつき:** 個体差により、実際の速度は指定値から最大±5%ほどずれることがあります。
* **低速時の滑らかさ:** 無負荷の低速回転では、動きが不規則・ぎこちなくなることがあります。

---

### `read()`
```cpp
int32_t read();
```
現在の角度を度単位で返します。
- **通信エラー**: 戻り値は`-2,147,483,648`になります。

---

### `stop()`
```cpp
void stop();
```
サーボをその場で即座に停止させます。

---

### `wait()`
```cpp
void wait();
```
今の動作が完了する(目標位置の±1°以内に入る)まで、実行をブロックします。
- **通信エラー**: ブロックを抜け出します。
- **`writeRotation()`実行中の注意**: `writeRotation()`による連続回転中に`wait()`を呼び出した場合はブロックされません。

---

### `isMoving(&hasError)`
```cpp
bool isMoving(bool* hasError = nullptr);
```
サーボが回転中なら`true`、止まっていれば`false`を返します。
- **通信エラー**: 戻り値は`false`になります。`hasError`にポインタを渡しておけば、エラーの有無を別途受け取れます(エラー時は`true`)。

---

### `setHold(hold)`
```cpp
void setHold(bool hold);
```
目標位置に到達した後、モーターが位置を保持するかどうかを設定します。電源を入れ直すと自動的にtrueへリセットされます。
- **`hold`**:
  - **`true` — アクティブホールド(デフォルト):** 動作完了後もモーターが位置を維持し、外力が加わってもその位置に戻ろうとします。
  - **`false` — パッシブモード:** 保持トルクを解除し、シャフトを手で自由に回せる状態にします。

---

### `setZero()`
```cpp
void setZero();
```
現在の角度を0°の基準点として設定します。設定は不揮発性メモリ(EEPROM/Flash)に保存され、電源を切っても消えません。実行すると、進行中の回転はその時点で止まります。
**注:** 保存されるのは絶対角度(0°～359°)のみで、累積回転数はリセットされます。

---

### `setSerialSpeed(baud)`
```cpp
bool setSerialSpeed(uint16_t baud);
```
通信速度を設定します。電源を入れ直すと**9600 baud**に戻ります。実行すると、進行中の回転はその時点で止まります。
- **`baud`**: `9600`・`19200`・`38400`・`57600`から選択。
- **戻り値**: 変更に成功すれば`true`、通信エラーや無効な`baud`値の場合は`false`。
- **注:** 通信速度を上げると通信エラーが起きやすくなり、特に`read()`の信頼性に影響が出ることがあります。

---

### `getFirmwareVersion()`
```cpp
uint8_t getFirmwareVersion();
```
接続中のサーボのファームウェアバージョンを返します。

- **最新バージョン**: `1`
- **注:** 常に最新バージョンに更新することを推奨します。詳しくは[ファームウェアのカスタマイズ](#ファームウェアのカスタマイズ)を参照してください。
- **通信エラー**: 戻り値は`0`になります。


## 使用上の注意

### 1. 動作上の制約と安全性
* **発熱:** 連続回転を長時間続けると、モーターが発熱することがあります。
* **磁気干渉:** 大型磁石や大電流ケーブルなど、強い磁場の近くでは使用しないでください。内蔵の磁気エンコーダーが影響を受けるおそれがあります。
* **配線の取り扱い:** 内部の配線は繊細なため、ケーブルを強く引っ張ったり、無理な力を加えたりしないでください。

### 2. 起動時の回転方向とキャリブレーション
* **回転方向が逆になる問題:** 電源を入れた直後、モーターが物理的に359°の位置にあったとします。本来なら1°だけ前に進めば0°(=360°)に着くはずですが、この状態で`write(0)`を実行すると、**359°分も逆方向へ**回転してしまいます。
* **対処法:** 次の2つの方法があります。
    * **方法1: 起動時に近い方へ移動させる**
    電源投入直後に現在位置を読み取り、0°と360°のどちらに近いかを判定して、近い方を目標にします。
    ```arduino
    long currentAngle = myservo.read();
    if (currentAngle > 180) {
      myservo.write(360); // 0°ではなく360°を目標にする
    } else {
      myservo.write(0);
    }
    ```
    * **方法2: 原点をキャリブレーションしておく**
    `setZero()`を一度実行しておくと、その時点のモーターの物理的な位置が0°として登録されます。この設定は不揮発性メモリに保存されるため、電源を切っても消えません。

### 3. ピンの割り当て
1つのピンに接続できるサーボの台数は、使用する関数によって異なります。

* **1ピンにつき1台のみ:**
  * `write()`（`wait = true` の場合）
  * `read()`
  * `isMoving()`
  * `wait()`
* **1ピンで複数台に同時送信可能:**
  * `attach()` ※1
  * `write()`（`wait = false` の場合）
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `setZero()`
  * `setSerialSpeed()` ※2
  * `getFirmwareVersion()` ※3

  **※1 `attach()` の注意点:**
  戻り値は「いずれかのサーボから応答があったか」を示すだけです。同じピンに接続されたすべてのサーボが正常に認識されているかまでは確認できません。

  **※2 `setSerialSpeed()` の注意点:**
  1台でも通信エラーが発生した場合は、そのピンに接続されているどのサーボの通信速度も変更されません。また、戻り値が `false` になります。

  **※3 `getFirmwareVersion()` の注意点:**
  ファームウェアバージョンが異なるサーボ同士を接続すると、不正確な戻り値になります。

### 4. 通信の特性と制限(遅延と割り込み)
* **通信中は割り込みがブロックされる:** 信号パルスのタイミングを正確に保つため、データの送受信中はグローバル割り込みが一時的に無効化されます(`noInterrupts()`)。デフォルトの9600 baudでは、1回のコマンド送受信で割り込みが**数ミリ秒**ブロックされることがあり、タイトなループで通信を繰り返すほどこの影響は大きくなります。結果として次のような影響が出ることがあります:
  * `millis()`や`micros()`など、時間計測系の関数にわずかなずれが生じる。
  * SoftwareSerial、I2C、ハードウェアタイマーなど、他の割り込み駆動のライブラリでデータ欠落やタイミングのズレが起きる。
* **通信の遅延:** ソフトウェアによるシリアル通信という性質上、PWMサーボや高性能なハードウェアシリアルサーボに比べると応答に遅れが出ます。
* **軽減策:** 厳密なリアルタイム性や低遅延が求められる用途では、`setSerialSpeed()`で通信速度を上げることで、ブロッキング時間を短くできます。


## コード例

### 1. 基本的な回転
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);  // D2ピンに接続
}

void loop() {
  myservo.write(180);  // 180°へ移動
  delay(1000);

  myservo.write(-180);  // -180°へ移動
  delay(2000);

  myservo.write(720);  // 720°へ移動
  delay(3000);
}
```

### 2. 接続確認
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);  // シリアル通信を開始(シリアルモニタは9600 baudに設定)

  if (myservo.attach(2)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed");  // ピン2でサーボが見つからなかった
    while (!myservo.attach(2)) {          // 接続できるまで待つ
      delay(100);
    }
    Serial.println("Connected");
  }
}

void loop() {
}
```

### 3. 速度制御
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.write(180, 600);  // 600 deg/sで180°へ移動
  delay(1000);

  myservo.write(-180, 300);  // 300 deg/sで-180°へ移動
  delay(3000);
}
```

### 4. 動作完了を待つ
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
  pinMode(LED_BUILTIN, OUTPUT);  // LED点滅の例のために設定
}

void loop() {
  // 方法1: 第3引数にtrueを渡してブロッキングにする
  myservo.write(180, 600, true);  // 180°へ移動し、完了(±1°以内)まで待つ

  // 方法2: wait()を別に呼び出す
  myservo.write(-180);  // -180°への移動を開始
  myservo.wait();       // -180°に到達するまで待つ

  // ノンブロッキング: モーターが動いている間もプログラムは先に進む
  myservo.write(720);           // 720°への移動を開始
  while (myservo.isMoving()) {  // モーター動作中に他の処理を実行
    // 例: LEDを点滅させる
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}
```

### 5. 現在の角度を読み取る
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

long currentAngle;  // read()の戻り値の型に合わせてlong(int32_t)を使用

void setup() {
  Serial.begin(9600);  // シリアル通信を開始(シリアルモニタは9600 baudに設定)

  myservo.attach(2);

  currentAngle = myservo.read();  // 現在の角度を読み取る(0 ≤ angle ≤ 359)
  Serial.println(currentAngle);   // シリアルモニタに表示
}

void loop() {
  myservo.write(360, 600, true);  // 360°へ移動し、完了まで待つ
  currentAngle = myservo.read();  // 現在の角度を読み取る(想定値: 360±1)
  Serial.println(currentAngle);

  myservo.write(0, 600, true);    // 0°へ移動し、完了まで待つ
  currentAngle = myservo.read();  // 現在の角度を読み取る(想定値: 0±1)
  Serial.println(currentAngle);

  myservo.write(1800);  // 1800°への移動を開始(ノンブロッキング)
  delay(1000);
  currentAngle = myservo.read();  // 回転中に角度を読み取る(モーターはまだ動いている)
  Serial.println(currentAngle);
  myservo.wait();  // 1800°に到達するまで待つ
}
```

### 6. 連続回転
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);
}

void loop() {
  myservo.writeRotation(100);  // 100 rpmで正転
  delay(3000);

  myservo.writeRotation(-50);  // 50 rpmで逆転
  delay(3000);

  myservo.writeRotation(0);  // 停止(stop()と同じ)
  delay(2000);
}
```

### 7. 複数サーボ
**注:** 複数のサーボを同時に動かす場合は、動作を安定させ電圧降下を防ぐために外部電源の使用をおすすめします。

```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1;
TekuteruServo myservo2;
// ソフトウェア側に台数の上限はなし — 利用できるI/OピンとRAM次第

void setup() {
  myservo1.attach(2);
  myservo2.attach(3);
}

void loop() {
  myservo1.write(180, 600);
  myservo2.write(180, 600);
  myservo1.wait();  // myservo1が180°に到達するまで待つ
  myservo2.wait();  // myservo2が180°に到達するまで待つ

  myservo1.write(-180, 600);
  myservo2.write(-180, 300);
  myservo1.wait();
  myservo2.wait();
}
```

### 8. ゼロ点設定
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);

  myservo.attach(2);
  myservo.setZero();  // 現在位置を0°として設定(不揮発性メモリに保存)

  Serial.println("setZero successful");
}

void loop() {
}
```

### 9. シリアル速度の設定
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);

  // 指定できる値: 9600, 19200, 38400, 57600
  // 注: 電源を入れ直すと9600 baudに戻ります。
  myservo.setSerialSpeed(19200);
}

void loop() {
}
```


## ファームウェアのカスタマイズ

サーボモーターを制御している内部プログラムは`firmware/TekuteruServo_firmware/TekuteruServo_firmware.ino`にあります。

### 事前準備

1. **ボードマネージャURL:** Arduino IDEの**ファイル > 基本設定**を開き、「追加のボードマネージャのURL」欄に次のURLを貼り付けます:
   ```
   https://drazzy.com/package_drazzy.com_index.json
   ```
2. **megaTinyCore:** Arduino IDEのボードマネージャからインストールします。
3. **jtag2updi:** [SpenceKonde/jtag2updi](https://github.com/SpenceKonde/jtag2updi)からスケッチをダウンロードします。
   * `jtag2updi`スケッチをArduinoに書き込みます。これでそのArduinoは、UPDIプログラマーとして使える状態になります。

### 配線

サーボ底面のネジ4本を外し、下部カバーを開けて内部基板にアクセスします。プログラマー用Arduinoを、以下の通りサーボに接続してください。

| Arduinoのピン | サーボ側の接続先 |
| :--- | :--- |
| **5V** | VCC |
| **GND** | GND |
| プログラミングピン(下表参照) | プログラミングパッド(UPDI) |

プログラミングピンは、使用するArduinoの種類によって異なります。

| Arduinoの種類 | Arduinoのピン |
| :--- | :--- |
| Uno R3 | D6 |
| Nano | D6 |
| Pro Mini | D6 |
| Mega 2560 | D18 |

**注:** ジャンパーピンをプログラミングパッドにしっかり差し込み、安定した接触を保ってください。4.7kΩのUPDI抵抗は内蔵済みなので、外付けの抵抗は不要です。

![ファームウェア書き込み図](images/firmware_flash.png)

### ファームウェアの書き込み

1. Arduino IDEで`TekuteruServo_firmware.ino`(またはカスタマイズしたスケッチ)を開きます。
2. **ツール**メニューで以下を設定します:
   * **ボード:** `ATtiny3226/3216/1626/1616/...` の中から `ATtiny1616`
   * **書き込み装置:** `jtag2updi`
   * **ポート:** プログラマー用Arduinoが割り当てられているCOMポート
3. ジャンパーピンをプログラミングパッドにしっかり押し当てたまま保持します。
4. その状態を保ちながら、Arduino IDEで**スケッチ > 書き込み装置を使って書き込む**をクリックし、ファームウェアを書き込みます。


## サポート&フィードバック
* **フィードバック:** 不具合や改善案があれば、[Issues](https://github.com/rikiya-ohashi/ooservo/issues)や[Discussions](https://github.com/rikiya-ohashi/ooservo/discussions)までお寄せください。
* **APIデザイン:** [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo)ライブラリを参考にしています。
