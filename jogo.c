#include <stdio.h>
#include <stdlib.h>
#include <time.h> //aleatoridade
#include <stdbool.h>
#include <ncurses.h>

#define LARGURA 40
#define ALTURA 20 
#define TAMANHO_MAX 100
#define ESPERA_PADRAO 100

typedef struct { 
    int x[TAMANHO_MAX];
    int y[TAMANHO_MAX];
    int comprimento;
    int direcaox;
    int direcaoy;
}Cobra;

typedef struct{
    int x;
    int y;
}Comida;

Cobra cobra; 
Comida comida;
bool gameOver = FALSE;

char telaBuffer[ALTURA][LARGURA];

void gerar_comida();
void inicializar();
void desenhar();
void atualizar_posi();
void processar_entrada();

int main(){
    inicializar();

    while(!gameOver){
        desenhar();
        processar_entrada();
        atualizar_posi();
        napms(ESPERA_PADRAO); //tempo de espera em milisegundos
    }

    endwin(); //limpa a tela 

    return 0;
}

void inicializar(){
    initscr();
    cbreak();
    nodelay(stdscr, TRUE); //serve para a cobra continuar mexendo sozinha
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    cobra.comprimento = 1;
    cobra.x[0] = LARGURA/2; //meio da tela (onde a cobra vai inicializar)
    cobra.y[0] = ALTURA/2; //meio da tela
    cobra.direcaox = 1;
    cobra.direcaoy = 0;

    gerar_comida();

    for(int i=0; i<ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            if(i==0 || i==ALTURA-1 || j==0 || j==LARGURA-1){
                telaBuffer[i][j]='*'; //o limite da tela tera esse caracter
            }else{
                telaBuffer[i][j]=' '; //o resto da tela sera vazio 
            }

        }
    }
    
}

void gerar_comida(){
    //sortear uma posição aleatoria dentro da area de jogo para colocar a comida
    comida.x = rand() % (LARGURA-2) + 1; 
    comida.y = rand() % (ALTURA-2) + 1; // o resto da divisao do numero rand(aleatorio) pela altura/largura-2 vai estar dentro do tamanho da tela,
    // o +1 serve para a comida nao começar no 0, que é a parece
}

void desenhar(){
    //limpar a tela; (cobrinha), sem isso, a cobra anda e a tela vai ficando cheia de @,
    // independente da cobra ter crescido ou nao pegando a fruta, ela cresce infinitamente ate der gameover 
    //(LIMPA O RASTRO DA COBRA)
    for(int i=1; i<ALTURA-1; i++){
        for(int j=1; j<LARGURA-1; j++){ //vai ser de 1 ate a largura/altura -1 para nao apagar a borda junto que vai de 0 ate altura/largura
            telaBuffer[i][j] = ' '; 
        }
    }

    //desenhar comida
    telaBuffer[comida.y][comida.x] = '$';


    //desenhar cobra
    for(int i=0; i<cobra.comprimento; i++){
        int x = cobra.x[i];
        int y = cobra.y[i];
        telaBuffer[y][x] = '@'; //na posição 0 (tamanho=1) da cobra na posicao x y
    }
    
    //desenhar 'cerca' da tela 
    for(int i=0; i<ALTURA; i++){
        for(int j=0; j<LARGURA; j++){
            mvaddch(i, j, telaBuffer[i][j]); //adciona caracter no buffer (virtual)
        }
    }

    refresh(); //sicroniza a tela virtual com a tela fusica (manda as mudanças para o terminal)
}

void atualizar_posi(){
    //vai fazer a cobra se mover
    int novoX = cobra.x[0] + cobra.direcaox;
    int novoY = cobra.y[0] + cobra.direcaoy;

    //detectar a COLISÃO (se a cobra encostou na parede)
    if(novoX<=0 || novoX>=LARGURA-1 || novoY<=0 || novoY>=ALTURA-1){
        gameOver = TRUE;
    }

    //detectar COLISÃO com a COMIDA
    if(novoX==comida.x && novoY==comida.y){
        cobra.comprimento++;
        gerar_comida();
    }


    cobra.x[0] = novoX;
    cobra.y[0] = novoY;
}

void processar_entrada(){
    //entrada do usuario
    int tecla = getch();

    //opcoes de entrada (teclas)
    switch(tecla){
        case KEY_UP:
            if(cobra.direcaoy==0){ //serve para caso a cobra esteja para cima ou para baixo, nao acontece nada, 
                //so caso ela esteja andando para direita ou para esquerda, ai sim ela pode subir ou descer
                cobra.direcaoy = -1;
                cobra.direcaox = 0;
            }
            break;

        case KEY_DOWN:
            if(cobra.direcaoy==0){ //mesma explicacao do if de cima
                cobra.direcaoy=1;
                cobra.direcaox=0;
            }
            break;

        case KEY_LEFT:
            if(cobra.direcaox==0){ //mesma explicacao do primeiro if, 
                //sendo que para direita ou para esquerda
                cobra.direcaox= -1;
                cobra.direcaoy=0;
            }
            break;

        case KEY_RIGHT:
            if(cobra.direcaox==0){ //mesma explicacao do primeiro if, 
                //sendo que para direita ou para esquerda
                cobra.direcaox=1;
                cobra.direcaoy=0;
            }
            break;

        case 'q':
            gameOver=TRUE;
            break;
    }
}