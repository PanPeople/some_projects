file_out = open("./file_fixed.html", "w",encoding="utf8")

file_in = open("./file.html",encoding="utf8")

text_in = ""
for line in file_in:
	text_in += line

searched_text = "<a class=\"md-toc-inner\" href=\"#undefined\">"
modified_string = "<a class=\"md-toc-inner\" href=\""

print(text_in)

text_out = text_in

print("__________________________")

for i in range(0,len(text_in)):

	temp_text = ""
	word_length= 0
	word_length = i + len(searched_text)
	
	if (word_length >= len(text_out)):
		word_length = len(text_out)
		
	for j in range(i, word_length):
		temp_text += text_out[j]

	if (searched_text == temp_text):
		print(temp_text)
		j = j+1
		temp_text = ""
		while(True):
			temp_text+=text_out[j]
			j +=1
			if(text_out[j]== "<"):
				break
		print(text_out[i+len(modified_string)])		
		print(temp_text)
		temp_text = temp_text.lower()
		temp_text = temp_text.replace(' ','-')
		temp_text = temp_text.replace('/','')
		temp_text = temp_text.replace('{','')
		temp_text = temp_text.replace('}','')
		temp_text = temp_text.replace('$$','')
		temp_text = temp_text.replace('$','-n547')
		temp_text = temp_text.replace(':','')
		temp_text = temp_text.replace('(','')
		temp_text = temp_text.replace(')','')
		temp_text = temp_text.replace('?','')
		temp_text = temp_text.replace('!','')
		temp_text = temp_text.replace('.','')
		temp_text = temp_text.replace(',','')
		alpha= "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
		for k in (range(0,len(alpha))):
			temp_text = temp_text.replace('\\'+alpha[k],alpha[k])
		#tutaj moje i jest początkiem linku
		text_out = text_out[:i + len(modified_string)] + "#"+ temp_text +text_out[i + len(modified_string)+len("#undefined"):]
		
		
		
		
file_out.write(text_out)
file_in.close()
file_out.close()
		
