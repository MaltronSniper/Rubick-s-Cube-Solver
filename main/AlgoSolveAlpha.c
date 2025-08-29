#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <esp_random.h>

char T[3][3] = {{'W','W','W'},{'W','W','W'},{'W','W','W'}};
char F[3][3] = {{'R','R','R'},{'R','R','R'},{'R','R','R'}};
char D[3][3] = {{'Y','Y','Y'},{'Y','Y','Y'},{'Y','Y','Y'}};
char B[3][3] = {{'O','O','O'},{'O','O','O'},{'O','O','O'}};
char R[3][3] = {{'B','B','B'},{'B','B','B'},{'B','B','B'}};
char L[3][3] = {{'G','G','G'},{'G','G','G'},{'G','G','G'}};

char To[3][3] = {{'W','W','W'},{'W','W','W'},{'W','W','W'}};
char Fo[3][3] = {{'R','R','R'},{'R','R','R'},{'R','R','R'}};
char Do[3][3] = {{'Y','Y','Y'},{'Y','Y','Y'},{'Y','Y','Y'}};
char Bo[3][3] = {{'O','O','O'},{'O','O','O'},{'O','O','O'}};
char Ro[3][3] = {{'B','B','B'},{'B','B','B'},{'B','B','B'}};
char Lo[3][3] = {{'G','G','G'},{'G','G','G'},{'G','G','G'}};

char (*rubick[6])[3][3] = {&T,&D,&R,&F,&L,&B};

char MovVal[10] = {'R','r','L','l','F','f','B','b','D','d'};

int stepcnt[16];
char validity[16] = {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'};
bool ActualDo[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

char steps[300];
int take = 0;
bool skip = false;
int iter = 0;
int shift = 0;

char way[250], way2[250];
int filler = -1, filler2 = -1;
int cont = 0;

// Cube Related Displays
void DisplayMoves_Count()
{
    for(int o = 0;o < take;o++)
    {
        printf("%c ", steps[o]);
    }
    printf("\n %d \n", take);
}

void DisplayCube()
{
    // Display Bottom
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 6;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",D[i][j]);
        }
        
        printf("\n");
    }

    printf("\n");
    // Display Back

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 6;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",B[i][j]);
        }
        
        printf("\n");
    }
printf("\n");
    // Dispaly D L T W

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            //printf("%c  ",D[i][j]);
            printf("   ");
        }
        
        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",L[i][j]);
        }

        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",T[i][j]);
        }
        
        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",R[i][j]);
        }

        printf("\n");
    }
printf("\n");
    // Display F

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 6;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("%c  ",F[i][j]);
        }
        
        printf("\n");
    }

}

void DisplayCubeValue()
{
    // Display Bottom
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 12;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("D[%d][%d]  ",i,j);
        }
        
        printf("\n");
    }

    printf("\n");
    // Display Back

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 12;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("B[%d][%d]  ",i,j);
        }
        
        printf("\n");
    }
printf("\n");
    // Dispaly D L T W

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            printf("   ");
        }
        
        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("L[%d][%d]  ",i,j);
        }

        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("T[%d][%d]  ",i,j);
        }
        
        printf(" ");

        for(int j = 0;j < 3;j++)
        {
            printf("R[%d][%d]  ",i,j);
        }

        printf("\n");
    }
printf("\n");
    // Display F

    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 12;j++)
        {
            printf("   ");
        }
        
        printf("  ");

        for(int j = 0;j < 3;j++)
        {
            printf("F[%d][%d]  ",i,j);
        }
        
        printf("\n");
    }

}

//  Filter Out Repeats and Anti Moves
void Multi(char taker, char replacer)
{
    filler2 = filler;
    for(int i = 0;i <= filler2;i++)
    {
        way2[i] = way[i];
        way[i] = ' ';
    }
    filler = -1;
    cont = 0;

    for(int i = 0;i <= filler2;i++)
    {
        if(way2[i] == taker)
        {
            cont++;
        }
        
        if(way2[i] != taker)
        {
            if(cont == 4)
            {
                filler++;
                way[filler] = way2[i]; 
                filler = filler;
            }
            else if(cont == 3)
            {   
                filler++;
                way[filler] = replacer; 
                filler++;
                way[filler] = way2[i];
            }
            else if(cont == 2)
            {
                filler++;
                way[filler] = taker; 
                filler++;
                way[filler] = taker; 
                filler++;
                way[filler] = way2[i];                
            }
            else if(cont == 1)
            {   
                filler++;
                way[filler] = taker; 
                filler++;
                way[filler] = way2[i];
            }
            else
            {
                filler++;
                way[filler] = way2[i];
            }

            cont = 0;
        }
    }

}

void AntiRepeatRefine(char taker, char replacer)
{
    filler2 = filler;
    for(int i = 0;i <= filler2;i++)
    {
        way2[i] = way[i];
        way[i] = ' ';
    }
    filler = 0;
    cont = 0;
    
    way[0] = way2[0];

    for(int i = 1;i <= filler2;i++)
    {
        if(way2[i] == taker && way2[i-1] == replacer)
        {
            filler--;
        }
        else
        {
            filler++;
            way[filler] = way2[i];
        }
    }

}

void Refine()
{
    for(int i = 0;i <= take;i++)
    {
        if(steps[i] == 'D')
        {
            cont++;
        }
        
        if(steps[i] != 'D' || steps[i] == '\0')
        {
            if(cont == 4)
            {
                filler++;
                way[filler] = steps[i]; 
                filler = filler;
            }
            else if(cont == 3)
            {   
                filler++;
                way[filler] = 'd'; 
                filler++;
                way[filler] = steps[i];
            }
            else if(cont == 2)
            {
                filler++;
                way[filler] = 'D'; 
                filler++;
                way[filler] = 'D'; 
                filler++;
                way[filler] = steps[i];                
            }
            else if(cont == 1)
            {   
                filler++;
                way[filler] = 'D'; 
                filler++;
                way[filler] = steps[i];
            }
            else
            {
                filler++;
                way[filler] = steps[i];
            }

            cont = 0;
        }
    }

    Multi('F','f');
    Multi('B','b');
    Multi('R','r');
    Multi('L','l');

    
    for(int i = 0;i < 5;i++)
    {
    AntiRepeatRefine('F','f');
    AntiRepeatRefine('f','F');
    AntiRepeatRefine('D','d');
    AntiRepeatRefine('d','D');
    AntiRepeatRefine('B','b');
    AntiRepeatRefine('b','B');
    AntiRepeatRefine('R','r');
    AntiRepeatRefine('r','R');
    AntiRepeatRefine('L','l');
    AntiRepeatRefine('l','L');
    }

}

