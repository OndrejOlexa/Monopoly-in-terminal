#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "monopoly.h" 
#include <string.h>

void vypis_hra(PLAYER hraci[], int pocet);
int set_winner2(PLAYER hraci[],int pocet);

int pocet_hracov(char* optarg)
{
    int hraci;
    sscanf(optarg, "%d", &hraci);
    return hraci;
}

void inc_hraci(PLAYER hraci[] ,int pocet)
{
    for (int i = 0; i < pocet; i++)
    {
        hraci[i].number = i + 1;
        hraci[i].space_number = 1;
        hraci[i].num_jail_pass = 0;
        hraci[i].is_in_jail = 0;
        hraci[i].num_properties = 0;
        if (pocet == 4)
        {
            hraci[i].cash = 16;
        }else if (pocet == 3)
        {
            hraci[i].cash = 18;
        }else if (pocet == 2)
        {
            hraci[i].cash = 20;
        }
    }
}
void koniec_plocha(PLAYER hraci[], int pocet)
{
    printf("Game board:\n");

    for (int i = 0; i < NUM_SPACES; i++)
    {
        SPACE pole = game_board[i];
        printf("%d.", i+1);
        if (pole.type == Property && pole.property != NULL)
        {
            printf(" %-20s %-2d %-2s", pole.property->name, pole.property->price, property_colors[pole.property->color]);
            for (int j = 0; j < pocet; j++)
            {
                
                int index = hraci[j].num_properties,monopol = 0,vlastni = 0;
                for (int k = 0; k < index; k++)
                {
                    if (pole.property->color == hraci[j].owned_properties[k]->color)
                    {
                        monopol++;
                    }
                    if (pole.property == hraci[j].owned_properties[k])
                    {
                        vlastni ++;
                    }
                }
                if (vlastni != 0 && monopol == 2)
                {
                    printf("    P%-2d   yes",hraci[j].number);
                }else if (vlastni != 0)
                {
                    printf("    P%-2d   no",hraci[j].number);
                }

            }
            printf("\n");
        }else
        {
            printf("  %s\n", space_types[pole.type]);
        }
    }

}

void vypis_plochy()
{
    printf("Game board:\n");

    for (int i = 0; i < NUM_SPACES; i++)
    {
        SPACE pole = game_board[i];
        printf("%d.", i+1);
        if (pole.type == Property && pole.property != NULL)
        {
            printf("  %-20s %-2d %-2s\n", pole.property->name, pole.property->price, property_colors[pole.property->color]);
        }else
        {
            printf("  %s\n", space_types[pole.type]);
        }
    }
    printf("WINNER: -\n\n");
}

int najdi_nehn(PROPERTY *prop)
{
    for (int i = 0; i < NUM_SPACES; i++)
    {
        if (game_board[i].property == prop)
            return i + 1;
    }
    return -1;
}

int set_winner2(PLAYER hraci[],int pocet)
{
    int winner = 0;
    for (int i = 0; i < pocet; i++)
    {
        if (hraci[i].cash > hraci[winner].cash) // l ma najviac penazi
        {
            winner = i;
        }else if (hraci[i].cash == hraci[winner].cash && winner != i)
        {
            int majetok_i = 0, majetok_winner = 0;
            for (int j = 0; j < hraci[i].num_properties; j++)
            {
                majetok_i += hraci[i].owned_properties[j]->price;
            }
            for (int k = 0; k < hraci[winner].num_properties; k++)
            {
                majetok_winner += hraci[winner].owned_properties[k]->price;
            }
            if (majetok_i > majetok_winner)
            {
                winner = i;
            }
            if (majetok_i == majetok_winner)
            {
                return -1;
            }     
        }
    }
    return winner + 1;
}

void vypis_hra(PLAYER hraci[],int pocet)
{
    printf("Players: \n");
    for (int i = 0; i < pocet; i++)
    {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n", (i+1), hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail ? "yes" : "no");
        for (int j = 0; j < hraci[i].num_properties; j++)
        {
            PROPERTY *p = hraci[i].owned_properties[j];
            int pos = najdi_nehn(p);
            printf("%s    %d  %s  S%d\n",hraci[i].owned_properties[j]->name,hraci[i].owned_properties[j]->price,property_colors[p->color],pos);
        }
    }
}      

