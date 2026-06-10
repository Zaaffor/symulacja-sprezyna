# symulacja-sprezyna

Program oblicza numerycznie tłumienie drgania obiektu zawieszonego na sprężynie za pomocą metody Eulera. Parametry sprężyn są podawane w zakładce "Ustawienia", można je także zmieniać w trakcie biegu symulacji. Wszystkie wersje symulacji czerpią z tych samych ustawień i przenoszą się one między sobą.

W programie można odhaczyć okienko które po zamknięciu programu wydrukuje nam wykres porównujący.

W trybie symulacji 3D użytkownik może obracać kamerą dookoła symulowanej sprężyny za pomocą kursora.

Zmienne które można modyfikować:
* Stała sprężystości sprężyny (k),
* Masa obiektu (m),
* Stała tłumienia (c),
* Wychylenie w osi Y,
* Wychylenie w osi X,
* Wychylenie w osi Z

Program został stworzony przy użyciu bibliotek raylib, raygui oraz matploplus. 