// Cube Chnages
void Manipulate(char in)
{
    char ts1[3], ts2[3];
    char temp[3][3];
    // Hold the cube White Up and Red Pointing to wards you

    // R is right towards you
    if(in == 'R')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = B[i][2]; // Ts1 O
            B[i][2] = T[i][2]; // O -> W
            ts2[i] = D[i][2]; // ts2 Y 
            D[i][2] = ts1[i]; // Y - > 0
            ts1[i] = F[i][2]; // Ts1 R
            F[i][2] = ts2[i]; // R -> Y
            T[i][2] = ts1[i];

            for(int j = 0; j < 3;j++)
            {
                temp[j][2-i] = R[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                R[i][j] = temp[i][j];
            }
        }
    }
    // r is right away from you
    if(in == 'r')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = F[i][2]; 
            F[i][2] = T[i][2];
            ts2[i] = D[i][2]; 
            D[i][2] = ts1[i]; 
            ts1[i] = B[i][2]; 
            B[i][2] = ts2[i]; 
            T[i][2] = ts1[i];
        
            for(int j = 0; j < 3;j++)
            {
                temp[2-j][i] = R[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                R[i][j] = temp[i][j];
            }
        }
    }
    // L is left towards you
    if(in == 'L')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = B[i][0]; // Ts1 O
            B[i][0] = T[i][0]; // O -> W
            ts2[i] = D[i][0]; // ts2 Y 
            D[i][0] = ts1[i]; // Y - > 0
            ts1[i] = F[i][0]; // Ts1 R
            F[i][0] = ts2[i]; // R -> Y
            T[i][0] = ts1[i];
        
            for(int j = 0; j < 3;j++)
            {
                temp[2-j][i] = L[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                L[i][j] = temp[i][j];
            }
        }
    }
    // l is left away from you
    if(in == 'l')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = F[i][0]; 
            F[i][0] = T[i][0];
            ts2[i] = D[i][0]; 
            D[i][0] = ts1[i]; 
            ts1[i] = B[i][0]; 
            B[i][0] = ts2[i]; 
            T[i][0] = ts1[i];
        
        for(int j = 0; j < 3;j++)
            {
                temp[j][2-i] = L[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                L[i][j] = temp[i][j];
            }
        }
    }
    // F is towards right
    if(in == 'F')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = R[2][i]; // Ts1 O
            R[2][i] = T[2][i]; // O -> W
            ts2[i] = D[0][2-i]; // ts2 Y 
            D[0][2-i] = ts1[i]; // Y - > 0
            ts1[i] = L[2][i]; // Ts1 R
            L[2][i] = ts2[i];
            T[2][i] = ts1[i];

            for(int j = 0; j < 3;j++)
            {
                temp[j][2-i] = F[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                F[i][j] = temp[i][j];
            }
        }
    }
    // f is towards left
    if(in == 'f')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = L[2][i]; // Ts1 O
            L[2][i] = T[2][i]; // O -> W
            ts2[i] = D[0][2-i]; // ts2 Y 
            D[0][2-i] = ts1[i]; // Y - > 0
            ts1[i] = R[2][i]; // Ts1 R
            R[2][i] = ts2[i];
            T[2][i] = ts1[i];

            for(int j = 0; j < 3;j++)
            {
                temp[2-j][i] = F[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                F[i][j] = temp[i][j];
            }
        }
    }
    // B is towards right
    if(in == 'B')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = R[0][i]; // Ts1 O
            R[0][i] = T[0][i]; // O -> W
            ts2[i] = D[2][2-i]; // ts2 Y 
            D[2][2-i] = ts1[i]; // Y - > 0
            ts1[i] = L[0][i]; // Ts1 R
            L[0][i] = ts2[i];
            T[0][i] = ts1[i];

            for(int j = 0; j < 3;j++)
            {
                temp[2-j][i] = B[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                B[i][j] = temp[i][j];
            }
        }
    }
    // b is towards left
    if(in == 'b')
    {
        for(int i = 0;i < 3;i++)
        {
            ts1[i] = L[0][i]; // Ts1 O
            L[0][i] = T[0][i]; // O -> W
            ts2[i] = D[2][2-i]; // ts2 Y 
            D[2][2-i] = ts1[i]; // Y - > 0
            ts1[i] = R[0][i]; // Ts1 R
            R[0][i] = ts2[i];
            T[0][i] = ts1[i];

            for(int j = 0; j < 3;j++)
            {
                temp[j][2-i] = B[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                B[i][j] = temp[i][j];
            }
        }
    }
    // D is towards left
    if(in == 'D')
    {
        char ts3[3], ts4[3], ts5[3],ts6[3];
        for(int i = 0;i < 3;i++)
        {     
            ts6[i] = B[0][i];
            ts3[i] = R[i][2]; // Ts1 O
            ts4[i] = F[2][i]; // ts2 Y 
            ts5[i] = L[i][0]; // Ts1 R
        }
        for(int i = 0;i < 3;i++)
        {           
            R[i][2] = ts6[i]; // O -> W
            F[2][2-i] = ts3[i]; // Y - > 0
            L[i][0] = ts4[i];
            B[0][2-i] = ts5[i];
            
            for(int j = 0; j < 3;j++)
            {
                temp[2-j][i] = D[i][j];
            }
        }
        
        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                D[i][j] = temp[i][j];
            }
        }
    }
    // d is towards right
    if(in == 'd')
    {
        char ts3[3], ts4[3], ts5[3],ts6[3];
        for(int i = 0;i < 3;i++)
        {     
            ts6[i] = B[0][i];
            ts3[i] = L[i][0]; // Ts1 O
            ts4[i] = F[2][i]; // ts2 Y 
            ts5[i] = R[i][2]; // Ts1 R
        }
        for(int i = 0;i < 3;i++)
        {           
            L[2-i][0] = ts6[i]; // O -> W
            F[2][i] = ts3[i]; // Y - > 0
            R[2-i][2] = ts4[i];
            B[0][i] = ts5[i];
            
            for(int j = 0; j < 3;j++)
            {
                temp[j][2-i] = D[i][j];
            }
        }

        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                D[i][j] = temp[i][j];
            }
        }
    }
 
}

void Engage(char t)
{
    Manipulate(t);
    steps[take] = t;
    take++;   
}

// Solving Cubes

