#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>

#include "lib_mosaico.h"

int main (int argc, char *argv[]) {

	//Parte dos argumentos da função;

	int opcao;

	char *imagem;
	char *diretorio;
	char *nomemosaico;

	int Iflag = 0;
	int Pflag = 0;
	int Oflag = 0;

	opterr = 0;

	while ((opcao = getopt (argc, argv, "i:o:p:")) != -1) {
		
		//Usuário habilita a opção -i, habilita:
		if (opcao == 'i'){
			imagem = optarg;
			Iflag = 1;
		}
		//Usuário habilita a opção -o, habilita:
		else if (opcao == 'o'){
			nomemosaico = optarg;
			Oflag = 1;
		}
		//Usuário habilita a opção -p, habilita:
		else if (opcao == 'p') {
			diretorio = optarg;
			Pflag = 1;
		}
		//Usuário entrou com uma opção que não existe
		else {
			fprintf (stderr, "Opção não identificada\n");
			exit (1);
		}
	
	}
	//Caso o usuário não tenha digitado nenhuma/algumas da opções, pega da entrada padrão, saida padrão
	//e o diretório ./tiles/

	if (!Iflag) {
		imagem = malloc(sizeof(char)*LGR_MAX);
		//fgets (imagem,sizeof(imagem),stdin);
		strcpy(imagem, "/dev/stdin");
		//puts(imagem);
	}
	if (!Pflag) {
		diretorio = malloc(sizeof(char)*MAX); 
		stpcpy (diretorio, "./tiles/");
	}
	if (!Oflag) {
		nomemosaico = malloc(sizeof(char)*LGR_MAX);
		//fgets (nomemosaico,sizeof(imagem),stdout);
		strcpy (nomemosaico, "/dev/stdout");
		//puts(nomemosaico);
	}

	int larB, altB;
	t_tiles *pastilhas;
	t_imagem *foto, *mosaico;

	//Carregando as pastilhas na memória
	pastilhas = openTiles (diretorio);

	//Calculando a cor média de cada bloco de pastilha
	for (int i = 0; i < pastilhas->size; i++) 
		pastilhas->vetor[i].m_pixels = crmdia_bloco (0,0,pastilhas->vetor[i].largura, pastilhas->vetor[i].altura, pastilhas->vetor[i].dado);
	larB = pastilhas->vetor[0].largura;
	altB = pastilhas->vetor[0].altura;

	//Bloco: Lendo a imagem grande
	//inicializando o t_imagem da foto
	foto = initImagem (1);
	readImg (foto, imagem);					//fim do bloco

	//Bloco:Inicializando a imagem mosaico e copiando os dados da foto 
	mosaico = initImagem (1);		
	copiaDados (mosaico,foto);		
	mosaico->dado = allctMat_p (mosaico->largura, mosaico->altura);			//fim do bloco
														
	for (int linharel = 0; linharel < foto->altura; linharel += altB) {
		for (int colunarel = 0; colunarel < foto->largura; colunarel += larB) {
			if ((linharel + altB > foto->altura) && (colunarel + larB > foto->largura)){
				foto->m_pixels = crmdia_bloco (colunarel, linharel, foto->largura - colunarel, foto->altura - linharel, foto->dado);
				cormaisProx (pastilhas,mosaico,foto->m_pixels,colunarel,linharel,foto->largura - colunarel,foto->altura - linharel);
			}
			else if (linharel + altB > foto->altura){
				foto->m_pixels = crmdia_bloco (colunarel, linharel, larB, foto->altura - linharel, foto->dado);
				cormaisProx (pastilhas,mosaico,foto->m_pixels,colunarel,linharel,larB,foto->altura - linharel);
			}
			else if (colunarel + larB > foto->largura){
				foto->m_pixels = crmdia_bloco (colunarel, linharel, foto->largura - colunarel, altB, foto->dado);
				cormaisProx (pastilhas,mosaico,foto->m_pixels,colunarel,linharel,foto->largura - colunarel,altB);
			}
			else {
				foto->m_pixels = crmdia_bloco (colunarel, linharel, larB, altB, foto->dado);
				cormaisProx (pastilhas,mosaico,foto->m_pixels,colunarel,linharel,larB,altB);
			}
		}
	}

	writeImg (nomemosaico, mosaico, pastilhas->vetor[0].tipo);

	//desalocaPas (pastilhas);
	desalocaImg (foto);
	desalocaImg (mosaico);

	return 0;
}
