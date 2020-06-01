#testownik_to_latex.py skrypt przeszukuje pytania w swoim katalogu (numerowane od 1 do -> <edytowalna wartoścć>)
#i generuje plik odpowiedzi_all, który można skompilować przy pomocy np. overleaf
file = open("odpowiedzi_all.txt", "a")#nazwa pliku docelowego


file.write("\\documentclass{article}\n")
file.write("\\usepackage[utf8]{inputenc}\n")
file.write("\\usepackage{polski}\n")
file.write("\n")
file.write("\\usepackage{nopageno}\n")
file.write("\\thispagestyle{empty}\n")
file.write("\n")
file.write("\n")

file.write("\\begin{document}\n")
file.write("\n")


file.write("\\begin{enumerate}\n")
file.write("\n")
file.write("\n")

for i in range(1,28+1):#przeszukuj pytania od 1 do 28, tak 28, nie 29
	file_name=str(i) +".tex"
	

	questions_file = open(file_name, "r")
	
	
	
	print("Pytanie numer: ",i)
	
	counter = 0
	for line in questions_file:
		line = line.strip("\n")
		
		#podmienianie na znaki specjalne latexa
		line = line.replace("_", "\\_")
		line = line.replace("$", "\\$")
		
		if counter == 0:
			line = line.strip()
			line = line.replace("X","")
			string_list = list(line)
			#good_answer = (int(string,base=2))
			#X0001 -> 4
			#X0010 -> 3
			#0100 -> 2
			#X1000 -> 1
			#X00010 -> 4
			good_answer = 1
			for x in string_list:
				if x == "0":
					good_answer+=1
				elif x == "1":
					break
			
			#print(string)
			#print(string_list)
			#print(good_answer)	
		elif counter == 1:#pytanie
			#line ="\t \item " + str(i) + ": " + line + "\n"
			line ="\t \item " + line + "\n"
			print(line)
			file.write(line)
			file.write("\t \t \\begin{itemize} \n")
			
		elif counter == (good_answer + 1):#znalezienie dobrego wersu z odpowiedzią
			str_tmp = "\t \t \t \\item \\textbf{" + line + "}\n"
			print(str_tmp)
			file.write(str_tmp)

		else: 
			if line == "":	
				pass
			else:
				line ="\t \t \t \\item " + line + "\n"
				print(line)
				file.write(line)
	
	
	
		counter+=1
		
	
	file.write("\t \t \\end{itemize}")
	file.write("\n")
	file.write("\n")

	print("")
	print("")

	questions_file.close()

	
	
file.write("\n")
file.write("\n")	
file.write("\\end{enumerate}\n")
file.write("\n")
file.write("\end{document}\n")

file.close()
