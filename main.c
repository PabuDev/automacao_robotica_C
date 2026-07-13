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
#define ROBO_CHAR "🤖"

// Variáveis globais para controle exclusivo pelas funções obrigatórias
char mundo[TAMANHO][TAMANHO];
char mapaMemoriaFrontalRobo[TAMANHO][TAMANHO]; // "Memória" Frontal de LUZITA
int mapaMemoriaTraseiraRobo[TAMANHO][TAMANHO]; //"Memória" Traseira de LUZITA
int posicaoRoboX = 19, posicaoRoboY = 19; // Linha X, Coluna Y
int ultimaPosicaoRoboX = -1, ultimaPosicaoRoboY = -1; // Variáveis para armazenar as posições anteriores do robô

// Protótipos das funções obrigatórias 

int carregarMundo(char mundo[TAMANHO][TAMANHO]);
void imprimirMundo(char mundo[TAMANHO][TAMANHO], int roboX, int roboY);
void imprimirMemorias(); 
int getRoboPositionY();
int getRoboPositionX(); 
int moveRobo(int moveX, int moveY);

// Funções extras 
void inicializarMemoria();
//void imprimirMemoria();

// Variável para mostrar se o robô se movimentou ou não
void imprimirEstatisticas(int totalRodadas, int colisoes);