void WhitePlus()
{   
    int slv = 0;
    // Solve the White Plus : Move Count [Min, Max] = 
    while(1)
        {
            // Down Face 'W' Presence
            while(1)
            {

            if(slv == 4 || (D[0][1] != 'W' && D[1][0] != 'W' && D[1][2] != 'W' && D[2][1] != 'W'))
                break;

            if(D[0][1] == 'W' && F[2][1] == F[1][1])
            {
                Manipulate('F');
                Manipulate('F');
                steps[take] = 'F';
                take++;    
                steps[take] = 'F';
                take++;
                slv++;    
                break;
            }

            if(D[1][0] == 'W' && L[1][0] == L[1][1])
            {
                Manipulate('L');
                Manipulate('L');
                steps[take] = 'L';
                take++;    
                steps[take] = 'L';
                take++;    
                slv++;
                break;
            }

            if(D[1][2] == 'W' && R[1][2] == R[1][1])
            {
                Manipulate('R');
                Manipulate('R');
                steps[take] = 'R';
                take++;    
                steps[take] = 'R';
                take++;    
                slv++;
                break;
            }

            if(D[2][1] == 'W' && B[0][1] == B[1][1])
            {
                Manipulate('B');
                Manipulate('B');
                steps[take] = 'B';
                take++;    
                steps[take] = 'B';
                take++;    
                slv++;
                break;
            }

            Manipulate('D');
            steps[take] = 'D';
            take++;
            
            }
            
            if(T[1][1] == 'W' && T[0][1] == 'W' && T[1][0] == 'W' && T[1][2] == 'W' && T[2][1] == 'W')
            {
                break;
            }

            // 3rd Layer 'W' Presence
            if(B[0][1] == 'W')
            {
               Engage('B');
               Engage('L');
               Engage('d');
               Engage('l');
               Engage('b');
               continue;
            }

            if(R[1][2] == 'W')
            {
                Engage('r');
                Engage('B');
                Engage('d');
                Engage('b');
                Engage('R');
                continue;
            }

            if(F[2][1] == 'W')
            {
                Engage('f');
                Engage('r');
                Engage('d');
                Engage('R');
                Engage('F');
                continue;
            }
            
            if(L[1][0] == 'W')
            {
               Engage('L');
               Engage('f');
               Engage('d');
               Engage('F');
               Engage('l');
               continue;
            }
            
            // Middle Layer 'W' Presence
            if(R[0][1] == 'W')
            {
                Engage('B');
                Engage('D');
                Engage('b');
                continue;
            }

            if(B[1][2] == 'W')
            {
                Engage('R');
                Engage('D');
                Engage('r');
                continue;
            }

            if(L[0][1] == 'W')
            {
                Engage('b');
                Engage('D');
                Engage('B');
                continue;
            }

            if(B[1][0] == 'W')
            {
                Engage('L');
                Engage('D');
                Engage('l');
                continue;           
            }

            if(R[2][1] == 'W')
            {
                Engage('F');
                Engage('D');
                Engage('f');
                continue;
            }
            
            if(F[1][2] == 'W')
            {
                Engage('r');
                Engage('D');
                Engage('R');
                continue;
            }

            if(L[2][1] == 'W')
            {
                Engage('f');
                Engage('D');
                Engage('F');
                continue;
            }

            if(F[1][0] == 'W')
            {
                Engage('l');
                Engage('D');
                Engage('L');
                continue;
            }
            
            // Top Face && First Layer 
            if(R[1][0] == 'W')
            {
                Engage('R');
                Engage('B');
                Engage('d');
                Engage('b');
                Engage('r');               
                continue;
            }

            if(T[1][2] == 'W' && R[1][0] != R[1][1])
            {
                Manipulate('R');
                steps[take] = 'R';
                take++;
                Manipulate('R');
                steps[take] = 'R';
                take++;
                Manipulate('D');
                steps[take] = 'D';
                take++;
                Manipulate('r');
                steps[take] = 'r';
                take++;
                Manipulate('r');
                steps[take] = 'r';
                take++;
                continue;
            }

            if(B[2][1] == 'W')
            {
                Engage('B');
                Engage('R');
                Engage('D');
                Engage('r');  
                Engage('b');  
                continue;
            }

            if(T[0][1] == 'W' && B[2][1] != B[1][1])
            {
                Manipulate('B');
                steps[take] = 'B';
                take++;
                Manipulate('B');
                steps[take] = 'B';
                take++;
                Manipulate('D');
                steps[take] = 'D';
                take++;
                Manipulate('b');
                steps[take] = 'b';
                take++;
                Manipulate('b');
                steps[take] = 'b';
                take++;
                continue;
            }

            if(L[1][2] == 'W')
            {
                Engage('L');
                Engage('b');
                Engage('D');
                Engage('B');
                Engage('l');     
                continue;
            }

            if(T[1][0] == 'W' && L[1][2] != L[1][1])
            {
                Manipulate('L');
                steps[take] = 'L';
                take++;
                Manipulate('L');
                steps[take] = 'L';
                take++;
                Manipulate('D');
                steps[take] = 'D';
                take++;
                Manipulate('l');
                steps[take] = 'l';
                take++;
                Manipulate('l');
                steps[take] = 'l';
                take++;
                continue;
            }

            if(F[0][1] == 'W')
            {
                Engage('F');
                Engage('r');
                Engage('d');
                Engage('R');
                Engage('f');
                continue;
            }

            if(T[2][1] == 'W' && F[0][1] != F[1][1])
            {
                Manipulate('F');
                steps[take] = 'F';
                take++;
                Manipulate('F');
                steps[take] = 'F';
                take++;
                Manipulate('D');
                steps[take] = 'D';
                take++;
                Manipulate('f');
                steps[take] = 'f';
                take++;
                Manipulate('f');
                steps[take] = 'f';
                take++;
                continue;
            }

            //Manipulate('D');
            //steps[take] = 'D';
            //take++;

            if(take > 290)
            {
                printf("1 Failed\n");
                validity[iter] = 'F';
                break;
            }
        }
            
}
// Optimized TBMA
void WhitePlusOptimized()
{   
    int slv = 0;
    // Solve the White Plus : Move Count [Min, Max] = 
    while(1)
        {

            if((T[1][1] == 'W' && T[0][1] == 'W' && T[1][0] == 'W' && T[1][2] == 'W' && T[2][1] == 'W'))// || slv == 4)
            {
                break;
            }

            // Down Face 'W' Presence
            while(true)
            {

                if(D[0][1] == 'W' && F[2][1] == F[1][1])
                {
                    Engage('F');
                    Engage('F');       
                    slv++;       
                    //printf("9\n");
                }

                if(D[1][0] == 'W' && L[1][0] == L[1][1])
                {
                    Engage('L');
                    Engage('L');
                    slv++;
                    //printf("10\n");
                }

                if(D[1][2] == 'W' && R[1][2] == R[1][1])
                {
                    Engage('R');
                    Engage('R');
                    slv++;
                    //printf("11\n");
                }

                if(D[2][1] == 'W' && B[0][1] == B[1][1])
                {
                    Engage('B');
                    Engage('B');
                    slv++;
                   //printf("12\n");
                }
            
                if(slv == 4 || (D[0][1] != 'W' && D[1][0] != 'W' && D[1][2] != 'W' && D[2][1] != 'W'))
                {
                    //printf("L1 : %d %d\n",slv, take);
                    break;
                }

                Engage('D');    
            }
           
            // 3rd Layer 'W' Presence
            while(true)
            {

                if(B[0][1] == 'W')
                {
                    if(D[2][1] == R[1][1])
                    {
                        Engage('b');
                        Engage('r');
                        Engage('B');
                    slv++;
                    //printf("13\n");
                    }

                    if(D[2][1] == L[1][1])
                    {
                        Engage('B');
                        Engage('l');
                        Engage('b');
                     slv++;   
                     //printf("14\n");
                    }
                }

                else if(R[1][2] == 'W')
                {
                    if(D[1][2] == F[1][1])
                    {
                        Engage('R');
                        Engage('f');
                        Engage('r');
                        slv++;
                        //printf("15\n");
                    }

                    if(D[1][2] == B[1][1])
                    {
                        Engage('r');
                        Engage('b');
                        Engage('R');
                        slv++;
                        //printf("16\n");
                    }
                    
                }

                else if(F[2][1] == 'W')
                {
                    if(D[0][1] == R[1][1])
                    {
                        Engage('f');
                        Engage('R');
                        Engage('F');
                        slv++;
                        //printf("17\n");
                    }

                    if(D[0][1] == L[1][1])
                    {
                        Engage('F');
                        Engage('L');
                        Engage('f');
                        slv++;
                        //printf("18\n");
                    }
                    
                }
                
                else if(L[1][0] == 'W')
                {
                    if(D[1][0] == B[1][1])
                    {
                        Engage('l');
                        Engage('B');
                        Engage('L');
                        slv++;
                        //printf("19\n");
                    }

                    if(D[1][0] == F[1][1])
                    {
                        Engage('L');
                        Engage('F');
                        Engage('l');
                        slv++;
                        //printf("20\n");
                    }
                }
                
                if(slv == 4 || (F[2][1] != 'W' && B[0][1] != 'W' && R[1][2] != 'W' && L[1][0] != 'W'))
                {
                    //printf("L2 : %d %d\n",slv, take);
                    break;
                }

                Engage('D');    
            }

            // Middle Layer 'W' Presence
            if(B[1][2] == 'W' || R[0][1] == 'W')
            {
                Engage('B');
                //printf("1\n");
            }
            
            else if(B[1][0] == 'W' || L[0][1] == 'W')
            {
                Engage('b');
               //printf("2\n");
                
            }

            else if(F[1][0] == 'W' || L[2][1] == 'W')
            {
                Engage('f');
                //printf("3\n");
            }

            else if(F[1][2] == 'W' || R[2][1] == 'W')
            {
                Engage('F');
                //printf("4\n");
            }

            // Top Face && First Layer 
            else if(F[0][1] == 'W' || (T[2][1] == 'W' && F[0][1] != F[1][1]))
            {
                Engage('F');
                Engage('F');
                //printf("5\n");
            }

            else if(R[1][0] == 'W' || (T[1][2] == 'W' && R[1][0] != R[1][1]))
            {
                Engage('R');
                Engage('R');
                //printf("6\n");   
            }

            else if(B[2][1] == 'W' || (T[0][1] == 'W' && B[2][1] != B[1][1]))
            {
                Engage('B');
                Engage('B');
                //printf("7\n");
            }

            else if(L[1][2] == 'W' || (T[1][0] == 'W' && L[1][2] != L[1][1]))
            {
                Engage('L');
                Engage('L');
                //printf("8\n");
            }
            

            if(take > 290)
            {
                printf("1 Failed\n");
                validity[iter] = 'F';
                break;
            }
        }
            
}


