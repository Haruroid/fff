## F(SMC) F(atFs) F(ONTX2)
STM32F4とCubeHALで動くILI9488, FatFs, FONTX2ドライバとそのサンプルです。SW4STM32向けです。テスト環境はSTM32F407VET6です。

ILI9488はFSMCで、FatFsはSPIで接続されたMMC/MMC互換カードで、FONTX2はドット描画の関数がある環境で動作が可能です。

詳しくはfff.iocを見るとわかると思います。
現状タッチパネルには対応していません。

全体的に深夜コードなのでガバガバです。ILI9488.cのputc周りは特にひどいと自分でも思ってます。なにか更新があったらついでに直すかもしれません。


[https://haruroid.0t0.jp/blog/?p=1291](https://haruroid.0t0.jp/blog/?p=1291)