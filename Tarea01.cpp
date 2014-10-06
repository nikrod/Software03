//Para compilar este archivo usar el comando: g++ Sof.cpp -g -I/usr/include/postgresql -I/usr/include -lpq -lgd
// Todas las fechas presentes en la Base de Datos tienen un rango que va desde el 16-03-2014 hasta 23-09-2014.

#include <iostream>
#include <libpq-fe.h>
#include <gd.h>
#include <gdfonts.h>
#include <fstream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include "utils.h"

#define Largo 200
#define _TIME_
#define N 15
#define IMG_WIDTH 800
#define IMG_HEIGHT 600
#define BORDE_ANCHO 70
#define BORDE_ALTO 50
#define IMG_BPP 24

using namespace std;

char Consulta[Largo];      
char *fechai,*fechaf;  	    
struct tm fdesde;            
struct tm fhasta;  			

//FUNCIÓN QUE VERIFICA LAS FECHAS
void Fvalidacion(char *Ini, char *Ter)
{
    char chartoint[5];      			
    fechai=Ini;
    fechaf=Ter;
    if((strlen(fechai)!=10)||(fechai[4]!='-')||(fechai[7]!='-'))
    {
        cout<<"Error en la fecha inicial"<<endl;
        exit(0);
    };
    if((strlen(fechaf)!=10)||(fechaf[4]!='-')||(fechaf[7]!='-'))
    {
        cout<<"Error en la fecha final"<<endl;
        exit(0);
    };
    chartoint[0]=fechai[0];chartoint[1]=fechai[1];chartoint[2]=fechai[2];chartoint[3]=fechai[3];chartoint[4]='\n';
    fdesde.tm_year=atoi(chartoint);
    chartoint[0]=fechai[5];chartoint[1]=fechai[6];chartoint[2]='\n';
    fdesde.tm_mon=atoi(chartoint)-1;
    chartoint[0]=fechai[8];chartoint[1]=fechai[9];chartoint[2]='\n';
    fdesde.tm_mday=atoi(chartoint);
    chartoint[0]=fechaf[0];chartoint[1]=fechaf[1];chartoint[2]=fechaf[2];chartoint[3]=fechaf[3];chartoint[4]='\n';
    fhasta.tm_year=atoi(chartoint);
    chartoint[0]=fechaf[5];chartoint[1]=fechaf[6];chartoint[2]='\n';
    fhasta.tm_mon=atoi(chartoint)-1;
    chartoint[0]=fechaf[8];chartoint[1]=fechaf[9];chartoint[2]='\n';
    fhasta.tm_mday=atoi(chartoint);

    //Comprueba si la fecha "existe"
    if((fdesde.tm_mon>11)||(fdesde.tm_mday>31))
    {
       cout<<"Error en la fecha inicial"<<endl;
       exit(0);
    };
    if((fhasta.tm_mon>11)||(fhasta.tm_mday>31))
    {
       cout<<"Error en la fecha final"<<endl;
       exit(0);
    };
    cout<<"Fechas Reconocidas..."<<endl;
}

//FUNCION QUE LIMPIA EL ARREGLO DE CONSULTAS
void clean(void)
{
    for(int i = 0; i < Largo; i++ )
    {
        Consulta[i] = '\0';
    }
}

