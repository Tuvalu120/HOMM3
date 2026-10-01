# Opis programów stworzonych w ramach pracy inżynierskiej "Sztuczna inteligencja do sterowania przeciwnikiem w HOMM3"
### Autor: Bartosz Han

## 1. Budowanie i uruchamianie aplikacji (w systemie operacyjnym Linux)

W celu zbudowania aplikacji należy najpierw pobrać i zainstalować odpowiednie pakiety:

    sudo apt update && sudo apt install build-essential qt6-base-dev

Następnie, należy wejść do głównego katalogu projektu (do tego, w którym znajduje się ten plik) i skompilować programy za pomocą komendy:

    qmake6 && make

Ta komenda uruchomi kompilację wszystkich programów, które
zostały stworzone w ramach tej pracy. Istnieje opcja kompilacji
tylko wybranych programów, za pomocą zmiennej BUILD_APPS.
Na przykład komenda:

    qmake6 "BUILD_APPS=game trainer" && make

uruchomi tylko kompilację gry oraz programu trenującego.

## 2. Struktura katalogu

Ten katalog składa się z następujących podfolderów:

 * **assets**: zawiera pliki i elementy graficzne wykorzystywane w grze.
 * **battle_example** : zawiera kod źródłowy programu do demonstracji działania sztucznej inteligencji (rozdział 5. tego pliku).
 * **common**: zawiera kod źródłowy klas, które, ze względu na ich przeznaczenie nie mogą być skatalogowane w jednym z pozostałych podfolderów i są częścią więcej niż jednego programu.
 * **game**: zawiera kod źródłowy środowiska gry (rozdział 3. tego pliku). Nie zawiera kodu źródłowego elementów graficznych.
 * **graphics**: zawiera kod źródłowy elementów graficznych wykorzystywanych w programach.
 * **models**: zawiera zestaw modeli sztucznej inteligencji stworzonych podczas trenowania graczy. Modele zostały stworzone z użyciem najlepszego znalezionego zestawu hiperparametrów (rozdział 6.2 pracy).
 * **out**: jest folderem, do którego trafiają pliki wykonywalne programów.
 * **tests**: zawiera kod źródłowy testów jednostkowych elementów innych programów (rozdział 6. tego pliku).
 * **trainer**: zawiera kod źródłowy programu trenującego sztuczną inteligencję (rozdział 4. tego pliku).

## 3 Gra

### 3.1 Uruchamianie

Grę można uruchomić, poprzez wywołanie programu znajdującego się w folderze out/game:

    ./out/game/heroes3 <liczba graczy> <ścieżka do pliku z modelem> <ziarno generatora>

gdzie:

* **liczba graczy** określa, ile graczy bierze udział w rozgrywce. Liczba graczy w grze musi wynosić 2-4.
Domyślnie w grze uczestniczy 4 graczy.
* **ścieżka do pliku z modelem** określa ścieżkę do pliku zawierającego model sztucznej inteligencji,
który będzie wykorzystywany przez neutralnych graczy. Domyślnym modelem jest assets/AI/model .
* **ziarno generatora** Określa ziarno generatora liczb
losowych dla gry. Domyślnie ziarno jest tworzone losowo.

Przykład uruchomienia:

    ./out/game/heroes3 2 models/best_model 123

Każdy argument może być podany opcjonalnie, program można również włączyć bez argumentów:

    ./out/game/heroes3

### 3.2 Rozgrywka

Rozgrywka rozpoczyna się od razu po uruchomieniu gry.
Każdy z graczy na początku rozgrywki otrzymuje jednego bohatera i
kontroluje jedno miasto.

W głównej pętli gry, gracz po kolei (w turach) poruszają się swoimi armiami.
Użytkownik może sterować armiami
znajdującymi się na mapie świata, poprzez kliknięcie na pole, do którego użytkownik chce, aby aktywnie obecnie armia
przesunęła się.

