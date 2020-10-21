/******************************************************************************



                            Online C Compiler.

                Code, Compile, Run and Debug C program online.

Write your code in this editor and press "Run" button to compile and execute it.



*******************************************************************************/

#include <stdio.h>

#include <math.h>

#include "RES_TO_TEMP.h"

#include "define.h"

struct point_t
{
int temp;
float resistans;
};

 struct point_t mas[136];
    
 struct point_t temp_struct0= {-30,122.291496};
 struct point_t temp_struct1= {-29,115.625999};
 struct point_t temp_struct2= {-28,109.380798};
 struct point_t temp_struct3= {-27,103.525703};
 struct point_t temp_struct4= {-26,98.033096};
 struct point_t temp_struct5= {-25,92.877602};
 struct point_t temp_struct6= {-24,88.035698};
 struct point_t temp_struct7= {-23,83.485603};
 struct point_t temp_struct8= {-22,79.207497};
 struct point_t temp_struct9= {-21,75.182899};
 struct point_t temp_struct10= {-20,71.394699};
 struct point_t temp_struct11= {-19,67.827103};
 struct point_t temp_struct12= {-18,64.465698};
 struct point_t temp_struct13= {-17,61.296799};
 struct point_t temp_struct14= {-16,58.307899};
 struct point_t temp_struct15= {-15,55.487400};
 struct point_t temp_struct16= {-14,52.824600};
 struct point_t temp_struct17= {-13,50.309502};
 struct point_t temp_struct18= {-12,47.932701};
 struct point_t temp_struct19= {-11,45.685600};
 struct point_t temp_struct20= {-10,43.560101};
 struct point_t temp_struct21= {-9,41.548901};
 struct point_t temp_struct22= {-8,39.644901};
 struct point_t temp_struct23= {-7,37.841599};
 struct point_t temp_struct24= {-6,36.133099};
 struct point_t temp_struct25= {-5,34.513500};
 struct point_t temp_struct26= {-4,32.977798};
 struct point_t temp_struct27= {-3,31.520901};
 struct point_t temp_struct28= {-2,30.138300};
 struct point_t temp_struct29= {-1,28.825701};
 struct point_t temp_struct30= {0,27.579000};
 struct point_t temp_struct31= {1,26.394501};
 struct point_t temp_struct32= {2,25.268600};
 struct point_t temp_struct33= {3,24.198200};
 struct point_t temp_struct34= {4,23.180000};
 struct point_t temp_struct35= {5,22.211300};
 struct point_t temp_struct36= {6,21.289101};
 struct point_t temp_struct37= {7,20.411200};
 struct point_t temp_struct38= {8,19.574900};
 struct point_t temp_struct39= {9,18.778099};
 struct point_t temp_struct40= {10,18.018700};
 struct point_t temp_struct41= {11,17.294701};
 struct point_t temp_struct42= {12,16.604200};
 struct point_t temp_struct43= {13,15.945400};
 struct point_t temp_struct44= {14,15.316800};
 struct point_t temp_struct45= {15,14.716600};
 struct point_t temp_struct46= {16,14.143600};
 struct point_t temp_struct47= {17,13.596200};
 struct point_t temp_struct48= {18,13.073300};
 struct point_t temp_struct49= {19,12.573400};
 struct point_t temp_struct50= {20,12.095600};
 struct point_t temp_struct51= {21,11.638700};
 struct point_t temp_struct52= {22,11.201600};
 struct point_t temp_struct53= {23,10.783400};
 struct point_t temp_struct54= {24,10.383100};
 struct point_t temp_struct55= {25,10.000000};
 struct point_t temp_struct56= {26,9.633100};
 struct point_t temp_struct57= {27,9.281700};
 struct point_t temp_struct58= {28,8.945000};
 struct point_t temp_struct59= {29,8.522400};
 struct point_t temp_struct60= {30,8.313200};
 struct point_t temp_struct61= {31,8.016700};
 struct point_t temp_struct62= {32,7.732300};
 struct point_t temp_struct63= {33,7.459600};
 struct point_t temp_struct64= {34,7.197900};
 struct point_t temp_struct65= {35,6.946800};
 struct point_t temp_struct66= {36,6.705700};
 struct point_t temp_struct67= {37,6.474200};
 struct point_t temp_struct68= {38,6.251900};
 struct point_t temp_struct69= {39,6.038400};
 struct point_t temp_struct70= {40,5.833300};
 struct point_t temp_struct71= {41,5.636200};
 struct point_t temp_struct72= {42,5.446700};
 struct point_t temp_struct73= {43,5.264500};
 struct point_t temp_struct74= {44,5.089400};
 struct point_t temp_struct75= {45,4.920900};
 struct point_t temp_struct76= {46,4.758800};
 struct point_t temp_struct77= {47,4.502900};
 struct point_t temp_struct78= {48,4.452800};
 struct point_t temp_struct79= {49,4.308400};
 struct point_t temp_struct80= {50,4.169300};
 struct point_t temp_struct81= {51,4.035400};
 struct point_t temp_struct82= {52,3.906400};
 struct point_t temp_struct83= {53,3.782200};
 struct point_t temp_struct84= {54,3.662500};
 struct point_t temp_struct85= {55,3.547100};
 struct point_t temp_struct86= {56,3.435900};
 struct point_t temp_struct87= {57,3.328700};
 struct point_t temp_struct88= {58,3.225400};
 struct point_t temp_struct89= {59,3.125700};
 struct point_t temp_struct90= {60,3.029600};
 struct point_t temp_struct91= {61,2.936900};
 struct point_t temp_struct92= {62,2.847400};
 struct point_t temp_struct93= {63,2.761000};
 struct point_t temp_struct94= {64,2.677700};
 struct point_t temp_struct95= {65,2.597200};
 struct point_t temp_struct96= {66,2.519600};
 struct point_t temp_struct97= {67,2.444600};
 struct point_t temp_struct98= {68,2.372100};
 struct point_t temp_struct99= {69,2.302100};
 struct point_t temp_struct100= {70,2.234500};
 struct point_t temp_struct101= {71,2.169200};
 struct point_t temp_struct102= {72,2.106000};
 struct point_t temp_struct103= {73,2.045000};
 struct point_t temp_struct104= {74,1.986000};
 struct point_t temp_struct105= {75,1.929000};
 struct point_t temp_struct106= {76,1.873800};
 struct point_t temp_struct107= {77,1.820400};
 struct point_t temp_struct108= {78,1.768800};
 struct point_t temp_struct109= {79,1.718900};
 struct point_t temp_struct110= {80,1.670600};
 struct point_t temp_struct111= {81,1.623800};
 struct point_t temp_struct112= {82,1.578600};
 struct point_t temp_struct113= {83,1.534800};
 struct point_t temp_struct114= {84,1.492400};
 struct point_t temp_struct115= {85,1.451300};
 struct point_t temp_struct116= {86,1.411500};
 struct point_t temp_struct117= {87,1.373000};
 struct point_t temp_struct118= {88,1.335700};
 struct point_t temp_struct119= {89,1.299600};
 struct point_t temp_struct120= {90,1.264600};
 struct point_t temp_struct121= {91,1.230600};
 struct point_t temp_struct122= {92,1.197700};
 struct point_t temp_struct123= {93,1.165900};
 struct point_t temp_struct124= {94,1.134900};
 struct point_t temp_struct125= {95,1.105000};
 struct point_t temp_struct126= {96,1.075900};
 struct point_t temp_struct127= {97,1.047800};
 struct point_t temp_struct128= {98,1.020400};
 struct point_t temp_struct129= {99,0.993900};
 struct point_t temp_struct130= {100,0.968200};
 struct point_t temp_struct131= {101,0.943200};
 struct point_t temp_struct132= {102,0.919000};
 struct point_t temp_struct133= {103,0.895500};
 struct point_t temp_struct134= {104,0.872700};
 struct point_t temp_struct135= {105,0.850600};
 