int main(int argc, char *argv[])
{
    char chartoint[5];      			
    int i=0, j=0, MesFinal, MesInicial, PivoteMes;
    ofstream archivoCSV;                
    long long int CantMensual [12], Mayor = 0, CantidadPeticiones = 0;
    	
    PGconn *cnn = NULL;
    PGresult *result = NULL;
    const char *host = "146.83.181.4";
    const char *port = "6432";
    const char *dataBase = "iswdb";
    const char *user = "estisw";
    const char *passwd = "estisw";
    char Resultado[30];

    gdImagePtr graficoB;
    FILE *archivo;
    gdFontPtr fuente = gdFontGetSmall();
    graficoB = gdImageCreateTrueColor(IMG_WIDTH, IMG_HEIGHT);
    int blanco, negro, colorAleatorio, alto = 50, ancho = 70, ValorGrafico, CantidadGraficada = 0;
    long paso = 0, maximo = 0;
    char titulo[513];
    double porcentaje = 0.0, PorcentajeParcial = 0.0;

    if((argc==2)||(argc==4))
    {
        if((argv[1][0]!='-'))
        {
            cout<<"Error en ingreso de argumentos."<<endl;
            return EXIT_SUCCESS;
        };
        switch (argv[1][1])
        {
            case 'g':     
                Fvalidacion(argv[2], argv[3]);
                clean();

                strcpy(Consulta,"SELECT EXTRACT (month FROM fecha) AS mes, COUNT(peticion) as contador FROM isw.accesos WHERE fecha >= '");
                strcat(Consulta, argv[2]);
                strcat(Consulta, " 00:00:00' and fecha <= '");
                strcat(Consulta, argv[3]);
                strcat(Consulta, " 23:59:59' GROUP BY EXTRACT (month FROM fecha) ORDER BY EXTRACT (month FROM fecha);");

                cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);

                    if (PQstatus(cnn) != CONNECTION_BAD) 
                    {
                    result = PQexec(cnn, Consulta);

                        if (result != NULL)
                        {
                            int tuplas = PQntuples(result);
                            int campos = PQnfields(result);

                            for (i=0; i < tuplas; i++)
                            {
                                for (j=0; j < campos; j++)
                                {

                                    strcpy(Resultado, PQgetvalue(result,i,j));

                                    if (i == 0 && j == 0)
                                    {
                                        MesInicial = atoi(Resultado);
                                        PivoteMes = MesInicial;
                                    }
                                    if (j == 1)
                                    {
                                        CantMensual[PivoteMes] = atoi(Resultado);
                                        if (Mayor <= CantMensual[PivoteMes]) Mayor = CantMensual[PivoteMes];
                                        PivoteMes++;
                                    }
                                    if (i == tuplas-1 && j == 0) MesFinal = atoi(Resultado);
                                }
                            }
                            //Generando el JPEG

                                blanco = gdImageColorAllocate(graficoB, 255, 255, 255);
                                negro = gdImageColorAllocate(graficoB, 0, 0, 0);

                                gdImageFill(graficoB, 0, 0, blanco);

                                memset(titulo, 0, 513);
                                snprintf(titulo, 513, "PETICIONES MENSUALES DESDE LA FECHA %s HASTA %s", argv[2], argv[3]);
                                gdImageString(graficoB, fuente, (int) IMG_WIDTH * 0.35, 90, (unsigned char *) titulo, negro);
                                fflush(stdout);

                              
                                for (int y = BORDE_ALTO; y <= (BORDE_ALTO + 500); y = y + 50)
                                {
                                    gdImageString(graficoB, fuente, 5, IMG_HEIGHT - y, (unsigned char *) Trans(paso), negro);
                                    paso += (Mayor / 9);
                                }

                                for (int mes = MesInicial; mes <= MesFinal ; mes++)
                                {
                                   
                                    colorAleatorio = gdImageColorAllocate(graficoB, rand() % 256, rand() % 256, rand() % 256);
                                    porcentaje = ((double) CantMensual[mes] / Mayor);
                                 
                                    alto = (int) (450 * porcentaje);
                                 
                                    gdImageFilledRectangle(graficoB, ancho, IMG_HEIGHT - BORDE_ALTO, ancho + 55, IMG_HEIGHT - (BORDE_ALTO + alto), colorAleatorio);
                                
                                    gdImageRectangle(graficoB, ancho, IMG_HEIGHT - BORDE_ALTO, ancho + 55, IMG_HEIGHT - (BORDE_ALTO + alto), negro);
                                   
                                    gdImageString(graficoB, fuente, ancho + 19, IMG_HEIGHT - BORDE_ALTO + 5, (unsigned char *) NombreMes(mes), colorAleatorio);
                                   
                                    gdImageString(graficoB, fuente, ancho + 10, IMG_HEIGHT - (BORDE_ALTO + alto + 20), (unsigned char *) Trans(CantMensual[mes]), negro);

                                    ancho += 55;
                                }

                                // Nombre del eje X
                                memset(titulo, 0, 513);
                                snprintf(titulo, 513, "MES (X)");
                                gdImageString(graficoB, fuente, IMG_WIDTH - BORDE_ANCHO, IMG_HEIGHT - BORDE_ALTO + 5, (unsigned char *) titulo, negro);
                                fflush(stdout);

                                 // Nombre del eje Y
                                memset(titulo, 0, 513);
                                snprintf(titulo, 513, "PETICION (Y)");
                                gdImageString(graficoB, fuente, 8, 23, (unsigned char *) titulo, negro);
                                fflush(stdout);

                          
                                gdImageLine(graficoB, BORDE_ANCHO, (IMG_HEIGHT - BORDE_ALTO), (IMG_WIDTH - BORDE_ANCHO), (IMG_HEIGHT - BORDE_ALTO), negro);
                                gdImageLine(graficoB, BORDE_ANCHO, BORDE_ALTO, BORDE_ANCHO, (IMG_HEIGHT - BORDE_ALTO), negro);

                                
                                archivo = fopen("Peticiones_por_Fecha.jpeg", "wb");
                                if (archivo != NULL)
                                {
                                    gdImageJpeg(graficoB, archivo, 100);
                                    fclose(archivo);
                                }
                                else cout<<"\n\n Archivo jpeg no creado\nIntentelo nuevamente"<<endl;

                                gdImageDestroy(graficoB);
                                cout<< "Peticiones_por_Fecha.jpeg generado correctamente" << endl;
                        }
                    }
                    else cout<<"Error al conectar a la base de datos"<<endl;

                PQfinish(cnn);
             break;

             case 't': 
                Fvalidacion(argv[2], argv[3]);
                clean();
		    strcpy(Consulta,"SELECT estado as estado, COUNT(peticion) AS contador FROM isw.accesos WHERE fecha >= '");
		    strcat(Consulta, argv[2]);
		    strcat(Consulta, " 00:00:00' and fecha <= '");
		    strcat(Consulta, argv[3]);
		    strcat(Consulta, " 23:59:59' GROUP BY estado ORDER BY contador DESC");

		    cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);

		    blanco = gdImageColorAllocate(graficoB, 255, 255, 255);
		    negro = gdImageColorAllocate(graficoB, 0, 0, 0);

		    gdImageFill(graficoB, 0, 0, blanco);

		    memset(titulo, 0, 513);
		    snprintf(titulo, 513, "PETICIONES POR ESTADOS DESDE LA FECHA %s HASTA %s", argv[2], argv[3]);
		    gdImageString(graficoB, fuente, (int) IMG_WIDTH * 0.1, 10, (unsigned char *) titulo, negro);
		    fflush(stdout);

		        if (PQstatus(cnn) != CONNECTION_BAD) {
		        result = PQexec(cnn, Consulta);

		            if (result != NULL)
		            {

		                int tuplas = PQntuples(result);
		                int campos = PQnfields(result);
		                for (i=0; i < tuplas; i++)
		                    for (j=0; j < campos; j++)
		                        if (j == 1) CantidadPeticiones = CantidadPeticiones + atoi(PQgetvalue(result,i,j));

		                int ValorAnterior = 0;
		                int pasoLineaColores = 385;
		                for (i=0; i < tuplas; i++)
		                {
		                    for (j=0; j < campos; j++)
		                    {
		                        if (j == 1)
		                        {
		                            PorcentajeParcial = ((double)atoi(PQgetvalue(result,i,j))/CantidadPeticiones);
		                            ValorGrafico = (PorcentajeParcial) * 360;

		                            if (ValorGrafico >= 1)
		                            {

		                                colorAleatorio = gdImageColorAllocate(graficoB, rand() % 256, rand() % 256, rand() % 256);
		                                gdImageFilledArc(graficoB, 220, 300, 400, 400, ValorAnterior, ValorGrafico + ValorAnterior, colorAleatorio, gdPie);
		                                gdImageFilledArc(graficoB, 570, pasoLineaColores, 10, 10, 0, 360, colorAleatorio, gdPie);
		                                ValorAnterior = ValorGrafico + ValorAnterior;
		                                pasoLineaColores = pasoLineaColores + 30;
		                                CantidadGraficada++;
		                            }
		                        }
		                    }
		                }

		                colorAleatorio = gdImageColorAllocate(graficoB, rand() % 256, rand() % 256, rand() % 256);
		                gdImageFilledArc(graficoB, 220, 300, 400, 400, ValorAnterior, 360, colorAleatorio, gdPie);

		                gdImageFilledArc(graficoB, 570, pasoLineaColores, 10, 10, 0, 360, colorAleatorio, gdPie);

		                int pasoLinea = 50;
		                memset(titulo, 0, 513);
		                snprintf(titulo, 513, "El total de peticiones es: %llu", CantidadPeticiones);
		                gdImageString(graficoB, fuente, 470, pasoLinea , (unsigned char *) titulo, negro);
		                fflush(stdout);
		                pasoLinea = pasoLinea + 20;

		                for (i=0; i < tuplas; i++)
		                {
		                    pasoLinea = pasoLinea + 13;
		                    memset(titulo, 0, 513);
		                    snprintf(titulo, 513, "Peticion %s --> %f %%", PQgetvalue(result,i,0), (atof(PQgetvalue(result,i,1))/CantidadPeticiones)*100);
		                    gdImageString(graficoB, fuente, 470, pasoLinea, (unsigned char *) titulo, negro);
		                    fflush(stdout);
		                }

		                pasoLinea = 380 ;
		                for (i=0; i < CantidadGraficada; i++)
		                {
		                    memset(titulo, 0, 513);
		                    snprintf(titulo, 513, "Peticion %s.", PQgetvalue(result,i,0));
		                    gdImageString(graficoB, fuente, 600, pasoLinea, (unsigned char *) titulo, negro);
		                    fflush(stdout);
		                    pasoLinea = pasoLinea + 30;
		                }

		                memset(titulo, 0, 513);
		                snprintf(titulo, 513, "Otros.");
		                gdImageString(graficoB, fuente, 600, pasoLinea, (unsigned char *) titulo, negro);
		                fflush(stdout);

		                archivo = fopen("Peticiones_por_Estado.jpeg", "wb");
		                if (archivo != NULL)
		                {
		                   gdImageJpeg(graficoB, archivo, 100);
		                   fclose(archivo);
		                }
		                else cout<<"\n\n Archivo jpeg no creado\nIntentelo nuevamente"<<endl;

		                gdImageDestroy(graficoB);
		                cout<< "Peticiones_por_Estado.jpeg generado correctamente" << endl;

		            }
		        }
		        else cout<<"Error al conectar a la base de datos"<<endl;
		    PQfinish(cnn);
         break;

             case 'a':   
                Fvalidacion(argv[2], argv[3]);
                clean();
                strcpy(Consulta,"SELECT peticion, COUNT (*) as contador FROM isw.accesos WHERE fecha >= '");
                strcat(Consulta, argv[2]);
                strcat(Consulta, " 00:00:00' and fecha <= '");
                strcat(Consulta, argv[3]);
                strcat(Consulta, " 23:59:59' GROUP BY peticion ORDER BY contador DESC limit 100;");

                //El archivo contiene numero; petición; total;
                archivoCSV.open ("Peticiones_Solicitadas.csv");

                //Conexión a la BDD
                cnn = PQsetdbLogin(host,port,NULL,NULL,dataBase,user,passwd);

                    if (PQstatus(cnn) != CONNECTION_BAD) {
                    result = PQexec(cnn, Consulta);

                        if (result != NULL)
                        {
                            int ID = 1;
                            int tuplas = PQntuples(result);
                            int campos = PQnfields(result);

                            for (i=0; i < tuplas; i++)
                            {
                                archivoCSV << ID << ";";
                                for (j=0; j < campos; j++)
                                {
                                    archivoCSV << PQgetvalue(result,i,j) << ";";
                                }
                                archivoCSV << "\n";
                                ID++;
                            }
                            archivoCSV.close();
    			cout<< "Peticiones_Solicitadas.csv generado correctamente" << endl;
                        }
                    }
                    else cout<<"Error al conectar a la base de datos"<<endl;
                PQfinish(cnn);
             break;

             case 'v':
                printf("\nIntegrantes del grupo:\n\n\tDámaris Ávila Fuentes.\n\tNicolás Rodríguez Ormazábal.\n\tBárbara Rojas Flores.\n\n");
         	    printf("Fecha de compilación: %s - %s\n\n ", __DATE__, __TIME__);
             break;
             };
    }
else cout<<"Opcion no valida, vualva a ingresar los parametros"<<endl;
return EXIT_SUCCESS;
}
