/* 
 * File:   tempsensor.c
 * Author: Felix Mook & Philipp Seitz

 */

#include<stdio.h>
#include"tempsensor.h"

int Temp() {
    char *code;
    char temp[6];
    temp[5] = '\0'; //char-array beenden/abschließen
    size_t n = 0;
    int c, x, i, k;
    int itemp;

    code = malloc(1000);
    x = 0;
    i = 0;
    k = 5;


    FILE *file = fopen("/sys/bus/w1/devices/28-000005a564f4/w1_slave", "r"); //öffne Datei

    if (file == NULL) //überrüft ob datei leer ist
        printf("could not open file");

    while ((c = fgetc(file)) != EOF)
    { 
        code[n++] = (char) c; //wenn zeichen= "t"
        if (c == 't') //dann postition von t in x
        {
            x = n;
        }
    }

    code[n] = '\0'; // don't forget to terminate with the null character    

    x++; //t=xxyyy ("=" abschneiden)

    for (i = 0; i < k; i++)
        temp[i] = code[x + i]; //werte in extra string schreiben 
    //in temp steht die Temperatur (6zeichen lang)

    itemp = atoi(temp); //char to double
    //printf("%i\n",itemp);	 //temp ausgeben

    fclose(file);
    return (itemp);
}

