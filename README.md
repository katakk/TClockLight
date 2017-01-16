# TClock Light kt [![Build status](https://ci.appveyor.com/api/projects/status/e2rljlqdk6gpql89/branch/master?svg=true)](https://ci.appveyor.com/project/katakk/tclocklight/branch/master) [![coverity](https://scan.coverity.com/projects/11483/badge.svg?flat=1)](https://scan.coverity.com/projects/tclocklight)

This is a modified version of TClock Light.

## :rabbit: 手元のWindows 10 の時計で秒表示、あわよくばミリ秒表示させたいために変更しました。

:rabbit2:ミリ秒表示をデフォルト設定として追加しました。フォーマット自動生成は固定値にしてます。config.h で設定してね。生成ルーチン消しました。

:rabbit2:時計の再描画間隔は5ms にしてみました。config.h で設定してね。

:rabbit2:よく考えたら 1/24 秒以下は読めないことに気が付きました。KK というフォーマット設定で　24コマ表記を追加しました。

## :rabbit: TClock Light kt の機能・ソース量削除版です。 ソースは消せるだけしました。

:rabbit2:config-kt は すべて　define TC_ENABLE* 0 に変更しました。その後ソース消しました。:clock10:も消しました。

:rabbit2:lang いらなくしました。生成ルーチン消しました。

:rabbit2:コンテキストメニューも消しましたので既存の時計のコンテキストメニューが表示されます。

## :rabbit2:おわるときはtcprop.exe から終了させれます。

## :rabbit:Windows XPで起動しなかったので起動するように変更。

dwmapi.dll の静的参照消した

フォントは XPだと Verdana 　10だと Segoe UI が見やすくて良いです。
