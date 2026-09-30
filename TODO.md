# TODO

## ビルドが通らない(最優先)

- [x] `CMakeLists.txt`: `target_include_directories`にスコープキーワード(`PRIVATE`)が無くCMake configureがエラーになる (修正済み)
- [x] `CMakeLists.txt`: `add_executable`に`src/main.cpp`しか登録されておらず、他の`.cpp`(bot, db, rcon, mc, commands, handlers)がビルド対象外 (修正済み、sqlite3のリンクも追加)
- [x] 上記修正後、実際に`cmake --build`が通るか確認する (ビルド通過確認済み)

## RCON実装(コア機能)

- [x] `RconClient::authenticate()`が空。Source RCONプロトコルの認証パケット(type=3, SERVERDATA_AUTH)を送信し、レスポンスのrequest_idが`-1`なら失敗として扱う処理を書く
- [x] `RconClient::execute()`が空。コマンド実行パケット(type=2, SERVERDATA_EXECCOMMAND)を送信し、レスポンスのbodyを返す処理を書く
- [ ] `authenticate()`と`execute()`で「ヘッダ受信→length取得→本体受信」の処理が重複している。共通の`recv_packet`ヘルパーに切り出すと保守しやすい(任意・優先度低)
- [x] `main.cpp`で`rcon.connect()`は呼んでいるが`rcon.authenticate()`を呼んでいない → 認証されないままコマンド実行することになる
- [x] `RconClient::disconnect()`を実装(socket close、二重close防止も対応済み)
- [ ] `RconClient`のデストラクタが無い。また`main.cpp`側でも`rcon.disconnect()`を明示的に呼んでいない(プログラム終了時にfdがリークする可能性)

## WhitelistHandlers

- [x] `WhitelistHandlers::execute_command`が`bool`を返していない(`return`文が無く未定義動作、ビルド時にも`-Wreturn-type`警告が出ている)。`rcon.execute()`の戻り値(レスポンス文字列)を見て成功/失敗を判定して返すようにする

## register / unregister ハンドラ

- [x] `handlers::register_handler`が「既に登録済みか」のチェックで終わっており、その先(RCONでwhitelist追加 → 成功したらDB保存 → Discordへ返信)が未実装
  - 順序注意: **先にRCONでwhitelist追加 → 成功したらDBに保存**。逆にするとDBとサーバーの状態がズレる
- [x] `handlers::unregister_handler`がDBから消すだけで、RCON側のwhitelist removeを一切呼んでいない。DBだけ消してもサーバーのホワイトリストには残ったままになる
  - 順序注意: **先にRCONでwhitelist削除 → 成功したらDBから消す**
- [x] `handlers::unregister_handler`の`else`節(未登録だった場合)の返信処理が空
- [x] `handlers::unregister_handler`のシグネチャが`dpp::slashcommand_t&`(non-const)で、`register_handler`は`const dpp::slashcommand_t&`。統一する
- [x] `unregister_handler`は`RconClient&`を受け取っていない。whitelist removeを呼ぶために引数追加が必要

## edition文字列の表記ゆれ(バグ)

- [x] `commands/register.cpp`・`commands/unregister.cpp`のBedrockの選択肢値が`"Bedrock"`(大文字始まり)になっているが、`db.cpp`側の判定は`edition == "bedrock"`(小文字)。このままだとBedrock選択時にDB処理が常に失敗する。どちらかに統一する(小文字`"bedrock"`推奨、Javaと合わせる)

## bot.cpp / main.cpp の配線漏れ

- [x] `bot.cpp`の`event_handler`で`unregister`コマンドのハンドラ呼び出しが空(`else if(command_name == "unregister"){ }`の中身が無い)
- [x] `Bot`コンストラクタで`commands::register_register(bot)`は呼んでいるが`commands::unregister_register(bot)`を呼んでいない → `/unregister`コマンド自体がDiscordに登録されない
- [x] `main.cpp`で`Bot bot(TOKEN, db, rcon);`を作った後、`bot.run()`を呼んでいない → プログラムがそのまま終了してしまう

## その他(後回しでOK)

- [ ] RCON接続が切れた場合の再接続処理(現状は起動時に1回`connect()`するだけ)
- [x] `.env.example`の内容が実際に必要な環境変数(`DISCORD_TOKEN`, `RCON_HOST`, `RCON_PORT`, `RCON_PASSWORD`)と一致しているか確認
