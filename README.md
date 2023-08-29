# river_crossing2023

Problem: Prevesti preko reke Linux hakere i Microsoft radnike (u zadatku sam stavila da ih npr. ima 10-10) brodicem koji mora primiti 4 osobe.
Zbog ogranicenja da u brodicu ne sme biti samo 1 haker ili samo 1 radnik dolazimo do 3 moguca slucaja:
1) u brodicu ce biti 4 zaposlena
2) u brodicu ce biti 4 hakera
3) u brodicu ce biti 2 zaposlena i 2 hakera

Da bi se ovo realizovalo koristila sam strukture podataka semafore i barijeru. Semafori vode racuna o tome koliko hakera i koliko zaposlenih ceka da se ukrca (zbog ogranicenja).
Barijera koja je inicijalizovana na 4 omogucava da niti (bilo zaposleni ili hakeri) ne mogu proci dok ih ne bude 4 (cekaju se). Kada se sakupi 4 niti i odaberemo kapetana brodic ce isploviti.
Takodje, koristila sam i mutex kako bih zakljucala kriticnu sekciju koda, da samo jedna nit moze pristupiti u datom trenutku.
