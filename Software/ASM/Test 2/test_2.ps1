Clear-Host
cl65 -t none -C test_2.cfg -o ..\..\..\ROMs\test_2.bin test_2.asm
if($?)
{
    Write-Host "Script assembled" -ForegroundColor Green
}
else
{
    Write-Host "Script not assembled" -ForegroundColor Red
}
