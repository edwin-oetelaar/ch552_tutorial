#!/usr/bin/env bash
#
# build_pdf.sh — Markdown -> PDF via pandoc + WeasyPrint
#
# Gebruik:
#   ./build_pdf.sh                      # bouwt alle docs/markdown/*.md
#   ./build_pdf.sh mijndoc.md           # bouwt docs/markdown/mijndoc.md
#   ./build_pdf.sh a.md b.md            # bouwt meerdere bestanden
#
# Vereisten (eenmalig installeren):
#   - pandoc        : https://pandoc.org/installing.html
#   - weasyprint    : pip install weasyprint
#
# De opmaak zit in docs/scripts/pdf_style.css. Markdown blijft de bron;
# gegenereerde PDF's gaan naar docs/pdf/.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCS_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
SOURCE_DIR="$DOCS_DIR/markdown"
OUTPUT_DIR="$DOCS_DIR/pdf"
CSS="$SCRIPT_DIR/pdf_style.css"
TEMPLATE="$SCRIPT_DIR/pdf_template.html"

mkdir -p "$OUTPUT_DIR"

# Welke bestanden? Argumenten, of anders alle *.md.
if [ "$#" -gt 0 ]; then
  FILES=()
  for arg in "$@"; do
    if [ -e "$arg" ]; then
      FILES+=("$arg")
    else
      FILES+=("$SOURCE_DIR/$arg")
    fi
  done
else
  FILES=("$SOURCE_DIR"/*.md)
fi

for md in "${FILES[@]}"; do
  [ -e "$md" ] || { echo "Overslaan (bestaat niet): $md"; continue; }
  name="$(basename "${md%.md}")"
  html="$OUTPUT_DIR/$name.tmp.html"
  pdf="$OUTPUT_DIR/$name.pdf"

  echo "==> $md"
  # 1) Markdown -> HTML (GitHub-flavored markdown, standalone document).
  #    --resource-path zorgt dat plaatjes met relatieve paden gevonden worden.
  pandoc "$md" \
    -f gfm -t html5 -s \
    --template="$TEMPLATE" \
    --metadata title=" " \
    --resource-path="$(dirname "$md"):$DOCS_DIR" \
    -o "$html"

  # 2) HTML -> PDF met de stylesheet.
  weasyprint -s "$CSS" "$html" "$pdf"

  rm -f "$html"
  echo "    klaar: $pdf"
done

echo "Alles gebouwd."