W trakcie rozgrywki, gracz może podejmować interakcję z innymi obiektami znajdującymi się na mapie
świata. Aby gracz podjął interakcję z tymi obiektami, musi on przesunąć swoją armię na
pole sąsiadujące z polem, na którym znajduje się obiekt. W przypadku niektórych obiektów,
(mędrcy, neutralne jednostki, skrzynie ze skarbami, a także w przypadku zdobycia nowego
poziomu bohatera),
gracz będzie musiał podejmować decyzje, w jaki sposób chciałby podjąć interakcję z danym obiektem, wybierając jedną z dostępnych opcji, które pojawią się na ekranie.

W niektórych przypadkach (podjęcie interakcji z armią wroga, wrogim lub neutralnym
miastem albo kopalnią, albo wybór opcji walki z neutralnymi jednostkami)
gracz może walczyć z innymi armiami. Bitwy w tej grze mają charakter turowy, podczas których gracze kontrolują swoje jednostki. Tury jednostek są z góry ustalone - gracz nie może decydować, którą jednostkę chce kontrolować w danej turze.

Gracz może kontrolować swoje stosy i swojego bohatera, poprzez kliknięcie w odpowiednie pole na polu bitwy, z którym to gracz chce podjąć jakąś akcję.
Rodzaj podjętej akcji zależy od stanu gry:

* Jeśli gracz kontroluje teraz stos (Stack), a pole docelowe jest puste, to jednostka gracza
przemieści się na to pole.
* Jeśli gracz kontroluje teraz stos, a na polu docelowym znajduje się teraz inna jednostka, to jednostka gracza zaatakuje wrogą jednostkę.
* Jeśli gracz kontroluje teraz bohatera, to gracz musi podjąć dwie decyzje: najpierw, gracz wybiera zaklęcie, które chce rzucić, a następnie wybiera cel  tego zaklęcia (czyli pole na polu bitwy).

W trakcie gry, gracz może także odwiedzać przyjazne miasta, w których to może, za pomocą interfejsu użytkownika zdobywać nowe zasoby, lub rozbudowywać i ulepszać samo miasto.

Gra kończy się w momencie, w którym w grze zostaje tylko jeden gracz. Gracz zostaje wyeliminowany z rozgrywki, kiedy nie kontroluje żadnej armii, lub nie kontroluje żadnego miasta. w momencie zakończenia gry okno z grą zamyka się automatycznie.

## 4. Program trenujący

### 4.1 Uruchamianie

Program trenujący można uruchomić, poprzez wywołanie programu znajdującego się w folderze out/trainer:

    ./out/trainer/trainer <ścieżka do pliku z hiperparametrami> <nazwa konfiguracji> <liczba wątków w puli wątków>

gdzie:

* **ścieżka do pliku z hiperparametrami** - oznacza ścieżkę (relatywną to ścieżki, w której znajduje się plik readme)
pliku, który deklaruje hiperparametry algorytmu. Przykład takiego pliku znajduje się w trainer/hyperparams.txt .

* **nazwa konfiguracji** - nazwa dla zestawu hiperparametrów wykorzystywanych podczas uruchomienia.
Nazwa ta to także nazwa folderu, w którym będą znajdować się wygenerowane modele, oraz inne pliki
powiązane z uruchomieniem algorytmu. Foldery z danymi uruchomień algorytmu znajdują się
w folderze trainer/output. Nie może istnieć więcej niż
jeden folder z daną nazwą konfiguracji.

* **liczba wątków w puli wątków** - Liczba wątków, która będzie wykorzystywana do jednoczesnego
przeprowadzania bitew w ramach ewaluacji populacji. Domyślnie liczba wątków wynosi 1.

Przykład uruchomienia:

    ./out/trainer/trainer trainer/hyperparams.txt test 16

### 4.2 Opis programu

