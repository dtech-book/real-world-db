# 実例で学ぶデータベース設計(仮)

書くぞ!!

## コンパイル

```
docker-compose build
bundle install # review及びその他もろもろライブラリ導入
bundle exec rake build_all # Web / EPUB / PDF を同時に生成する

bundle exec rake clean # 不必要なファイルを消すときに利用する
bundle exec rake web # Webのみ出力する際に利用する
bundle exec rake pdf # PDFのみ出力する際に利用する
bundle exec rake epub # EPUBのみ出力する際に利用する
```
