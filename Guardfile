guard :shell do
  watch(%r{(.*)\.re}) { |m| `bundle exec rake clean; bundle exec rake web` }
end
