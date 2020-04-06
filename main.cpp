#include <iostream>
#include <stdio.h>
#include "cstdlib"
#include <string>
#include <time.h>
#include <ctime>
using namespace std;


struct item{
    float peso;
    float beneficio;
    string nome;
};

struct individuo {
    float peso;
    float beneficio;
    int pertinencia[6];
};

//FUNCAO AUXILIAR PARA CALCULAR PESO
float calc_peso(item itens[], individuo ind){
    int i;
    float peso_retorno = 0.0;
    for (i = 0; i < 6; i++){
        if (ind.pertinencia[i] == 1){
            peso_retorno += itens[i].peso;
        }
    }
    return peso_retorno;
}

//FUNCAO AUXILIAR PARA CALCULAR BENEFICIO
float calc_beneficio(item itens[], individuo ind){
    int i;
    float beneficio = 0.0;
    for (i = 0; i < 6; i++){
        if (ind.pertinencia[i] == 1){
            beneficio += itens[i].beneficio;
        }
    }
    return beneficio;
}

// GERAR INDIVIDUO (SOLUCAO) ALEATORIA
individuo gerar_individuo(item itens[]){
    //cria individuo para retornar
    individuo ind;
    ind.peso = 0.0;
    ind.beneficio = 0.0;

    //variável com valor aleatório
    int x = rand()%2;

    //variável para percorrer o laço
    int i;

    //preenche aleatoriamente o vetor de pertinencia e atualiza peso e beneficio total
    for (i = 0; i < 6; i++){
        //srand (time(NULL));
        x = rand()%2;
        ind.pertinencia[i] = x;
    }

    ind.peso = calc_peso(itens, ind);
    ind.beneficio = calc_beneficio(itens, ind);
    //Se peso > limite, atribuir beneficio 0 para penalização
    if (ind.peso > 30.0){
        ind.beneficio = 0.0;
    }

    return ind;
}

//GERAR NOVOS FILHOS ATRAVÉS DA ELITE
void crossover (individuo populacao[], int tamanho, int Y, int Z){
    int pai,mae,moeda;
    for (int filho = Y; filho < 20; filho++){
        pai = rand()% (Y);  //escolhe um pai aleatório entre a elite
        mae = rand()% (Y);  //escolhe uma mãe aleatória entre a elite
        for(int j = 0; j < 6; j++){    //percorre o vetor de pertinência
            moeda = rand()%2;   //moeda para simular cara ou coroa
            if (moeda == 1){    //pega característica do pai
                populacao[filho].pertinencia[j] = populacao[pai].pertinencia[j];
            } else {        //pega caracteristica da mae
                    populacao[filho].pertinencia[j] = populacao[mae].pertinencia[j];
            }
        }
    }
}

//GERAR MUTAÇOES NOS FILHOS DA ELITE
void mutacao(individuo populacao[], int tamanho, int Y, int Z){
    int moeda;
    for (int filho = Y; filho < Y+4 ; filho++){ // os quatro primeiros filhos sofreram mutações (20% da população de tamanho 20)
        for(int j = 0; j < 6; j++){    //percorrer o vertor de pertinência do filho para realizar a mutação
            moeda = rand()%5; //20% de chance de mutar um valor do vetor de pertinência
            if(moeda == 0){   //se decidir mutar...
                if (populacao[filho].pertinencia[j] == 1){  //troca 1 por 0 no vetor de pertinência
                    populacao[filho].pertinencia[j] = 0;
                } else {                                    //senao, troca 0 por 1
                        populacao[filho].pertinencia[j] = 1;
                }
            }
        }
    }
}

//AVALIAR A POPULACAO E ATUALIZAR OS PESOS E BENEFICIOS APÓS CROSSOVER E MUTACAO
void avaliar(individuo populacao[], item itens[]){
    for (int filho = 0; filho < 20; filho++){
        populacao[filho].peso = calc_peso(itens, populacao[filho]);
        populacao[filho].beneficio = calc_beneficio(itens, populacao[filho]);
        if (populacao[filho].peso > 30.0){
            populacao[filho].beneficio = 0.0;
        }
    }
}


// ORDERNAR A POPULACAO PELO BENEFICIO PARA ADQUIRIR A NOVA ELITE DA POPULACAO
void ordenar (individuo populacao[]){
    individuo aux;
    int i, j;

    for(i = 0; i < 20; i++){
        for (j = 0; j < 20; j++){
            if (populacao[i].beneficio > populacao[j].beneficio){
                //trocar de lugar para ordenar
                aux = populacao[i];
                populacao[i] = populacao[j];
                populacao[j] = aux;
            }
        }
    }
}
int main ()
{
    //inicializar random seed
    srand (time(NULL));

    //itens da mochila
    item itens[6];
    itens[0].nome = "Saco de dormir";
    itens[0].peso = 15.00;
    itens[0].beneficio = 15.00;
    itens[1].nome = "Corda";
    itens[1].peso = 3.00;
    itens[1].beneficio = 7.00;
    itens[2].nome = "Canivete";
    itens[2].peso = 2.00;
    itens[2].beneficio = 10.00;
    itens[3].nome = "Tocha";
    itens[3].peso = 5.00;
    itens[3].beneficio = 5.00;
    itens[4].nome = "Garrafa";
    itens[4].peso = 9.00;
    itens[4].beneficio = 8.00;
    itens[5].nome = "Comida";
    itens[5].peso = 20.00;
    itens[5].beneficio = 17.00;
    

    //cria um vetor de individuos para representar a população(

    cout<<"Criando a populacao com soluções aleatórias..."<<endl;
    individuo populacao[20];

    for (int i = 0; i < 20; i++){
        populacao[i] = gerar_individuo(itens);
    }

    cout<<"Ordenando a populacao..."<<endl;

    //ordena essa população aleatória
    ordenar(populacao);

    int Y = 6;      //tamanho da elite
    int Z = 14;     //número de filhos da elite
    int counter = 0;

    //variáveis para controlar a condição de parada
    clock_t inicio, fim;
    float tempo;
    inicio = clock();
    fim = clock();
    tempo = (fim-inicio)/(float)CLOCKS_PER_SEC;

    cout<<"Iniciando o loop para gerar filhos e mutacoes..."<<endl;
    //enquanto o algoritmo tiver rodado por menos de 30 segundos...
    while (tempo < 30.0){

        //crossover
        crossover(populacao, 20, Y, Z);

        //mutacao
        mutacao(populacao, 20, Y, Z);

        //avaliar
        avaliar(populacao, itens);

        //ordenar
        ordenar(populacao);

        //atualizar o tempo
        fim = clock();
        tempo = (fim-inicio)/(float)CLOCKS_PER_SEC;

    }

    //imprimir a melhor solução, ou seja populacao[0]

    cout<<"A melhor solucao possui os seguintes itens..."<<endl;
    for (int i = 0; i < 6 ; i++){
        if (populacao[0].pertinencia[i] == 1){
            cout<< itens[i].nome << "\t  Peso: "<< itens[i].peso << "\t  Beneficio: " << itens[i].beneficio << endl;
        }
    }
    cout<<"Peso Total: "<<populacao[0].peso<<endl;
    cout<<"Beneficio Total: "<<populacao[0].beneficio<<endl;
    return 0;
}
