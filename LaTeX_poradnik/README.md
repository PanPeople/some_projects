# LaTeX poradnik

## Wstęp
### Przedmowa
> Nie wiem na razie czym ma być ten twór, na razie planuję aby był to zbiór różnych rzeczy do LaTeX-a. Całość chcę podzielić na tematyczne rozdziały.

### Parę zdań o LaTeX
> 

## Spis treści:

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
> * Przydatne linki
> * Listy
> * Zapis matematyczny
> * Przydatne praktyki
> * Pierdoły (linki, komendy)

## Biblioteki

> Generalnie im "więcej tym, nic się nie stanie". Polecam mieć już szablon używanych bibliotek.
>
> Moje biblioteki są do wielu rzeczy i rzadko trzeba jakąś jeszcze dodać.
>
> ```latex
> \documentclass{article}% do sprawka to oczywiście
> \usepackage[utf8]{inputenc}
> \usepackage{polski}
> \usepackage{amssymb}
> \usepackage{wasysym}
> \usepackage{graphicx}
> \usepackage{multirow}
> \usepackage{multicol}
> \usepackage{wrapfig} %obraz w multicol
> \usepackage[table,xcdraw]{xcolor}
> \usepackage[shortlabels]{enumitem}
> \usepackage{bm}
> \usepackage[top=1.5cm, bottom=1.5cm, left=1.7cm, right=1.7cm]{geometry}% ustawianie marginesów
> \usepackage[english]{babel}
> \usepackage{blindtext}
> ```
>
> 

## Zdjęcia

> Najważniejsza rzecz w sumie i najpiękniejsze co LaTex oferuje, już nigdy (prawie) zdjęcie nie rozwali Ci całego sprawka jak jakiś tam Word. Wrzucanie zdjęć to żadne filozofia
>
> ```latex
> \begin{figure}[!h] % b-bottom t-top
> \centering
> \includegraphics[width=0.45\textwidth]{./picture.png}
> \captionsetup{labelformat=empty}%jak nie chcesz mieć przy opisie napisane "Figure *numer*" to pozostaw odkomentowane
> \caption{podpis}%chyba nie muszętłumaczyć xD
> \end{figure}
> ```
> * Co robi **[!h]** ?
> * "Każe" zdjęciu być tam gdzie jest to napisane, np. po jakimś bloku tekstu.
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
**$\sim$ z dedykacją dla Michała**

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
>    howpublished = {\url{https://www.matemaks.pl/odchylenie-standardowe.html}},
>    note = {dostęp: 2020-01-03}
>}
>
>@misc{regula_trzech_sigm,
>    title={Reguła trzech sigm},
>    author={naukowiec org},
>    howpublished = {\url{https://www.naukowiec.org/wiedza/statystyka/regula-trzech-sigm\_709.html}},
>    note = {dostęp: 2020-01-03}
>}
>```
>
>**@misc** to nazwa przypisu, coś jak nazwa zmiennej czy #define w C, jej używamy aby odwołać się w teksie do przypisu
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
>Samą bibliografię trzeba jeszcze wstawić. Robi się to najczęściej na końcu:
>
>```latex
>\bibliography{twoja_nazwa}
>\bibliographystyle{IEEEtran}%plain %<- jest to rodzaj bibliografii
>```
>
>