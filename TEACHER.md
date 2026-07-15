# TEACHER.md - Docentenhandleiding Computerarchitectuur met de CH552

Dit document biedt didactische handvatten, discussiepunten en achtergrondinformatie voor de docent om de concepten in het `ch552_blink` project uit te leggen aan studenten.

Dit project is de eerste stap in een leerlijn. De CH552 laat studenten de basis zien op een kleine 8052-achtige machine. Daarna kan dezelfde aanpak worden herhaald op een moderne goedkope RISC-V chip van WCH, bijvoorbeeld de **CH32V003F4P6**.

Gebruik de Arduino Uno / AVR ATmega328P niet als hoofdroute in deze leerlijn. Daarvoor bestaan al veel online tutorials en de Arduino-laag verbergt juist veel architectuurdetails. Studenten mogen die route zelfstandig verkennen, maar de kernvergelijking hier is: eenvoudige 8052-architectuur eerst, daarna moderne RISC-V.

*   **Datasheet-referentie:** `docs/datasheets/ch552_datasheet_en.pdf`
*   **Intel HEX documentatie:** `docs/markdown/hex_format.md`

---


## 💻 Didactische Kernpunten & Concepten

Gebruik de code in `src/main.c` en `include/ch552.h` om de volgende concepten tastbaar te maken:

### 1. Special Function Registers (SFRs) vs. Memory-Mapped I/O
*   **Wat te vertellen:** In modernere systemen (zoals ARM Cortex-M of RISC-V) delen randapparatuur (GPIO, Timers) en RAM dezelfde 32-bit adresruimte. Bij de 8051-architectuur is dit gescheiden. Er is een aparte geheugenruimte van 128 bytes genaamd de SFR-ruimte (`0x80` t/m `0xFF`).
*   **Visualisatie in code:** Wijs studenten op `__sfr __at (0xB0) P3;` in `include/ch552.h`. Dit vertelt de compiler dat het label `P3` direct gekoppeld is aan het hardware-register op adres `0xB0`.
*   **Discussievraag:** *Waarom heeft de 8051 een aparte SFR-ruimte in plaats van alles in één grote adresruimte te stoppen?* (Antwoord: Door een aparte, kleine SFR-ruimte te gebruiken, kunnen instructies korter en sneller zijn. Er is minder adres-decodering logica nodig in de CPU).

---

### 2. Accumulator-based Architectuur vs. Register-File (RISC)
*   **Wat te vertellen:** De CH552 is een CISC-achtige accumulator-machine (8051 core). In tegenstelling tot RISC (zoals ARM) die 16 of 32 algemene registers (`r0` t/m `r15`) heeft, gaan bij de 8051 bijna alle rekenkundige bewerkingen via één enkel register: de **Accumulator (ACC / Register A)**.
*   **Didactische tip:** Laat studenten nadenken over de assembly die gegenereerd wordt voor de delay-loop. Elke optelling moet eerst naar de accumulator geladen worden, daar verwerkt worden, en weer teruggeschreven worden naar RAM. Dit maakt de instructiedichtheid heel anders dan bij RISC.

---

### 3. Bit-Adresseerbaarheid & SETB / CLR Instructies
*   **Wat te vertellen:** Dit is een unieke feature van de 8051. Sommige SFR-registers (die eindigen op adres `0` of `8`, zoals `0x90` en `0xB0`) zijn **bit-adresseerbaar**. 
*   **De link met C:** 
    *   Voor `P3` (adres `0xB0`) kunnen we in C direct een individuele bit aansturen: `LED_PIN = 1;`. De compiler vertaalt dit naar een enkele assembly instructie: `SETB P3.0` (Set Bit). Dit is extreem snel en atomair.
    *   Voor `P3_MOD_OC` (adres `0x96`) kan dit *niet*, omdat dit register niet bit-adresseerbaar is. Hier moeten we een traditionele bitmasker bewerking doen: `P3_MOD_OC &= ~(1 << 0);`. Dit vertaalt zich naar meerdere assembly-instructies (Read-Modify-Write).

---

### 4. Register Breedte (8-bit vs. 16/32-bit)
*   **Het Experiment:** Laat studenten kijken naar de delay-functie `delay_ms(unsigned int ms)`. 
    *   Omdat `unsigned int` 16-bit is op SDCC, moet een 8-bit CPU meerdere instructies uitvoeren om te controleren of de loop-variabele de eindwaarde heeft bereikt (eerst low byte vergelijken, dan high byte vergelijken met carry).
*   **Opdracht voor studenten:** 
    1. Vraag hen om `unsigned int` te veranderen in `unsigned char` (wat 8-bit is).
    2. Laat hen observeren dat de LED nu veel sneller knippert (ongeveer 4 tot 8 keer zo snel). 
    3. Leg uit dat dit komt doordat de CPU nu in één enkele 8-bit instructie de loop-conditie kan evalueren in plaats van een complexe 16-bit simulatie te draaien.

---

