system("clear") or (system "cls")

def to_bits(byt)
  pixels = "%08d" % byt.to_s(2)

  row = []
  pixels.each_byte do |px|
      row << px.chr.to_i
  end
  row
end
