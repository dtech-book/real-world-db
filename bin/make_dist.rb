require 'erb'

base_dir = "#{File.dirname(__FILE__)}"
template = open("#{base_dir}/../layouts/dist.html.erb").read
@erb = ERB.new(template)
File.open("#{base_dir}/../dist/index.html", "w") do |f|
  f.write @erb.result
end
