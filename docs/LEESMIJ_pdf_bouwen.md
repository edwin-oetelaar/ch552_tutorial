# PDF's bouwen uit de Markdown

De Markdown-bestanden (`.md`) zijn de bron. De PDF's worden daaruit gegenereerd
met twee gratis tools:

1. **pandoc** — zet Markdown om naar HTML.
2. **WeasyPrint** — zet die HTML met de stylesheet `scripts/pdf_style.css` om naar PDF.

De build gebruikt ook `scripts/pdf_template.html`. Die template is bewust minimaal,
zodat Pandoc geen eigen browsergerichte default-CSS injecteert. Daardoor blijven
pagina-afmetingen, marges en paginatelling beter reproduceerbaar.

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

De Markdown-bronnen staan in `markdown/`. De scripts staan in `scripts/` en
schrijven de PDF's naar `pdf/`.

**Windows (PowerShell):**

```powershell
.\scripts\build_pdf.ps1                         # alle markdown/*.md
.\scripts\build_pdf.ps1 ch552_architectuur_introductie.md
```

**Linux / macOS:**

```bash
chmod +x scripts/build_pdf.sh                    # eenmalig uitvoerrechten geven
./scripts/build_pdf.sh                           # alle markdown/*.md
./scripts/build_pdf.sh ch552_architectuur_introductie.md
```

Elk `markdown/naam.md` wordt `pdf/naam.pdf`.

## Kleine aanpassingen maken

- **Inhoud** (tekst, tabellen, codeblokken): bewerk de `.md` en bouw opnieuw.
- **Uiterlijk** (lettertype, kleuren, marges, paginanummer): bewerk
  `scripts/pdf_style.css`. Bijvoorbeeld de accentkleur staat op `#0b3d5c` (donkerblauw);
  vervang die overal om een andere huisstijlkleur te krijgen. De paginagrootte
  en marges staan bovenaan in het `@page`-blok.

## Werkt het met plaatjes?

Ja. Verwijs in de Markdown naar een afbeelding met een **relatief pad**:

```markdown
![Blokschema van de 8051](afbeeldingen/blokschema.png)
```

Leg het bestand `blokschema.png` dan in `docs/afbeeldingen/`. Het script geeft
pandoc de juiste zoekmap mee (`--resource-path`), zodat
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
| `markdown/*.md` | Bronteksten (bewerk deze) |
| `pdf/*.pdf` | Gegenereerde PDF's |
| `afbeeldingen/` | Afbeeldingen voor Markdown en PDF |
| `scripts/pdf_style.css` | Opmaak/stijl van de PDF |
| `scripts/pdf_template.html` | Minimale Pandoc-template voor stabiele PDF-layout |
| `scripts/build_pdf.sh` | Buildscript voor Linux/macOS |
| `scripts/build_pdf.ps1` | Buildscript voor Windows/PowerShell |