void init_temp()
{    
  mas[0] =temp_struct0; 
  mas[1] =temp_struct1; 
  mas[2] =temp_struct2; 
  mas[3] =temp_struct3; 
  mas[4] =temp_struct4; 
  mas[5] =temp_struct5; 
  mas[6] =temp_struct6; 
  mas[7] =temp_struct7; 
  mas[8] =temp_struct8; 
  mas[9] =temp_struct9; 
  mas[10] =temp_struct10; 
  mas[11] =temp_struct11; 
  mas[12] =temp_struct12; 
  mas[13] =temp_struct13; 
  mas[14] =temp_struct14; 
  mas[15] =temp_struct15; 
  mas[16] =temp_struct16; 
  mas[17] =temp_struct17; 
  mas[18] =temp_struct18; 
  mas[19] =temp_struct19; 
  mas[20] =temp_struct20; 
  mas[21] =temp_struct21; 
  mas[22] =temp_struct22; 
  mas[23] =temp_struct23; 
  mas[24] =temp_struct24; 
  mas[25] =temp_struct25; 
  mas[26] =temp_struct26; 
  mas[27] =temp_struct27; 
  mas[28] =temp_struct28; 
  mas[29] =temp_struct29; 
  mas[30] =temp_struct30; 
  mas[31] =temp_struct31; 
  mas[32] =temp_struct32; 
  mas[33] =temp_struct33; 
  mas[34] =temp_struct34; 
  mas[35] =temp_struct35; 
  mas[36] =temp_struct36; 
  mas[37] =temp_struct37; 
  mas[38] =temp_struct38; 
  mas[39] =temp_struct39; 
  mas[40] =temp_struct40; 
  mas[41] =temp_struct41; 
  mas[42] =temp_struct42; 
  mas[43] =temp_struct43; 
  mas[44] =temp_struct44; 
  mas[45] =temp_struct45; 
  mas[46] =temp_struct46; 
  mas[47] =temp_struct47; 
  mas[48] =temp_struct48; 
  mas[49] =temp_struct49; 
  mas[50] =temp_struct50; 
  mas[51] =temp_struct51; 
  mas[52] =temp_struct52; 
  mas[53] =temp_struct53; 
  mas[54] =temp_struct54; 
  mas[55] =temp_struct55; 
  mas[56] =temp_struct56; 
  mas[57] =temp_struct57; 
  mas[58] =temp_struct58; 
  mas[59] =temp_struct59; 
  mas[60] =temp_struct60; 
  mas[61] =temp_struct61; 
  mas[62] =temp_struct62; 
  mas[63] =temp_struct63; 
  mas[64] =temp_struct64; 
  mas[65] =temp_struct65; 
  mas[66] =temp_struct66; 
  mas[67] =temp_struct67; 
  mas[68] =temp_struct68; 
  mas[69] =temp_struct69; 
  mas[70] =temp_struct70; 
  mas[71] =temp_struct71; 
  mas[72] =temp_struct72; 
  mas[73] =temp_struct73; 
  mas[74] =temp_struct74; 
  mas[75] =temp_struct75; 
  mas[76] =temp_struct76; 
  mas[77] =temp_struct77; 
  mas[78] =temp_struct78; 
  mas[79] =temp_struct79; 
  mas[80] =temp_struct80; 
  mas[81] =temp_struct81; 
  mas[82] =temp_struct82; 
  mas[83] =temp_struct83; 
  mas[84] =temp_struct84; 
  mas[85] =temp_struct85; 
  mas[86] =temp_struct86; 
  mas[87] =temp_struct87; 
  mas[88] =temp_struct88; 
  mas[89] =temp_struct89; 
  mas[90] =temp_struct90; 
  mas[91] =temp_struct91; 
  mas[92] =temp_struct92; 
  mas[93] =temp_struct93; 
  mas[94] =temp_struct94; 
  mas[95] =temp_struct95; 
  mas[96] =temp_struct96; 
  mas[97] =temp_struct97; 
  mas[98] =temp_struct98; 
  mas[99] =temp_struct99; 
  mas[100] =temp_struct100; 
  mas[101] =temp_struct101; 
  mas[102] =temp_struct102; 
  mas[103] =temp_struct103; 
  mas[104] =temp_struct104; 
  mas[105] =temp_struct105; 
  mas[106] =temp_struct106; 
  mas[107] =temp_struct107; 
  mas[108] =temp_struct108; 
  mas[109] =temp_struct109; 
  mas[110] =temp_struct110; 
  mas[111] =temp_struct111; 
  mas[112] =temp_struct112; 
  mas[113] =temp_struct113; 
  mas[114] =temp_struct114; 
  mas[115] =temp_struct115; 
  mas[116] =temp_struct116; 
  mas[117] =temp_struct117; 
  mas[118] =temp_struct118; 
  mas[119] =temp_struct119; 
  mas[120] =temp_struct120; 
  mas[121] =temp_struct121; 
  mas[122] =temp_struct122; 
  mas[123] =temp_struct123; 
  mas[124] =temp_struct124; 
  mas[125] =temp_struct125; 
  mas[126] =temp_struct126; 
  mas[127] =temp_struct127; 
  mas[128] =temp_struct128; 
  mas[129] =temp_struct129; 
  mas[130] =temp_struct130; 
  mas[131] =temp_struct131; 
  mas[132] =temp_struct132; 
  mas[133] =temp_struct133; 
  mas[134] =temp_struct134; 
  mas[135] =temp_struct135; 
}
      
int get_temp(float num)
{
  float current_val = num / 1000;
  float pogresh = current_val / 100 * 2.5;
  int i;
  printf("\npogresh 2.5% = %f",pogresh);

  for (i = 0; i < sizeof(mas); i++)
  {
    if ((mas[i].resistans - current_val < 0.00001) ||
        (mas[i].resistans < (current_val + pogresh) &&
         mas[i].resistans > (current_val - pogresh))) {
      return mas[i].temp;
    }
  }
}

