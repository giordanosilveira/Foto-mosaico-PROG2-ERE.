/*Giordano Henrique Silveira. GRR:20197154*/
#include "lib_mosaico.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>

char * allctMen (int max) {

    char *vetor;

    //Aloca espaço e testa para ver se alocou
    vetor = malloc(sizeof(char)*max);
    if (!vetor) {
        fprintf (stderr, "Não foi possível alocar memória\n");
        exit (1);
    }

    return vetor;

}

t_imagem * allctPas (int n) {

    t_imagem *vetor;

    //Aloca espaço e testa para ver se alocou
    vetor = malloc(sizeof(t_imagem)*n);
    if (!vetor) {
        fprintf (stderr, "Não foi possível alocar memória\n");
        exit (1);
    }

    return vetor;
}

t_imagem * initImagem (int tam) {

    t_imagem * vetor;

    vetor = malloc(sizeof(t_imagem)*tam);

    if (!vetor) {
        fprintf (stderr, "Não foi possível alocar memórias para as imagens das pastilhas\n");
        exit(1);
    }

    return vetor;
}

t_tiles * initTiles (int n, int tam) {

    t_tiles *p;

    //Todo: inicializa o tipo pastilha
    p = malloc(sizeof(t_tiles)*n);
    if (!p) {
        fprintf (stderr, "Não foi possível alocar memória para o tipo pastilha\n");
        exit (1);
    }

    //Inicializa o vetor "tipo imagem das pastilhas"
    p->vetor = initImagem(tam);

    //inicializa a o contador de quantidade de pastilhas
    p->size = ZERO;

    return p;
}

void anlzndo_coments (FILE * file) {

    char c;

    //Enquanto for # vai pegando caractere
    c = getc (file);
    while ((c == '#') || (c == '\n')) {
        while (c != '\n') 
            c = getc (file);
        c = getc (file);
    }

    //quando ele sai ele pega um caractere a mais, devolvo ela para o arquivo
    ungetc (c,file);

}
t_pixel **allctMat_p (int lar, int alt) {

    t_pixel **matriz;

    //Aloca espaço para a matriz e ajeita os ponteiros
    matriz = malloc (alt * sizeof(t_pixel *));
    matriz[0] = malloc (lar * alt * sizeof (t_pixel));
    for (int i= 1; i < alt; i++)
        matriz[i] = matriz[0] + i*lar;

    return matriz;
}
void loadPas (FILE * file, t_tiles * past) {

    int red, green, blue;
    int largura = past->vetor[0].largura; 
    int altura = past->vetor[0].altura;

    //Bloco: aloca espaço para o vetor de cores médias
    //e para a matriz de pixels
    past->vetor[past->size].m_pixels = malloc(sizeof(t_pixel));
    past->vetor[past->size].dado = allctMat_p (largura, altura);

    //Se o tipo da imagem for P3
    if (strcmp(past->vetor[past->size].tipo, "P3") == 0) {
        for (int i = 0; i < altura; i++){
            for (int j = 0; j < largura; j++) {
            
                //Bloco: carrega os pixels em variaveis locais (melhorar isso)
                if ( fscanf(file, "%d %d %d", &red, &green, &blue) != RGB) {
                    fprintf (stderr, "Números de componentes errados\n");
                    exit (1);
                }

                //Bloco: carrega os pixels retirados do arquivo
                //na matriz de pixels
                past->vetor[past->size].dado[i][j].red = red;
                past->vetor[past->size].dado[i][j].green = green;
                past->vetor[past->size].dado[i][j].blue = blue;
            }
        }    
    }
    //Bloco: se for P6, carrega tudo na memória
    if ((strcmp(past->vetor[past->size].tipo, "P6") == ZERO)) {
        if (fread(past->vetor[past->size].dado[0], 3 * largura, altura, file) != altura) {
            fprintf (stderr, "Erro ao carregar a imagem\n");
            exit (1);
        }
    }
}
void readPas (char *fname, t_tiles *past) {

    FILE *file;

    //Abrindo o aquivo corrente
    file = fopen (fname, "r");
    if (!file) {
        perror ("Error");
        fprintf (stderr, "Não foi possível abrir o arquivo\n");
        exit (1);
    }

    //Lendo o tipo da imagem;
    if (!fgets (past->vetor[past->size].tipo, MAX_TYPE + 1, file)) {
        fprintf (stderr, "Não foi possível ler o tipo da imagem\n");
        exit (1);
    }

    //tratando os comentários
    anlzndo_coments (file);

    //Lê o tamanho da imagem
    if (fscanf (file, "%d %d", &past->vetor[past->size].largura, &past->vetor[past->size].altura) != CSIMG) {
        fprintf (stderr, "Números de argumentos inválidos\n");
        exit (1);
    }

    //tratando os comentários
    anlzndo_coments (file);

    //lê o componente rgb
    if (fscanf (file, "%d", &past->vetor[past->size].rgb) != 1) {
        fprintf (stderr, "Números de argumentos inválidos\n");
        exit(1);
    }

    //Checando para ver se o componente RGB é válido
    if (past->vetor[past->size].rgb != COMP_RGB) {
        fprintf (stderr, "Componente inválido\n");
        exit (1);
    }

    //carrega a imagem na memória
    loadPas (file,past);
    fclose (file);
}