int main(int argc, char *argv[]) {
    int opt, prepinac_s = 0, prepinac_p = 0, prepinac_g = 0;
    int pocet = 2;
    char* optstring = ":spgn:";
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 's':
                prepinac_s = 1;
                break;
            case 'p':
                prepinac_p = 1;
                break;
            case 'g':
                prepinac_g = 1;
                break;
            case 'n':
                pocet = pocet_hracov(optarg);
                break;
            default:
                printf("Neplatny");
                return 1;
        }
    }
    PLAYER hraci[MAX_PLAYERS];
    inc_hraci(hraci, pocet);
    printf("Players:\n");
    for (int i = 0; i < pocet; i++)
    {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n", (i+1), hraci[i].space_number, hraci[i].cash, hraci[i].num_jail_pass, hraci[i].is_in_jail ? "yes" : "no");
    }
    vypis_plochy();

    int turn = 1;
    while (1)
    {
        for (int i = 0; i < pocet; i++)       
        {
            int hod_kockou;
            if (scanf("%d",&hod_kockou) != 1) {
                vypis_hra(hraci, pocet);
                koniec_plocha(hraci,pocet);
                int vitaz = set_winner2(hraci,pocet);
                if (vitaz == -1)
                {
                    printf("WINNER: ?\n");
                    return 0;
                }
                printf("WINNER: P%-2d\n", vitaz);
                return 0;
            }
            printf("R: %d\nTurn: %d\nPlayer on turn: P%d\n\n",hod_kockou, turn, i+1);
            turn ++;
            if (hraci[i].is_in_jail)
            {
                hraci[i].cash -= 1;
                if (hraci[i].cash < 0) //hrac i skrachoval  
                    {
                        hraci[i].cash += 1;
                        vypis_hra(hraci, pocet);
                        koniec_plocha(hraci,pocet);
                        int vitaz = set_winner2(hraci,pocet);
                        if (vitaz == -1)
                            {
                                printf("WINNER: ?\n");
                                return 0;
                            }
                        printf("WINNER: P%-2d\n", vitaz);
                        return 0;
                    }
                hraci[i].is_in_jail = 0; //oprava
            }
            int old_pos = hraci[i].space_number;
            hraci[i].space_number = (hraci[i].space_number + hod_kockou - 1) % 24 + 1;
            if (old_pos + hod_kockou > 24)
            {
                hraci[i].cash += 2;
            }
            SPACE pole = game_board[hraci[i].space_number - 1];
            if (pole.type == Property) //nula by mala byt enum pre property
            {
                PROPERTY *prop = pole.property;
                int index, vlastnik = -1;
                for (int j = 0; j < pocet; j++)
                {
                    index = hraci[j].num_properties;
                    for (int k = 0; k < index; k++)
                    {
                        if (prop == hraci[j].owned_properties[k]) //hladam ci niekto vlastni    
                        {
                            vlastnik = j;
                            break;
                        }
                    }
                    if (vlastnik != -1) break;
                }
                if (vlastnik == -1) //kupa nehn
                {
                    hraci[i].owned_properties[hraci[i].num_properties] = prop;
                    hraci[i].num_properties++;
                    hraci[i].cash -= prop->price;
                    if (hraci[i].cash < 0) //hrac i skrachoval
                        {
                            hraci[i].cash += prop->price;
                            hraci[i].num_properties--;
                            vypis_hra(hraci, pocet);
                            koniec_plocha(hraci,pocet);
                            int vitaz = set_winner2(hraci,pocet);
                            if (vitaz == -1)
                            {
                                printf("WINNER: ?\n");
                                return 0;
                            }
                            printf("WINNER: P%-2d\n", vitaz);
                            return 0;
                        }
                }
                else if (vlastnik == i) //vlastni hrac na tahu
                {
                }
                else{
                    int monopol = 0;
                    for (int k = 0; k < hraci[vlastnik].num_properties; k++)
                    {
                        if (prop->color == hraci[vlastnik].owned_properties[k]->color) //hladam ci ma vlastnik aj druhu nehnutelnost rovnakej farby
                        {
                            monopol++;
                        }
                    }
                    if (monopol == 2)
                    {
                        int cena = prop->price;
                        hraci[i].cash -= 2*cena;
                        hraci[vlastnik].cash += 2*cena;
                        if (hraci[i].cash < 0) //hrac i skrachoval
                        {
                            hraci[vlastnik].cash -= 2*cena; //vymena set winner
                            hraci[i].cash += 2*cena;
                            vypis_hra(hraci, pocet);
                            koniec_plocha(hraci,pocet);
                            int vitaz = set_winner2(hraci,pocet);
                            if (vitaz == -1)
                            {
                                printf("WINNER: ?\n");
                                return 0;
                            }
                            printf("WINNER: P%-2d\n", vitaz);
                            return 0;
                        }   
                    }else{
                        int cena = prop->price;
                        hraci[i].cash -= cena;
                        hraci[vlastnik].cash += cena;
                        if (hraci[i].cash < 0) //hrac i skrachoval
                        {
                            hraci[vlastnik].cash -= cena;
                            hraci[i].cash += cena;
                            vypis_hra(hraci, pocet);
                            koniec_plocha(hraci,pocet);
                            int vitaz = set_winner2(hraci,pocet);
                            if (vitaz == -1)
                            {
                                printf("WINNER: ?\n");
                                return 0;
                            }
                            printf("WINNER: P%-2d\n", vitaz);
                            return 0;
                        }
                    }
                }
            }
            if (pole.type == Free_parking || pole.type == In_jail) // ak hrac na parking alebo jail tak nic
            {
            }
            if (pole.type == Go_to_jail)
            {
                if (hraci[i].num_jail_pass > 0)
                {
                    hraci[i].num_jail_pass -= 1;
                    hraci[i].is_in_jail = 0;
                }else{
                    hraci[i].space_number = 7; //index jail
                    hraci[i].is_in_jail = 1;
                }
            }
            if (pole.type == Jail_pass)
            {
                hraci[i].num_jail_pass +=1;
            }
            if (prepinac_s)
            {
                koniec_plocha(hraci, pocet);
                printf("\n");
            }
            if (prepinac_p)
            {
                vypis_hra(hraci,pocet);
                printf("\n");
            }
            if (prepinac_g)
            {
                vypis_hra(hraci,pocet);
                koniec_plocha(hraci,pocet);
                printf("WINNER: -\n\n");
            } 
        }
    }
    return 0;
}