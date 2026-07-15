<#
  build_pdf.ps1 — Markdown -> PDF via pandoc + WeasyPrint (Windows / PowerShell)

  Gebruik (in PowerShell, vanuit deze map):
    .\build_pdf.ps1                 # bouwt alle *.md in deze map
    .\build_pdf.ps1 mijndoc.md      # bouwt alleen mijndoc.md -> mijndoc.pdf
    .\build_pdf.ps1 a.md b.md       # meerdere bestanden

  Vereisten (eenmalig installeren):
    - pandoc     : https://pandoc.org/installing.html   (of: winget install --id JohnMacFarlane.Pandoc)
    - weasyprint : pip install weasyprint

  Tip: als PowerShell het uitvoeren van scripts blokkeert, draai eenmalig:
    Set-ExecutionPolicy -Scope CurrentUser RemoteSigned

  De opmaak zit in pdf_style.css (in dezelfde map). Pas die aan voor een ander
  uiterlijk; de Markdown blijft de inhoudsbron.
#>

param(
  [Parameter(ValueFromRemainingArguments = $true)]
  [string[]]$Files
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$Css = Join-Path $ScriptDir "pdf_style.css"

if (-not $Files -or $Files.Count -eq 0) {
  $Files = Get-ChildItem -Path $ScriptDir -Filter *.md | ForEach-Object { $_.FullName }
}

foreach ($md in $Files) {
  if (-not (Test-Path $md)) { Write-Host "Overslaan (bestaat niet): $md"; continue }
  $base = [System.IO.Path]::ChangeExtension($md, $null).TrimEnd('.')
  $html = "$base.tmp.html"
  $pdf  = "$base.pdf"
  $dir  = Split-Path -Parent $md

  Write-Host "==> $md"
  # 1) Markdown -> HTML
  pandoc $md -f gfm -t html5 -s --metadata title=" " --resource-path="$dir" -o $html
  # 2) HTML -> PDF met stylesheet
  weasyprint -s $Css $html $pdf
  Remove-Item $html -Force
  Write-Host "    klaar: $pdf"
}

Write-Host "Alles gebouwd."
