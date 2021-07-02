
with open("waterfall.html","r") as readFile, open("arduinoHtml","w") as writeFile:
    writeFile.write("char ptr[] = ")
    for line in readFile:
        i = 0
        while i < len(line):
            if line[i] == "\"" or line[i] == "'":
                line = line[:i] + "\\" + line[i:]
                i+=1
            i+=1
        writeFile.write("\"" + line.rstrip() +"\\n\"\n")
        