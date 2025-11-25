#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MaxTerri 5 
 
struct war
{
    char nome [30];
    char cor [10];
    int tropas;
};
void limparbuff() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

}
int main(){
    struct war territorios [MaxTerri];
   
    
int i = 0;
printf("======================================\n          Bem Vindo a war\n======================================\n");
printf("Vamos Cadastrar os cinco territorios\n\n ");

for (int i = 0; i < MaxTerri; i++)
{



    printf("Digite o nome do territorio %d\n", i+1 );
    fgets(territorios[i].nome,30 , stdin);
    printf("Digite o a cor do exercito\n");
    fgets(territorios[i].cor,10, stdin);
    printf("numero de tropas \n");
    scanf("%d" , &territorios[i].tropas);
        limparbuff();


}

printf("Agora listaremos oque voce escolheu");
for ( i = 0; i < MaxTerri; i++)
{
    printf("==Territorio %d==\n\n",i+1 );
    printf("- Nome do Territorio %s\n", territorios[i].nome);
    printf("- Cor do exercito dominante %s\n",territorios[i].cor);
    printf("- Numero de tropas %d\n", territorios[i].tropas);

}

























}