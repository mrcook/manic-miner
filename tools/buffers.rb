# Trnaslates a buffer address to the actual Screen/Attrs address

# Screen File                 16384  22527  6144
# Attr File                   22528  23295  768
# ------                      23296  23551  256
# Attr Buffer                 23552  24063  512
# Attr Buffer (Blank Cavern)  24064  24575  512
# Scr Buffer                  24576  28671  4096
# Scr Buffer (Blank Cavern)   28672  32767  4096

def translate_address(address)
  buff = ""

  case
  when address >= 23552 && address <= 24063
    addr = address - 1024
  when address >= 24064 && address <= 24575
    addr = address - 1536
    buff = "Blank "
  when address >= 24576 && address <= 28671
    addr = address - 8192
  when address >= 28672 && address <= 32767
    addr = address - 12288
    buff = "Blank "
  end

  [buff, addr]
end

# quit unless our script gets one command line argument
if ARGV.length != 1
  puts "Only one argument allowed! Include a screen Address: e.g. 24576"
  exit
end
address = ARGV[0].to_i
if address < 23552 || address > 32767
  puts "Buffer addresses should be between 23552 and 32767"
  exit
end

buff, addr = translate_address(address)

puts "%sBuffer: %d" % [buff, address]
puts "%sScreen: %d" % [buff, addr]