t_tiles * openTiles (char * nome_dir) {

    DIR * dir;
    t_tiles *pastilhas;
    struct dirent* file;

    //inicializando o tipo pastilha
    pastilhas = initTiles (1,MAX);

    //Abrindo o diretório e testando para ver se ele foi aberto
    dir = opendir (nome_dir);
    if (!dir) {
        fprintf (stderr, "Não foi possível abrir o diretório\n");
        exit (1);
    }

    //Esse while vai abrir cada arquivo e guardar ele na memória
    //Realoc se necessário a cada MAX pastilhas
    int multiplicador = 1;
    char nome_arq[LGR_MAX];
    while ((file = readdir(dir))) {


        //Teste para ver se não é os diretórios corrente e pai
        if (! strcmp (file->d_name, ".")) 
            continue;
        if (! strcmp (file->d_name, ".."))
            continue;
        
        //Aloca espaço para o vetor nome do arquivo com LGR_MAX posições
        //Torna o aquivo legivel e carrega na memórias todas as imagens
        strcpy (nome_arq, nome_dir);
        strcat (nome_arq, file->d_name);

        readPas (nome_arq, pastilhas);
        pastilhas->size = pastilhas->size + 1;

        //Realoc para o número de pastilhas se necessário
        if (pastilhas->size >= MAX) {
            multiplicador++;
            pastilhas->vetor = realloc(pastilhas->vetor, sizeof(t_imagem) * (MAX * multiplicador));
        }
        
    }
    (void) closedir(dir);
    return pastilhas;
}
t_pixel* crmdia_bloco (int crel, int lrel, int largura, int altura, t_pixel ** mt_pixel) {

    int red, green, blue;
    int mred = 0, mgreen = 0, mblue = 0;
    t_pixel *media;

    media = malloc(sizeof(t_pixel));
    //Bloco anda pelo bloco e calcula a media dos pixels
    //Somatorio dos pixels/quantidade de pixels
    for (int i = lrel; i < altura + lrel; i++) {
        for (int j = crel; j < largura + crel; j++) {
            red = mt_pixel[i][j].red;
            green = mt_pixel[i][j].green;
            blue = mt_pixel[i][j].blue;
            
            mred = mred + red;
            mgreen = mgreen + green;
            mblue = mblue + blue;
        }
    }
    media->red = mred/(altura*largura);
    media->green = mgreen/(altura*largura);
    media->blue = mblue/(altura*largura);               //Fim do bloco

    return media;
}
void loadImg (FILE *file, t_pixel ** matp, char * tipo, int lar, int alt) {
    int red, green, blue;

    //Bloco: se a imagem for tipo P3
    if (strcmp(tipo, "P3") == 0) {
        for (int i = 0; i < alt; i++){
            for (int j = 0; j < lar; j++) {

                //Bloco: carrega o pixel em variáveis locais
                if ( fscanf (file, "%d %d %d", &red, &green, &blue) != RGB) {
                    fprintf (stderr, "Números de componentes errados\n");
                    exit(1);
                }  

                //Bloco: carrega o pixel retirado do arquivo na matriz de pixels
                matp[i][j].red = red;
                matp[i][j].green = green;
                matp[i][j].blue = blue;
            }
        }
    }
    //Bloco: Se for P6 carrega tudo na memória
    else {
        //Carrega em matp[0], 3 * char, altura vezes
        if (fread(matp[0], 3 * lar, alt, file) != alt) {
            fprintf (stderr, "Erro ao carregar a imagem\n");
            exit(1);
        }
    }
}
void readImg (t_imagem * foto, char *arq) {

    FILE *file;

    //Bloco: abrindo a imagem
    //Testando para ver se abriu o arquivo
    file = fopen (arq, "r");
    if (!file) {
        fprintf (stderr, "Não foi possível abrir o arquivo\n");
        exit(1);
    }

    //Bloco: Lendo o tipo da imagem
    if (!fgets(foto->tipo, MAX_TYPE + 1, file)) {
        fprintf (stderr, "Não foi possível ler o tipo da imagem\n");
        exit (1);
    }

    //Tratando os comentários
    anlzndo_coments (file);

    //Lê o tamanho da imagem
    if (fscanf(file, "%d %d", &foto->largura, &foto->altura) != CSIMG) {
        fprintf (stderr, "Não foi possível ler o tamanho da imagem\n");
        exit (1);
    }

    //Tratando os comentários
    anlzndo_coments (file);

    //Lendo o componente rgb e testando
    if (fscanf(file, "%d", &foto->rgb) != 1) {
        fprintf (stderr, "Número de argumentos inválidos\n");
        exit(1);
    }

    //Checando para ver se o componente rgb é válido
    if (foto->rgb != COMP_RGB) {
        fprintf (stderr, "Componente inválido\n");
        exit (1);
    }

    //Carrega a imagem na memória;
    foto->dado = allctMat_p (foto->largura, foto->altura);
    loadImg (file, foto->dado, foto->tipo, foto->largura, foto->altura);
    fclose (file);

}
void copiaDados (t_imagem * destino, t_imagem * origem) {

    //Bloco: copia cada irformação relevante para o destino
    strcpy (destino->tipo, origem->tipo);
    destino->altura = origem->altura;
    destino->largura = origem->largura;
    destino->rgb = origem->rgb;                             //Fim do bloco

}
float calculaCMP (t_pixel *mediaPas, t_pixel * mediaBloco) {
    
    float deltaC, red1, red2, mred;
    float green1, green2;
    float blue1, blue2;

    //char -> float 
    red1 = mediaBloco->red; red2 = mediaPas->red;
    green1 = mediaBloco->green; green2 = mediaPas->green;
    blue1 = mediaBloco->blue; blue2 = mediaPas->blue; 

    //Bloco: quela conta da proximidade de duas cores
    mred = (red1 + red2)/2;
    deltaC = sqrt((2 + mred/COMP_RGBM1)*pow(red2-red1,2)+4*pow(green2-green1,2)+(2 + (COMP_RGB - mred)/COMP_RGBM1)*pow(blue2-blue1,2));
    if (deltaC < 0)
        deltaC = deltaC * (-1);                             //Fim do bloco

    return deltaC;
}
void cormaisProx (t_tiles *pastilhas, t_imagem *mosaico, t_pixel *media, int colunarel, int linharel, int larB, int altB) {

    int indmenor = 0;
    float deltaC, retrn;

    //Inicializando a variavel deltaC com a primeira pastilha
    deltaC = calculaCMP (pastilhas->vetor[0].m_pixels, media);

    //Bloco: Calcula o deltaC de todas as pastilhas e pega o menos
    for (int i = 1; i < pastilhas->size; i++) {
        retrn = calculaCMP (pastilhas->vetor[i].m_pixels, media); 
        if (retrn < deltaC) {
            deltaC = retrn;
            indmenor = i;
        }
    }                                                       //Fim do bloco

    //Bloco: achado o menor deltaC, vai no vetor de pastilhas, pega essa pastilha
    //e copia na matriz de pixel do mosaico
    int i = 0;
    for (int linha = linharel; linha < altB + linharel; linha++) {
        int j = 0;
        for (int col = colunarel; col < larB + colunarel; col++) {
            mosaico->dado[linha][col].red = pastilhas->vetor[indmenor].dado[i][j].red;
            mosaico->dado[linha][col].green = pastilhas->vetor[indmenor].dado[i][j].green;
            mosaico->dado[linha][col].blue = pastilhas->vetor[indmenor].dado[i][j].blue;
            j++;
        }
        i++;
    }                                                      //Fim do bloco
}
void writeImg (char *nomeimg, t_imagem *mosaico, char * tipoP) {

    FILE * file;

    //Bloco: abre o arquivo com o nome desejado   
    file = fopen (nomeimg, "w");
    if (!file) {
        fprintf (stderr, "Não foi possível abrir o arquivo\n");
        exit (1);
    }                                                      //Fim do bloco

    //Bloco: cabeçalho da função
    fprintf (file, "%s\n", mosaico->tipo);

    fprintf (file, "# Autor: Giordano Henrique Silveira\n");
    fprintf (file, "# Não copia comédia\n");

    fprintf (file, "%d %d\n", mosaico->largura, mosaico->altura);//Fim do bloco

    //Se a imagem for p6 printa como raw
    if (strcmp(mosaico->tipo, "P6") == 0){
        if (strcmp(tipoP, "P3")==0){
            fprintf (file, "%d\n  ", COMP_RGB);
            fwrite (mosaico->dado[0], 3 * mosaico->largura, mosaico->altura, file);
        }
        else{
            fprintf (file, "%d", COMP_RGB);
            fwrite (mosaico->dado[0], 3 * mosaico->largura, mosaico->altura, file);
        }
    }
    //Senão printa como ascii
    else{
        fprintf (file, "%d\n", COMP_RGB);
        for (int i = 0; i < mosaico->altura; i++){
            for (int j = 0; j < mosaico->largura; j++)
                fprintf (file, "%d %d %d ", mosaico->dado[i][j].red, mosaico->dado[i][j].green, mosaico->dado[i][j].blue);
        }
    }

    fclose (file);
}
void desalocaPas (t_tiles *pastilhas) {

    int i;

    //Da free na matriz de pixel e no vetor de media do bloco
    for (i = pastilhas->size - 2; i >= 0; i-- ) {
        free (pastilhas->vetor[i].dado[0]);
        free (pastilhas->vetor[i].dado);
        free (pastilhas->vetor[i].m_pixels);
    }

    pastilhas->size = ZERO;
    free (pastilhas);

}
void desalocaImg (t_imagem *imagem) {

    //Da free na matriz de pixel e no vetor de media do bloco e no tipo imagem
    free(imagem->dado[0]);
    free(imagem->dado);
    free(imagem);

}