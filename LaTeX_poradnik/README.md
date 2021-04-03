# LaTeX poradnik

## Wstęp
### Przedmowa
> Nie wiem na razie czym ma być ten twór, na razie planuję aby był to zbiór różnych rzeczy do LaTeX-a. Całość chcę podzielić na tematyczne rozdziały.
>
> Nie jest to taki poradnik dla początkujących, ale jest szansa, że zrobię wersje w dodatkiem dla początkujących, bo LaTeX fajny jest. Dlatego czasami coś tłumaczę a czasami nie.
> 

### Parę zdań o LaTeX
> bajojajobajojajo

## Spis treści (nie klikalny, potem coś zaczaruję):

> * Wstęp
>   * Przedmowa
>   * Parę zdań o LaTeX
> * Biblioteki
> * Zdjęcia
> * Tabelki
>   * Wstęp
>   * Jak to wrzucać w ogóle
>   * "Co gdy LaTeX postanowi wypierdolić twoją tabletkę/zdjęcie 3 strony dalej"
> * Bibliografia
>   * Wstęp
>   * Jak to wrzucać w ogóle
> * Zapis matematyczny
>   * begin equasion i $$
> * Listy
>   * itemize
>   * enumerate
> * Kolumny (lub wejście w 4 gęstość)
> * Przydatne praktyki
> * Pierdoły (linki, komendy)
>   * język polski w Overleaf
>   * aby po polsku było spis treści
>   * \iffalse \fi

## Biblioteki

> Generalnie im "więcej tym, nic się nie stanie". Polecam mieć już szablon używanych bibliotek.
>
> Moje biblioteki są do wielu rzeczy i rzadko trzeba jakąś jeszcze dodać.
>
> ```latex
> \documentclass{article}
> \usepackage[utf8]{inputenc}
> \usepackage{polski}
> \usepackage{amssymb}
> \usepackage{wasysym}
> \usepackage{graphicx}
> \usepackage[polski]{babel}
> \usepackage{multirow}
> \usepackage{multicol}
> \usepackage[table,xcdraw]{xcolor}
> \usepackage[shortlabels]{enumitem}
> \usepackage{bm}
> \usepackage[top=1.5cm, bottom=1.5cm, left=1.7cm, right=1.7cm]{geometry}
> \usepackage{blindtext}
> \usepackage{hyperref}
> ```
> 
>

## Zdjęcia

> Najważniejsza rzecz w sumie i najpiękniejsze co LaTex oferuje, już nigdy (prawie) zdjęcie nie rozwali Ci całego sprawka jak jakiś tam Word. Wrzucanie zdjęć to żadne filozofia
>
> ```latex
> \begin{figure}[!h] % b-bottom t-top
> \centering
> \includegraphics[width=0.45\textwidth, angle = 0]{./picture.png}
> \captionsetup{labelformat=empty}%jak nie chcesz mieć przy opisie napisane "Figure *numer*" to pozostaw odkomentowane
> \caption{podpis}%chyba nie muszętłumaczyć xD
> \end{figure}
> ```
> * Co robi **[!h]** ?
> * "Każe" zdjęciu być tam gdzie jest to napisane, np. po jakimś bloku tekstu.
>
> `angle = 0` kąt o jaki obracać obrazek
>
> jest też wersja super biedacka co czasami uratuje wam pupę:
>
> ```latex
> \includegraphics[width=0.45\textwidth, angle = 0]{./picture.png}
> ```
>
> 

