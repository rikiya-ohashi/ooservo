[English](README.md) | 日本語

# TekuteruServo

TekuteruServoは、SG90と同じ感覚で扱えるシリアルサーボモーターです。

±21億°まで回転角度を指定でき、リアルタイムの位置フィードバックにも対応しています。角度制御に加え、速度を指定した連続回転も可能です。また、標準のServoライブラリに似た`attach()`や`write()`を採用しているため、簡単にプログラムできます。

ファームウェアは完全オープンソースで、Arduinoボード1枚で自由にカスタマイズできます(専用のプログラマーは不要)。

> **⚠ 互換性に関する重要な注意**
> 本ライブラリでは専用のシリアルプロトコルを使用しており、PWM方式のサーボライブラリとは異なります。そのため、標準的なPWMサーボには対応しておらず、Servo.hライブラリを使ってTekuteruServoを制御することもできません。

TekuteruServoのハードウェアは、次のページから購入できます。**[TekuteruServoを購入する](https://tekuteru.handcrafted.jp/items/121327019)**

TekuteruServoに関する不明点は[Gemini NotebookのAIチャット](https://notebook.google.com/notebook/272725f0-6a1c-4c52-9597-6384a2f88f91)で質問できます。


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
* **速度調整:** 回転速度は1 deg/s単位で設定可能。
* **リアルタイム位置フィードバック:** `read()`を呼べば、いつでも現在の角度を取得可能。
* **既存パーツとの互換性:** SG90と同じ配線、同程度のサイズ。
* **同じ感覚のAPI:** 標準Arduino Servoライブラリと同じ`attach()`や`write()`を使用可能。
* **幅広いボードに対応:** Arduino、ESP32、Raspberry Pi Picoなど、様々なマイコンで利用可能。
* **複数台に対応:** ライブラリ上の台数制限はなし。
* **自由に再プログラム可能:** Arduinoボードをプログラマーとして使うことで、専用のツールを用意することなくサーボ本体のファームウェアを更新可能。


## 仕様
* **動作電圧:** 4.8 V〜8.4 V
* **ロジック電圧:** 3.3 V〜5 V
* **最大速度:** 920 deg/s(0.066 s/60°、153 rpm) **※8.4 V時**
* **ストールトルク:** 2.1 kgf·cm **※8.4 V時**
* **ストール電流:** 1.4 A **※8.4 V時**
* **通信速度:** デフォルト9600 baud(最大57600 baudまで変更可)
* **ギア素材:** ステンレス
* **寸法:** 31.8 x 12 x 30.1 mm
* **重量:** 13 g
* **ケーブル長:** 24 cm

### 性能表
| 供給電圧 | 最大速度(deg/s) | 最大速度(rpm) | 最大速度(s/60°) | ストールトルク |
| ---: | ---: | ---: | ---: | ---: |
| **5.0 V** | 640 deg/s | 107 rpm | 0.094 s/60° | 1.5 kgf·cm |
| **6.0 V** | 720 deg/s | 120 rpm | 0.084 s/60° | 1.7 kgf·cm |
| **7.4 V** | 835 deg/s | 139 rpm | 0.072 s/60° | 2.0 kgf·cm |
| **8.4 V** | 920 deg/s | 153 rpm | 0.066 s/60° | 2.1 kgf·cm |

最大速度は無負荷時の値です。実際の速度・トルクは、個体差、電源電圧、電源容量、負荷および温度によって変化します。

### 寸法図
![寸法図](images/size.png)

## Pythonライブラリ(Raspberry Pi)
Raspberry PiでTekuteruServoをPythonから制御したい場合は、専用のPythonライブラリを使用してください:
[TekuteruServo-Python](https://github.com/tekuteru/TekuteruServo-Python)


## 配線ガイド
TekuteruServoの配線は、標準的なPWMサーボとまったく同じです。

**注:** Arduinoの接続端子やブレッドボード、ジャンパー線の接続が不安定だと誤作動の原因になります。

### Arduinoの5 Vピンを使用する場合

| 線の色 | 役割 | 接続先 |
| :--- | :--- | :--- |
| 茶 | GND | Arduino GND |
| 赤 | VCC | Arduino 5 V |
| 黄 | 信号線 | Arduino I/Oピン |

![配線図](images/wiring.png)

**注:** 動作が不安定になる場合は、[外部電源](#外部電源を使用する場合)からの給電に切り替えてください。

### 外部電源を使用する場合

| 線の色 | 役割 | 接続先 |
| :--- | :--- | :--- |
| 茶 | GND | 外部電源 GND および Arduino GND(両方のGNDを接続してください) |
| 赤 | VCC | 外部電源 +V (4.8 V〜8.4 V) |
| 黄 | 信号線 | Arduino I/Oピン |

![配線図(外部電源)](images/wiring_ext.png)

## インストール(Arduino IDE)
1. **Arduino IDE**を開きます。
2. **ライブラリマネージャ**を開きます。
3. 検索欄に「**TekuteruServo**」と入力します。
4. 最新バージョンを選び、**インストール**をクリックします。


## クラスメソッド

| メソッド | 概要 |
| :--- | :--- |
| [`attach(pin)`](#attachpin) | サーボを指定ピンに接続 |
| [`attach(pin, baud)`](#attachpin-baud) | ピン接続および通信速度を設定 |
| [`write(angle)`](#writeangle) | 最大速度で目標角度へ回転 |
| [`write(angle, speed)`](#writeangle-speed) | 指定速度(deg/s)で目標角度へ回転 |
| [`write(angle, speed, wait)`](#writeangle-speed-wait) | 速度とブロッキング挙動を指定して回転 |
| [`writeRotation(speed)`](#writerotationspeed) | 指定速度(rpm)で連続回転 |
| [`read()`](#read) | 現在の角度を取得 |
| [`stop()`](#stop) | 即座に停止 |
| [`wait()`](#wait) | 動作完了までブロック |
| [`isMoving(error)`](#ismovingerror) | 回転中かどうかを取得|
| [`setHold(hold)`](#setholdhold) | 停止後のホールド挙動を設定 |
| [`zero()`](#zero) | 現在角度を0°として保存 |
| [`firmwareVersion()`](#firmwareversion) | ファームウェアバージョンを取得 |

---

### `attach(pin)`
サーボを指定したピンに接続します。ボード上の任意のデジタルI/Oピンに接続可能です。戻り値で、接続に成功したかどうかを確認できます。

- **戻り値**: サーボが正常に接続されていれば`true`、通信エラー時は`false`。

> **注:** 他のメソッドを使う前に、必ず`attach()`を呼び出してください。

---

### `attach(pin, baud)`
ピン接続に加えて、通信速度を設定します。`9600 baud`・`19200 baud`・`38400 baud`・`57600 baud`から選択できます。サポートされていない値を指定した場合は、`9600 baud`が適用されます。サーボの電源を入れ直すと、サーボ本体の通信速度は`9600 baud`にリセットされますが、ライブラリ側が自動的に通信速度を合わせて再接続を試みます。

- **`baud`**: `9600`・`19200`・`38400`・`57600`から選択。
- **戻り値**: サーボが正常に接続されていれば`true`、通信エラー時は`false`。

**`baud`を省略して`attach(pin)`を呼び出したときの動作:**

| 状況 | 使用される`baud` |
| :--- | :--- |
| はじめて`attach()`を呼び出す場合 | `9600` |
| 2回目以降、`baud`を省略して呼び出す場合 | 前回設定した`baud` |

**通信速度:** 通信速度を上げると通信エラーが起きやすくなります。通信速度の目安は次の表を参考にしてください(◯: 安定動作 / △: 通信エラーが増加 / ×: 動作不可)。

| ボード | 9600 | 19200 | 38400 | 57600 |
| :--- | :---: | :---: | :---: | :---: |
| **Arduino Uno R3 / Nano / Pro Mini / Mega** | ◯ | △ | × | × |
| **ESP32 / Raspberry Pi Pico** | ◯ | ◯ | ◯ | ◯ |

---

### `write(angle)`
指定した角度まで、最大速度でサーボを回転させます。

電源投入直後は、現在位置が0°〜359°の範囲にマッピングされます。詳しくは[起動時の回転方向とキャリブレーション](#2-起動時の回転方向とキャリブレーション)を参照してください。

- **`angle`**: 範囲は`-2,147,483,647`〜`+2,147,483,647`。

---

### `write(angle, speed)`
指定した速度(単位: **deg/s**)で目標角度まで回転させます。

- **`speed`**: 回転速度(**deg/s**)。
  - **`0`**: 停止(`stop()`と同じ)
  - **`1`**: 最小速度(1 deg/s)
  - **上限値**: 供給電圧によって以下のように変わります:

| 供給電圧 | 上限値 |
| ---: | ---: |
| **5.0 V** | 640 deg/s |
| **6.0 V** | 720 deg/s |
| **7.4 V** | 835 deg/s |
| **8.4 V** | 920 deg/s |

**速度に関する補足:**
* **負荷がかかったとき:** 一時的に外部負荷で減速した場合は、可能な範囲でその後の速度を上げ、目標とする移動時間との差を補正します。
* **速度のばらつき:** 個体差により、実際の速度は指定値から最大±5%ほどずれることがあります。
* **低速時の動作:** 低速回転では、動きが不規則・ぎこちなくなることがあります。

---

### `write(angle, speed, wait)`
速度とブロッキング挙動を指定して、目標角度まで回転させます。

- **`angle`**: 目標角度(度)。
- **`speed`**: 回転速度(**deg/s**)。
- **`wait`**: `true`にすると、モーターが目標角度の±1°以内に到達するまでプログラムをブロックします。
- **通信エラー**: ブロックを抜け出します(`wait`が`true`のとき)。

---

### `writeRotation(speed)`
指定した速度(単位: **rpm**)でサーボを連続回転させます。

**注:** 可動範囲は`-2,147,483,647°`〜`+2,147,483,647°`です。限界値に達するとモーターは停止します。

- **`speed`**: 回転速度(**rpm**)。
  - **`1`〜上限値**: 正転
  - **`-1`〜-上限値**: 逆転
  - **`0`**: モーター停止(`stop()`と同じ)。

最大速度は供給電圧によって以下のように変わります:

| 供給電圧 | 上限値 |
| ---: | ---: |
| **5.0 V** | 107 rpm |
| **6.0 V** | 120 rpm |
| **7.4 V** | 139 rpm |
| **8.4 V** | 153 rpm |

**速度に関する補足:**
* **負荷がかかったとき:** 一時的に外部負荷で減速した場合、そのまま設定速度を維持するだけで、遅れを取り戻すための加速はしません。
* **速度のばらつき:** 個体差により、実際の速度は指定値から最大±5%ほどずれることがあります。
* **低速時の動作:** 低速回転では、動きが不規則・ぎこちなくなることがあります。

---

### `read()`
現在の角度を度単位で返します。

- **通信エラー**: 戻り値は`-2,147,483,648`(INT32_MIN)になります。
- **注:** 電源投入時の角度は360°未満にマッピングされますが、整数への丸め処理により360と表示される場合があります。

---

### `stop()`
サーボをその場で即座に停止させます。

---

### `wait()`
今の動作が完了する(目標角度の±1°以内に入る)まで、ブロックします。

- **通信エラー**: ブロックを抜け出します。
- **`writeRotation()`実行中の注意**: `writeRotation()`による連続回転中に`wait()`を呼び出した場合はブロックされません。

---

### `isMoving(error)`
`write()`または`writeRotation()`による移動コマンドの実行中かどうかを返します。回転中なら`true`、停止(目標角度の±1°以内に到達)していれば`false`です。
`write()`(`wait = true`)や`wait()`では、通信エラー発生時に単にブロックを抜けるだけですが、`isMoving(error)`をノンブロッキングで利用することで、通信エラーを明示的に検出できます。

- **`error`**: 通信エラーの状態を受け取るポインタ(`bool*`)です(省略可)。
- **通信エラー**: 戻り値は`false`になり、`error`に`true`がセットされます。
- **注:** 戻り値`true`はコマンドを実行中であることを示すのみで、実際にシャフトが回転していることを保証するものではありません(例: 外力でシャフトが拘束されている場合など)。

---

### `setHold(hold)`
目標角度に到達した後、モーターが位置を保持(`true`)するか、自由に回せる状態(`false`)にするかを設定します。サーボの電源を入れ直すと`true`にリセットされます。

- **`hold`**:
  - **`true` — ホールドモード:** 動作完了後もモーターが位置を維持し、外力が加わってもその位置に戻ろうとします。
  - **`false` — フリーモード:** 動作完了後、保持トルクを解除し、シャフトを手で自由に回せる状態にします。

- **電源投入時:** デフォルトで`true`ですが、実際に保持されるのは、`write()`・`stop()`・`setHold(true)`・`zero()`のいずれかで目標角度が一度確定した後です。

---

### `zero()`
現在の角度を0°の基準点として設定します。設定はサーボのEEPROMに保存され、電源を切っても消えません。実行すると、進行中の回転はその時点で止まります。

**注:** 保存されるのは絶対角度(0°〜359°)のみで、累積回転数はリセットされます。

---

### `firmwareVersion()`
接続中のサーボのファームウェアバージョンを返します。

- **最新バージョン**: `1`
- **注:** 常に最新バージョンに更新することを推奨します。詳しくは[ファームウェアのカスタマイズ](#ファームウェアのカスタマイズ)を参照してください。
- **通信:** `firmwareVersion()`は新たな通信を行わず、直近の`attach()`で取得した値を返します。`attach()`で通信エラーが発生すると、戻り値は`0`になります。


## 使用上の注意

### 1. 動作上の制約と安全性
* **発熱:** 連続回転を長時間続けると、モーターが発熱することがあります。
* **磁気干渉:** 大型磁石や大電流ケーブルなど、強い磁場の近くでは使用しないでください。内蔵の磁気エンコーダーが影響を受けるおそれがあります。
* **配線の取り扱い:** 内部の配線は繊細なため、ケーブルを強く引っ張ったり、無理な力を加えたりしないでください。

### 2. 起動時の回転方向とキャリブレーション
* **回転方向が逆になる問題:** 次の手順で発生します。
    1. `write(0)`を実行し、0°に移動する。
    2. 電源を切る(保持トルクがなくなる)。
    3. 外力で軸がわずかに動き、物理的に359°の位置にずれる。
    4. 電源を入れ直し、再び`write(0)`を実行する。
    5. 本来なら1°だけ前に進めば0°に着くはずが、**359°分も逆方向へ**回転してしまう。
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
    `zero()`を一度実行しておくと、その時点のモーターの物理的な位置が0°として登録されます。この設定はEEPROMに保存されるため、電源を切っても消えません。

### 3. ピンの割り当て
関数によって、1ピンに接続できるサーボの台数が異なります。サーボごとに異なる値(角度やエラー状態など)を1台ずつ受け取る必要がある関数は、1ピンにつき1台限定です。一方、全サーボへ同じ内容を送るだけで個別の応答を必要としない関数は、複数台へ同時送信できます。

* **1ピンにつき1台のみ:**
  * `write()`(`wait = true` の場合)
  * `read()`
  * `isMoving()`
  * `wait()`
* **1ピンで複数台に同時送信可能:**
  * `attach()` ※1
  * `write()`(`wait = false` の場合)
  * `writeRotation()`
  * `stop()`
  * `setHold()`
  * `zero()`
  * `firmwareVersion()` ※2

  **※1 `attach()` の注意点:**
  戻り値は「いずれかのサーボから応答があったか」を示すだけです。同じピンに接続されたすべてのサーボが正常に認識されているかまでは確認できません。

  **※2 `firmwareVersion()` の注意点:**
  ファームウェアバージョンが異なるサーボ同士を接続すると、不正確な戻り値になります。

### 4. 通信の特性と制限(割り込みと遅延)
* **通信中の割り込みブロック:** 信号パルスのタイミングを正確に保つため、データの送受信中はグローバル割り込みが一時的に無効化されます。デフォルトの9600 baudでは、1バイトの送受信につき**約1 ms**の割り込み禁止が発生します。1回のコマンド送受信は複数バイトで構成されるため、その分だけ割り込み禁止が複数回発生することになり、タイトなループで通信を繰り返すほどこの影響は大きくなります。結果として次のような影響が出る可能性があります:
  * `millis()`や`micros()`など、時間計測系の関数にわずかなずれが生じる。
  * SoftwareSerial、I2C、ハードウェアタイマーなど、他の割り込み駆動のライブラリでデータ欠落やタイミングのズレが起きる。
* **通信の遅延:** ソフトウェアによるシリアル通信という性質上、PWMサーボや高性能なハードウェアシリアルサーボに比べると応答に遅れが出ます。
* **軽減策:** リアルタイム性が求められる用途では、[`attach(pin, baud)`](#attachpin-baud)で通信速度を上げて通信時間を短縮してください。


## コード例

### 1. 角度制御
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2);  // D2ピンに接続
}

void loop() {
  myservo.write(180);  // 180°へ移動
  delay(1800);

  myservo.write(-180);  // -180°へ移動
  delay(900);

  myservo.write(720);  // 720°へ移動
  delay(1800);
}
```

### 2. 接続確認
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  Serial.begin(9600);  // シリアル通信を開始(シリアルモニタは9600 baudに設定)

  if (myservo.attach(2)) {
    Serial.println("Connected");          // 接続成功
  } else {
    Serial.println("Connection failed");  // D2ピンでサーボが見つからなかった
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
  delay(900);

  myservo.write(-180, 300);  // 300 deg/sで-180°へ移動
  delay(1500);
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

  delay(1000);

  // 方法2: wait()を別に呼び出す
  myservo.write(-180);  // -180°への移動を開始
  myservo.wait();       // -180°に到達するまで待つ

  delay(1000);

  // 方法3: ノンブロッキング(移動中もプログラムを止めずに他の処理を実行)
  myservo.write(720);           // 720°への移動を開始
  while (myservo.isMoving()) {  // 回転が終了するまで他の処理を実行
    // 例: LEDを点滅させる
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  delay(1000);
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

  currentAngle = myservo.read();  // 現在の角度を読み取る(0 ≤ angle ≤ 360)
  Serial.println(currentAngle);   // シリアルモニタに表示
}

void loop() {
  myservo.write(360, 600, true);  // 360°へ移動し、完了まで待つ
  currentAngle = myservo.read();  // 現在の角度を読み取る(想定値: 360±1)
  Serial.println(currentAngle);

  myservo.write(0, 600, true);    // 0°へ移動し、完了まで待つ
  currentAngle = myservo.read();  // 現在の角度を読み取る(想定値: 0±1)
  Serial.println(currentAngle);

  myservo.write(1800, 600);  // 1800°への移動を開始(ノンブロッキング)
  delay(1000);
  currentAngle = myservo.read();  // 回転中に角度を読み取る(想定値: 約580)
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
**注:** 複数のサーボを同時に動かす場合は、電圧降下を防ぐために[外部電源](#外部電源を使用する場合)の使用をおすすめします。

同じピンに複数のサーボを接続する場合は、[ピンの割り当て](#3-ピンの割り当て)を参照してください。

```arduino
#include <TekuteruServo.h>

TekuteruServo myservo1;
TekuteruServo myservo2;
// ソフトウェア側に台数の上限はなし(利用できるI/OピンとRAM次第)

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
  myservo.zero();  // 現在位置を0°として設定(EEPROMに保存)

  long currentAngle = myservo.read();
  if (currentAngle != -2147483648 && labs(currentAngle) < 3) {
    Serial.println("zero successful"); // zero 成功
  } else {
    Serial.println("zero failed");  // zero 失敗
  }
}

void loop() {
}
```

### 9. シリアル速度の設定
```arduino
#include <TekuteruServo.h>

TekuteruServo myservo;

void setup() {
  myservo.attach(2, 19200);  // D2ピンに接続し、通信速度を19200 baudに設定

  // 指定できる値: 9600, 19200, 38400, 57600
}

void loop() {
}
```


## ファームウェアのカスタマイズ

サーボモーターを制御しているファームウェアは`firmware/firmware.ino`にあります。

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
| **5 V** | VCC |
| **GND** | GND |

さらに、下表のプログラミングピンを、下図のプログラミングパッドに接続してください。

| Arduinoの種類 | UPDIピン |
| :--- | :--- |
| Uno R3 / Nano / Pro Mini | D6 |
| Mega | D18 |

![ファームウェア書き込み図](images/firmware_flash.png)

**注:** 4.7 kΩのUPDI抵抗は内蔵済みなので、外付けの抵抗は不要です。

### ファームウェアの書き込み

1. Arduino IDEで`firmware.ino`(またはカスタマイズしたスケッチ)を開きます。
2. **ツール**メニューで以下を設定します:
   * **ボード:** `ATtiny3226/3216/1626/1616/...` の中から `ATtiny1616`
   * **書き込み装置:** `jtag2updi`
   * **ポート:** プログラマー用Arduinoが割り当てられているCOMポート
3. ジャンパーピンをプログラミングパッドにしっかり押し当てたまま保持します。
4. その状態を保ちながら、Arduino IDEで**スケッチ > 書き込み装置を使って書き込む**をクリックし、ファームウェアを書き込みます。

**注:** ブートローダの書き込みが必要な場合は、**クロック速度**を`10 MHz`に設定してください。


## サポート&フィードバック
* **フィードバック:** 不具合や改善案があれば、[Issues](https://github.com/tekuteru/TekuteruServo/issues)や[Discussions](https://github.com/tekuteru/TekuteruServo/discussions)までお寄せください。
* **APIデザイン:** [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo)ライブラリを参考にしています。
