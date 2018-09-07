## F(SMC) F(atFs) F(ONTX2)
STM32F4とCubeHALで動くILI9488, FatFs, FONTX2ドライバとそのサンプルです。SW4STM32向けです。テスト環境はSTM32F407VET6です。

![img1](https://haruroid.0t0.jp/blog/wp-content/uploads/2018/09/20180907_230013.jpg)

ILI9488はFSMCで、FatFsはSPIで接続されたMMC/MMC互換カードで、FONTX2はドット描画の関数がある環境で動作が可能です。

詳しくはfff.iocを見るとわかると思います。
~~現状タッチパネルには対応していません。~~
→ガバガバ対応を施しました。あくまで若干読めてるレベルの出来です。

全体的に深夜コードなのでガバガバです。~~ILI9488.cのputc周りは特にひどいと自分でも思ってます。なにか更新があったらついでに直すかもしれません。~~→多少改善しました

現在できること:

・ILI9488を操作する

・BMPを表示する

・FONTX2形式のフォントを描画する

・SPI接続のSDをChan氏のFatFsで操作する

・HR2046(TSC2046互換？)を若干操作する

・ソフトウェアキーボードでアルファベットを入力する

このデモの使い方:
まず電源を入れるとSDの中のtux.bmpを表示します。どこかタッチするとソフトウェアキーボードが出てくるの何か入力してEnter(E)を押すと次の画面で表示されます

![img2](https://haruroid.0t0.jp/blog/wp-content/uploads/2018/09/20180907_225953.jpg)

![img3](https://haruroid.0t0.jp/blog/wp-content/uploads/2018/09/20180907_230007.jpg)



[https://haruroid.0t0.jp/blog/?p=1291](https://haruroid.0t0.jp/blog/?p=1291)