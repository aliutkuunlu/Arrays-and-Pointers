#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAX_SOLDIERS 100
#define LINE_MAX_LENGTH 128
#define NUM_MAX_LINES 5000

static const char filename[] = "commands.txt";
char lineG [NUM_MAX_SOLDIERS];

float side_1[NUM_MAX_SOLDIERS*2];
float side_2[NUM_MAX_SOLDIERS*2];

int top_index1;
int top_index2;

int turn;

float *soldier1;
float *soldier2;

int num_dead1;
int num_dead2;

int num_soldiers1;
int num_soldiers2;

float total_damage1;
float total_damage2;

float* pop(int army)//extraction function
{
    float *tmp;

    if (army == 1)
    {
        if(num_soldiers1 <= 0)
        {
            return NULL;
        }
        else
        {
            tmp = &side_1[top_index1];

            side_1[top_index1]  = -1;
            side_1[top_index1+1]= -1;

            num_soldiers1--;
            top_index1 += 2;
            num_dead1++;

                if(num_soldiers1 <= 0)
                {
                    num_soldiers1  = 0;
                }

            printf("1 has a casualty\n");
        }
    }
    else
    {
        if(num_soldiers2 <= 0)
        {
            return NULL;
        }
        else
        {
            tmp = &side_2[top_index2];

            side_2[top_index2]= -1;
            side_2[top_index2+2] = -1;

            num_soldiers2--;
            top_index2 += 2;
            num_dead2++;

                if(num_soldiers2 <= 0)
                {
                    num_soldiers2  = 0;
                }
            printf("2 has a casualty\n");
        }
    }
    return tmp;
}
void push(int army, float* man)//adding function
{
    if (num_soldiers1 == NUM_MAX_SOLDIERS && army == 1)
        return;
    if (num_soldiers2 == NUM_MAX_SOLDIERS && army == 2)
        return;

    if (army == 1)
    {
        top_index1 -= 2;
        num_soldiers1 ++;

        side_1[top_index1]   = man[0];
        side_1[top_index1+1] = man[1];


        printf("S - H: %.0f, S:%.0f\n", side_1[top_index1],side_1[top_index1+1]);
    }
    else
    {
        num_soldiers2++;
        top_index2 -= 2;

        side_2[top_index2]   = man[0];
        side_2[top_index2+1] = man[1];

        printf("S - H: %.0f, S:%.0f\n",  side_2[top_index2],side_2[top_index2+1]);

    }

}
float* top(int army)//returns to top function
{
    float *tmp;

    if (army==1)
    {
        if(num_soldiers1 <= 0)

            return NULL;

        tmp = &side_1[top_index1];
    }

    else
    {
        if(num_soldiers2 <= 0)

            return NULL;

        tmp = &side_2[top_index2];
    }

        return tmp;
}

void fight()// F function

{
    if(num_soldiers2 <= 0 || num_soldiers1 <= 0)
        return ;

    float damage;
    float* tmp_soldier;

    float attacker_strength;
    float defender_health;
    float defender_strength;



    soldier1 = top(1);
    soldier2 = top(2);


    if (turn == 1)
    {
       attacker_strength =  soldier1[1];
       defender_strength =  soldier2[1];
       defender_health   =  soldier2[0];

       damage =  (attacker_strength-defender_strength)*0.05 + 50.0;
       defender_health -= damage;
       total_damage1 += damage;
       printf("1 hit %.2f damage\n" , damage);

            if (defender_health <= 0.0)
            {
                tmp_soldier=pop(2);
            }

            else
            soldier2[0] = defender_health;

                turn = 2;
    }

    else
    {
       attacker_strength = soldier2[1];
       defender_strength = soldier1[1];
       defender_health   = soldier1[0];

       damage =  (attacker_strength-defender_strength)*0.05 + 50;
       defender_health -= damage;
       total_damage2 += damage;
            printf("2 hit %.2f damage\n" , damage);
            if (defender_health <= 0.0)
            {
                tmp_soldier = pop(1);
            }

       else
        {
            soldier1[0] = defender_health;
        }

            turn = 1;

    }
}
void critical_shot() // casualty function
{
    if(turn == 1)
    {
        if(num_soldiers2 > 0)
        {
            printf("Critical shot.\n");
            pop(2);
            turn = 2;
        }
    }
    else
    {
        if(num_soldiers1 > 0)
        {
            printf("Critical shot.\n");
            pop(1);
            turn = 1;
        }
    }
}

