require 'fileutils'
require 'rake/clean'

BOOK = "book"
BOOK_PDF = BOOK+".pdf"
BOOK_EPUB = BOOK+".epub"
CONFIG_FILE = "./src/config.yml"
WEBROOT = "webroot"

def build(mode, chapter)
  sh "review-compile --target=#{mode} --footnotetext --stylesheet=style.css #{chapter} > tmp"
  mode_ext = {"html" => "html", "latex" => "tex",
              "idgxml" => "xml"}
  FileUtils.mv "tmp", chapter.gsub(/re\z/, mode_ext[mode])
end

def build_all(mode)
  sh "review-compile --target=#{mode} --footnotetext --stylesheet=style.css"
end

task :default => :html_all

desc "build html (Usage: rake build re=target.re)"
task :html do
  if ENV['re'].nil?
    puts "Usage: rake build re=target.re"
    exit
  end
  build("html", ENV['re'])
end

desc "build all html"
task :html_all do
  build_all("html")
end

desc 'generate PDF and EPUB file'
task :all => [:pdf, :epub]

desc 'generate PDF file'
task :pdf => BOOK_PDF

desc 'generate stagic HTML file for web'
task :web => WEBROOT

desc 'generate EPUB file'
task :epub => BOOK_EPUB

SRC = FileList['*.re'] + [CONFIG_FILE]

file BOOK_PDF => SRC do
  FileUtils.rm_rf [BOOK_PDF, BOOK, BOOK+"-pdf"]
  # sh "review-pdfmaker #{CONFIG_FILE}"
  sh "docker-compose run review review-pdfmaker config.yml"
  sh "mv src/book.pdf ./build/book.pdf"
end

file BOOK_EPUB => SRC do
  FileUtils.rm_rf [BOOK_EPUB, BOOK, BOOK+"-epub"]
  sh "review-epubmaker #{CONFIG_FILE}"
end

file WEBROOT => SRC do
  sh "echo 'start web compile'"
  FileUtils.rm_rf [WEBROOT]
  sh "review-webmaker #{CONFIG_FILE}"
  generate_my_web_index
  sh "cp ./web/js/*.js ./webroot/"
  sh "cp ./web/css/*.css ./webroot/"
  sh "mkdir ./webroot/src"
  sh "cp -rf ./src/images ./webroot/src/"
end

# Indexページに付けるタイトルを指定する
BOOK_TITLE = "実例で学ぶデータベース設計"
# WebIndex用のレイアウトファイル
WEB_INDEX_LAYOUT = "src/layouts/web-index.html.erb"

def generate_my_web_index
  # Not Work fine FIXME
  require 'review'
  require 'erb'
  include ERB::Util

  base_dir = File.dirname(__FILE__)
  template = open("#{base_dir}/#{WEB_INDEX_LAYOUT}").read
  # @book = ReVIEW::Book.load(base_dir + "/src/chapters/")
  # @title = BOOK_TITLE
  @erb = ERB.new(template)

  # @toc = @book.chapters.map do |chapter|  # FIXME: deprecated warnings
  #   sections = ReVIEW::TOCParser
  #     .chapter_node(chapter)
  #     .children
  #     .select { |v| v.is_a?(ReVIEW::TOCParser::Section) }
  #   {
  #     chapter: chapter,
  #     sections: sections
  #   }
  # end
  @toc = []

  File.open("#{base_dir}/webroot/index.html", "w") do |f|
    f.write @erb.result
  end
end

task :build_all do
  sh "bundle exec rake clean"
  sh "bundle exec rake pdf"
  sh "bundle exec rake epub"
  sh "bundle exec rake web"
end

task :deploy do
  sh "bundle exec ruby ./bin/make_dist.rb"
end

CLEAN.include([BOOK, BOOK_PDF, BOOK_EPUB, BOOK+"-pdf", BOOK+"-epub", WEBROOT])
