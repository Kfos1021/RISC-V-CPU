import struct

with open("software/simple.bin", "rb") as f:
    data = f.read()

with open("programs/simple.hex", "w") as out:    
    for i in range(0, len(data), 4):
        word = data[i:i+4]
        # Pad the final word if necessary.
        if len(word) < 4:
            word += b"\x00" * (4 - len(word))

        value = struct.unpack("<I", word)[0]

        out.write(f"{value:08x}\n")

print("Generated programs/simple.hex")