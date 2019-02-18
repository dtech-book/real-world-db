= 商品マスタの設計

この章では、Spree及びEC-CUBEの具体的な商品マスタ設計を見ていきます。
データのもたせ方や必要な項目を見ながら、目的に合わせた最適なDB設計は何かを考えていきます。


== バリエーションを持つ商品

Spree/EC-CUBEともに、登録した商品毎にバリエーションを持たせることができます。
例えば同じTシャツでも、色違いのもの、サイズ違いのものといったように、特定の特性だけが異なる商品が存在します(@<img>{ec-product-variant-pd1})。
Spreeでは、これら特性の違う商品を@<strong>{バリアント}と呼び、EC-CUBEでは@<strong>{規格}と呼んでいます。
今後、商品バリエーションのことは@<strong>{商品規格}と呼ぶことにします。

//image[ec-product-variant-pd1][商品と商品規格][scale=0.5]

Spreeでは、商品と商品規格は@<img>{ec-product-variant-spree-p1}のような関係を持ちます。
商品はproducts、商品規格はvariantsテーブルに持ちます。

#@# ここは具体的なERに差し替える
//image[ec-product-variant-spree-p1][Spreeの商品とバリエーションの関係][scale=0.5]

EC-CUBEでも、Spreeとテーブル名が異なるだけで@<img>{ec-product-variant-eccube-p1}のような関係のテーブルで
商品と商品規格を持ちます。

#@# ここは具体的なERに差し替える
//image[ec-product-variant-eccube-p1][EC-CUBEの商品とバリエーションの関係][scale=0.5]


=== 単品商品のデータのもたせ方

#@# 単品でもかならずバリアントがある

#@# バリエーションは必要か？例えばほんの通販ならばいらない。シリーズをまとめたい場合はその上にグルーピングのテーブルを作れば良い
#@# バリエーションと間違えないように


== 商品価格のもたせ方