void WhiteFace()
{
    while(1)
    {   
     
        // Convert Botton 'W' to last Layer 'W'
        if(D[0][0] == 'W' && T[2][0] != 'W')
        {
            Manipulate('f');
            steps[take] = 'f';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('F');
            steps[take] = 'F';
            take++;
        }
        
        if(D[0][2] == 'W' && T[2][2] != 'W')
        {
            Manipulate('F');
            steps[take] = 'F';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('f');
            steps[take] = 'f';
            take++;
        }
        
        if(D[2][0] == 'W' && T[0][0] != 'W')
        {
            Manipulate('b');
            steps[take] = 'b';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('B');
            steps[take] = 'B';
            take++;
        }

        if(D[2][2] == 'W' && T[0][2] != 'W')
        {
            Manipulate('B');
            steps[take] = 'B';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('b');
            steps[take] = 'b';
            take++;
        }
    
        // Convert Layer 1 into bottom or last Face
     
        if(R[2][0] == 'W')
        {
            Manipulate('F');
            steps[take] = 'F';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('f');
            steps[take] = 'f';
            take++;
        }

        if(R[0][0] == 'W')
        {
            Manipulate('B');
            steps[take] = 'B';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('b');
            steps[take] = 'b';
            take++;
        }
        
        if(L[0][2] == 'W')
        {
            Manipulate('b');
            steps[take] = 'b';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('B');
            steps[take] = 'B';
            take++;
        }
    
        if(L[2][2] == 'W')
        {
            Manipulate('f');
            steps[take] = 'f';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('F');
            steps[take] = 'F';
            take++;
        }
    
        if(F[0][0] == 'W')
        {
            Manipulate('l');
            steps[take] = 'l';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('L');
            steps[take] = 'L';
            take++;
        }
    
        if(F[0][2] == 'W')
        {
            Manipulate('r');
            steps[take] = 'r';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('R');
            steps[take] = 'R';
            take++;
        }
    
        if(B[2][0] == 'W')
        {
            Manipulate('L');
            steps[take] = 'L';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('l');
            steps[take] = 'l';
            take++;
        }
    
        if(B[2][2] == 'W')
        {
            Manipulate('R');
            steps[take] = 'R';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('r');
            steps[take] = 'r';
            take++;
        }
    
        // Set The last layer 'W' to the correct Top Face Spot
        // Right of face
     
        if(R[2][2] == 'W' && D[0][2] == F[1][1])
        {
            Manipulate('l');
            steps[take] = 'l';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('L');
            steps[take] = 'L';
            take++;
        
        }
    
        if(F[2][0] == 'W' && D[0][0] == L[1][1])
        {
            Manipulate('b');
            steps[take] = 'b';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('B');
            steps[take] = 'B';
            take++;
        }
    
        if(L[0][0] == 'W' && D[2][0] == B[1][1])
        {
            Manipulate('R');
            steps[take] = 'R';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('r');
            steps[take] = 'r';
            take++;
        
        }
    
        if(B[0][2] == 'W' && D[2][2] == R[1][1])
        {
            Manipulate('F');
            steps[take] = 'F';
            take++;
            Manipulate('D');
            steps[take] = 'D';
            take++;
            Manipulate('f');
            steps[take] = 'f';
            take++;
        
        }
    
        // Left of face
     
        if(R[0][2] == 'W' && D[2][2] == B[1][1])
        {
            Manipulate('L');
            steps[take] = 'L';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('l');
            steps[take] = 'l';
            take++;
        }
    
        if(F[2][2] == 'W' && D[0][2] == R[1][1])
        {
            Manipulate('B');
            steps[take] = 'B';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('b');
            steps[take] = 'b';
            take++;
        }
    
        if(L[2][0] == 'W' && D[0][0] == F[1][1])
        {
            Manipulate('r');
            steps[take] = 'r';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('R');
            steps[take] = 'R';
            take++;
        
        }
    
        if(B[0][0] == 'W' && D[2][0] == L[1][1])
        {
            Manipulate('f');
            steps[take] = 'f';
            take++;
            Manipulate('d');
            steps[take] = 'd';
            take++;
            Manipulate('F');
            steps[take] = 'F';
            take++;
        
        }

        // Top Shift

        if(T[0][0] == 'W' && B[2][0] != B[1][1])
        {
            Engage('b');
            Engage('d');
            Engage('B');
        }

        if(T[0][2] == 'W' && B[2][2] != B[1][1])
        {
            Engage('B');
            Engage('D');
            Engage('b');
        }

        if(T[2][0] == 'W' && F[0][0] != F[1][1])
        {
            Engage('f');
            Engage('D');
            Engage('F');
        }

        if(T[2][2] == 'W' && F[0][2] != F[1][1])
        {
            Engage('F');
            Engage('d');
            Engage('f');
        }

        Manipulate('D');
        steps[take] = 'D';
        take++;

        if(T[0][0] == 'W' && T[0][2] == 'W' && T[2][0] == 'W' && T[2][2] == 'W')
        {
            break;
        }

        if(take > 290)
        {
            printf("2 Failed\n");
            validity[iter] = 'F';
            break;
        }

    }
}
// Optimized TBMA
void WhiteFaceOptimized()
{
    while(1)
    {   
        // Last Layer Setter
        while(true)
        {

            if(F[2][2] != 'W' && R[0][2] != 'W' && B[0][0] != 'W' && L[2][0] != 'W' && F[2][0] != 'W' && R[2][2] != 'W' && B[0][2] != 'W' && L[0][0] != 'W')
            {
                break;
            }

            // Right Setter
            if(F[2][2] == 'W' && D[0][2] == R[1][1])
            {
                Engage('B'); Engage('d'); Engage('b');
            }
            if(R[0][2] == 'W' && D[2][2] == B[1][1])
            {
                Engage('L'); Engage('d'); Engage('l');
            }
            if(B[0][0] == 'W' && D[2][0] == L[1][1])
            {
                Engage('f'); Engage('d'); Engage('F');
            }
            if(L[2][0] == 'W' && D[0][0] == F[1][1])
            {
                Engage('r'); Engage('d'); Engage('R');
            }
            
            // Left Setter
            if(F[2][0] == 'W' && D[0][0] == L[1][1])
            {
                Engage('b'); Engage('D'); Engage('B');
            }
            if(L[0][0] == 'W' && D[2][0] == B[1][1])
            {
                Engage('R'); Engage('D'); Engage('r');
            }
            if(B[0][2] == 'W' && D[2][2] == R[1][1])
            {
                Engage('F'); Engage('D'); Engage('f');
            }
            if(R[2][2] == 'W' && D[0][2] == F[1][1])
            {
                Engage('l'); Engage('D'); Engage('L');
            }
   
            Engage('D');
        }

        // Bottom White Shifter
        if(D[0][0] == 'W' && (T[2][0] != 'W' || (T[2][0] == 'W' && F[0][1] != F[0][0])))
        {
            Engage('l');Engage('D');Engage('L');
            continue;
        }
        else if(D[2][0] == 'W' && (T[0][0] != 'W' || (T[0][0] == 'W' && L[0][2] != L[1][2])))
        {
            Engage('b');Engage('D');Engage('B');
            continue;
        }
        else if(D[2][2] == 'W' && (T[0][2] != 'W' || (T[0][2] == 'W' && R[0][0] != R[1][0])))
        {
            Engage('B');Engage('d');Engage('b');
            continue;
        }
        else if(D[0][2] == 'W' && (T[2][2] != 'W' || (T[2][2] == 'W' && F[0][1] != F[0][2])))
        {
            Engage('F');Engage('D');Engage('f');
            continue;
        }

        // Top unfitter Shifter
        if((T[0][0] == 'W' && L[0][2] != L[1][2]) || (L[0][2] == 'W' || B[0][2] == 'W'))
        {
            Engage('b');Engage('d');Engage('B');
            continue;
        }
        else if((T[0][2] == 'W' && R[0][0] != R[1][0]) || (R[0][0] == 'W' || B[2][2] == 'W'))
        {
            Engage('B');Engage('D');Engage('b');
            continue;
        }
        else if((T[2][0] == 'W' && F[0][1] != F[0][0]) || (F[0][0] == 'W' || L[2][2] == 'W'))
        {
            Engage('f');Engage('D');Engage('F'); 
            continue;
        }
        else if((T[2][2] == 'W' && F[0][1] != F[0][2]) || (F[0][2] == 'W' || R[2][0] == 'W'))
        {
            Engage('F');Engage('d');Engage('f');
            continue;
        }

        Engage('D');

        if(T[0][0] == 'W' && T[0][2] == 'W' && T[2][0] == 'W' && T[2][2] == 'W' && F[0][0] == F[0][1] && F[0][2] == F[0][1] && B[2][0] == B[2][1] && B[2][1] == B[2][2])
        {
            break;
        }

        if(take > 290)
        {
            printf("2 Failed\n");
            validity[iter] = 'F';
            break;
        }

    }
}


