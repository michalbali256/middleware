Chat sa spusta jednoducho pomocou skriptu run.sh, v nom je potrebne nastavit PYTHONPATH na cestu k instalacii python-chord.

Problem tejto implementacie je, ze nerata s tym, ze nejaky uzol moze neocakavane skoncit - vtedy sa informacie o vsetkych channeloch a menach, ktore mal na starosti stratia.
Riesenie by som videl v replikacii - vsetky nelokalne data by drzali (aspon) dva nasledujuce uzly - prvy v poradi nazvime master a druhy slave. Kedze je ale struktura
cyklicka, kazdy uzol ma prave jednu funkciu master a prave jednu slave. Ak vypadne master, slave sa
o tom dozvie, ked mu zacnu chodit spravy, ktore doteraz chodili mastrovi, a musi spravit zalohu na dalsi uzol. Zaroven musi upozornit uzol, ktory prechadzal mastrovi, ze
jeho slave vypadol, a vypytat si od neho data.