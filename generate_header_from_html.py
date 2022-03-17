from pathlib import Path

def generate(filename):
    path = Path("src/resources/" + filename)
    filename = path.name
    header = path.parent / Path(path.stem + ".h")
    variable = path.stem + "_html"

    buffer = b""
    with open(path, 'r') as index:
        lines = index.readlines()
        for line in lines:
            buffer += line.encode("utf-8")


    with open (header, 'w') as header:
        header.write("// Header is generated automatically\n")
        header.write("constexpr char " + variable + "[] = {\n")

        counter = 0
        for c in buffer:
            if counter%16 == 0:
                header.write(f"\n\t")
            header.write(f"{c:#x}, ")
            counter += 1
        header.write("\n};\n")
        header.write(f"constexpr int {variable}_size = {len(buffer)};\n")

generate("webserver/index.html")
generate("webserver/submit.html")
generate("spotify_player.html")