void statistics()  //print statistic function
{
    printf("Fight ended.\n");
    printf("1 has %d soldier left, %d soldier died.\n", num_soldiers1, num_dead1);
    printf("2 has %d soldier left, %d soldier died.\n", num_soldiers2, num_dead2);
    printf("1 harmed %.2f total damege.\n", total_damage1);
    printf("2 harmed %.2f total damege.\n", total_damage2);
}




static char* readLine()
{    static FILE* file;
            if(file == NULL)   file = fopen ( filename, "r" );
                        //open file
            if ( file != NULL )
                {              if( fgets ( lineG, sizeof (lineG), file ) != NULL )
                                    /* read a line*/
                                     return lineG;

                                else     fclose ( file );
                }
                else  perror ( filename ); /* why didn't the file open? */

            return NULL;
}

void process_line(char line[], int line_no)
{
    int i,j;
    float tmp[2];
    char* ctmp;
    int army_no;
    char first_char;
    i=0;


    ctmp = strtok (line," ,;");
    if (ctmp != NULL)
    {
        first_char = ctmp[0];

    }
    else
    {
        return;
    }

    if (first_char =='F' || first_char=='f')
    {
        fight();
    }
    else if (first_char =='C' || first_char=='c')
    {
        critical_shot();
    }
    else if (first_char=='S' || first_char=='s')
    {
        statistics();
    }
    else if (first_char=='A' || first_char=='a')
    {

        j = 0;

        ctmp = strtok (NULL, " ,;");
        while (ctmp != NULL)
        {
            //ctmp = strtok (NULL, " ,;");
            if (j==0)
            {
                //printf("army: %s\n",ctmp);
                army_no = atoi(ctmp);
                j++;
                 if(army_no == 1)
                        printf("add soldiers to side 1\n");
                 else if(army_no == 2)
                        printf("add soldiers to side 2\n");
            }
            else
            {
                if(army_no == 1)
                {
                    if (j%2==1)
                    {
                        tmp[0] = (float) atoi(ctmp);
                        //printf("%d health: %f \n", j,tmp[0]);
                    }
                    else
                    {
                        tmp[1] = (float) atoi(ctmp);
                        //printf("%d strength: %f \n", j, tmp[1]);

                        push(1,tmp);
                    }
                }
                else
                {
                    if (j%2==1)
                    {
                        tmp[0] = (float) atoi(ctmp);
                        //printf("%d health: %f \n", j,tmp[0]);
                    }
                    else
                    {
                        tmp[1] = (float) atoi(ctmp);
                        //printf("%d strength: %f \n", j, tmp[1]);
                        push(2,tmp);
                    }

                }
                j++;
            }
            ctmp = strtok (NULL, " ,;");
        }
    }
}

int main (void)
 {
    top_index1 = 2*NUM_MAX_SOLDIERS;
    top_index2 = 2*NUM_MAX_SOLDIERS;

    //printf("top index 1:%d\n",top_index1);//
    //printf("top index 2:%d\n",top_index2);

    total_damage1=0;
    total_damage2=0;

    num_soldiers1 = 0;
    num_soldiers2 = 0;

    turn = 1;

    //printf("started ok\n");
    char* line;
    int i =0;
    for(i=0; i<NUM_MAX_LINES; i++)
    {
        //printf("\n\nline no %d \n",i);
        line = readLine();
        //printf("content: %s \n",line);

        if(line != NULL)
        {
            process_line(line,i);
        }
        else
        {
            break;
        }
    }
    return 0;
 }
