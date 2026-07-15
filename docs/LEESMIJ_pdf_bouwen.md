# PDF's bouwen uit de Markdown

De Markdown-bestanden (`.md`) zijn de bron. De PDF's worden daaruit gegenereerd
met twee gratis tools:

1. **pandoc** — zet Markdown om naar HTML.
2. **WeasyPrint** — zet die HTML met de stylesheet `pdf_style.css` om naar PDF.

Je bewerkt dus altijd de `.md`, draait het script, en krijgt een verse PDF.

## Eenmalig installeren

- **pandoc**: download van <https://pandoc.org/installing.html>
  (op Windows kan ook: `winget install --id JohnMacFarlane.Pandoc`).
- **WeasyPrint**: `pip install weasyprint`
  (WeasyPrint heeft Python nodig; op Windows installeert `pip` meestal ook de
  benodigde onderdelen. Zie <https://doc.courtbouillon.org/weasyprint> bij
  problemen.)

Controleer daarna in een terminal:

```
pandoc --version
weasyprint --info
```

## Bouwen

Zet `build_pdf.sh` (Linux/Mac) of `build_pdf.ps1` (Windows), samen met
`pdf_style.css`, in dezelfde map als je `.md`-bestanden.

**Windows (PowerShell):**

```powershell
.\build_pdf.ps1                    # alle *.md in de map
.\build_pdf.ps1 CH552_Architectuur_Introductie.md   # één bestand
```

**Linux / macOS:**

```bash
chmod +x build_pdf.sh              # eenmalig uitvoerrechten geven
./build_pdf.sh                     # alle *.md in de map
./build_pdf.sh CH552_Architectuur_Introductie.md    # één bestand
```

Elk `naam.md` wordt `naam.pdf`.

## Kleine aanpassingen maken

- **Inhoud** (tekst, tabellen, codeblokken): bewerk de `.md` en bouw opnieuw.
- **Uiterlijk** (lettertype, kleuren, marges, paginanummer): bewerk
  `pdf_style.css`. Bijvoorbeeld de accentkleur staat op `#0b3d5c` (donkerblauw);
  vervang die overal om een andere huisstijlkleur te krijgen. De paginagrootte
  en marges staan bovenaan in het `@page`-blok.

## Werkt het met plaatjes?

Ja. Verwijs in de Markdown naar een afbeelding met een **relatief pad**:

```markdown
![Blokschema van de 8051](afbeeldingen/blokschema.png)
```

Leg het bestand `blokschema.png` dan in een submap `afbeeldingen/` naast je
`.md`. Het script geeft pandoc de juiste zoekmap mee (`--resource-path`), zodat
WeasyPrint de plaatjes vindt en insluit in de PDF.

Tips voor plaatjes:

- Ondersteunde formaten: **PNG, JPG, SVG** (SVG geeft de scherpste lijnen voor
  diagrammen).
- Grootte bijsturen kan met HTML in de Markdown, bijv.
  `<img src="afbeeldingen/blokschema.png" width="450">`.
- Gebruik bij voorkeur relatieve paden (geen `C:\Users\...`), dan blijft het
  project verplaatsbaar.

## Bestandenoverzicht

| Bestand | Functie |
|---|---|
| `*.md` | Bronteksten (bewerk deze) |
| `pdf_style.css` | Opmaak/stijl van de PDF |
| `build_pdf.sh` | Buildscript voor Linux/macOS |
| `build_pdf.ps1` | Buildscript voor Windows/PowerShell |