### 5. De Assembly NOP (No Operation)
*   **Wat te vertellen:** In `src/main.c` staat `__asm__("nop");`. Dit is een directe instructie aan de CPU om één klokcyclus (of instructiecyclus) te wachten en niets te doen.
*   **Architectuur-les:** Dit illustreert de *Instruction Cycle* (Fetch, Decode, Execute). Zelfs als een processor "niets" doet, doorloopt hij nog steeds deze stappen en verbruikt hij stroom en tijd.

---

### 6. Geavanceerd: SPI, USB en Direct Memory Access (DMA)
*   **Wat te vertellen:** De CH552 bevat geavanceerde ingebouwde hardware die uniek is voor dit chipformaat, zoals een complete USB 2.0 Device Controller en een SPI interface. Deze worden aangestuurd via de definities in `include/ch552_peripherals.h`.
*   **Didactisch concept (DMA):** 
    *   Laat studenten kijken naar de registers `UEPn_DMA_H` en `UEPn_DMA_L` (Endpoint DMA buffers).
    *   Leg uit dat USB-data met hoge snelheid binnenkomt. Als de CPU elke byte handmatig via interrupts moest verwerken, zou hij geen tijd meer hebben voor andere taken.
    *   In plaats daarvan gebruikt de CH552 **Direct Memory Access (DMA)**. De CPU schrijft het startadres van een buffer in de DMA-registers. De USB-peripheral schrijft binnenkomende data vervolgens *rechtstreeks* in het RAM (xRAM) zonder dat de CPU hieraan te pas komt. Zodra het pakket compleet is, krijgt de CPU pas een interrupt (via `USB_INT_FG`).
*   **Sleutelwoord:** *Hardware Autonomie*. Peripherals zijn onafhankelijke processors die parallel aan de E8051 CPU draaien en via SFRs geconfigureerd worden.

---

### 7. Geavanceerd: Memory Overlaying (Geheugen Overlappen)
*   **Wat te vertellen:** De 8051-architectuur heeft extreem weinig intern RAM (slechts 128 of 256 bytes). Normale talen (zoals C op x86/ARM) zetten lokale variabelen van functies op een dynamische stack. Op de 8051 is dit heel inefficiënt omdat de stack-pointer 8-bit is en de CPU geen instructies heeft om snel relatief ten opzichte van de stack-pointer data te lezen/schrijven.
*   **De oplossing (Overlays):** De compiler (SDCC of Keil) maakt een call-tree (aanroepschema) van het programma. Als functie A en functie B elkaar nooit kunnen aanroepen (ze sluiten elkaar uit), dan geeft de compiler de lokale variabelen van beide functies **exact dezelfde fysieke geheugenadressen** in het RAM!
*   **Didactische tip:** Dit is een prachtig voorbeeld van hoe compiler/linker software samenwerkt om hardwarebeperkingen op te lossen.
*   **De valkuilen (Belangrijk voor programmeurs):**
    *   **Recursie:** Als een functie zichzelf aanroept, overschrijft hij zijn eigen lokale variabelen (overlay werkt niet). Recursieve functies moeten gemarkeerd worden met `__reentrant` om ze te dwingen een stack te gebruiken.
    *   **Interrupts:** Een functie die door zowel `main` als een interrupt (ISR) wordt aangeroepen, mag niet overlayed worden. De ISR kan immers de main onderbreken en zo de gedeelde variabelen halverwege corrupt maken.
    *   **Functiepointers:** Bij indirecte aanroepen kan de linker de call-tree niet automatisch bepalen, wat kan leiden tot corrupte overlays als je de linker niet handmatig helpt.

---

## 🛠️ Aanbevolen Lesopbouw (Micro-lab)

1.  **De Basis:** Compileer de code met `make` en upload deze naar de CH552 met `make flash`. Zie de LED knipperen.
2.  **Verander de Pin:** Laat de studenten in de datasheet (of online pinout) zoeken naar een andere pin (bijvoorbeeld `P1.1`) en laat hen de registers `P1_MOD_OC` en `P1_DIR_PU` correct instellen in `src/main.c` om een externe LED op een breadboard te laten knipperen.
3.  **Inspecteer de ASM:** (Optioneel) SDCC genereert bij compilatie ook een `.asm` bestand (bijv. `build/main.asm`). Open dit bestand en laat de studenten de `delay_ms` functie in assembly opzoeken. Wijs ze op de hoeveelheid instructies die nodig zijn voor de 16-bit vergelijking.
4.  **Optimalisatie:** Verander de delay variabelen naar `unsigned char` (8-bit) en ervaar het verschil in snelheid en code-grootte.

## Vervolg: CH32V003 / RISC-V

De tweede introductie kan dezelfde didactische volgorde gebruiken, maar dan op de CH32V003F4P6:

1.  **Reset en opstart:** Waar begint de RISC-V core na reset, en welke code draait voor `main`?
2.  **Vector table:** Waar staan exception- en interrupt-entrypoints, en hoe verschilt dit van de 8052-vectoren?
3.  **Minimale GPIO:** Configureer een enkele pin zonder framework en vergelijk de registertoegang met de CH552.
4.  **Moderne peripherals:** Ga daarna pas naar timers, systick/interrupts, communicatieperipherals en DMA-achtige hardwareblokken.

Het doel is dat studenten herkennen dat de vragen hetzelfde blijven, terwijl de architectuur moderner en rijker wordt.
