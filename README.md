# AddMCIDBot

Discord のスラッシュコマンドで、Minecraft の MCID(Java 版 / 統合版)をホワイトリストに登録・解除する Bot です。
[Geyser](https://geysermc.org/) と [Floodgate](https://geysermc.org/wiki/floodgate/) を使って、Java 版と統合版の両方のプレイヤーが参加できるようにしたサーバーを対象にしています。
RCON 経由でサーバーを操作し、Discord ID と MCID の対応を SQLite に保存します。

## コマンド

| コマンド | 引数 | 動作 |
|---|---|---|
| `/register` | `edition`(Java / Bedrock)、`mcid` | ホワイトリストに追加し、DB に保存する |
| `/unregister` | `edition` | ホワイトリストから削除し、DB から削除する |

- 1ユーザーが、Edition ごとに1つの MCID を登録できます。
- サーバー操作(RCON)が成功した後に DB を更新します。DB の更新に失敗した場合は、サーバー側を元に戻します。

## 必要なもの

- C++20 対応のコンパイラ、CMake 3.20 以上
- ライブラリ
  - [D++ (DPP)](https://dpp.dev/)
  - [laserpants/dotenv-0.9.3](https://github.com/laserpants/dotenv-cpp)
  - SQLite3
- RCON を有効にした Minecraft サーバー
  - 統合版のプレイヤーは Geyser 経由で参加し、Floodgate の `fwhitelist` コマンドでホワイトリストに登録します。
  - Geyser と Floodgate が入っていない場合、Bedrock の登録は使えません(Java 版のみ)。

## 依存ライブラリのインストール

Debian / Ubuntu 系での例です。

```sh
# ビルドツールと SQLite3、D++ の依存ライブラリ
sudo apt update
sudo apt install -y build-essential cmake git wget \
    libsqlite3-dev libssl-dev zlib1g-dev libopus-dev libsodium-dev
```

```sh
# D++ (.deb パッケージ)
wget -O dpp.deb https://dl.dpp.dev/
sudo apt install -y ./dpp.deb
```

```sh
# laserpants/dotenv (ソースからビルドして /usr/local にインストール)
git clone https://github.com/laserpants/dotenv-cpp.git
cd dotenv-cpp
cmake -S . -B build
cmake --build build
sudo cmake --install build
cd ..
```

## セットアップ

1. `.env.example` をコピーして `.env` を作成し、値を設定します。
2. Minecraft サーバーの `server.properties` で RCON を有効にします。
   ```properties
   enable-rcon=true
   rcon.port=25575
   rcon.password=your_password
   ```
3. [Discord Developer Portal](https://discord.com/developers/applications) で Bot を作成し、サーバーに招待します。
   招待 URL の scope には `bot` と `applications.commands` の両方を指定してください。

## 環境変数

| 変数 | 内容 |
|---|---|
| `DISCORD_TOKEN` | Bot のトークン |
| `RCON_HOST` | RCON の接続先ホスト |
| `RCON_PORT` | RCON のポート番号 |
| `RCON_PASSWORD` | RCON のパスワード |
| `DB_PATH` | DB ファイルの場所(省略時は `data/mc.db`) |

## ビルドと実行

```sh
./build.sh
./run.sh
```

`.env` と `data/mc.db` はカレントディレクトリを基準に読み込むため、プロジェクトのルートから実行してください。
`data/` ディレクトリが無い場合は、起動時に自動で作成されます。

## 注意事項

- Velocity などのプロキシ配下で使う場合、RCON の接続先はプロキシではなく、ホワイトリストを持つバックエンドのサーバーにしてください。
- 統合版の `fwhitelist` コマンドは RCON に応答を返しません。そのため、存在しない Gamertag を指定しても、Bot は成功として扱います。
- `data/mc.db` は Git で管理していません。

## ライセンス

[MIT License](LICENSE)