## Tabelki
### Wstęp
> W sumie to mięsko LaTeX-a, genialne tabelki. Proszę nie słuchać jakiś opinii, że to nie działa, że tabelki to gówno i że się je klepie jak zwierzę.
>
> Zapamiętaj:
> Do LaTeX-a używa się [generatora tabel](https://www.tablesgenerator.com/latex_tables).
>
> **Protip:** Nie koloruj tabelek w Excelu jak przeklejasz, bo się kolorki wysypują.
>
### Jak to wrzucać w ogóle

> ```latex
> \begin{table}[!h]%tabelka będzie tam gdzie chcemy
> \begin{center}
> \resizebox{0.8\textwidth}{!} {
> 
> \begin{tabular}{|c|c|c|}
> \hline
> a & b & c \\ \hline
> d & e & f \\ \hline
> g & h & i \\ \hline
> \end{tabular}
> 
> }
> \end{center}
> \end{table}
> ```
>
> <img src="C:\Users\PanPeople\AppData\Roaming\Typora\typora-user-images\image-20210322000915268.png" alt="image-20210322000915268" style="zoom: 33%;" />
>
> Z generatora wklejamy tylko to co jest w otoczeniu **table** reszta jak z kodu wyżej.
>
> * Dlaczego?
> * Nie wiem, wiem tylko że jak chcę skalować tabelki to tylko tak to działa xD

### "Co gdy LaTeX postanowi wypierdolić twoją tabletkę/zdjęcie 3 strony dalej"
**![math}}{2a}](https://latex.codecogs.com/svg.latex?\Large&space;\sim) z dedykacją dla Michała**

> Na takie zjawisko składać się może kilka powodów, od razu zaznaczam że Word nie jest lepszy w tym. LaTeX ma powody aby coś odwalić.
>
> Najczęściej chodzi o rozmiar i tu mowa o szerokości jak i wysokości, w każdym z tych przypadków będzie problem.
>
> Aby sobie temu zaradzić polecam dać **\newpage** przed tabelką aby upewnić się, że z samą tabelką nie ma żadnego problemu jeśli chodzi o jej kształt. Jeśli wtedy się wysypie to pewnie tabelka jest zła.
>
> Naprawdę mało jest przypadków, że tabelka żyje swoim życiem i prawie zawsze chodzi o rozmiar.

## Bibliografia
### Co to, po co i dlaczego?
>Bibliografia przydaję się w dwóch przypadkach:
>
>* masz laby akustyki z Krukiem
>* piszesz inżynierkę
>
>W pozostałych przypadkach dajesz link w nawiasie i każdy jest zadowolony
>
>Bajer cytowania polega na wstawieniu kwadratowego nawiasu z numerkiem [1] i podciąganie pod numerek odpowiedniej pozycji
>
>Nie wiem po co to napisałem ale nie było to bez sensu więc zostawiłem.

### Jak to wrzucać w ogóle
>Po pierwsze musimy stworzyć plik z naszymi pozycjami naukowymi.
>
>Nazwa pliku to:  **twoja_nazwa.lib**
>
>Struktura każdej pozycji zawiera elementy takie jak tytuł, autor, sposób publikacji,notatka (często tzw. dostęp)
>
>```latex
>@misc{matemaks_odchylenie_standardowe,
>    title={Odchylenie standardowe},
>    author={ Michał Budzyński},
>    howpublished = {\href{https://www.matemaks.pl/odchylenie-standardowe.html}{matemaks.pl}},
>    note = {dostęp: 2020-01-03}
>}
>
>@misc{regula_trzech_sigm,
>    title={Reguła trzech sigm},
>    author={naukowiec org},
>    howpublished = {\href{https://www.naukowiec.org/wiedza/statystyka/regula-trzech-sigm_709.html}{naukowiec.org}},
>    note = {dostęp: 2020-01-03}
>}
>```
>
>**@misc** to nazwa przypisu, coś jak nazwa zmiennej czy #define w C, jej używamy aby odwołać się w teksie do przypisu. Tutaj też ważna sprawa że zawartość tego pliku jest niejako wklejana do końcowego tekstu, czego powodem jest zachowanie uwagi na składnie, czasami trzeba samemu sformatować link
>
>Przypis dodajemy wrzucając w pisany tekst wstawkę/komendę/funkcję(ty bardziej słowo wywołać pasuje):
>
>``` late
>\cite{matemaks_odchylenie_standardowe}
>```
>
>
>
><img src="C:\Users\PanPeople\AppData\Roaming\Typora\typora-user-images\image-20210322021831459.png" alt="image-20210322021831459" style="zoom:67%;" />
>
>
>
>Samą bibliografię trzeba jeszcze wstawić w sensie rozpiskę, taką przeformatowaną zawartość **twoja_nazwa.lib**. Robi się to najczęściej na końcu wstawką:
>
>```latex
>\bibliography{twoja_nazwa}%tutaj załączamy nasz plik z bibliografią
>\bibliographystyle{IEEEtran}%plain %<- jest to rodzaj bibliografii
>```

## Zapis matematyczny
### Skąd mieć wiedzę jak to pisać w ogóle
>No generalnie przez praktykę i podręczne dokumenty.
>
>Teraz Wordowe świry mogą powiedzieć, że oni mają takie fajnie do klikania te formuły matematyczne no  ale to nie dość, że działa średnio to nic z tym poza Wordem nie zrobisz.
>
>Polecane źródła:
>
>* [do wyklikania](http://www.hostmath.com/) <- i już argument o wyklikiwaniu leci out
>* [do przeglądania symboli](https://garsia.math.yorku.ca/MPWP/LATEXmath/latexsym.html) <- nawet nie waż się śmiać z tej strony xD
>* [do przeglądania symboli vol 2](https://artofproblemsolving.com/wiki/index.php/LaTeX:Symbols)
>* [do wykrukowania](https://www.egr.msu.edu/~renjian/LaTeX-Math-Symbols.pdf) <- to mam wydrukowane i leży obok na biurku zawsze
### Trzy otoczenia
> Istnieją trzy sposoby prezentowania wstawek matematycznych:
>
> * jako pewne otoczenie/zdęcie, coś jako otoczenie **figure** ale bez numeracji
>
> <img src="https://latex.codecogs.com/svg.latex?\Large&space;x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}" alt="math}}{2a}" style="zoom:80%;" />
>
> * jako pewne otoczenie/zdęcie, coś jako otoczenie **figure** ale z  numeracji każdej matematycznej formułki (w sprawku jak ktoś wymaga numeracji wzorków to tym lecimy)
>
> <img src="https://latex.codecogs.com/svg.latex?\Large&space;x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}" alt="math}}{2a}" style="zoom:80%;" />
>
>
> * w tekście, tak jak zwykłe literki
>
> <img src="https://latex.codecogs.com/svg.latex?\Large&space;x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}" alt="math}}{2a}" style="zoom:80%;" /> 
>
> I tu nie ma, które lepsze czy coś, bierzemy to czego potrzebujemy
>
> Do jakiegoś poważnego sprawka czy innej inżynierki chcemy korzystać z tego pierwszego
>
> A jak w tekście mamy ochotę wrzucić <img src="https://latex.codecogs.com/svg.latex?\Large&space;\delta" alt="math}}{2a}" style="zoom:80%;" /> to korzystamy z drugiego otoczenia.

### Otoczenie $$

> Pierwsze otoczenie, które pozwala nam zapisać wzór pierwszym sposobem
>
> Wykorzystujemy do tego **$$** na początku i na końcu:
>
> ```latex
> $$ a^2 + b^2 = c^2 $$
> ```
>
> <img src="https://latex.codecogs.com/svg.latex?\Large&space;a^2 + b^2 = c^2" alt="math}}{2a}" style="zoom:80%;" />
>
> Generalnie do sprawek jak jakiś gruby wzór z objaśnieniem to można korzystać z tego.
>
> Potem jakaś lista z opisem zmiennych i wygląda to ładnie i profesjonalnie
> 

### Otoczenie \begin{equasion}

> To samo co wyżej **tylko** wzór dostaje swój numerek, gdyby nie to to można by używać z **$$** zamiennie
### Otoczenie $

> Tego to się naużywasz.
>Korzystamy z **$** na początku i na końcu:
>
> ```latex
> $$ a^2 + b^2 = c^2 $$
> ```
>Dzięki czemu wzorek: <img src="https://latex.codecogs.com/svg.latex?\Large&space;a^2 + b^2 = c^2" alt="math}}{2a}" style="zoom:70%;" /> można wrzucić elegancko w tekst.

### Poważy błąd w ilości dwóch sztuk:

> Skoro mogę to napiszę. Najważniejszy błąd w LaTeX to zapominanie o wrzuceniu zapisu matematycznego w otoczenie matematyczne.
> Indeksy dolne i górne to już jest zapis matematyczny. 
>
> Dlatego jak masz ochotę napisać  <img src="https://latex.codecogs.com/svg.latex?\Large&space;x^2" alt="math}}{2a}" style="zoom:80%;" /> lub <img src="https://latex.codecogs.com/svg.latex?\Large&space;x_2" alt="math}}{2a}" style="zoom:80%;" /> w zdaniu to proszę zapisać to tak:
>
> ```latex
> Dlatego jak masz ochotę napisać $x^2$ lub $x_$ w zdaniu to proszę zapisać to tak: %dobrze
> Dlatego jak masz ochotę napisać x^2 lub x_ w zdaniu to proszę zapisać to tak: %źle
> ```
> 
> Drugi błąd to zapominanie o klamerkach:
>Jeśli mamy ochotę na coś więcej w indeksach wpisać, to trzeba użyć klamerki np. <img src="https://latex.codecogs.com/svg.latex?\Large&space;x_{123}" alt="math}}{2a}" style="zoom:80%;" /> to zapiszemy to tak:
> 
> ```latex
>$x_{123}$ %dobrze
> $x_123 $ %źle
> ```
> no chyba że chcesz uzyskać coś takiego: <img src="https://latex.codecogs.com/svg.latex?\Large&space;x_123" alt="math}}{2a}" style="zoom:80%;" />  co ma miejsce bardzo często to wtedy zapiszemy to :
> 
> ```latex
>$x_123 $
> ```
> 

## Listy
### Wstęp
>Kolejny potężny bajer w LateX-u. Dzielimy je na dwa rodzaje:
>
>* itemize (wymienianie, no w senise np. kropki)
>* enumerate (numerowana)
>
>[Do poczytania](https://www.overleaf.com/learn/latex/lists)
### itemize
>```latex
>\begin{itemize}
>\item pierwsza rzecz
>\item druga rzecz
>\item trzecia rzecz
>\end{itemize}
>```
>
>* pierwsza rzecz
>* druga rzecz
>* trzecia rzecz
>
>Jak widać nie jest no nic skomplikowanego. Listy można oczywiście zagnieżdżać, w Word to takie wcięcie w sumie, tutaj elegancko jest zapisane co do czego należy:
>```latex
>\begin{itemize}
>  \item pierwsza rzecz
>       \begin{itemize}
>           \item pierwsza rzecz pierwszej rzeczy
>           \item druga rzecz pierwszej rzeczy
>           \item trzecia rzecz pierwszej rzeczy
>       \end{itemize}
>  \item druga rzecz
>  \item trzecia rzecz
>\end{itemize}
>```
>
>* pierwsza rzecz
>	* pierwsza rzecz pierwszej rzeczy
>	* druga rzecz pierwszej rzeczy
>	* trzecia rzecz pierwszej rzeczy
>* druga rzecz
>* trzecia rzecz

#### Edytowanie itemize

> Ta część trochę słów pochłonie ale to ważne i nie chcę tu na chama wkleić komend i elo.
> [Do poczytania](https://www.overleaf.com/learn/latex/lists#Unordered_lists_2)
>
> Edytowanie polega na wrzuceniu własnych symboli przy listowaniu
> Domyślnie prezentuję się to tak:
>
> - Poziom pierwszy  `\textbullet` (•)
> - Poziom drugi `\textendash` (–) 
> - Poziom trzeci `\textasteriskcentered` (*)
> - Poziom czwarty `\textperiodcentered` (·)
>
> Robimy to na dwa **bardzo różne** sposoby przy pomocy:
> > ```latex
> > \begin{itemize}[label={}]
> > ```
> > tutaj przy deklarowaniu/użyciu (programiści proszę mnie nie bić xD) ustawiamy styl **tylko na to itemize którego aktualnie używamy**, robimy to poprzez wpisanie w obszar **{} **(obok label)  <u>"symbolu", literki czy innej rzeczy</u>. Można zostawić puste (tak jak w przykładzie), dzięki czemu nie ma żadnych kropek, a zdąża się , że ktoś akurat takiego czegoś potrzebuje.
> > np. można zrobić: `\begin{itemize}[label={\textbullet}]`
> >
> > ______
> > ```latex
> > \renewcommand\labelitemii{\textbullet}  
> > ```
> > Ten sposób jest o wiele inny. W tym przypadku zmieniamy globalnie dla danego poziomu wcięcia symbol przy każdym itemie/myślniku. I teraz proszę posłuchać jaki tu jest k00zak patent na to:
> >
> > No jaki widać tak się wydaje, że coś z tym `\labelitemii` jest ważnego (label + item + cyfra rzymska)
> >
> > Tak więc aby poddać edycji globalnie poziom drugi **itemize** należy wpisać:
> > `\renewcommand\labelitemii{\textbullet}` no bo **2 = ii**, dla czwartego wcięcia byłoby **iv**.
> >
> > No i tutaj ten interesujący nasz symbol wpisujemy w **{}** (obok niczego w sumie xD). Może być puste jak potrzeba.
> >
> > I to też jest fajny bajer, który można fajnie wykorzystać aby stworzyć swoją unikalną templatkę pod sprawko czy inne inżynierki.
>

### enumerate

>```latex
>\begin{enumerate}
>    \item pierwsza rzecz
>    \item druga rzecz
>    \item trzecia rzecz
>\end{enumerate}
>```
>
>1. pierwsza rzecz
>2. druga rzecz
>3. trzecia rzecz
>
>Tak jak w **itemize** można to zagnieżdżać, dla przykładu:
>
> ```latex
>\begin{enumerate}
> \item pierwsza rzecz
>      \begin{enumerate}
>          \item pierwsza rzecz pierwszej rzeczy
>          \item druga rzecz pierwszej rzeczy
>          \item trzecia rzecz pierwszej rzeczy
>      \end{enumerate}
> \item druga rzecz
> \item trzecia rzecz
>\end{enumerate}
> ```
>
>1. pierwsza rzecz
>     I pierwsza rzecz pierwszej rzeczy
>     II druga rzecz pierwszej rzeczy
>  III trzecia rzecz pierwszej rzeczy
>2. druga rzecz
>3. trzecia rzecz
>
> 

#### Edytowanie enumerate

> [Do poczytania](https://www.overleaf.com/learn/latex/lists#Ordered_lists_2)
>
> analogia z **itemize** tylko tutaj korzystamy z ciut innych opisów na poszczególne sposoby numeracji
>
> Do edycji jest wykorzystywana biblioteka/pakiet:
>
> ```latex
> \usepackage[shortlabels]{enumitem}
> ```
>
> Edytowanie polega na wrzuceniu własnych symboli przy wyliczaniu
> Domyślnie prezentuję się to tak:
>
> - Poziom pierwszy `\arabic*` 1. 2. 3.
> - Poziom drugi `(\alph*)` (a) (b) (c)
> - Poziom trzeci `\Roman*` i. ii. iii.
> - Poziom czwarty `\Alph* .` A. B. C. 
>
> Edycję stylu numeracji najłatwiej zmieniać będzie się w sposób jak dla **itemize**
>
> ```latex
> \begin{enumerate}[label={}]
> ```
>
> tylko tutaj drobiny szczegół bo jak widzieliście jakaś tam *** ** zawinęła się przy tych przykładowych numeracjach, no po prostu taki jest i tyle, wiele nie zmienia, zapamiętać nie trudno więc nie marudzimy. 
>
> Przykładowe numeracje:
>
> ```latex
> \begin{enumerate}[label={(\alph*) xDDDD.}]
> 	\item pierwszy element
> 	\item drugi element
> 	\item trzeci element
> \end{enumerate}
> ```
>
> ​	(a) xDDDD. pierwszy element
>
> ​	(b) xDDDD. drugi element
>
> ​	(c) xDDDD. trzeci element
>
> ____
>
> Drugi sposób to ta globalna zmiana (też przydatna, pamiętajcie)
>
> ``` latex
> \renewcommand\labelenumi{\arabic*}  
> ```
>
> ilość **i** na końcu to stopień numeracji/ zagnieżdżenia, gdzie co wpisać jest analogicznie jak w **itemize**. No i to by było tyle na temat list

## Kolumny (lub wejście w 4 gęstość)

### Wstęp

> Teraz zajmiemy się kolumnami ale w ogólnym rozrachunku wstawianiem różnorakich rzeczy w dwie kolumny.
>
> [Do poczytania](https://www.overleaf.com/learn/latex/Multiple_columns)

### multicols

> Domyślnie załatwiamy to pakietem:
>
> ```latex
> \usepackage{multicol}
> ```
>
> Samo użycie kolumny nie jest niczym skomplikowanym.
>
> Działa to zaskakująco dobrze dla dwóch zdjęć albo gołego tekstu. Wtedy to się ładnie na  pół porozlewa i jest cud miód.
>
> Znaczy  tekst + zdjęcie też będzie to działać, tylko wtedy zdjęcie wrzucamy biedackim sposobem (patrz "rozdział" zdjęcia)
>
> `multicols` ma jeszcze bajer polegający na tym, że możemy elegancko na chama wymusić nową kolumnę.
>
> Robimy to:
>
> ```latex
> \columnbreak
> ```
>
> 











\doit







https://tex.stackexchange.com/questions/129951/enumerate-tag-using-the-alphabet-instead-of-numbers



https://tex.stackexchange.com/questions/186981/is-there-a-subsubsubsection-command

