//RecuParcial
#include<stdio.h>
#include<stdint.h>



/*struct HEADER//no conviene usar esta distribucion
{ /*
    uint16_t ID      :10;
    uint16_t LLDV    : 6;//ID DE DISPOSITIVO y cantidad de dispositivos debajo de el 
    uint16_t DT_Nh   : 4;
    uint16_t info    :10;
    uint16_t DT_NL   : 4;
    uint16_t rsv1    : 3;
    uint16_t UPLD    :10;
    uint16_t rsv2    : 3;
   // uint16_t ID_lldc :10;//esta al revess!!!!!!
   uint16_t rsv2    : 3;
   uint16_t UPLD    :10;
   uint16_t rsv1    : 3;
   uint16_t DT_NL   : 4;
   uint16_t info    :10;
   uint16_t DT_Nh   : 4;
   uint16_t LLDV    : 6;
   uint16_t ID      :10;
};*/

//Campos de Bits
struct HD1//HEADER que lee la el id principal y lower level devices
{
  // uint16_t ID  :10;
   uint16_t LLD :6 ;//less
   uint16_t ID  :10;//most
};
struct HD2//Lee el segundo paquete de 16bits
{
   /* uint16_t Dt_NH :3 ;
    uint16_t Info  :10;
    uint16_t Dt_NL :3;*/
   uint16_t nsb1    : 3;//lessbit
   uint16_t DT_NL   : 1;
   //---------------------
   //uint16_t info    : 8;//los nsb son bits sin informacion
   uint16_t nsb3    : 2;
   uint16_t Actuador: 1;
   uint16_t nsb5    : 1;
   uint16_t Sensor  : 2;
   uint16_t nsb4    : 2;
   //---------------------
   uint16_t DT_Nh   : 1;
   uint16_t nsb2    : 3;//MOSBIT
};
struct HD3
{
    uint16_t rsv1    : 3;
    uint16_t UPLD_ID :10;
    uint16_t rsv2    : 3;
};

struct MASTER
{
    HD1 TOP;
    HD2 MIDDLE;
    HD3 LOWER;
};
//--------------Funciones---------------------//
void reader(FILE *f);
void Unificator(FILE *f);
int tam(FILE *f);
//--------------------------------------------//



int main ()
{
    FILE *f;
    //MASTER A;//estructura donde guardo los ids
    f=fopen("network_structure.dat","rb");
     if(fopen("network_structure.dat","rb")==NULL)
    {
        printf("ERROR NO SE ENCUENTRA EL ARCHIVO");
    }
    //reader(f);
   // Unificator(f);
    //------------------------------------------zona de Pruebas-----------------------------------------//
    int band=0;
    int Final=tam(f);
    int inicio=0;
    fseek(f,0,SEEK_SET);

for(int i=0;band!=1;i++)
{
    HD1 A;
    fread(&A,sizeof(uint16_t),1,f);
    printf("El ID es[%u]\n",A.ID);
    printf("La cantidad de dispositivos (esclavos) es [%u]\n",A.LLD);
    HD2 B;
    fread(&B,sizeof(uint16_t),1,f);
    //-------------could be a function---------------------Pido perdon por los ifs excesivos, con mas tiempo lo podria optimizar
    if(B.DT_NL|B.DT_Nh==00)
    {
        printf("es un CPU\n");
    }
    if(B.DT_NL|B.DT_Nh==01)
    {
       printf("es un Sensor\n");
       if(B.Sensor==10)
        {
            printf("es un sensor de Caudal\n");
        }
         if(B.Sensor==11)
        {
            printf("es un sensor de Temperatura\n");
        }
         if(B.Sensor==2)
        {
            printf("es un sensor de Presion\n");
        }
         if(B.Sensor==3)
        {
            printf("es un sensor de nivel\n");
        }
       
    }
    if(B.DT_NL|B.DT_Nh==2)
    {
        printf("es un Actuador");
        if(B.Actuador==00)
        {
            printf("es una valvula\n");
        }
        if(B.Actuador==01)
        {
            printf("es una Motor\n");
        }
        
    }
    if((B.DT_NL|B.DT_Nh)==3)
    {
        printf("es un Concentrador\n");
    }
    //---------------------------------------------------
    HD3 C;
    fread(&C,sizeof(uint16_t),1,f);
    printf("Cantidad de dispositivos Superiores [%u]\n",C.UPLD_ID);
    uint16_t ND;
    fread(&ND,sizeof(uint16_t),A.LLD,f);
    printf("\n\nSiguiente Dispositivo \n");
    if(inicio==Final)
    {
        band=1;
    }else{
        inicio=ftell(f);
    }
}
    //------------------------------------------zona de Pruebas-----------------------------------------//
    fclose(f);
    return 0;
}






