# RCON実装の流れ

## 全体シーケンス

```
1. TCP接続 (connect)                         ← 実装済み
2. 認証 (authenticate)                        ← 未実装
3. コマンド送信・応答受信を繰り返す (execute)   ← 未実装
4. 切断 (disconnect)
```

## パケットの構造(送受信共通)

```
+--------+--------+--------+---------+------+
| Length | Req ID |  Type  |  Body   | 2x\0 |
| int32  | int32  | int32  | string  |      |
+--------+--------+--------+---------+------+
```

- `Length`: このフィールド自身を除いた残りのバイト数 (4+4+body.size()+2)
- `Type`: `3` = SERVERDATA_AUTH(認証), `2` = SERVERDATA_EXECCOMMAND(コマンド実行), `0` = SERVERDATA_RESPONSE_VALUE(応答)
- `Body`: 文字列本体
- 末尾に `\0` が2つ (bodyの終端 + パケット終端)

送信側 (`send_packet`) は実装済み。足りないのは受信側。

## 1. authenticate() の流れ

1. `send_packet(request_id=適当な数, type=3(AUTH), body=password)`
2. サーバーからの応答を受信する
   - 最初にheaderの4byte(length)だけ受信 → lengthが分かる
   - 残りlengthバイトを受信 → その中にrequest_id, type, bodyが入っている
3. 受信したrequest_idを見る
   - 送った時のrequest_idと一致していれば認証成功
   - `-1` が返ってきたら認証失敗(パスワード間違い)

注意: 認証成功時、サーバーが2パケット返すことがある(空のSERVERDATA_RESPONSE_VALUE + 認証結果)。まず1つ受信してrequest_idを見る形で作り、動作確認しながら調整する。

## 2. execute() の流れ

1. `send_packet(request_id=適当な数, type=2(EXECCOMMAND), body=command)`
2. 受信も同様(header読む → length分読む)
3. 受信したbody文字列を戻り値として返す

## 追加で必要な部品

`recv_all` は実装済み(バイト数指定で確実に受信する関数)。それを使って1パケット分を受信・分解する関数を新たに作る。

```cpp
struct RconPacket {
    int32_t request_id;
    int32_t type;
    std::string body;
};

bool recv_packet(int socket_fd, RconPacket& out);
```

これを1つ作れば `authenticate()` と `execute()` の両方から使い回せる。

## 実装順

1. `read_int32_le`(`write_int32_le` の逆)を書く
2. `recv_packet` を書く
   - `recv_all` で4byte読んでlength取得
   - 残りlength分を `recv_all` で読んでrequest_id/type/bodyに分解
3. `authenticate()` を `send_packet` + `recv_packet` で実装
4. `execute()` を同様に実装
5. `main.cpp` に `rcon.authenticate()` の呼び出しを追加(現状呼ばれていない)
