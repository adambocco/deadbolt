
with open("index.html","r") as readFile, open("arduinoHtml","w") as writeFile:
    for line in readFile:
        i = 0
        while i < len(line):
            if line[i] == "\"" or line[i] == "'":
                print(i)
                line = line[:i] + "\\" + line[i:]
                i+=1
            i+=1
        if line.rstrip() == "":
            continue
        writeFile.write("\"" + line.rstrip() +"\" + \n")
        