void reader(FILE *f)//funtion to verificate the correct reading of the file
{

   if(fopen("network_structure.dat","rb")==NULL)
    {
        printf("ERROR NO SE ENCUENTRA EL ARCHIVO");
    }
   // struct HEADER A;
   HD1 A;
    fread(&A,sizeof(uint16_t),1,f);
    printf("El ID es[%u]\n",A.ID);
    printf("La cantidad de dispositivos (esclavos) es [%u]\n",A.LLD);
    HD2 B;
    fread(&B,sizeof(uint16_t),1,f);
    //-------------could be a function---------------------
    if(B.DT_NL|B.DT_Nh==0)
    {
        printf("es un CPU\n");
    }
    if(B.DT_NL|B.DT_Nh==1)
    {
       printf("es un Sensor\n");
    }
    if(B.DT_NL|B.DT_Nh==2)
    {
        printf("es un Actuador");
    }
    if(B.DT_NL|B.DT_Nh==3)
    {
        printf("es un Concentrador");
    }
    //---------------------------------------------------
    HD3 C;
    fread(&C,sizeof(uint16_t),1,f);
    printf("Cantidad de dispositivos Superiores [%u]\n",C.UPLD_ID);
    fread(NULL,sizeof(uint16_t),A.LLD,f);

}


int tam(FILE *f)//devulve tamaño en bytes
{
    int Tam;
   Tam = fseek(f,0,SEEK_END);
   fseek(f,0,SEEK_SET);
   return Tam;
}

void Unificator(FILE *f)//funtion to verificate the correct reading of the file
{
    int band=0;
    int Final=tam(f);
    int inicio=0;
   for(int i=0;band==1;i++)
   {
     if(fopen("network_structure.dat","rb")==NULL)
    {
        printf("ERROR NO SE ENCUENTRA EL ARCHIVO");
    }
   // struct HEADER A;
   HD1 A;
    fread(&A,sizeof(uint16_t),1,f);
    printf("El ID es[%u]\n",A.ID);
    printf("La cantidad de dispositivos (esclavos) es [%u]\n",A.LLD);
    HD2 B;
    fread(&B,sizeof(uint16_t),1,f);
    //-------------could be a function---------------------Pido perdon por los ifs excesivos, con mas tiempo lo podria optimizar
    if(B.DT_NL|B.DT_Nh==0)
    {
        printf("es un CPU\n");
    }
    if(B.DT_NL|B.DT_Nh==1)
    {
       printf("es un Sensor\n");
       if(B.Sensor==0)
        {
            printf("es un sensor de Caudal\n");
        }
         if(B.Sensor==1)
        {
            printf("es un sensor de Temperatura\n");
        }
         if(B.Sensor==2)
        {
            printf("es un sensor de Presion\n");
        }
         if(B.Sensor==3)
        {
            printf("es un sensor de nivel\n");
        }
       
    }
    if(B.DT_NL|B.DT_Nh==2)
    {
        printf("es un Actuador");
        if(B.Actuador==0)
        {
            printf("es una valvula");
        }
        if(B.Actuador==1)
        {
            printf("es una Motor");
        }
        
    }
    if(B.DT_NL|B.DT_Nh==3)
    {
        printf("es un Concentrador");
    }
    //---------------------------------------------------
    HD3 C;
    fread(&C,sizeof(uint16_t),1,f);
    printf("Cantidad de dispositivos Superiores [%u]\n",C.UPLD_ID);
    fread(NULL,sizeof(uint16_t),A.LLD,f);
    if(i==2)
    {
        band==1;
    }
   }

}






