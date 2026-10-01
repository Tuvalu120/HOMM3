# Informacja o pliku "all_performed_actions"

### Autor: Bartosz Han

Podczas trenowania sztucznej inteligencji, tworzony jest plik all_performed_actions,
który zawiera informację o generacji i identyfikatorze osobnika, a także zawiera ciąg znaków
reprezentujący wszystkie akcje, jakie osobnik wykonał podczas pojedynków
z innymi osobnikami w trakcie ewaluacji. Każdy znak w tym ciągu jest dużą literą,
która odpowiada jednemu typowi akcji wykonanej przez agenta:

* **D** - Zaatakuj wroga, który ma największą liczbę punktów zdrowia
* **E** - Zaatakuj wroga z najmniejszą liczbę punktów ataku
* **G** - Idź naprzód
* **H** - Ukryj stos
* **I** - Idź w kierunku przyjaznych stosów
* **J** - Oddal się od przyjaznych stosów
* **K** - Zaatakuj wroga, aby najbardziej zmniejszyć atak przeciwnej armii
* **L** - Zniszcz wrogi stos z największym atakiem
* **M** - Zaatakuj wroga z największym zasięgiem
* **N** - Zaatakuj wroga, który może zniszczyć moje jednostki
* **O** - Zaatakuj wroga, który może najbardziej znacząco zmniejszyć atak mojej armii
* **P** - Zaatakuj wroga, który jest następny w kolejce do ataku
* **Q** - Chroń najsilniejszy stos
* **R** - Nie podejmuj żadnej akcji
