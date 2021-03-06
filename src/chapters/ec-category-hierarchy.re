= 階層をもつカテゴリーの設計

本章では、ECシステムのオープンソースプロダクトであるEC-CUBEとSpreeのカテゴリー設計を取り上げます。
今回取り上げるカテゴリーは、階層構造を持つことができます。

== 階層構造を扱うことの難しさ

EC-CUBE・Spreeともに、カテゴリーを@<img>{ec-category-hierarchy-first-tree}のように親と子を持った階層構造として表現できます。

//image[ec-category-hierarchy-first-tree][階層構造をもつカテゴリー]

リレーショナル・データベース(RDB)は、データを列と行を用いた2次元の表形式で保存します。
このため、木構造のような再帰的構造をもつデータを表現するのが苦手とされています。

この弱点を克服するために、RDBの製品によっては階層的なデータ構造を扱えるよう、再帰クエリをサポートしているものがあります。
再帰クエリをサポートしていない場合は、RDBで扱いやすいようなデータモデルを採用する必要があります。


== EC-CUBEのカテゴリー設計

EC-CUBEは、カテゴリーを@<img>{ec-category-hierarchy-ec-cube-naive-with-table}のようなテーブルで表現します。
このモデルは@<b>{隣接リスト}と呼ばれ、各レコードに親へのIDを持つことで木構造を表現しています。

//image[ec-category-hierarchy-ec-cube-naive-with-table][EC-CUBEのカテゴリー設計]

隣接リストはテーブル構造がシンプルな反面、再帰的なSQLの表現をサポートしているRDBでなければ
データの取得が困難である、いう欠点も持っています。
例えば、「食品」カテゴリーの下にあるすべてのカテゴリーを取り出すといったクエリは、
再帰SQL(WITH RECURSIVE)を用いないと簡単に書くことはできません。

隣接モデルの利点欠点、データ取得方法については、後述の@<hd>{ec-category-hierarchy|階層型データを格納するためのモデル設計}にて詳しく解説します。

== Spreeのカテゴリー設計

SPREEは、カテゴリーを@<img>{ec-category-hierarchy-spree-cset}のようなテーブルで表現します。
このモデルは、@<b>{隣接リスト}と@<b>{入れ子集合(Nested Set)}を組み合わせたものです。

//image[ec-category-hierarchy-spree-cset][Spreeのカテゴリー設計]

隣接リストはEC-CUBEのカテゴリーテーブルでも利用されていましたが、入れ子集合モデルとはなんでしょうか。
入れ子集合では、left/rightという2つの値を用いて階層を表現します。
このモデルは木構造を集合を用いて表現するため、一見すると分かりづらいかと思われます。

入れ子集合モデルの利点欠点、データ取得方法については、後述の@<hd>{ec-category-hierarchy|階層型データを格納するためのモデル設計}にて詳しく解説します。

Spreeは入れ子集合のデータ検索の欠点を補うために、さらに隣接リストも組み合わせるモデルを採用しています。

== 階層型データを格納するためのモデル設計

EC-CUBEおよびSpreeにて使われている@<b>{隣接リスト}および@<b>{入れ子集合(Nested Set)}は、いずれもデータベースで木構造を表現するためのデータモデルです。
木構造を表現する方法は、他に@<b>{クロージャーテーブル}と呼ばれるモデルもあります。

これらのモデルを説明するまえに、木構造の基本をおさらいしておきます。

木構造はグラフの一種です。
グラフとは、@<img>{ec-category-hierarchy-graph}のように、ノード（頂点）とエッジ（辺）のペアで構成されるデータ構造です。

//image[ec-category-hierarchy-graph][グラフ構造]

@<img>{ec-category-hierarchy-graph}のグラフは閉路を持ったグラフです。
ノードaからたどり、a -> e -> c -> a と移動すると元の場所に戻ることができます。これを閉路をもつグラフと呼びます。

また、エッジの向きが存在しない事を、無向グラフと呼びます。
無向グラフでは、a -> e と移動できますし、e -> a と逆にたどる事もできます。
反対に、エッジの向きを制限するグラフは有向グラフと呼ばれます。a -> e と移動できても、e -> a という移動は制限するようなグラフです。

木構造は@<img>{ec-category-hierarchy-tree}のような閉路を持たない有向グラフです。
各ノードには親と子の関係があります。
@<b>{木構造は閉路を持たない}ということが重要です。
これから紹介するモデルでは、閉路を持たないことはアプリケーションレベルで保証しなければなりません。

//image[ec-category-hierarchy-tree][木構造]

隣接リスト、入れ子集合(Nested Set)、クロージャーテーブルのうち、
どのモデルを採用するかは、扱うデータ・アプリケーションの性質やRDBで再帰クエリをサポートしているかによって変わります。
どれが良い悪いではなく、これらのモデルの長所短所を正しく理解し、構築するシステムに合わせた最適なモデルを選択しましょう。

