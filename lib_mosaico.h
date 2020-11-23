//Biblioteca do programa mosaico.c
/*Giordano Henrique Silveira. GRR:20197154*/

#ifndef __LBM__
#define __LBM__

#include <stdio.h>
#define MAX 1024		//Controla o tanto de arquivos no diretório
#define LGR_MAX 128 	//O máximo da largura de cada arquivo
#define ZERO 0
#define MAX_TYPE 2
#define CSIMG 2 		//Componentes do tamanho da imagem - largura, altura.
#define RGB 3
#define COMP_RGB 255
#define COMP_RGBM1 256

//Essa struct é referente às cores do pixel
typedef struct {
	unsigned char red, green, blue;
} t_pixel;

//Essa struct contem o tamanho da imagem, a matrix de pixel, o tipo da imagem
//e a cor média dos pixels;
typedef struct {
    t_pixel *m_pixels;				//Media do bloco
	t_pixel **dado;
	char tipo[MAX_TYPE + 1];
	int largura, altura, rgb;
} t_imagem;

//Struct só para os tiles
typedef struct {
	int size;
	t_imagem *vetor;
} t_tiles;

//funcão que aloca memória
char *allctMen (int );
 
//Função q aloca um espaço para as pastilhas
t_imagem * allctPas (int);

//Aloca uma matriz de pixels
//Retorna o ponteiro da matriz
t_pixel ** allctMat_p (int, int);

//Abre o diretório das pastilhas e carrega elas na memórias
//Retorna o ponteiro das pacleastilhas
t_tiles * openTiles (char * );

//Inicia o tipo pastilha
//Retorna o ponteiro da pastilha
t_tiles * initTiles (int , int );

//Inicializa o tipo imagem
//Retorna o ponteiro do vetor de imagem
t_imagem * initImagem (int );

//Trata dos comentários
void anlzndo_coments (FILE *);

//Função que faz todo o trabalho de Analizar, tratar e carregar a imagem na memória
void readPas (char *, t_tiles *);

//Carrega na memórias as pastilhas
void loadPas (FILE *, t_tiles *);

//Função geral para calcular cor média do bloco
//tem como argumento, variaver para guardar cor média, largura, altura e a matriz de pixel
t_pixel* crmdia_bloco (int, int, int, int, t_pixel **);

//Copia as irformações da foto para o mosaico
void copiaDados (t_imagem* destino, t_imagem * origem);

//Função que faz todo o trabalho de Analizar, tratar e carregar a imagem na memória
void readImg (t_imagem *, char *);

//Carrega na memória a imagem
void loadImg (FILE *, t_pixel **, char *, int, int);

//calcula cor mais próxima e copia para a matriz de pixel do mosaico o bloco correspondente
void cormaisProx (t_tiles *, t_imagem *, t_pixel *, int, int, int, int);

//Cria o arquivo e escreve a imagem
void writeImg (char *, t_imagem *, char *);

//Função que da free na area usada pelas pastilhas
void desalocaPas (t_tiles *);

//Função que da free na area usada pelas imagens;
void desalocaImg (t_imagem *);
#endif