int main(){
    int rodada = 1;
    int encontrouPremio = 0;
    int colisoes = 0; //Contador para a estatística

    if (!carregarMundo(mundo)){
        printf("Falha ao carregar o mundo, tente novamente!\n");
        return 1;
    }

    inicializarMemoria();
    SetConsoleOutputCP(CP_UTF8);

    printf(" === SIMULACAO DO ROBO === \n\n");

    while(rodada <= 80){

        int moveAtualX = getRoboPositionX();
        int moveAtualY = getRoboPositionY();

        // Determina o próximo movimento
        int proximoMoveX = moveAtualX;
        int proximoMoveY = moveAtualY;
        int tentativaValida = 0;

        // Prioridade 1: explorar casas ainda desconhecidas, sem voltar para a casa de onde veio
        if (moveAtualX > 0 && mapaMemoriaFrontalRobo[moveAtualX - 1][moveAtualY] == '.'
            && !(moveAtualX - 1 == ultimaPosicaoRoboX && moveAtualY == ultimaPosicaoRoboY)
            && mapaMemoriaTraseiraRobo[moveAtualX - 1][moveAtualY] != 2) {
            proximoMoveX = moveAtualX - 1;
            tentativaValida = 1;
        }
        else if (moveAtualY > 0 && mapaMemoriaFrontalRobo[moveAtualX][moveAtualY - 1] == '.'
            && !(moveAtualX == ultimaPosicaoRoboX && moveAtualY - 1 == ultimaPosicaoRoboY)
            && mapaMemoriaTraseiraRobo[moveAtualX][moveAtualY - 1] != 2) {
            proximoMoveY = moveAtualY - 1;
            tentativaValida = 1;
        }
        else if (moveAtualY < TAMANHO - 1 && mapaMemoriaFrontalRobo[moveAtualX][moveAtualY + 1] == '.'
            && !(moveAtualX == ultimaPosicaoRoboX && moveAtualY + 1 == ultimaPosicaoRoboY)
            && mapaMemoriaTraseiraRobo[moveAtualX][moveAtualY + 1] != 2) {
            proximoMoveY = moveAtualY + 1;
            tentativaValida = 1;
        }
        else if (moveAtualX < TAMANHO - 1 && mapaMemoriaFrontalRobo[moveAtualX + 1][moveAtualY] == '.'
            && !(moveAtualX + 1 == ultimaPosicaoRoboX && moveAtualY == ultimaPosicaoRoboY)
            && mapaMemoriaTraseiraRobo[moveAtualX + 1][moveAtualY] != 2) {
            proximoMoveX = moveAtualX + 1;
            tentativaValida = 1;
        }

        // Prioridade 2: se não houver casa desconhecida, andar para uma casa livre já conhecida
        // Este bloco tenta evitar o retorno imediato à casa anterior e impedir o ciclo em becos sem saída.
        if (!tentativaValida) {
            if (moveAtualX > 0 && mapaMemoriaFrontalRobo[moveAtualX - 1][moveAtualY] == '0'
                && !(moveAtualX - 1 == ultimaPosicaoRoboX && moveAtualY == ultimaPosicaoRoboY)
                && mapaMemoriaTraseiraRobo[moveAtualX - 1][moveAtualY] != 2) {
                proximoMoveX = moveAtualX - 1;
                tentativaValida = 1;
            }
            else if (moveAtualY > 0 && mapaMemoriaFrontalRobo[moveAtualX][moveAtualY - 1] == '0'
                && !(moveAtualX == ultimaPosicaoRoboX && moveAtualY - 1 == ultimaPosicaoRoboY)
                && mapaMemoriaTraseiraRobo[moveAtualX][moveAtualY - 1] != 2) {
                proximoMoveY = moveAtualY - 1;
                tentativaValida = 1;
            }
            else if (moveAtualY < TAMANHO - 1 && mapaMemoriaFrontalRobo[moveAtualX][moveAtualY + 1] == '0'
                && !(moveAtualX == ultimaPosicaoRoboX && moveAtualY + 1 == ultimaPosicaoRoboY)
                && mapaMemoriaTraseiraRobo[moveAtualX][moveAtualY + 1] != 2) {
                proximoMoveY = moveAtualY + 1;
                tentativaValida = 1;
            }
            else if (moveAtualX < TAMANHO - 1 && mapaMemoriaFrontalRobo[moveAtualX + 1][moveAtualY] == '0'
                && !(moveAtualX + 1 == ultimaPosicaoRoboX && moveAtualY == ultimaPosicaoRoboY)
                && mapaMemoriaTraseiraRobo[moveAtualX + 1][moveAtualY] != 2) {
                proximoMoveX = moveAtualX + 1;
                tentativaValida = 1;
            }
        }

        // Prioridade 3: se não houver outra opção, voltar para a casa anterior
        if (!tentativaValida && ultimaPosicaoRoboX >= 0 && ultimaPosicaoRoboY >= 0
            && abs(moveAtualX - ultimaPosicaoRoboX) + abs(moveAtualY - ultimaPosicaoRoboY) == 1) {
            proximoMoveX = ultimaPosicaoRoboX;
            proximoMoveY = ultimaPosicaoRoboY;
            tentativaValida = 1;
        }
    

        // Se o robô encontrou uma direção teoricamente viável 
        if(tentativaValida){
            // Executa a tentativa do movimento
            int sucesso = moveRobo(proximoMoveX, proximoMoveY);
            // Aprendizagem do robô
            if(sucesso){

                //Registra as pegadas na posição antiga de LUZITA
                mapaMemoriaTraseiraRobo[moveAtualX][moveAtualY] = 2;

                //Atualiza o conhecimento de terreno da Memória Frontal
                mapaMemoriaFrontalRobo[moveAtualX][moveAtualY] = '0';

                // Guarda a posição anterior para evitar voltar instantaneamente no próximo passo
                ultimaPosicaoRoboX = moveAtualX;
                ultimaPosicaoRoboY = moveAtualY;

                // ainda a acresentar
                if(mapaMemoriaFrontalRobo[proximoMoveX][proximoMoveY] == '.'){
                    mapaMemoriaFrontalRobo[proximoMoveX][proximoMoveY] = '0';
                }
                // Se moveu com sucesso, o robô aprende que o caminho está livre
                printf("Rodada %d: Moveu com sucesso para (%d, %d)", rodada, proximoMoveX, proximoMoveY);
            }else{
                // Se o movimento falhou, o robô aprende que o caminho tem obstáculo
                mapaMemoriaFrontalRobo[proximoMoveX][proximoMoveY] = '1';
                printf("Rodada %d: Bateu na parede em (%d, %d)", rodada, proximoMoveX, proximoMoveY);
                colisoes++;
            }
        }else{
            // Caso o robô se cerque por obstáculos 
            printf("Rodada %d: Cercado por paredes (%d, %d)", rodada, proximoMoveX, proximoMoveY);
        }

        // Exibe o estado visual da simulação 
        imprimirMundo(mundo, getRoboPositionX(), getRoboPositionY());
        //imprimirMemoria();
        printf("\n=================================================\n");


        Sleep(2000); // Delay de 1 segundo para visualização da simulação

        // Verifica se o robô alcançou o prêmio na posição (0,0)
        if (getRoboPositionX() == 0 && getRoboPositionY() == 0){
            encontrouPremio = 1;
            break;
        }

        rodada++;
        system("cls"); 
    }
    // Para exibir a mensagem final do jogo
    if(encontrouPremio){
        printf("\nPARABENS, A ROBO LUZITA %s ACANCOU O PREMIO!!!", ROBO_CHAR);
    }else {
        printf("\nFIM DE JOGO!! A ROBO %s NAO CONSEGUIU O PREMIO!!", ROBO_CHAR);
        rodada--; //Para ajustar o contador para exibir 40 rodadas
    }
    //imprimirMemorias();
    //Chama a função de relatório da estatística de aprendizagem
    imprimirEstatisticas(rodada, colisoes);

    return 0;
}