=== 隣接リスト

隣接リストは、紹介するモデルの中で最もシンプルなデータ構造を持ちます。
各レコードに親のノードIDを含めてデータを表現します(@<img>{ec-category-hierarchy-naive-tree-model})。

//image[ec-category-hierarchy-naive-tree-model][隣接リスト]


隣接リストは、@<list>{ec-product-variant-naive-tree-create}のSQLにて表すことができます。

//list[ec-product-variant-naive-tree-create][隣接リストのテーブル設計][SQL]{
  CREATE TABLE `categories` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `parent_id` int(11) DEFAULT NULL,
    `name` varchar(255) NOT NULL,
    PRIMARY KEY (`id`)
  );
//}

隣接リストを用いた場合、指定したカテゴリーの直近の子または親を取得するのは簡単です。
例えば、「食品」カテゴリーの子レコードは@<list>{ec-product-variant-naive-tree-child}のSQLで取得できます。

//list[ec-product-variant-naive-tree-child][「食品」カテゴリーの子レコードを取得する][SQL]{
  SELECT c1.*
  FROM categories c1
  WHERE parent_id = 2;
//}

指定ノード配下のすべてのレコードを取得するためには、再帰SQL(WITH RECURSIVE)を用いる必要があります。
WITH RECURSIVEクエリは、PostgreSQL8.4以上、MySQLだと8.0以上でサポートされています。
またOracleやSQL Serverなどの商用RDBでもサポートされています。

WITH RECURSIVEを用いると、@<list>{ec-product-variant-naive-tree-with-recursive}のように、
「カテゴリー」配下のレコードを取得できます。

//list[ec-product-variant-naive-tree-with-recursive][再帰クエリを用いた配下のレコードの取得][SQL]{
  WITH RECURSIVE tcat(id, parent_id, name) AS (
    SELECT c1.id, c1.parent_id, c1.name
    FROM categories c1
    WHERE c1.parent_id IS NULL -- 「カテゴリー」配下を取得する

    UNION ALL

    SELECT c2.id, c2.parent_id, c2.name
    FROM categories c2
    INNER JOIN tcat ON tcat.id = c2.parent_id
  )
  SELECT * FROM tcat;
//}


RDBが再帰クエリをサポートしていない場合、配下や先祖のレコードを取得するのは困難です。
木構造の階層が2段であれば、@<list>{ec-product-variant-naive-tree-depth2}のようなSQLでデータを取得できます。

//list[ec-product-variant-naive-tree-depth2][2階層下までのレコードを取得][SQL]{
  SELECT c1.*, c2.*
  FROM   categories c1
         LEFT OUTER JOIN categories c2
                      ON c2.parent_id = c1.id
  WHERE  c1.parent_id IS NULL; -- 「カテゴリー」配下を取得する
//}

3階層下までレコードを取得するためには、さらにテーブルをJOINする必要があります。

//list[ec-product-variant-naive-tree-depth2][3階層下までのレコードを取得][SQL]{
  SELECT c1.*, c2.*, c3.*
  FROM   categories c1
         LEFT OUTER JOIN categories c2
                      ON c2.parent_id = c1.id
         LEFT OUTER JOIN categories c3
                      ON c3.parent_id = c2.id
  WHERE  c1.parent_id IS NULL; -- 「カテゴリー」配下を取得する
//}

再帰クエリを利用しない場合、取得可能な階層数に上限をもうける必要があります。
このため、再帰クエリをサポートしていない場合、隣接リストの採用はおすすめできません。


隣接リストのメリット・デメリットは次のとおりです。

 * 隣接リストのメリット
 ** データ構造がシンプルである
 ** データの挿入、削除が簡単である
 ** 自ノードの直近の親、子を簡単に取得できる
 * 隣接リストのデメリット
 ** 再帰クエリをサポートしていないRDBだと、自ノードの子孫をすべて取得する、または先祖を取得するといった検索が困難である


=== 入れ子集合 (Nested Set)

入れ子集合は、木構造を集合を用いて表すデータ構造です。
@<img>{ec-category-hierarchy-nested-set-graph}のように親ノードの中に子ノードが含まれると考えてデータを格納します。


//image[ec-category-hierarchy-nested-set-graph][入れ子集合を用いた木構造の表現]

各集合には、left / right という2つの値を割り当てます。
例えば、「食品」というカテゴリーにはleft=2、right=9 という値を割り当て、「食品」カテゴリー内のスイーツにはleft=3、right=6という値を割り当てます。
この状態で「食品」カテゴリーの子ノードは「スイーツ」であると表現します。
子ノードのleftの値は必ず親ノードのleftよりも大きく、また子ノードのrightの値は親ノードのrightよりも小さくなります。

