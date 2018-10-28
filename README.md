# MeglimathSimulator
とある陣取りゲームのシミュレーションが行えるプログラムです

## 概要
+ このシステムは2つのクライアントとサーバーから構成されます
+ サーバーに二つのクライアントが接続されるとゲームが開始されます

## ルール
大会要項を確認してください

## 動作の流れ
![demo](https://github.com/Eulerd/MeglimathSimulator/blob/master/media/demo.gif)
+ サーバを起動する
+ 別アプリケーションからサーバにフィールド情報をjson形式で送信する
+ クライアントをコマンドライン引数を2つ入力して起動する
    - 第一引数
        * 0 : Blue
        * 1 : Red
    - 第二引数
        0 ~ 9でクライアントを選択する
        * 4 : 本戦で使用したクライアント
        * 5 : UIクライアント
+ ターンごとにクライアントがアクションを送信する
+ サーバは新しいフィールドを二つのクライアントに送信する

# 使用ライブラリ
+ [boost 1.67.0](https://www.boost.org/users/history/version_1_67_0.html)
    - インクルードパス : BOOST_INCLUDE_PATH
+ [rapidjson](https://github.com/Tencent/rapidjson/tree/master/include/rapidjson)
    - インクルードパス : RAPIDJSON
+ [OpenSiv3D](https://scrapbox.io/Siv3D/OpenSiv3D_%E3%82%92%E3%81%AF%E3%81%98%E3%82%81%E3%82%8B%E6%BA%96%E5%82%99_(Windows))
    - インクルードパス : (自動設定)
  
+ 各パスをユーザ環境変数として設定してください。