Program ten trenuje sztuczną inteligencję za pomocą algorytmu NEAT, poprzez przeprowadzanie pojedynków między
osobnikami. Program będzie trenował tyle generacji, ile to zostało podane w hiperparametrach.
W trakcie działa algorytmu, w terminalu będzie pojawiać się informacja o ukończonym
trenowaniu generacji.

### 4.3 Zapisywane dane

W trakcie działania algorytmu, w folderze z danymi będą pojawiać i aktualizować się
pliki. Do najważniejszych plików należą:

* all_fitnesses - wszystkie wartości funkcji przystosowania uzyskane przez osobniki w każdej generacji. Wartości w pojedynczej linii oznaczają kolejno: numer generacji, identyfikator osobnika i jego wartość funkcji przystosowania w tej generacji.

* best_fitnesses - najlepsze
wartości funkcji przystosowania
w każdej generacji.

* best_model - najlepszy
model wytworzony w ostatniej generacji algorytmu.

* final_test_score -
wyniki walidacji najlepszego
modelu ostatniej generacji.
Wynik powstaje poprzez rozegranie 10000
bitew przeciwko graczowi
opartemu o algorytm VCMI.
Plik zawiera dane o współczynniku
zwycięstw.

* random_attacking_validation_score, random_validation_score i
vcmi_10_validation_score - wyniki walidacji najlepszego modelu
w każdej generacji
przeciwko kolejno:
graczowi wykonującemu
losowe akcje atakujące
i graczowi wykonującemu
losowe akcje i graczowi
wykorzystującemu algorytm z projektu VCMI, o poziomie trudności równym 10.
Każda wartość w tych
plikach oznacza liczbę
wygranych pojedynków
(przy czym za każdym razem rozgrywanych jest 100 bitew).

W folderze z danymi
znajduje się także
folder models,
w którym znajdują się
wszystkie modele
stworzone podczas
działania algorytmu.
W folderze models,
każdy folder zawiera dane
o osobnikach w jednej
generacji (foldery te
są ponumerowane pod
względem numeracji generacji).
Każdy folder zawiera pliki
opisujące modele sztucznej
inteligencji, które
zostały stworzone
w tej generacji. Każdy
plik posiada nazwę będącą
identyfikatorem sieci neuronowej opisywanej przez ten plik.

## 5. Program demonstrujący sztuczną inteligencję

### 5.1 Uruchamianie

Do zaprezentowania działania sztucznej inteligencji,
stworzono program battle_example. Program ten uruchamia się w następujący sposób:

    ./out/battle_example/battle_example <ścieżka do pliku z modelem> <ziarno generatora>

gdzie <ścieżka do pliku z modelem> określa ścieżkę do pliku zawierającego model sztucznej inteligencji,
który będzie wykorzystywany przez komputerowego przeciwnika, a
<ziarno generatora> określa ziarno generatora liczb
losowych dla gry. Domyślnie ziarno jest tworzone losowo.

Przykład uruchomienia:

    ./out/battle_example/battle_example models/best_model 123

### 5.2 Opis programu

Po uruchomieniu programu, gra zostaje ustawiona na fazę bitwy.
Użytkownik programu steruje armią atakującą, a sztuczna
inteligencja steruje armią atakowaną. Obydwie
armie zostały stworzone w sposób losowy,
na takich samych zasadach, jak podczas trenowania
sztucznej inteligencji. Obydwaj gracze posiadają na początku gry takie same armie.

Pojedynek kończy się, gdy jeden z graczy utraci
wszystkie stosy (tak, jak w przypadku normalnej wersji gry).
Wówczas, program kończy swoje działanie, a w terminalu
wypisywany jest wynik gry.

## 6. Testy jednostkowe

Do gry i sztucznej inteligencji stworzono także zestaw testów jednostkowych.
Testy jednostkowe są uruchamiane automatycznie podczas kompilacji programów
(jeśli użytkownik zadeklarował, że chce skompilować testy).

Testy można także uruchomić w następujący sposób:

    ./out/tests/test_runner