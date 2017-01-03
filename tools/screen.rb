require_relative "characters"

class Screen
  SCREEN_BYTES   = 6144
  SCREEN_ROW_LEN = 32 * 8
  attr_accessor :memory

  def initialize
    @memory = Array.new(SCREEN_BYTES, 0)
    @screen_dump = Array.new(SCREEN_BYTES, 0)
  end

  def add_char_at(char, addr)
    raise "Address OutOfBounds" if addr >= SCREEN_BYTES - SCREEN_ROW_LEN

    c = font_character(char)
    c.each_slice(8) do |b|
      char_byte = b.join.to_i(2) # turns binary array in to a single byte
      memory[addr] = char_byte

      # move addr forward 8 pixel rows
      addr += SCREEN_ROW_LEN
    end
  end

  def display
    export_memory_to_screen
    display_bits(screen_dump)
  end

  def dump_memory
    display_bits(memory)
  end

  private

  attr_accessor :screen_dump

  def export_memory_to_screen
    block1 = memory[0..2047]
    block2 = memory[2048..4095]
    block3 = memory[4096..6143]

    [block1, block2, block3].each_with_index do |block, bid|
      block_addr_offset = bid * 2048

      addr = 0
      line = 0
      offset = 0

      block.each_slice(32) do |pixel_row|
        pixel_row.each do |b|
          screen_dump[block_addr_offset + addr] = b
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
  end

  def display_bits(data)
    data.each_slice(32) do |row|
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
  end

  def font_character(char)
    # get character from font set
    font_num = characters.index(char)
    font_pos = font_num * 64

    # return all 64 pixels for the character
    font_set[font_pos...font_pos+64]
  end
end
