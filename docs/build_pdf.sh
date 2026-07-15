#!/usr/bin/env bash
#
# build_pdf.sh — Markdown -> PDF via pandoc + WeasyPrint
#
# Gebruik:
#   ./build_pdf.sh                      # bouwt alle *.md in deze map
#   ./build_pdf.sh mijndoc.md           # bouwt alleen mijndoc.md -> mijndoc.pdf
#   ./build_pdf.sh a.md b.md            # bouwt meerdere bestanden
#
# Vereisten (eenmalig installeren):
#   - pandoc        : https://pandoc.org/installing.html
#   - weasyprint    : pip install weasyprint
#
# De opmaak zit in pdf_style.css (in dezelfde map). Pas die aan om het
# uiterlijk te wijzigen; de Markdown blijft de inhoudsbron.

set -euo pipefail

# Map waar dit script staat -> zo werkt het ook vanuit een andere cwd.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CSS="$SCRIPT_DIR/pdf_style.css"

# Welke bestanden? Argumenten, of anders alle *.md.
if [ "$#" -gt 0 ]; then
  FILES=("$@")
else
  FILES=("$SCRIPT_DIR"/*.md)
fi

for md in "${FILES[@]}"; do
  [ -e "$md" ] || { echo "Overslaan (bestaat niet): $md"; continue; }
  base="${md%.md}"
  html="${base}.tmp.html"
  pdf="${base}.pdf"

  echo "==> $md"
  # 1) Markdown -> HTML (GitHub-flavored markdown, standalone document).
  #    --resource-path zorgt dat plaatjes met relatieve paden gevonden worden.
  pandoc "$md" \
    -f gfm -t html5 -s \
    --metadata title=" " \
    --resource-path="$(dirname "$md")" \
    -o "$html"

  # 2) HTML -> PDF met de stylesheet.
  weasyprint -s "$CSS" "$html" "$pdf"

  rm -f "$html"
  echo "    klaar: $pdf"
done

echo "Alles gebouwd."
