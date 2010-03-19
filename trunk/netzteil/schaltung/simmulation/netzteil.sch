<Qucs Schematic 0.0.15>
<Properties>
  <View=0,-50,1113,800,1,0,22>
  <Grid=10,10,1>
  <DataSet=netzteil.dat>
  <DataDisplay=netzteil.dpl>
  <OpenDisplay=1>
  <showFrame=0>
  <FrameText0=Titel>
  <FrameText1=Gezeichnet von:>
  <FrameText2=Datum:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <Vdc V1 1 150 200 18 -26 0 1 "24 V" 1>
  <Diode D3 1 380 190 -92 -26 0 3 "1e-15 A" 1 "1" 1 "10 fF" 1 "0.5" 0 "0.7 V" 0 "0.5" 0 "0.0 fF" 0 "0.0" 0 "2.0" 0 "0.0 Ohm" 0 "0.0 ps" 0 "0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "16V" 0 "1 mA" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "Zener" 0>
  <Diode D1 1 380 250 15 -26 0 1 "1e-15 A" 1 "1" 1 "10 fF" 1 "0.5" 0 "1 V" 0 "0.5" 0 "0.0 fF" 0 "0.0" 0 "2.0" 0 "0.0 Ohm" 0 "0.0 ps" 0 "0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0" 0 "1 mA" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <Diode D2 1 380 310 15 -26 0 1 "1e-15 A" 1 "1" 1 "10 fF" 1 "0.5" 0 "1 V" 0 "0.5" 0 "0.0 fF" 0 "0.0" 0 "2.0" 0 "0.0 Ohm" 0 "0.0 ps" 0 "0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0" 0 "1 mA" 0 "26.85" 0 "3.0" 0 "1.11" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "1.0" 0 "normal" 0>
  <.DC DC1 1 80 510 0 43 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <R R1 1 350 160 -26 15 0 0 "1000 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe Pr1 1 640 230 28 -31 0 0>
  <_BJT T1 1 760 310 8 -26 0 0 "npn" 1 "1e-16" 1 "1" 1 "1" 0 "0" 0 "0" 0 "0" 1 "0" 0 "0" 0 "1.5" 0 "0" 0 "2" 0 "100" 1 "1" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0.75" 0 "0.33" 0 "0" 0 "0.75" 0 "0.33" 0 "1.0" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT T2 1 750 160 -26 -140 0 1 "npn" 1 "1e-16" 1 "1" 1 "1" 0 "0" 0 "0" 0 "0" 1 "0" 0 "0" 0 "1.5" 0 "0" 0 "2" 0 "100" 1 "1" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0.75" 0 "0.33" 0 "0" 0 "0.75" 0 "0.33" 0 "1.0" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <_BJT T3 1 780 130 -26 -140 0 1 "npn" 1 "1e-16" 1 "1" 1 "1" 0 "0" 0 "0" 0 "0" 1 "0" 0 "0" 0 "1.5" 0 "0" 0 "2" 0 "100" 1 "1" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0.75" 0 "0.33" 0 "0" 0 "0.75" 0 "0.33" 0 "1.0" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <R R6 1 700 460 -26 15 0 0 "50 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R7 1 760 460 -26 15 0 0 "50 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R5 1 730 430 15 -26 0 1 "2.2K" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R4 1 880 340 -26 15 0 0 "1 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R8 1 910 240 15 -26 0 1 "1.5K" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe Pr2 1 1020 150 28 -31 0 0>
  <R R3 1 510 250 15 -26 0 1 "9K" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R2 1 510 190 15 -26 0 1 "1 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
</Components>
<Wires>
  <150 230 150 340 "" 0 0 0 "">
  <150 340 380 340 "" 0 0 0 "">
  <150 160 320 160 "" 0 0 0 "">
  <150 160 150 170 "" 0 0 0 "">
  <380 160 510 160 "" 0 0 0 "">
  <570 160 570 250 "" 0 0 0 "">
  <570 250 630 250 "" 0 0 0 "">
  <650 250 650 340 "" 0 0 0 "">
  <380 340 510 340 "" 0 0 0 "">
  <510 340 650 340 "" 0 0 0 "">
  <510 280 510 340 "" 0 0 0 "">
  <510 220 550 220 "" 0 0 0 "">
  <550 220 550 280 "" 0 0 0 "">
  <550 280 750 280 "" 0 0 0 "">
  <650 340 670 340 "" 0 0 0 "">
  <320 100 320 160 "" 0 0 0 "">
  <320 100 720 100 "" 0 0 0 "">
  <750 100 750 130 "" 0 0 0 "">
  <720 100 750 100 "" 0 0 0 "">
  <720 100 720 160 "" 0 0 0 "">
  <750 280 760 280 "" 0 0 0 "">
  <750 190 750 280 "" 0 0 0 "">
  <760 340 850 340 "" 0 0 0 "">
  <810 130 910 130 "" 0 0 0 "">
  <910 130 910 170 "" 0 0 0 "">
  <910 270 910 340 "" 0 0 0 "">
  <730 310 730 400 "" 0 0 0 "">
  <790 460 970 460 "" 0 0 0 "">
  <970 340 970 400 "" 0 0 0 "">
  <910 340 970 340 "" 0 0 0 "">
  <670 340 760 340 "" 0 0 0 "">
  <670 340 670 460 "" 0 0 0 "">
  <910 170 910 210 "" 0 0 0 "">
  <910 170 1010 170 "" 0 0 0 "">
  <1030 170 1030 400 "" 0 0 0 "">
  <970 400 970 460 "" 0 0 0 "">
  <970 400 1030 400 "" 0 0 0 "">
  <510 160 570 160 "" 0 0 0 "">
  <380 160 380 160 "mess" 410 130 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
