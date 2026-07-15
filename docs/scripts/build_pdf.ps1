<#
  build_pdf.ps1 — Markdown -> PDF via pandoc + WeasyPrint (Windows / PowerShell)

  Gebruik (in PowerShell):
    .\build_pdf.ps1                 # bouwt alle docs/markdown/*.md
    .\build_pdf.ps1 mijndoc.md      # bouwt docs/markdown/mijndoc.md
    .\build_pdf.ps1 a.md b.md       # meerdere bestanden

  Vereisten (eenmalig installeren):
    - pandoc     : https://pandoc.org/installing.html   (of: winget install --id JohnMacFarlane.Pandoc)
    - weasyprint : pip install weasyprint

  Tip: als PowerShell het uitvoeren van scripts blokkeert, draai eenmalig:
    Set-ExecutionPolicy -Scope CurrentUser RemoteSigned

  De opmaak zit in docs/scripts/pdf_style.css. Markdown blijft de bron;
  gegenereerde PDF's gaan naar docs/pdf/.
#>

param(
  [Parameter(ValueFromRemainingArguments = $true)]
  [string[]]$Files
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$DocsDir = Split-Path -Parent $ScriptDir
$SourceDir = Join-Path $DocsDir "markdown"
$OutputDir = Join-Path $DocsDir "pdf"
$Css = Join-Path $ScriptDir "pdf_style.css"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

if (-not $Files -or $Files.Count -eq 0) {
  $Files = Get-ChildItem -Path $SourceDir -Filter *.md | ForEach-Object { $_.FullName }
} else {
  $Files = $Files | ForEach-Object {
    if (Test-Path $_) { $_ } else { Join-Path $SourceDir $_ }
  }
}

foreach ($md in $Files) {
  if (-not (Test-Path $md)) { Write-Host "Overslaan (bestaat niet): $md"; continue }
  $name = [System.IO.Path]::GetFileNameWithoutExtension($md)
  $html = Join-Path $OutputDir "$name.tmp.html"
  $pdf  = Join-Path $OutputDir "$name.pdf"
  $dir  = Split-Path -Parent $md

  Write-Host "==> $md"
  # 1) Markdown -> HTML
  pandoc $md -f gfm -t html5 -s --metadata title=" " --resource-path="$dir;$DocsDir" -o $html
  # 2) HTML -> PDF met stylesheet
  weasyprint -s $Css $html $pdf
  Remove-Item $html -Force
  Write-Host "    klaar: $pdf"
}

Write-Host "Alles gebouwd."
