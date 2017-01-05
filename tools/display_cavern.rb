require_relative "helpers"
require_relative "cavern_data"

# CAVERN index = 0
# add = 0
#
# char row 1 @ addr 0
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
# xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#
# CAVERN index = 31
# add = 31

# CAVERN index = 32
# addr += 1
# addr += 32 * 7   (i.e. 256)
#
# char row 2 @ addr 256
# ................................
# ................................
# ................................
# ................................
# ................................
# ................................
# ................................
# ................................

memory = Array.new(512, 0)

addr = 0 # aka 28672
col = 0

# each cavern tile ID is ine chartacter of 8x8 pixels
CAVERN.each do |id|
  if BACKGROUND[0] == id
    sprite = BACKGROUND[1..8]
  elsif FLOOR[0] == id
    sprite = FLOOR[1..8]
  elsif CRUMBLING[0] == id
    sprite = CRUMBLING[1..8]
  elsif WALL[0] == id
    sprite = WALL[1..8]
  elsif CONVEYOR[0] == id
    sprite = CONVEYOR[1..8]
  elsif NASTY1[0] == id
    sprite = NASTY1[1..8]
  elsif NASTY2[0] == id
    sprite = NASTY2[1..8]
  elsif EXTRA[0] == id
    sprite = EXTRA[1..8]
  else
    puts "Help"
    exit
  end

  sprite.each_with_index do |pixel_row, i|
    row = i * 32
    memory[addr + row] = pixel_row
  end
  addr += 1 # move to next memeory address

  if col < 31
    col += 1
  else
    col = 0
    addr += 32 * 7 # move to next row
  end
end

memory.each_slice(32) do |row|
  row.each do |byt|
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