入れ子集合を木構造になおすと、@<img>{ec-category-hierarchy-nested-set-tree}および@<img>{ec-category-hierarchy-nested-set-data}
のように各ノードにleft/rightの値を割り付けます。
これで各集合をテーブルで表現できます。


//image[ec-category-hierarchy-nested-set-tree][入れ子集合を用いた木構造の表現]

//image[ec-category-hierarchy-nested-set-data][入れ子集合を用いた木構造の表現]


入れ子集合で指定ノードの配下のレコードをすべて取得するのは簡単です。
@<list>{ec-category-hierarchy-nested-set-table}のようなテーブルに対して、
@<list>{ec-category-hierarchy-nested-set-descendant}のクエリで、「カテゴリー」配下のノードをすべて取得できます。

//list[ec-category-hierarchy-nested-set-table][入れ子集合のテーブル設計][SQL]{
  CREATE TABLE `categories` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `name` varchar(255) NOT NULL,
    `left` int(11) NOT NULL,
    `right` int(11) NOT NULL,
    PRIMARY KEY (`id`)
  );
//}


//list[ec-category-hierarchy-nested-set-descendant][入れ子集合の指定ノード配下のレコードをすべて取得する][SQL]{
  SELECT     c1_child.*
  FROM       categories c1_child
  INNER JOIN categories c2_parent
  WHERE      c2_parent.id = 1 -- id = 1 は「カテゴリー」ノード
  AND        c1_child.left BETWEEN c2_parent.left AND c2_parent.right;
//}


指定ノードの先祖も@<list>{ec-category-hierarchy-nested-set-ascendant}のクエリで簡単に取得できます。

//list[ec-category-hierarchy-nested-set-ascendant][入れ子集合の先祖ノードをすべて取得する][SQL]{
  SELECT     c1_parent.*
  FROM       categories c1_parent
  INNER JOIN categories c2_child
  WHERE      c2_child.id = 15 -- id = 15 は「チョコレート」ノード
  AND        c1_parent.left < c2_child.left AND c1_parent.right > c2_child.right;
//}


入れ子集合では、自ノードの子孫または先祖すべてを取得するといった検索を簡単に行うことができる反面、
ノードの追加・削除が難しいというデメリットもあります。
ノードの追加・削除が発生すると、最悪のケースではカテゴリーテーブルのレコード全体のleft/right値を更新しなおさなければならないためです。

また指定カテゴリーの直近の親または子を取得するといったクエリは、簡単に書くことができません。
Spreeの商品カテゴリーテーブルでは、入れ子集合に隣接リストのモデルも組み合わせることで、この問題を解決しています。

カテゴリーの更新が少なく、特定カテゴリー配下または先祖のすべてのレコードを取得するといった操作が多い場合は
入れ子集合モデルを採用してもよいでしょう。


入れ子集合モデルのメリット・デメリットは次のとおりです。

  * 入れ子集合のメリット
  ** 自ノード配下または先祖のレコードすべてを簡単に取得できる
  * 入れ子集合のデメリット
  ** ノードの追加・削除が難しい。最悪のケースでは、すべてのレコードを更新する必要がある
  ** 直近の親または子ノードを簡単に取得できない



=== クロージャーテーブル

最後に紹介するのは、クロージャーテーブルと呼ばれるモデルです。

EC-CUBE、Spreeともにクロージャーテーブルは使われていませんでしたが、
このモデルは構造がシンプルで、データの追加・削除といった操作が簡単にできます。
また、子孫・先祖すべてを取得といった操作がしやすいという特徴を持っています。
反面、ノードの数が多いとデータ量が多くなとなるというデメリットもあります。

クロージャーテーブルでは、各ノードの直近の親・子の関係だけでなく、
ノードは自分自身と子孫すべての接続情報をテーブルに持ちます(@<img>{ec-category-hierarchy-closure-tree-data})。
また、この接続情報を格納する別のテーブルが必要です(@<list>{ec-category-hierarchy-closure-tree-def})。


//image[ec-category-hierarchy-closure-tree-data][クロージャーテーブル]

//list[ec-category-hierarchy-closure-tree-def][クロージャーテーブルのテーブル設計][SQL]{
  -- カテゴリーテーブル
  CREATE TABLE `categories` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `name` varchar(255) NOT NULL,
    PRIMARY KEY (`id`)
  );

  -- ノードの接続情報を格納するテーブル
  CREATE TABLE `category_tree_paths` (
    `ancestor` int(11) NOT NULL,
    `descendant` int(11) NOT NULL,
    PRIMARY KEY (`ancestor`, `descendant`)
  );
//}