void MiddleLayer()
{     
        while(1)
        {
            if(D[0][1] == 'Y' || F[2][1] == 'Y')
            {
                if(F[1][0] != 'Y' && L[2][1] != 'Y')
                {
                    Engage('d');
                    Engage('l');
                    Engage('D');
                    Engage('L');
                    Engage('D');
                    Engage('f');
                    Engage('d');
                    Engage('F');
                }
            
                if(F[1][2] != 'Y' && R[2][1] != 'Y')
                {
                    Engage('D');
                    Engage('r');
                    Engage('d');
                    Engage('R');
                    Engage('d');
                    Engage('F');
                    Engage('D');
                    Engage('f');
                }
            }

            if(D[2][1] == 'Y' || B[0][1] == 'Y')
            {
                if(B[1][0] != 'Y' && L[0][1] != 'Y')
                {
                    Engage('D');
                    Engage('L');
                    Engage('d');
                    Engage('l');
                    Engage('d');
                    Engage('b');
                    Engage('D');
                    Engage('B');
                }
            
                if(B[1][2] != 'Y' && R[0][1] != 'Y')
                {
                    Engage('d');
                    Engage('R');
                    Engage('D');
                    Engage('r');
                    Engage('D');
                    Engage('B');
                    Engage('d');
                    Engage('b');
                }
            }

            Engage('D');

            if((F[1][0] == 'Y' || L[2][1] == 'Y') && (F[1][2] == 'Y' || R[2][1] == 'Y') && (B[1][0] == 'Y' || L[0][1] == 'Y') && (B[1][2] == 'Y' || R[0][1] == 'Y'))
            {
                //printf("YES\n");
                break;
            }
        
            if(take > 290)
            {
                printf("3-1 Failed\n");
                validity[iter] = 'F';
                break;
            }
        }

        while(1)
        {
            shift = 0;
            if(D[0][1] != 'Y' && F[2][1] != 'Y')
            {
                if(F[1][1] == F[2][1])
                {
                    Manipulate('F');
                    if(L[1][1] == L[2][1])
                    {
                        shift = 1;
                    }

                    Manipulate('f');
                    Manipulate('f');
                    
                    if(R[1][1] == R[2][1])
                    {
                        shift = -1;
                    }

                    Manipulate('F');

                    if(shift == 1)
                    {
                        Engage('d');
                        Engage('l');
                        Engage('D');
                        Engage('L');
                        Engage('D');
                        Engage('f');
                        Engage('d');
                        Engage('F');
                    }

                    if(shift == -1)
                    {
                        Engage('D');
                        Engage('r');
                        Engage('d');
                        Engage('R');
                        Engage('d');
                        Engage('F');
                        Engage('D');
                        Engage('f');
                    }
                }
            }
         
            shift = 0;
            if(D[1][2] != 'Y' && R[1][2] != 'Y')
            {
                if(R[1][2] == R[1][1])
                {
                    Manipulate('R');
                    if(F[1][1] == F[1][2])
                    {
                        shift = 1;
                    }

                    Manipulate('r');
                    Manipulate('r');
                    
                    if(B[1][1] == B[1][2])
                    {
                        shift = -1;
                    }
                    
                    Manipulate('R');

                    if(shift == 1)
                    {
                        Engage('d');
                        Engage('F');
                        Engage('D');
                        Engage('f');
                        Engage('D');
                        Engage('r');
                        Engage('d');
                        Engage('R');
                    }

                    if(shift == -1)
                    {
                        Engage('D');
                        Engage('B');
                        Engage('d');
                        Engage('b');
                        Engage('d');
                        Engage('R');
                        Engage('D');
                        Engage('r');
                    }
                }
            }
         
            shift = 0;
            if(D[1][0] != 'Y' && L[1][0] != 'Y')
            {
                if(L[1][0] == L[1][1])
                {
                    Manipulate('L');
                    if(F[1][1] == F[1][0])
                    {
                        shift = 1;
                    }

                    Manipulate('l');
                    Manipulate('l');
                    
                    if(B[1][1] == B[1][0])
                    {
                        shift = -1;
                    }
                    
                    Manipulate('L');

                    if(shift == -1)
                    {
                        Engage('d');
                        Engage('b');
                        Engage('D');
                        Engage('B');
                        Engage('D');
                        Engage('L');
                        Engage('d');
                        Engage('l');
                    }

                    if(shift == 1)
                    {
                        Engage('D');
                        Engage('f');
                        Engage('d');
                        Engage('F');
                        Engage('d');
                        Engage('l');
                        Engage('D');
                        Engage('L');
                    }
                }
            }
         
            shift = 0;
            if(D[2][1] != 'Y' && B[0][1] != 'Y')
            {
                if(B[0][1] == B[1][1])
                {
                    Manipulate('B');
                    if(L[1][1] == L[0][1])
                    {
                        shift = 1;
                    }

                    Manipulate('b');
                    Manipulate('b');
                    
                    if(R[1][1] == R[0][1])
                    {
                        shift = -1;
                    }

                    Manipulate('B');

                    if(shift == -1)
                    {
                        Engage('d');
                        Engage('R');
                        Engage('D');
                        Engage('r');
                        Engage('D');
                        Engage('B');
                        Engage('d');
                        Engage('b');
                    }

                    if(shift == 1)
                    {
                        Engage('D');
                        Engage('L');
                        Engage('d');
                        Engage('l');
                        Engage('d');
                        Engage('b');
                        Engage('D');
                        Engage('B');
                    }
                }
            }

            Engage('D');
            
            if((F[1][1] == F[1][2] && R[2][1] == R[1][1] && F[1][1] == F[1][0] && L[2][1] == L[1][1] && B[1][1] == B[1][0] && L[0][1] == L[1][1] && B[1][1] == B[1][2] && R[0][1] == R[1][1]) && (T[0][0] == 'W' && T[0][1] == 'W' && T[0][2] == 'W' && T[1][0] == 'W' && T[1][1] == 'W' && T[1][2] == 'W' && T[2][0] == 'W' && T[2][1] == 'W' && T[2][2] == 'W'))
            {
                break;
            }

            if(take > 290)
            {
                printf("3-2 Failed\n");
                validity[iter] = 'F';
                break;
            }
        }
}
// Optimized TBMA
void MiddleLayerOptimized()
{     
        while(1)
        {

            if(R[1][2] == F[1][1] && D[1][2] == L[1][1])
            {
                Engage('l');Engage('D');Engage('L');Engage('D');Engage('f');Engage('d');Engage('F');
                //printf("1\n");
            }

            if(L[1][0] == F[1][1] && D[1][0] == R[1][1])
            {
                Engage('r');Engage('d');Engage('R');Engage('d');Engage('F');Engage('D');Engage('f');
                //printf("2\n");
            }

            if(F[2][1] == R[1][1] && D[0][1] == B[1][1])
            {
                Engage('B');Engage('d');Engage('b');Engage('d');Engage('R');Engage('D');Engage('r');
                //printf("3\n");
            }

            if(B[0][1] == R[1][1] && D[2][1] == F[1][1])
            {
                Engage('F');Engage('D');Engage('f');Engage('D');Engage('r');Engage('d');Engage('R');
                //printf("4\n");
            }

            if(F[2][1] == L[1][1] && D[0][1] == B[1][1])
            {
                Engage('b');Engage('D');Engage('B');Engage('D');Engage('L');Engage('d');Engage('l');
                //printf("5\n");
            }

            if(B[0][1] == L[1][1] && D[2][1] == F[1][1])
            {
                Engage('B');Engage('d');Engage('b');Engage('d');Engage('R');Engage('D');Engage('r');
                //printf("6\n");
            }

            if(R[1][2] == B[1][1] && D[1][2] == L[1][1])
            {
                Engage('L');Engage('d');Engage('l');Engage('d');Engage('b');Engage('D');Engage('B');
                //printf("7\n");
            }

            if(L[1][0] == B[1][1] && D[1][0] == R[1][1])
            {
                Engage('R');Engage('D');Engage('r');Engage('D');Engage('B');Engage('d');Engage('b');
                //printf("8\n");
            }

            if((F[2][1] == 'Y' || D[0][1] == 'Y') && (R[1][2] == 'Y' || D[1][2] == 'Y') && (L[1][2] == 'Y' || D[1][0] == 'Y') && (B[0][1] == 'Y' || D[2][1] == 'Y') )
            {
                if(F[1][1] != F[1][0])
                {
                    Engage('l');Engage('D');Engage('L');Engage('D');Engage('f');Engage('d');Engage('F');
                    //printf("9\n");
                }   

                if(F[1][1] != F[1][2])
                {
                    Engage('r');Engage('d');Engage('R');Engage('d');Engage('F');Engage('D');Engage('f');
                    //printf("10\n");
                }

                if(B[1][1] != B[1][0])
                {
                    Engage('L');Engage('d');Engage('l');Engage('d');Engage('b');Engage('D');Engage('B');
                    //printf("11\n");
                }

                if(B[1][1] != B[1][2])
                {
                    Engage('R');Engage('D');Engage('r');Engage('D');Engage('B');Engage('d');Engage('b');
                    //printf("12\n");
                }
            }

            Engage('D');
            //printf("D\n");

            if((F[1][1] == F[1][2] && R[2][1] == R[1][1] && F[1][1] == F[1][0] && L[2][1] == L[1][1] && B[1][1] == B[1][0] && L[0][1] == L[1][1] && B[1][1] == B[1][2] && R[0][1] == R[1][1]) && (T[0][0] == 'W' && T[0][1] == 'W' && T[0][2] == 'W' && T[1][0] == 'W' && T[1][1] == 'W' && T[1][2] == 'W' && T[2][0] == 'W' && T[2][1] == 'W' && T[2][2] == 'W'))
            {
                break;
            }

            if(take > 290)
            {
                printf("3-2 Failed\n");
                validity[iter] = 'F';
                break;
            }
        }
}


