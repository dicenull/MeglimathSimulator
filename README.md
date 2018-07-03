# MeglimathSimulator
とある陣取りゲームのシミュレーションが行えるプログラムです。

## 概要
+ このシステムは2つのクライアントとサーバーから構成されます。
+ サーバーに二つのクライアントが接続されるとゲームが開始されます。

## ルール
大会要項を確認してください

## 操作方法
### キーボート操作
+ 右方向から反時計回りに、D, E, W, Q, A, Z, X, Cキーで方向を決めます
+ Shiftキーを押しながらの場合、タイル除去 押さない場合、移動となります。
+ 停留はSキーを押します。

## 動作の流れ
![demo](https://github.com/Eulerd/MeglimathSimulator/blob/release/media/demo.gif)
+ サーバーを起動した後、クライアントを二つ起動する
+ クライアントのアクションが完了するとサーバーがターンを進める

# 使用ライブラリ
+ [boost 1.67.0](https://www.boost.org/users/history/version_1_67_0.html)
    - インクルードパス : BOOST_INCLUDE_PATH
+ [rapidjson](https://github.com/Tencent/rapidjson/tree/master/include/rapidjson)
    - インクルードパス : RAPIDJSON
+ [OpenSiv3D](https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(Windows))
    - インクルードパス : (自動設定)
  
+ 各パスをユーザ環境変数として設定してください。