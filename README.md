# 🎲 Monopoly in Terminal

Klasická stolová hra **Monopoly** prepracovaná do textového prostredia terminálu. Tento projekt je napísaný v jazyku **C** a využíva **CMake** na jednoduchú kompiláciu a správu závislostí. Rozhranie je vytvorené pomocou **ncurses** knižnice pre interaktívnu hru priamo v termináli.

## Vlastnosti hry

* **Hra pre viacerých hráčov:** Podpora pre 2 až 4 hráčov lokálne v jednom termináli.
* **Kompletná hracia doska:** Obsahuje 24 políčok s 16 nehnuteľnosťami a špeciálnymi poliami.
* **Ekonomický systém:** Nakupovanie nehnuteľností, platenie nájmu, monopoly a bankrot.
* **Intuitívne grafické rozhranie:** Farebné zobrazenie dosky, vlastností hráčov a stavu hry.
* **Systém väzenia:** Hracia logika s väzením a priepustkami.

## Požiadavky

Pred spustením sa uisti, že máš nainštalované nasledujúce nástroje:
* **C kompilátor** (GCC, Clang alebo ďalší C11 kompilátor)
* **CMake** (verzia 3.26 alebo novšia)
* **ncurses knižnica** (pre grafické rozhranie v termináli)
* **Git** (voliteľné, na klonovanie repozitára)

### Inštalácia závislostí

#### Na Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libncurses-dev
```

#### Na macOS:
```bash
brew install cmake ncurses
```

#### Na Windows:
Nainštaluj [MinGW](https://www.mingw-w64.org/) alebo [MSVC](https://visualstudio.microsoft.com/) a [CMake](https://cmake.org/download/).

## Inštalácia a kompilácia

### Krok 1: Klonuj repozitár
```bash
git clone https://github.com/OndrejOlexa/Monopoly-in-terminal.git
cd Monopoly-in-terminal
```

### Krok 2: Vytvor build priečinok
```bash
mkdir build
cd build
```

### Krok 3: Spusti CMake a precompiluj
```bash
cmake ..
make
```

### Krok 4: Spusti hru
```bash
./z4
```

## Ako hrať

### Spustenie hry s príkazovým riadkom

```bash
./z4 -n 2
```

**Možnosti príkazového riadku:**
- `-n <počet>` - Nastav počet hráčov (2-4, predvolené: 2)
- `-s` - Zobraz kompletnú hraciu dosku po každom ťahu
- `-p` - Zobraz informácie o hráčoch po každom ťahu
- `-g` - Zobraz všetko (dosku aj hráčov) po každom ťahu

### Pravidlá hry

#### Štart hry
- Každý hráč začína na štartovacom poli (pole č. 1) s pevne stanoveným počtom peňazí:
  - 2 hráči: 20 peňazí
  - 3 hráči: 18 peňazí
  - 4 hráči: 16 peňazí

#### Ťah hráča
1. Hra ťa požiada na vstup: **"Enter dice roll (1-6)"**
2. Zadaj číslo od 1 do 6 (reprezentuje hod kockou)
3. Tvoja figúrka sa posunie o daný počet polí dopredu

#### Prechod štartom
- Keď prejdeš polem štart (počas pohybu aj bez pohybu), získaš **2 peniaze** navyše

#### Nákup nehnuteľností
- Keď pristanúš na vlastníctve bez majiteľa, máš možnosť ju kúpiť za zobrazovanú cenu
- Ak máš dosť peňazí, nehnuteľnosť sa ku tebe pripíše
- Ak nemáš dosť peňazí, nehnuteľnosť si nevlásti a pokračuje sa ďalej

#### Platenie nájmu
- Keď pristanúš na cudzej nehnuteľnosti, musíš vlastníkovi zaplatiť nájom
- **Základný nájom:** Cena nehnuteľnosti
- **Monopoly nájom:** 2 × cena nehnuteľnosti (keď vlastník má obe nehnuteľnosti tej istej farby)

#### Väzenie
- Keď pristanúš na poli "GO TO JAIL", prejdeš do väzenia
- Vo väzení musíš zaplatiť **1 peniaz** na vstup a vo väzení zostávaš **1 ťah**
- Keď je to znovu tvoj ťah, vyjdeš z väzenia
- **Priepustky z väzenia:** Keď pristanúš na poli "JAIL PASS", zbereš si priepustku
  - Priepustka ti umožňuje vyjsť z väzenia bez platby

#### Koniec hry - Bankrot
- Keď nemáš dosť peňazí na zaplatenie nájmu alebo nákupu, sa **vyraďuješ z hry**
- Hra pokračuje bez teba

#### Výherca
- Posledný hráč, ktorý nie je bankrotovaný, **vyhráva hru**
- V prípade rovnosti (viacerí hráči s rovnakým počtom peňazí a nehnuteľností), je to **remíza**

### Rozhranie hry

Hra zobrazuje v reálnom čase:

1. **Doska (ľavá strana):**
   - Všetky 24 políčka hracej dosky
   - Názvy nehnuteľností a ich ceny
   - Farby nehnuteľností (kódované v termináli)
   - Aktuálni majitelia nehnuteľností

2. **Informácie o hráčoch (pravá strana):**
   - Číslo hráča
   - Aktuálna pozícia na doske
   - Počet peňazí
   - Počet priepustiek z väzenia
   - Stav väzenia (áno/nie)
   - Zoznam vlastnených nehnuteľností

3. **Informácie o ťahu (pravá spodná časť):**
   - Aktuálny ťah (číslo)
   - Hráč na rade
   - Posledný hod kockou

## Príklady spustenia

### Hra pre 2 hráčov bez dodatočného výstupu
```bash
./z4 -n 2
```

### Hra pre 4 hráčov so zobrazením všetkých informácií
```bash
./z4 -n 4 -g
```

### Hra pre 3 hráčov so zobrazením dosky
```bash
./z4 -n 3 -s
```

## Štruktúra projektu

```
Monopoly-in-terminal/
├── CMakeLists.txt           # Konfigurácia CMake
├── README.md                # Tento súbor
├── include/
│   ├── monopoly.h           # Hlavička hry s dátovými štruktúrami
│   └── ui.h                 # Hlavička pre UI rozhranie
└── src/
    ├── z4.c                 # Hlavný program a herná logika
    ├── monopoly.c           # Inicializácia dosky a dát
    └── ui.c                 # Grafické rozhranie (ncurses)
```

## Poznámky

- Hra je vyprojektovaná pre **zjednodušenú verziu Monopoly** s 24 poliami a 16 nehnuteľnosťami
- Rozhranie sa prispôsobí veľkosti tvojho terminálu (minimálne 80×24 znakov)
- Farby sa použijú iba ak tvoj terminál ich podporuje

## Autori

Projekt vytvoril Ondrej Olexa

## Licencia

Projekt je momentálne bez špecifikovanej licencie. Podľa potreby si ju môžeš pridať.
