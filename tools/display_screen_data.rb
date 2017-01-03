require_relative "helpers"
require_relative "title_screen_data"

memory = Array.new(6144, 0)

[TITLESCR1, TITLESCR2, Array.new(2048, 0)].each_with_index do |block, bid|
  block_addr_offset = bid * 2048

  addr = 0
  line = 0
  offset = 0

  block.each_slice(32) do |pixel_row|
    pixel_row.each do |b|
      memory[block_addr_offset + addr] = b
      addr += 1
    end
    addr += 32*7
    line += 1

    if line == 8
      line = 0

      offset += 1
      addr = offset * 32
    end
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
