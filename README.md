# misora2_qr
## 内容
 - 受け取った画像からqrコードの検出・デコードを行う
 - 検出成功時: デコード内容(String), その時の画像(Image)を返す
## 実行前に
 - QRコード専用ライブラリ**ZBAR**をインストールしなければならない
~~~bash!
sudo apt install libzbar-dev
sudo apt update
sudo apt upgrade
~~~
## 実行コード
 - ノード単体として実行
~~~bash!
colcon build --symlink-install
source install/setup.bash
ros2 run misora2_qr qr_node # qr_imageトピックで画像を受け取る
~~~
 - C++プログラムとして実行
~~~bash!
colcon build --symlink-install
./build/misora2_qr/qr_reader <画像パス> #ワークスペースから実行 src/misora2_qr/test.png
~~~