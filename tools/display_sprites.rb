require_relative "helpers"
require_relative "title_screen_data"

memory = Array.new(6144, 0)
sprite = Array.new(160, 0)

addr = 0
line = 0
offset = 0

[MANDAT416, WILLYR2216].each do |sprite|
  sprite.each_slice(2) do |pixel_row|
    pixel_row.each do |byt|
      to_bits(byt).each do |b|
        if b == 1
          print '██'
        else
          print "  "
        end
      end
    end
    puts
  end
end