//Função para carregar o mundo!
int carregarMundo(char mundo[TAMANHO][TAMANHO]){
    FILE *arquivo = fopen("mundo.txt", "r");

    if (arquivo == NULL){
        return 0;
    }

    for (int i = 0; i < TAMANHO; i++){
        for (int j = 0; j < TAMANHO; j++){
            if (fscanf(arquivo, " %c", &mundo[i][j]) != 1){
                fclose(arquivo);
                return 0;
            }

            if (mundo[i][j] != '_' && mundo[i][j] != '*'){
                fclose(arquivo);
                return 0;
            }
        }
    }

    fclose(arquivo);
    return 1;
}
//Função para imprimir o mundo
void imprimirMundo(char mundo[TAMANHO][TAMANHO], int roboX, int roboY){
    printf("\n=== MAPA ===\n");
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(i == roboX && j == roboY){
                printf("%s", ROBO_CHAR);
            } else {
                printf("%c", mundo[i][j]);
            }
        }
        printf("\n");
    }
}

// Retorna a Linha atual do robô
int getRoboPositionX() { return posicaoRoboX; }

int getRoboPositionY() { return posicaoRoboY; }

// Função para zerar a memória no início do jogo
void inicializarMemoria(){
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            mapaMemoriaFrontalRobo[i][j] = '.'; // Não testado
            mapaMemoriaTraseiraRobo[i][j] = 0; // Nenhuma casa visita
        }
    }printf("\n");
}

// Imprimindo a memória do robô LUZITA
void imprimirMemorias(){
    printf("\n === MEMORIA DE APRENDIZADO DO ROBO === \n");
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(i == posicaoRoboX && j == posicaoRoboY){
                printf("%s ", ROBO_CHAR); // A posição atual é prioridade na visualização
            }else if(mapaMemoriaFrontalRobo[i][j] == '1'){
                printf("1 "); // Se for barreira, irá exibir "1"
            }else if(mapaMemoriaTraseiraRobo[i][j] == 2){
                printf("2 "); // Se já passou, irá exibir a pegada "2"
            }else{
                printf("%c ", mapaMemoriaFrontalRobo[i][j]); // Exibe "0" ou "."
            }
        }
    }printf("\n");
}

//Função de Estatísticas
void imprimirEstatisticas(int totalRodadas, int colisoes){
    int descobertas = 0;
    int barreiras = 0;

    //Varrer o Mapa Memória para contar o que o robô aprendeu
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(mapaMemoriaFrontalRobo[i][j] == '0'){
                descobertas++;
            }
            if(mapaMemoriaFrontalRobo[i][j] == '1'){
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
    printf("\nTotal de rodadas gastas: %d de 80", totalRodadas);
    printf("\nColisoes/Erros registrados: %d", colisoes);
    printf("\nCasas descobertas (Livres): %d", descobertas - barreiras);
    printf("\nObstaculos catalogados: %d", barreiras);
    printf("\nArea total explorada: %d de %d casas (%.2f%%)", descobertas, TAMANHO * TAMANHO, porcentagem);
    printf("\n=======================================\n");
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

