#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Trabalho referente à terceira avaliação da matéria de Algoritmos.
/*Estratégia utilizada: O robô Mila, ao se chocar com um obstáculo, "grava" essa informação em sua memória,
 evitando passar por aquele obstáculo novamente. Realizamos uma segunda matriz para armazenar o valor 
 nela, assim o robô vai "olhar" para ela e vai identificar se tem ou não obstáculo, evitando 
 repetir o erro novamente!
 */

#define TAMANHO 20

// Variáveis globais para controle exclusivo pelas funções obrigatórias
char mundo[TAMANHO][TAMANHO];
char mapaMemoriaRobo[TAMANHO][TAMANHO]; // "Memória" de Mila
int posicaoRoboX = 19, posicaoRoboY = 19; // Linha X, Coluna Y

// Protótipos das funções obrigatórias 

int carregarMundo(char mundo[TAMANHO][TAMANHO]);
void imprimirMundo(char mundo[TAMANHO][TAMANHO], int roboX, int roboY); 
int getRoboPositionY();
int getRoboPositionX(); 
int moveRobo(int moveX, int moveY);

// Funções extras 
void inicializarMemoria();
void imprimirMemoria();

// Variável para mostrar se o robô se movimentou ou não
void imprimirEstatisticas(int totalRodadas, int colisoes);
//Variável para mostrar se o robô se movimentou ou não
int movimentoFeito();
//Colocar um provável if else na parte que for printar que ele conseguiu andar ou não.
/*Nomear um novo int, ex: int mov = movimentoFeito(); pra poder se encaixar no valor da variável e 
ir atualizando, por isso cria-se uma nova.
*/

int main(){
    int rodada = 1;
    int encontrouPremio = 0;
    int colisoes = 0; //Contador para a estatística

    if (!carregarMundo(mundo)){
    printf("Falha ao carregar o mundo, tente novamente!\n");
    return 1;
}
 
    //Inícia a leitura do arquivo do mundo 
    arquivo = fopen("mundo.txt", "r");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo do mundo.\n");
        return 1;
    }

    //Leitura do conteúdo do arquivo 
    for (int i = 0; i < TAMANHO; i++){
        for (int j = 0; j < TAMANHO; j++){
            fscanf(arquivo, " %c", &mundo[i][j]);
        //Verificar caractere 
        if (mundo[i][j] != '_' && mundo[i][j] != '*'){
                printf("O arquivo do mundo contém caracteree inválido!\n");
                fclose(arquivo);
                return 1;
            }
        }
    }

    printf(" === SIMULACAO DO ROBO === \n\n");

    while(rodada <= 80){

        int moveAtualX = getRoboPositionX();
        int moveAtualY = getRoboPositionY();

        // Determina o próximo movimento
        int proximoMoveX = moveAtualX;
        int proximoMoveY = moveAtualY;
        int tentativaValida = 0;

        // Tentar fazer o movimento para cima
        if (moveAtualX > 0 && mapaMemoriaRobo[moveAtualX - 1][moveAtualY] != '1'){
            proximoMoveX = moveAtualX - 1;
            tentativaValida = 1;
        }
        // Tentar o movimento para a esquerda
        else if (moveAtualY > 0 && mapaMemoriaRobo[moveAtualX][moveAtualY - 1] != '1' ){
                proximoMoveY = moveAtualY - 1;
                tentativaValida = 1;
            }
        // Tentar o movimento para a direita
        else if(moveAtualY < TAMANHO - 1 && mapaMemoriaRobo[moveAtualX][moveAtualY + 1] != '1'){
                proximoMoveY = moveAtualY + 1;
                tentativaValida = 1;
        }
        // Tentar o movimento para baixo
        else if (moveAtualX < TAMANHO - 1 && mapaMemoriaRobo[moveAtualX + 1][moveAtualY] != '1'){
                proximoMoveX = moveAtualX + 1;
                tentativaValida = 1;
        }

        // Se o robô encontrou uma direção teoricamente viável 
        if(tentativaValida){
            // Executa a tentativa do movimento
            int sucesso = moveRobo(proximoMoveX, proximoMoveY);
            // Aprendizagem do robô
            if(sucesso == 1){
                // Se mmoveu com sucesso, o robô aprende que o caminho está livre
                mapaMemoriaRobo[proximoMoveX][proximoMoveY] = '0';
                printf("Rodada %d: Moveu com sucesso para (%d, %d)", rodada, proximoMoveX, proximoMoveY);
            }else{
                // Se o movimento falhou, o robô aprende que o caminho tem obstáculo
                mapaMemoriaRobo[proximoMoveX][proximoMoveY] = '1';
                printf("Rodada %d: Bateu na parede em (%d, %d)", rodada, proximoMoveX, proximoMoveY);
                colisoes++;
            }
        }else{
            // Caso o robô se cerque por obstáculos 
            printf("Rodada %d: Cercado por paredes (%d, %d)", rodada, proximoMoveX, proximoMoveY);
        }

        // Exibe o estado visual da simulação 
        imprimirMundo(mundo, getRoboPositionX(), getRoboPositionY());
        imprimirMemoria();
        printf("\n=================================================\n");


        Sleep(1000); // Delay de 1 segundo para visualização da simulação

        // Verifica se o robô alcançou o prêmio na posição (0,0)
        if (getRoboPositionX() == 0 && getRoboPositionY() == 0){
            encontrouPremio = 1;
            break;
        }

        rodada++; 
    }
    // Para exibir a mensagem final do jogo
    if(encontrouPremio){
        printf("\nPARABENS, O ROBO MILA ACANCOU O PREMIO!!!");
    }else {
        printf("\nFIM DE JOGO!! O ROBO NAO CONSEGUIU O PREMIO!!");
        rodada--; //Para ajustar o contador para exibir 40 rodadas
    }
    //Chama a função de relatório da estatística
    imprimirEstatisticas(rodada, colisoes);

    return 0;
}



