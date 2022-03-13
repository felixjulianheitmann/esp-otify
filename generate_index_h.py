
buffer = b""
with open("src/resources/webserver/index.html", 'r') as index:
    lines = index.readlines()
    for line in lines:
        buffer += line.encode("utf-8")

with open ("src/resources/webserver/index.h", 'w') as header:
    header.write("// Header is generated automatically\n")
    header.write("constexpr char index_html[] = {\n")

    counter = 0
    for c in buffer:
        if counter%16 == 0:
            header.write(f"\n\t")
        header.write(f"{c:#x}, ")
        counter += 1
    header.write("\n};\n")
    header.write(f"constexpr int index_html_size = {len(buffer)};\n")
