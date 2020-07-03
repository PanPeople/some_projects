file = open("lista_studentow.txt",encoding="utf8")
output = open("lista_studentow_fixed.txt", "a")
for line in file:
	line = line.replace('.','')
	line = line.replace('Nazwa','')
	line = line.replace('Stanowisko','')
	line = line.replace('Lokalizacja','')
	line = line.replace('Tagi','')
	line = line.replace('Rola','')
	line = line.replace('Członek','')
	line = line.replace('Obraz profilowy użytkownika ','')
	
	line = line.strip("\n")
	
	if (line == "" ):
		pass
	else:
		if (line == line2) :
			pass
		else:
			print(line)
			output.write(line + "\n")
	line2 = line

file.close()
output.close()
