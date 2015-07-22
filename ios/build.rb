require 'xcodeproj'

if File.exist?('Lab.xcodeproj')
  puts "Lab.xcodeproj already exists."
  exit 1
end

proj = Xcodeproj::Project.new('Lab.xcodeproj')

target = proj.new_target(:static_library, 'lab', :ios)

target.add_system_framework('CoreGraphics')

conf = target.build_configuration_list

conf.set_setting('HEADER_SEARCH_PATHS', [
  '..',
  '../vendor/freetype2/include',
  '../vendor/harfbuzz/src',
  '../vendor/nanovg/src'
])

conf.set_setting('CLANG_CXX_LANGUAGE_STANDARD', 'gnu++14')

conf.set_setting('OTHER_LDFLAGS', [
  '-L../vendor/harfbuzz/src/.libs',
  '-L../vendor/freetype2/objs',
  '-lharfbuzz',
  '-lfreetype'
])

sources = target.source_build_phase
%w[
  ../ui/canvas.cc
  ../ui/atom.cc
  ../ui/fonts.cc
  ../ui/text.cc
  ../ui/math.cc
  ../vendor/nanovg/src/nanovg.c
].each do |file|
  sources.add_file_reference(proj.new_file(file))
end

proj.save

