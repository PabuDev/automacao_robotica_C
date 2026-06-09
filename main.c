#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 20

//Variáveis globais para controle exclusivo pelas funções obrigatórias
char mundo[TAMANHO][TAMANHO];
int posicaoRoboX = 19, posicaoRoboY = 19; //Linha X, Coluna Y

//Protótipos das funções abrigatórias
int moveRobo(int moveX, int moveY);



int main(){
    int rodada = 1;
    int encotrouPremio = 0;

    //Carregar o arquivo texto (mundo)
    carregarMundo(mundo);
}


//Movimentação do robô para a posição informada
int moveRobo(int moveX, int moveY){
    //Validação 1: Limites da matriz
    if (moveX < 0 || moveX >= TAMANHO || moveY < 0 || moveY >= TAMANHO){
        return 0;
    }

    //Validação 2: Obstáculo
    if (mundo[moveY][moveY] == '*'){
        return 0;
    }

    //Validação 3: Adjacência (garante que o Robô não se mova nadiagonal ou pule casa)
    int distanciaX = abs(moveX - posicaoRoboX);
    int distanciaY = abs(moveY - posicaoRoboY);
    if ((distanciaX + distanciaY) != 1){
        return 0;
    }

    //Se passou nas validações, atualiza a posição interna do robô
    posicaoRoboX = moveX;
    posicaoRoboY = moveY;
    return 1;

}