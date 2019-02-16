======================================
執筆ノート
======================================


コンセプト
======================================

- 実例で学ぶデータベース設計

実際のプロダクトで使われているデータベースの構造から、
現場で使えるデータベース設計のノウハウやアンチパターンを学ぶ。



DB設計 論争になる箇所
======================================



- 論理削除
- unique key成約
- nullの扱い unique keyと絡めて
  - できるだけnullにしない
- 壊れやすい構造
  - A,B,Cテーブルが有り、それぞれ参照を持っている
- foreign keyをはらない
- サロゲートキー
- primary keyが画面から変更できる
- マスタをDBに持つか
- カラム名を国際標準に揃えるか
  - https://github.com/EC-CUBE/ec-cube/issues/2598


基本知識
======================================

- 正規化を理解する
- アンチパターンを理解する
- IDEF1X/IE記法
- enumなどは規定の値があればそれに従う (都道府県コード/wdayなど)
- normalizeした値を入れる (postal codeが 080-0000-0000 と 08000000000 が入り乱れるなど)





NG集
======================================

- 金額データの扱い
  - https://teratail.com/questions/65879
  - varcharは論外、int / decimal



ノート
======================================

- EC Cube 規格を3つ以上増やせない
  - https://xoops.ec-cube.net/modules/newbb/viewtopic.php?topic_id=18795&forum=11

- ポイントの設計