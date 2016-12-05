# Seki
> Prostě sekačka co seká sama

<img src="https://github.com/731mat/Seki-ardumower/blob/master/documentation/photo/20160316_193008.jpg" alt="Motory ze stahovací oken Seat Toledo" align="left" height="100" width="100" />
<img src="https://github.com/731mat/Seki-ardumower/blob/master/documentation/photo/20160318_170407.jpg" alt="Zátěžové testy" align="left" height="100" width="100" />
<img src="https://github.com/731mat/Seki-ardumower/blob/master/documentation/photo/20160830_115834.jpg" alt="Namontování regulátorů pro motoryy" align="left" height="100" width="100" />
<img src="https://github.com/731mat/Seki/blob/master/documentation/photo/IMG_20161126_120355.jpg" alt="Namontování regulátorů pro motoryy" align="left" height="100" width="180" />
<img src="https://github.com/731mat/Seki/blob/master/documentation/photo/IMG_20161126_232140.jpg" height="100" width="180" />
<br>
## Představa
Moje vize je vyrobit stroj(pohybem tank)  tak aby jezdil po místnosti bez jakéhokoliv nárazu. Díky přesným ultrazvukovým senzorům bude rozpoznávat vzdálenost od objektu který je předním. Při chybě a nárazu zareágují nárazové čidla které jsou připojeny na přerušení procesoru, aby nedošlo k velkému náporu na motory a následnému odpálení regulárotů. Seki jde ovládat před bluetooth pomocí android aplikace, výpis dat má přes esp8266 který vysílá wifi.

## Project site
http://www.mat.8u.cz


## Čeho bych chtěl dosáhnout
- [x] Zakomponovat regulátory IBT-2 do projektu seki
- [x] Razhýbat motory s vymyslet jednoduchý algoritmus na pohyb
- [x] Přidát více senzorů pro lepčí pohyb v prostředí (nározové čidla, více ultrazvukových senzorů)
- [x] Přidat manuální ovládání přes bluetooth
- [ ] Výpis přes esp8266 který bude dělat AP, webServer
- [ ] Zjistit jak funguje triangulace
- [ ] aplikace triangulace
- [ ] zapracvání triangulace do projektu SEKI
