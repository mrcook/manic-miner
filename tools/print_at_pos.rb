require "optparse"
require_relative "helpers"
require_relative "screen"

@options = {}
OptionParser.new do |opts|
  opts.banner = "Usage: main.rb [options]"
  opts.on("-a", "--[no-]address", "Use screen address as input") do |v|
    @options[:address] = v
  end
  opts.on("-p", "--[no-]print", "Print character at screen position") do |v|
    @options[:output] = v
  end
  opts.on("-h", "--help", "Prints this help") do
    puts "ZX Spectrum Screen Utils"
    puts "Convert between character position and screen address values, and/or print to screen"
    puts
    puts opts
    exit
  end
end.parse!
# quit unless our script gets two command line arguments
if ARGV.length != 1
  puts "Only one argument allowed!"
  puts "Either a screen Address, or 'Y,X' coords (comma, no spaces)"
  exit
end

def is_address?
  @options[:address]
end

def parse_input_address
  if is_address?
    y = ARGV[0].to_i - 16384
    # msg = "Address must be in range: 16384-22527" if y < 0 || y > 6143
  else
    positions = ARGV[0].split(",")
    y, x = positions[0].to_i, positions[1].to_i
    msg = "Max screen position is 23,31" if x < 0 || y < 0 || x >= 32 || y >= 24
  end

  [msg, y, x]
end

def address_to_position(y)
  base_row, cols = y.divmod(256)
  add_rows, col = cols.divmod(32)

  [base_row + add_rows, col]
end

def position_to_address(y, x)
  block, cells = y.divmod(8)
  rows = (block * 8) * 256
  cols =  (cells * 32) + x
  cols + rows
end

msg, y, x = parse_input_address
if msg
  puts msg
  exit
end

if is_address?
  x, y = address_to_position(y)
  addr = y
else
  addr = position_to_address(y, x)
end

unless @options[:output]
  if is_address?
    puts "#{x}, #{y}"
  else
    puts addr + 16384
  end
  exit
end

screen = Screen.new

"x".each_char do |c|
  screen.add_char_at(c, addr)
  addr += 1 # next byte/char column
end

# addr = 20480 - 16384
# "         Central Cavern         ".each_char do |c|
#   screen.add_char_at(c, addr)
#   addr += 1 # next byte/char column
# end

# addr = 20512 - 16384
# "AIR".each_char do |c|
#   screen.add_char_at(c, addr)
#   addr += 1 # next byte/char column
# end


# addr = 20576 - 16384
# "High Score 000000   Score 000000".each_char do |c|
#   screen.add_char_at(c, addr)
#   addr += 1 # next byte/char column
# end


screen.display
# screen.dump_memory


