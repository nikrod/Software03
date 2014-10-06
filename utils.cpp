#include "utils.h"

char* NombreMes(int NumMes) 
{
    char* NomMes = NULL;
    NomMes = (char *) calloc(65, sizeof (char *));
    if (NumMes == 1) 
    {
        strcpy(NomMes, "Enero");
        return NomMes;
    } else if (NumMes == 2) 
    {
        strcpy(NomMes, "Febrero");
        return NomMes;
    } else if (NumMes == 3) 
    {
        strcpy(NomMes, "Marzo");
        return NomMes;
    } else if (NumMes == 4) 
    {
        strcpy(NomMes, "Abril");
        return NomMes;
    } else if (NumMes == 5) 
    {
        strcpy(NomMes, "Mayo");
        return NomMes;
    } else if (NumMes == 6) 
    {
        strcpy(NomMes, "Junio");
        return NomMes;
    } else if (NumMes == 7) 
    {
        strcpy(NomMes, "Julio");
        return NomMes;
    } else if (NumMes == 8) 
    {
        strcpy(NomMes, "Agosto");
        return NomMes;
    } else if (NumMes == 9) 
    {
        strcpy(NomMes, "Septiembre");
        return NomMes;
    } else if (NumMes == 10) 
    {
        strcpy(NomMes, "Octubre");
        return NomMes;
    } else if (NumMes == 11) 
    {
        strcpy(NomMes, "Noviembre");
        return NomMes;
    } else if (NumMes == 12) 
    {
        strcpy(NomMes, "Diciembre");
        return NomMes;
    } else 
    {
        strcpy(NomMes, "");
        return NomMes;
    }
}

char* Trans(long NumMes) 
{
    char* caracter = NULL;
    caracter = (char *) calloc(65, sizeof (char *));
    snprintf(caracter, 64, "%ld", NumMes);
    return caracter;
}