//Função de Estatísticas
void imprimirEstatisticas(int totalRodadas, int colisoes){
    int descobertas = 0;
    int barreiras = 0;

    //Varrer o Mapa Memória para contar o que o robô aprendeu
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(mapaMemoriaRobo[i][j] == '0'){
                descobertas++;
            }
            if(mapaMemoriaRobo[i][j] == '1'){
                descobertas++;
                barreiras++;
            }
        }
    }

    //Calcular a porcentagem do labirinto total mapeado
    float porcentagem = ((float)descobertas / (TAMANHO * TAMANHO)) * 100;

    printf("\n=======================================");
    printf("\n       RELATORIO DE EFICIENCIA         ");
    printf("\n=======================================");
    printf("\nTotal de rodadas gastas: %d de 40", totalRodadas);
    printf("\nColisoes/Erros registrados: %d", colisoes);
    printf("\nCasas descobertas (Livres): %d", descobertas - barreiras);
    printf("\nObstaculos catalogados: %d", barreiras);
    printf("\nArea total explorada: %d de %d casas (%.2f%%)", descobertas, TAMANHO * TAMANHO, porcentagem);
    printf("\n=======================================\n");
}


// Retorna a Linha atual do robô
int getRoboPositionX() { return posicaoRoboX; }

int getRoboPositionY() { return posicaoRoboY; }

// Função para zerar a memória no início do jogo
void inicializarMemoria(){
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            mapaMemoriaRobo[i][j] = '.'; // Não testado
        }
    }
}

// Imprimindo a memória do robô Mila
void imprimirMemoria(){
    printf("\n === MEMORIA DE APRENDIZADO DO ROBO === \n");
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            printf("%c", mapaMemoriaRobo[i][j]);
        }
    }printf("\n");
}

// Movimentação do robô para a posição informada
int moveRobo(int moveX, int moveY){
    /*Validação 1: Limites da matriz - 
    Verificar se o robô está dentro do mundo, evitando coordenas além do tamanho da matriz
    */
    if (moveX < 0 || moveX >= TAMANHO || moveY < 0 || moveY >= TAMANHO){
        return 0;
    }

    // Validação 2: Obstáculo - verificar quando o robô encontra o obstáculo
    if (mundo[moveX][moveY] == '*'){
        return 0;
    }

    //  Validação 3: Adjacência (garante que o Robô não se mova na diagonal ou pule casas)
    int distanciaX = abs(moveX - posicaoRoboX);
    int distanciaY = abs(moveY - posicaoRoboY);
    if ((distanciaX + distanciaY) != 1){
        return 0;
    }

    // Se passou nas validações, atualiza a posição interna do robô
    posicaoRoboX = moveX;
    posicaoRoboY = moveY;
    return 1;

    }   