クロージャーテーブルでは、子孫または先祖のノードをすべて取得するといった操作が簡単にできます。
例えば、「カテゴリー」ノード(ID: 1)の子孫をすべて取得する場合は、
@<list>{ec-category-hierarchy-closure-tree-desc-fetch}のようにしてレコードを取得します。

//list[ec-category-hierarchy-closure-tree-desc-fetch][クロージャーテーブルから特定ノードの子孫をすべて取得する][SQL]{
  SELECT c.*
  FROM `categories` c
  INNER JOIN `category_tree_paths` t ON c.id = t.descendant
  WHERE t.ancestor = 1; -- id = 1 は 「カテゴリー」
//}


ノードの追加は簡単にできます。
「スイーツ」カテゴリーの下に新たに「アイスクリーム」というノード(ID: 11)を足したい場合は、
@<list>{ec-category-hierarchy-closure-tree-add}のように自分自身への参照を追加します。
その上で、自身の先祖にあたるノードすべてに対して、
@<list>{ec-category-hierarchy-closure-tree-add}のようにレコードを追加します。

//list[ec-category-hierarchy-closure-tree-add][クロージャーテーブルへのレコードの追加][SQL]{
  INSERT INTO `categories`(`id`, `name`)
  VALUES (11, "アイスクリーム");

  -- 自ノードへの参照は必ず必要
  INSERT INTO `category_tree_paths`(`ancestor`, `descendant`)
  VALUES (11, 11);

  -- 自身の直近の親の子孫すべての関連を追加
  INSERT INTO `category_tree_paths`(`ancestor`, `descendant`)
    SELECT t.ancestor, 11
    FROM `category_tree_paths` t
    WHERE t.descendant = 3; -- id = 3 はスイーツ
//}

ノードを削除したい場合は、@<list>{ec-category-hierarchy-closure-tree-delete}のように
子孫として参照している行をすべて削除するだけです。

//list[ec-category-hierarchy-closure-tree-delete][クロージャーテーブルのレコード削除][SQL]{
  -- id: 11を削除したい場合
  DELETE FROM `category_tree_paths`
  WHERE descendant = 11;

  DELETE FROM `categories`
  WHERE id = 11;
//}

直近の親や子を取得するといった操作は、このテーブル構造のままではできません。
しかし、category_tree_pathsテーブルにノード間の距離情報を格納しておくことで、簡単にデータを取得できます。
例えば、食品(ID: 2)とスイーツ(ID:3)、お酒(ID: 5)の距離は1といった情報をpath_lengthカラムに格納しておくことで、
@<list>{ec-category-hierarchy-closure-tree-path-length}のようにして食品の直近の子を取得できます。

//list[ec-category-hierarchy-closure-tree-path-length][ノード間距離を使った子ノードの取得][SQL]{
  SELECT c.*
  FROM `categories` c
  INNER JOIN `category_tree_paths` t ON c.id = t.descendant
  WHERE t.ancestor = 2 AND t.path_length = 1; -- ID: 2 は「食品」
//}


クロージャーテーブルのメリット・デメリットは次のとおりです。

 * クロージャーテーブルのメリット
 ** テーブル構造がシンプルで直感的に理解しやすい
 ** 子孫・先祖すべてを取得するといった操作がしやすい
 ** 直近の親・子を取得するといった操作がしやすい
 ** データの挿入、削除が簡単にできる
 * クロージャーテーブルのデメリット
 ** ノード数が増えるとテーブルの行数が多くなる


=== 階層型データモデルの設計まとめ

隣接リスト・入れ子集合(Nested Set)・クロージャーテーブルの特徴は、@<table>{ec-category-hierarchy-merideri}のとおりです。

//table[ec-category-hierarchy-merideri][階層型データモデルの設計まとめ]{
モデル		親・子の取得		子孫・先祖の取得		データ追加・削除		データ量
--------------------------------------------------------------------
隣接リスト		簡単		(再帰クエリ無しだと)難しい		簡単		ノード数と同じ
入れ子集合 (Nested Set)		難しい		簡単		難しい	ノード数と同じ
クロージャーテーブル		簡単		簡単	簡単	ノード数×子孫の数
//}

RDBではグラフ構造を扱うことが難しいことを忘れないでください。
どのモデルを選択したとしても、木構造の定義が崩れていないかはデータベースレベルで保証することはできず、
アプリケーションレベルで担保する必要があります。
例えば隣接リストで閉路が存在している状態で再帰クエリを発行すると、SQLエラーとなってしまいます。

複雑なグラフ構造、木構造を扱わなければいけない場合は、RDB以外を選択することも考えましょう。
例えばグラフデータベースを利用すれば、もっとシンプルにデータを表現できます。

今回紹介したどのモデルにもメリット・デメリットがあります。扱うデータの性質を見極めて、最適なモデルを選択してみてください。