void BottomFace()
{
    while(1)
    {   

        if(D[0][0] == 'Y' && D[0][1] == 'Y' && D[0][2] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][0] == 'Y' && D[2][1] == 'Y' && D[2][2] == 'Y')
        {
            break;
        }

    // Plus & 2
    if(D[2][2] == 'Y' && D[2][1] == 'Y' && D[2][0] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[0][1] == 'Y')
        {
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('d');
            Engage('l');
            Engage('d');
            Engage('d');
            Engage('L');
        }

    // Plus & 1
    if(D[0][1] == 'Y' && D[0][2] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && D[2][2] != 'Y')
        {
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('d');
            Engage('l');
            Engage('d');
            Engage('d');
            Engage('L');
        }

    // Plus
    if(D[0][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && R[2][2] == 'Y' && D[0][0] != 'Y' && D[0][2] != 'Y' && D[2][0] != 'Y' && D[2][2] != 'Y' )
        {
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('d');
            Engage('l');
            Engage('d');
            Engage('d');
            Engage('L');
        }

    // ---
    if(D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] != 'Y')
    {
        Engage('F');
        Engage('l');
        Engage('d');
        Engage('L');
        Engage('D');
        Engage('f');
    }
    
    // L
    if(D[2][1] == 'Y' && D[1][2] == 'Y' && D[1][1] == 'Y' && D[0][1] != 'Y' && D[1][0] != 'Y')
    {
        Engage('F');
        Engage('l');
        Engage('d');
        Engage('L');
        Engage('D');
        Engage('f');
    }

    // .
    if(D[1][1] == 'Y' && D[0][1] != 'Y' && D[1][0] != 'Y' && D[1][2] != 'Y'  && D[2][1] != 'Y') //&& D[0][0] != 'Y'  && D[0][2] != 'Y' && D[2][0] != 'Y' && D[2][2] != 'Y')
        {
            Engage('F');
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('D');
            Engage('f');
        }
        
    Engage('D');

    if(take > 290)
    {
        printf("4 Failed\n");
        validity[iter] = 'F';
        break;
    }

    }

}
// Optimized TBMA
void BottomFaceOptimized()
{
    while(1)
    {   

        if(D[0][0] == 'Y' && D[0][1] == 'Y' && D[0][2] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][0] == 'Y' && D[2][1] == 'Y' && D[2][2] == 'Y')
        {
            break;
        }

    // Plus & 2 On Diagonals
    if(D[2][2] == 'Y' && D[2][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[0][1] == 'Y' && D[0][0] == 'Y' && F[2][2] == 'Y' && L[0][0] == 'Y')
        {
            Engage('L');
            Engage('F');
            Engage('l');
            Engage('B');
            Engage('L');
            Engage('f');
            Engage('l');
            Engage('b');
        }
    
    // Plus & 2 on Line & Divergence
    if(D[2][0] == 'Y' && D[2][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[0][1] == 'Y' && D[0][0] == 'Y' && F[2][2] == 'Y' && B[0][2] == 'Y')
        {
            Engage('R');
            Engage('F');
            Engage('L');
            Engage('f');
            Engage('r');
            Engage('F');
            Engage('l');
            Engage('f');
        }

    // Plus & 2 on Line & Parallel (Axis Limitation)
    if(D[2][0] == 'Y' && D[2][1] == 'Y' && D[2][2] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[0][1] == 'Y' && F[2][2] == 'Y' && F[2][0] == 'Y')
        {
            Engage('b');
            Engage('d');
            Engage('R');
            Engage('D');
            Engage('r');
            Engage('B');
        }

    // Plus & 1 Sune
    if(D[0][1] == 'Y' && D[0][2] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && D[2][2] != 'Y' && D[0][0] != 'Y' && D[2][0] != 'Y' && F[2][0] == 'Y')
        {
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('d');
            Engage('l');
            Engage('d');
            Engage('d');
            Engage('L');
        }

    // Plus & 1 Anti - Sune
    if(D[0][0] == 'Y' && D[0][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && D[2][2] != 'Y' && D[2][0] != 'Y' && D[0][2] != 'Y' && F[2][2] == 'Y')
        {
            Engage('r');
            Engage('D');
            Engage('R');
            Engage('D');
            Engage('r');
            Engage('D');
            Engage('D');
            Engage('R');
        }

    // Plus Parallel Normals
    if(D[0][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && F[2][0] == 'Y' && F[2][2] == 'Y' && B[0][0] == 'Y' && B[0][2] == 'Y')
        {
            Engage('F');

            Engage('l');
            Engage('d');
            Engage('L');
            Engage('D');

            Engage('l');
            Engage('d');
            Engage('L');
            Engage('D');

            Engage('l');
            Engage('d');
            Engage('L');
            Engage('D');

            Engage('f');
        }
    
    // Plus Parallel & Divergent
    if(D[0][1] == 'Y' && D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] == 'Y' && F[2][0] == 'Y' && R[2][2] == 'Y' && R[0][2] == 'Y' && B[0][0] == 'Y')
    {
            Engage('l');
            Engage('D');
            Engage('D');
            Engage('l');
            Engage('l');
            Engage('D');
            Engage('l');
            Engage('l');
            Engage('D');
            Engage('l');
            Engage('l');
            Engage('d');
            Engage('d');
            Engage('l');
    }

    // ---
    if(D[1][0] == 'Y' && D[1][1] == 'Y' && D[1][2] == 'Y' && D[2][1] != 'Y')
    {
        Engage('F');
        Engage('l');
        Engage('d');
        Engage('L');
        Engage('D');
        Engage('f');
        //DisplayCube();
    }
    
    // L
    if(D[0][1] == 'Y' && D[1][1] == 'Y' && D[1][0] == 'Y' && D[1][2] != 'Y')
    {
        Engage('b');
        Engage('d');
        Engage('R');
        Engage('D');
        Engage('r');
        Engage('B');
    }

    // .
    if(D[1][1] == 'Y' && D[0][1] != 'Y' && D[1][0] != 'Y' && D[1][2] != 'Y'  && D[2][1] != 'Y') //&& D[0][0] != 'Y'  && D[0][2] != 'Y' && D[2][0] != 'Y' && D[2][2] != 'Y')
        {
            Engage('F');
            Engage('l');
            Engage('d');
            Engage('L');
            Engage('D');
            Engage('f');

            Engage('b');
            Engage('d');
            Engage('R');
            Engage('D');
            Engage('r');
            Engage('B');
        }
        
    Engage('D');

    if(take > 290)
    {
        printf("4 Failed\n");
        validity[iter] = 'F';
        break;
    }

    }

}

// TBMA
void LastLayer()
{
    while(1)
    {
        if(take > 290)
        {
            printf("5 Failed\n");
            validity[iter] = 'F';
            break;
        }

        if(B[0][0] == B[0][1] && B[0][2] == B[0][1] && B[0][1] == B[1][1] && F[2][0] == F[2][1] && F[2][1] == F[2][2] && F[2][1] == F[1][1]) // Match all three on F and B with Centers to exit the While
        {
            break;
        }


        if(B[0][0] == B[0][1] && B[0][2] == B[0][1] && F[2][0] == F[2][1] && F[2][1] == F[2][2]) // Match of all three on all 4 faces, but not in correct Orientation
        {
            
            Engage('D');
            continue;
        }

        if((B[0][0] == B[0][1] && B[0][2] == B[0][1]) && (F[2][0] != F[2][2] && R[0][2] != R[2][2] && L[0][0] != L[2][0])) // If Back has all 3 but other face side layers are uneven
        {
            Engage('L');
            Engage('F');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('l');
            Engage('f');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('L');
            Engage('L');
            continue;
        }

        if((R[0][2] == R[1][2] && R[2][2] == R[1][2]) || (L[0][0] == L[1][0] && L[2][0] == L[1][0]) || (F[2][2] == F[2][1] && F[2][0] == F[2][1])) // If any of the last 3 are matched, spin to take to the Back
        {
            Engage('D');
            continue;
        }


        if(B[0][0] == B[0][1] && B[0][2] == B[0][1]) // Back All Three
        {
            if(F[2][1] == L[1][1] || F[2][1] == B[1][1] || F[1][1] == F[2][1])
            {
                Engage('F');
                Engage('F');
                Engage('d');
                Engage('R');
                Engage('L');
                Engage('F');
                Engage('F');
                Engage('r');
                Engage('l');
                Engage('d');
                Engage('F');
                Engage('F');
                continue;
            }

            if(F[2][1] == R[1][1])
            {
                Engage('F');
                Engage('F');
                Engage('d');
                Engage('R');
                Engage('L');
                Engage('F');
                Engage('F');
                Engage('r');
                Engage('l');
                Engage('d');
                Engage('F');
                Engage('F');
                continue;
            }
        }
        
        if(F[2][0] == F[1][0] && F[2][2] == F[1][2] && R[0][2] == R[0][1] && R[2][2] == R[2][1] && L[0][0] == L[0][1] && L[2][0] == L[2][1] && B[0][0] == B[1][0] && B[0][2] == B[1][2]) // Matched sides uneven Middle
        {
            if(F[2][1] == L[1][1] || F[2][1] == B[1][1] || F[1][1] == F[2][1])
            {
                Engage('F');
                Engage('F');
                Engage('d');
                Engage('R');
                Engage('L');
                Engage('F');
                Engage('F');
                Engage('r');
                Engage('l');
                Engage('d');
                Engage('F');
                Engage('F');
                continue;
            }

            if(F[2][1] == R[1][1])
            {
                Engage('F');
                Engage('F');
                Engage('D');
                Engage('R');
                Engage('L');
                Engage('F');
                Engage('F');
                Engage('r');
                Engage('l');
                Engage('D');
                Engage('F');
                Engage('F');
                continue;
            }
        }

         if(F[2][0] == F[2][2] && B[0][0] == B[0][2] && R[0][2] == R[2][2] && L[0][0] == L[2][0]) // Unmatches sides and Middle
        {
            Engage('D');
            continue;
        }

        if(F[2][0] != F[2][2] && R[0][2] != R[2][2] && L[0][0] != L[2][0] && B[0][0] != B[0][2]) // ALl uneven
        {
            Engage('L');
            Engage('F');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('l');
            Engage('f');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('L');
            Engage('L');
            continue;
        }
        
        if(B[0][0] == B[0][2] && F[2][0] != F[2][2] && R[0][2] != R[2][2] && L[0][0] != L[2][0]) // Back matched sides and uneven Middle and other three are uneven
        {
            Engage('L');
            Engage('F');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('l');
            Engage('f');
            Engage('L');
            Engage('B');
            Engage('B');
            Engage('L');
            Engage('L');
            continue;
        }
    
        Engage('D');
    }
}

char act[20];

uint32_t avg1 = 0;
uint32_t avg2 = 0;
uint32_t avg3 = 0;
uint64_t avg4 = 0;
uint64_t avg5 = 0;

//char follower[10] = {'R','R','B','R','b','R','f','F','r','R'};
char follower[10] = {'B','b','d','F','f','f','R','R','L','R'};

// For only onw type of Criteria for follower
void StaticShow(bool patten[])
{
    printf("\n");

    for(int t = 0;t < 10;t++)
    {
        Manipulate(follower[t]);
        act[t] = follower[t];
        printf("%c ",follower[t]);
    }

    printf("   %d %d %d %d   ",patten[0],patten[1],patten[2],patten[3]);

    // Creates the Plus
    if(patten[0] == 0)
        WhitePlusOptimized();
    else if(patten[0] == 1)
        WhitePlus();
    DisplayMoves_Count();

    // Completes the Top Face and First Layer
    if(patten[1] == 0)
        WhiteFaceOptimized();
    else if(patten[1] == 1)
        WhiteFace();
    DisplayMoves_Count();

    // Completes the second Layer
    if(patten[2] == 0)
        MiddleLayerOptimized();
    else if(patten[2] == 1)
        MiddleLayer();
    DisplayMoves_Count();

    // Completes the Bottom Face
    if(patten[3] == 0)
        BottomFaceOptimized();
    else if(patten[3] == 1)
        BottomFace();
    DisplayMoves_Count();

    // Completes The last Layer
    LastLayer();
    DisplayMoves_Count();

    Refine();
     
    printf("\n");
    DisplayCube();

    for(int i = 0;i <= filler;i++)
    {
        printf("%c ", way[i]);
    }

    printf("%d\n",filler);
}

// For all Criteria using elements of follower
void SwarmStatic(bool patten[])
{
    printf("\n");

    for(int t = 0;t < 10;t++)
    {
        Manipulate(follower[t]);
        act[t] = follower[t];
        printf("%c ",follower[t]);
    }
    
/*
    for(int t = 0;t < 10;t++)
    {
        int rand = esp_random() % 10;
        Manipulate(MovVal[rand]);
        act[t] = MovVal[rand];
        printf("'%c',",MovVal[rand]);
    }
*/

    //DisplayCube();

    printf("   %d %d %d %d   ",patten[0],patten[1],patten[2],patten[3]);

    // Creates the Plus
    if(patten[0] == 0)
        WhitePlusOptimized();
    else if(patten[0] == 1)
        WhitePlus();

    // Completes the Top Face and First Layer
    if(patten[1] == 0)
        WhiteFaceOptimized();
    else if(patten[1] == 1)
        WhiteFace();
    
    // Completes the second Layer
    if(patten[2] == 0)
        MiddleLayerOptimized();
    else if(patten[2] == 1)
        MiddleLayer();
    
    // Completes the Bottom Face
    if(patten[3] == 0)
        BottomFaceOptimized();
    else if(patten[3] == 1)
        BottomFace();
 
    // Completes The last Layer
    LastLayer();

    Refine();

    printf("%d\n",filler);

    // Wipes Out

    for(int x = 0;x < 3;x++)
    {
        for(int z = 0;z < 3;z++)
        {
            F[x][z] = Fo[x][z];
            T[x][z] = To[x][z];
            R[x][z] = Ro[x][z];
            L[x][z] = Lo[x][z];
            B[x][z] = Bo[x][z];
            D[x][z] = Do[x][z];
        }
    }

    memset(way,'\0',250);
    memset(way2,'\0',250);
    memset(act,'\0',20);
    memset(steps,'\0',300);
    filler = -1; filler2 = -1;
    take = 0;
    skip = false;
    iter = 0;
    shift = 0;

}

int counter = 0;

// For All Criteria in Random Shuffle
void SwarmDynamic(bool patten[])
{
    printf("\n");
    
    if(counter == 0)
    {
        for(int t = 0;t < 10;t++)
        {
            int rand = esp_random() % 10;
            Manipulate(MovVal[rand]);
            act[t] = MovVal[rand];
            follower[t] = MovVal[rand];
            printf("'%c',",MovVal[rand]);
        }
    }
    else
    {
        for(int t = 0;t < 10;t++)
        {
            Manipulate(follower[t]);
            act[t] = follower[t];
            printf("'%c'",follower[t]);
        }
    }
    counter++;
    //DisplayCube();

    printf("   %d %d %d %d   ",patten[0],patten[1],patten[2],patten[3]);

    // Creates the Plus
    if(patten[0] == 0)
        WhitePlusOptimized();
    else if(patten[0] == 1)
        WhitePlus();

    // Completes the Top Face and First Layer
    if(patten[1] == 0)
        WhiteFaceOptimized();
    else if(patten[1] == 1)
        WhiteFace();
    
    // Completes the second Layer
    if(patten[2] == 0)
        MiddleLayerOptimized();
    else if(patten[2] == 1)
        MiddleLayer();
    
    // Completes the Bottom Face
    if(patten[3] == 0)
        BottomFaceOptimized();
    else if(patten[3] == 1)
        BottomFace();
 
    // Completes The last Layer
    LastLayer();

    Refine();

    printf("%d\n",filler);

    stepcnt[iter] = filler;

    // Wipes Out

    for(int x = 0;x < 3;x++)
    {
        for(int z = 0;z < 3;z++)
        {
            F[x][z] = Fo[x][z];
            T[x][z] = To[x][z];
            R[x][z] = Ro[x][z];
            L[x][z] = Lo[x][z];
            B[x][z] = Bo[x][z];
            D[x][z] = Do[x][z];
        }
    }

    for(int i = 0;i < 10;i++)
    {
        Manipulate(follower[i]);
    }

    for(int i = 0;i < filler;i++)
    {
        Manipulate(way[i]);
    }

    bool leaver = false;

    for(int x = 0;x < 3;x++)
    {
        for(int z = 0;z < 3;z++)
        {
            if(F[x][z] == Fo[x][z] && T[x][z] == To[x][z] && R[x][z] == Ro[x][z] && L[x][z] == Lo[x][z] && B[x][z] == Bo[x][z] && D[x][z] == Do[x][z])
            {
                ActualDo[iter] = 1;
            }
            else
            {
                ActualDo[iter] = 0;
                leaver = true;
                break;
            }
        }
        if(leaver == true)
        {
            break;
        }
    }

    for(int x = 0;x < 3;x++)
    {
        for(int z = 0;z < 3;z++)
        {
            F[x][z] = Fo[x][z];
            T[x][z] = To[x][z];
            R[x][z] = Ro[x][z];
            L[x][z] = Lo[x][z];
            B[x][z] = Bo[x][z];
            D[x][z] = Do[x][z];
        }
    }

    memset(way,'\0',250);
    memset(way2,'\0',250);
    memset(act,'\0',20);
    memset(steps,'\0',300);
    filler = -1; filler2 = -1;
    take = 0;
    skip = false;
    iter = 0;
    shift = 0;

}

int iter;

void ALGO()
{
    bool a,b,c,d;
    for(int i = 0;i < 16;i++)
    {
        a = (i >> 3) & 1;
        b = (i >> 2) & 1;
        c = (i >> 1) & 1;
        d = (i >> 0) & 1;
        iter = i;
        SwarmDynamic((bool[]){a,b,c,d}); // SwarmStatic((bool[]){a,b,c,d});
    }
    
    for(int i = 0;i < 16;i++)
    {
        printf("Actual Doability : %d Validity : %c For the Count of : %d\n",ActualDo[i],validity[i],stepcnt[i]);
    }

    int toSelect = 0; 
    int co = 300;

    for(int i = 0;i < 16;i++)
    {
        if(validity[i] == 'P' && ActualDo[i] == 1)
        {
            if(stepcnt[i] < co)
            {
                toSelect = i;
                co = stepcnt[i];
            }
        }
    }
    
    printf("I will use %d indexed value with a value of %d\n",toSelect,co);

    StaticShow((bool[]){(toSelect >> 3) & 1,(toSelect >> 2) & 1,(toSelect >> 1) & 1,(toSelect >> 0) & 1});
}
