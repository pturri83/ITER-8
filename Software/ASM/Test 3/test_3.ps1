Clear-Host
cl65 -t none -C test_3.cfg -o ..\..\..\ROMs\test_3.bin test_3.asm
if($?)
{
    Write-Host "Script assembled" -ForegroundColor Green
}
else
{
    Write-Host "Script not assembled" -ForegroundColor Red
}
