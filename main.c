#include <stdio.h>
#include <stdlib.h>

// pablo boboca

#define TAMANHO 20

//Variáveis globais para controle exclusivo pelas funções obrigatórias
char mundo[TAMANHO][TAMANHO];
int posicaoRoboX = 19, posicaoRoboY = 19; //Linha X, Coluna Y

//Protótipos das funções abrigatórias

int getRoboPositionY();
int getRoboPositionx(); 
int moveRobo(int moveX, int moveY);



int main(){
    int rodada = 1;
    int encontrouPremio = 0;

    //Carregar o arquivo texto (mundo)
    carregarMundo(mundo);

    printf(" === SIMULACAO DO ROBO === \n\n");

    while(rodada <= 60){

        int moveAtualX = getRoboPositionX();
        int moveAtualY = getRoboPositionY();

        //Determina o próximo movimento
        int proximoMoveX = moveAtualX;
        int proximoMoveY = moveAtualY;

        // Movimento para cima
        if (moveAtualX > 0 && mundo[moveAtualX - 1][moveAtualY] != "*"){
            proximoMoveX = moveAtualX - 1;
        }

    }
}



// Retorna a Linha atual do robô
int getRoboPositionX(){
    return posicaoRoboX;
}

int getRoboPositionY(){
    return posicaoRoboY;
}


//Movimentação do robô para a posição informada
int moveRobo(int moveX, int moveY){
    /*Validação 1: Limites da matriz - 
    Verificar se o robô está dentro do mundo, evitando coordenas além do tamanho da matriz
    */
    if (moveX < 0 || moveX >= TAMANHO || moveY < 0 || moveY >= TAMANHO){
        return 0;
    }

    //Validação 2: Obstáculo - verificar quando o robô encontra o obstáculo
    if (mundo[moveX][moveY] == '*'){
        return 0;
    }

    //Validação 3: Adjacência (garante que o Robô não se mova na diagonal ou pule casas)
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