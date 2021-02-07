//Projeto Final Miguel Bragança Gama UC19202031
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct posicao{
	int linha;
	int coluna;
}posicao;

typedef struct barco{
	posicao coord[5];
	int	tam;
	int partes_restantes;
	int afundado;
	char nome[20];
}barco;

typedef struct usuario{
	int total_navios;
	int tiros;
	int acertos;
	char nome[20];
	barco navios[10];
}usuario;

typedef struct mapa{
	char grid[10][10];
}mapa;

typedef struct rank{
	int vitorias;
	int partidas;
	float pontos;
	char nome[20];
}rank;

//mostra o menu para o usuario e recebe a escolha
void mostra_menu(int jogos_salvos);

//lê um arquivo com dados para continuar um jogo previamente salvo
void continua_jogo();

//um novo jogo é feito (um jogo antes salvo é perdido)
void novo_jogo();

//recebe nome dos jogadores para o jogo
void nome_jogador(usuario *jogador);

//inicia o mapa para futura edição
void gera_mapa(usuario *jogador, mapa *mar, mapa *tela);

//inicia os navios
void inicia_navios(usuario *jogador);

//adiciona navios no grid tela
int adicionar_navios(usuario *jogador,mapa *tela);

//mostra a tela para auxiliar no posicionamento de navios
void mostra_tela(usuario *jogador,mapa *tela,int i);

//inicia a tela auxiliar que serve como tela pré confirmação, caso o jogador mude de ideia
void inicia_tela_aux(char tela_aux[10][10], mapa *tela,int i);

//mostra a tela auxiliar antes de alterar a tela principal
void mostra_tela_aux(char tela_aux[10][10]);

//essa função é uma gerencia macro de como o posicionamento funciona
void posiciona_navio(usuario *jogador,int i,mapa *tela);

//escolhe um barco para posicionar e mostra o status dos barcos
int escolhe_barco(usuario *jogador, int i);

//escolhe cordenadas, confirma e valida a confirmação, alem de passar a tela auxiliar para a tela que sera usada no jogo	
int escolhe_coordenadas(usuario *jogador,mapa *tela,int i,int barco);

//o jogo em si
int jogo(usuario *jogador,mapa *tela,mapa *mar,int turno);

//mostra a tela que o jogador deve atirar
void mostra_mar(usuario *jogador,mapa *mar,int i);

//a validação do tiro e as concequencias
void tiro(int linha,int coluna,usuario *jogador,mapa *tela,mapa *mar,int i);

//essa função verifica se o jogo terminou
int valida_vencedor(usuario *jogador);

//guarda um jogo não terminado em um arquivo
void salva_jogo(usuario *jogador,mapa *tela,mapa *mar,int turno);

//guarda os dados de um jogo terminado em um arquivo
void registra_vencedor(usuario *jogador,int vencedor,int perdedor);

//mostra um ranking de jogadores e suas vitorias e derrotas
void ranking();
	
int main(){
	int executa,jogos_salvos=0;
	char confirma;
	FILE *arq;
	
	arq = fopen("jogo_salvo","rb");
	assert(arq != NULL);
	fscanf(arq,"%i",&jogos_salvos);
	fclose(arq);
	
	setlocale(LC_ALL,"portuguese");
	
	if(jogos_salvos>0){
		do{
			mostra_menu(jogos_salvos);
			scanf("%i",&executa);
			switch(executa){
			 	case 1:
			 		continua_jogo();
			 		break;
			 	case 2:
			 		printf("Se voce começar outro jogo, o jogo anterior sera perdido \ndeseja proceder mesmo assim?(y/n)");
			 		fflush(stdin);
			 		confirma = getchar();
			 		if(confirma=='y' || confirma=='Y'){
			 			novo_jogo(jogos_salvos);
					 }
			 		break;
				case 3:
					ranking();
					break;
				case 4:
					break;
				default:
					printf("numero informado é invalido");
					
			}
		}while(executa!=4);
	}else{
		do{
			mostra_menu(jogos_salvos);
			scanf("%i",&executa);
			switch(executa){
			 	case 1:
			 		novo_jogo(jogos_salvos);
			 		break;
				case 2:
					ranking();
					break;
				case 3:
					break;
				default:
					printf("numero informado é invalido");	
			 }
		 }while(executa!=3);
		}
	return 0;
		
}

void mostra_menu(int jogos_salvos){
	system("cls");
	if(jogos_salvos>0){
		printf("\n\n\n\t\t\t______________________________________________________________");
		printf("\n\t\t\t|       __                                                   |");
		printf("\n\t\t\t|      |  %c        _______                                   |",92);
		printf("\n\t\t\t|      |__/    /%c     |      /%c     |     |   |    /%c        |",92,92,92);
		printf("\n\t\t\t|      |  %c   /__%c    |     /__%c    |     |---|   /__%c       |",92,92,92,92);
		printf("\n\t\t\t|      |__/  /    %c   |    /    %c   |___  |   |  /    %c      |",92,92,92);
		printf("\n\t\t\t|                   |%c  |                                    |",92);
		printf("\n\t\t\t|                   | %c |  /%c  %c  /  /%c  |                   |",92,92,92,92);
		printf("\n\t\t\t|                   |  %c| /--%c  %c/  /--%c |__                 |",92,92,92,92);
		printf("\n\t\t\t|____________________________________________________________|");
		printf("\n\t\t\t\t     |      1-Continuar jogo salvo     |");
		printf("\n\t\t\t\t     |      2-Novo jogo                |");
		printf("\n\t\t\t\t     |      3-Ranking                  |");
		printf("\n\t\t\t\t     |      4-fechar jogo              |");
		printf("\n\t\t\t\t     |_________________________________|");
	}else{
		printf("\n\n\n\t\t\t______________________________________________________________");
		printf("\n\t\t\t|       __                                                   |");
		printf("\n\t\t\t|      |  %c        _______                                   |",92);
		printf("\n\t\t\t|      |__/    /%c     |      /%c     |     |   |    /%c        |",92,92,92);
		printf("\n\t\t\t|      |  %c   /__%c    |     /__%c    |     |---|   /__%c       |",92,92,92,92);
		printf("\n\t\t\t|      |__/  /    %c   |    /    %c   |___  |   |  /    %c      |",92,92,92);
		printf("\n\t\t\t|                   |%c  |                                    |",92);
		printf("\n\t\t\t|                   | %c |  /%c  %c  /  /%c  |                   |",92,92,92,92);
		printf("\n\t\t\t|                   |  %c| /--%c  %c/  /--%c |__                 |",92,92,92,92);
		printf("\n\t\t\t|____________________________________________________________|");
		printf("\n\t\t\t\t     |          1-Novo jogo            |");
		printf("\n\t\t\t\t     |          2-Ranking              |");
		printf("\n\t\t\t\t     |          3-fechar jogo          |");
		printf("\n\t\t\t\t     |_________________________________|");
	}
}

void continua_jogo(){
	usuario jogador[2];
	mapa mar[2];
	mapa tela[2];
	int vencedor,aux,i,j,k,turno;
	FILE *arq;
	//leitura do jogo salvo
	arq = fopen("jogo_salvo.txt", "rb");
	assert(arq != NULL);
	fscanf(arq,"%i\n",&aux);
	fscanf(arq,"%i\n",&turno);
	for(i=0;i<2;i++){
		fscanf(arq,"%[^|]",&jogador[i].nome);
		fscanf(arq,"|%i",&jogador[i].acertos);
		fscanf(arq,"|%i",&jogador[i].tiros);
		fscanf(arq,"|%i",&jogador[i].total_navios);
		
		for(j=0;j<10;j++){
			fscanf(arq,"|%i",&jogador[i].navios[j].afundado);
			fscanf(arq,"|%[^|]",&jogador[i].navios[j].nome);
			fscanf(arq,"|%i",&jogador[i].navios[j].tam);
			fscanf(arq,"|%i",&jogador[i].navios[j].partes_restantes);
			for(k=0;k<5;k++){
				fscanf(arq,"|%i|%i",&jogador[i].navios[j].coord[k].linha,&jogador[i].navios[j].coord[k].coluna);
			}
			fscanf(arq,"|");
			for(k=0;k<10;k++){
				tela[i].grid[j][k] = fgetc(arq);
			}
			for(k=0;k<10;k++){
				mar[i].grid[j][k] = fgetc(arq);
			}
		}
	}
	fclose(arq);
	
	//o jogo em si
	vencedor=jogo(jogador,tela,mar,turno);
	if(vencedor==1){
		registra_vencedor(jogador,vencedor,0);
	}else{
		if(vencedor==0){
			registra_vencedor(jogador,vencedor,1);
		}
	}
}

void novo_jogo(){
	usuario jogador[2];
	mapa mar[2];
	mapa tela[2];
	int vencedor;
	
	
	nome_jogador(jogador);
	gera_mapa(jogador,mar,tela);
	inicia_navios(jogador);
	vencedor = adicionar_navios(jogador,tela);
	if(vencedor !=2){
		vencedor = jogo(jogador,tela,mar,0);
		if(vencedor==1){
			registra_vencedor(jogador,vencedor,0);
		}else{
			if(vencedor==0){
				registra_vencedor(jogador,vencedor,1);
			}
		}
		
	}
}

void nome_jogador(usuario *jogador){
	char nome[25];
	int confirma,i=0;
	system("cls");
	do{		
		printf("Informe o nome do jogador %i:",i+1);
		fflush(stdin);
		gets(nome);
		if(strcmp(nome," ")<=0 && strcmp(nome,jogador[0].nome)==0){
			system("cls");
			printf("nome invalido\n");
		}else{
			strcpy(jogador[i].nome,nome);
			i=i+1;
		}	
	}while(i<2);
	system("cls");
}

void gera_mapa(usuario *jogador, mapa *mar,mapa *tela){
	int i,j,k;
	for(i=0;i<2;i++){
		for(j=0;j<10;j++){
			for(k=0;k<10;k++){
				mar[i].grid[j][k]=putchar('~');
				tela[i].grid[j][k]=putchar('O');
			}
		}system("cls");
	}			
}

void inicia_navios(usuario *jogador){
	int i,j;
	
	for(i=0;i<2;i++){
		jogador[i].tiros=0;
		jogador[i].acertos=0;
		for(j=0;j<10;j++){
			switch(j){
				case 0:
					jogador[i].navios[j].tam=5;
					jogador[i].navios[j].partes_restantes=5;
					jogador[i].navios[j].afundado=0;
					strcpy(jogador[i].navios[j].nome,"Porta-aviões");
					break;
				case 1:
				case 2:
					jogador[i].navios[j].tam=4;
					jogador[i].navios[j].partes_restantes=4;
					jogador[i].navios[j].afundado=0;
					strcpy(jogador[i].navios[j].nome,"Encouraçado");
					break;
				case 3:
				case 4:
				case 5:
					jogador[i].navios[j].tam=3;
					jogador[i].navios[j].partes_restantes=3;
					jogador[i].navios[j].afundado=0;
					strcpy(jogador[i].navios[j].nome,"Cruzado");
					break;
				case 6:
				case 7:
				case 8:
				case 9:
					jogador[i].navios[j].tam=2;
					jogador[i].navios[j].partes_restantes=2;
					jogador[i].navios[j].afundado=0;
					strcpy(jogador[i].navios[j].nome,"Submarinos");
					break;
			}
		}
	}
}

int adicionar_navios(usuario *jogador,mapa *tela){
	int i;
	char confirma='n';
	
	for(i=0;i<2;i++){
		jogador[i].total_navios=0;
		do{
			system("cls");
			printf("Faltam %i\n%s, posicione seus navios:\n\n",10-jogador[i].total_navios,jogador[i].nome);
			posiciona_navio(jogador,i,tela);
			system("cls");
			mostra_tela(jogador,tela,i);
			//erro que eu não vou ter tempo para concertar até o envio (esse if não esta propriamente)
			if(jogador[i].total_navios<=9){
				jogador[i].total_navios=jogador[i].total_navios+1;
			}
			printf("\n\t\tDeseja terminar a edição?(y/n)");
				fflush(stdin);
				confirma=getchar();
			if(jogador[i].total_navios<9 && (confirma=='y' || confirma=='Y')){
				printf("Sair agora resultara na perda de todo o processo. Tem certeza que deseja sair? (y/n)");
				fflush(stdin);
				confirma=getchar();
				if(confirma=='y' || confirma=='Y'){
					return 2;
				}
			}
		}while(jogador[i].total_navios<10 && (confirma!='y' || confirma !='Y'));
	}	
}

void mostra_tela(usuario *jogador,mapa *tela,int i){
	int k,j,pos,confirma;
	system("cls");
	printf("\n\n");
	printf("\t\t\t    0  1  2  3  4  5  6  7  8  9");
	for(k=0;k<10;k++){
		printf("\n");
		printf("\t\t\t %c ",k+65);
		for(j=0;j<10;j++){
			printf(" %c ",tela[i].grid[k][j]);
		}	
	}
	printf("\n\n");
}

void inicia_tela_aux(char tela_aux[10][10], mapa *tela,int i){
	int k,j,confirma;
	for(k=0;k<10;k++){
		for(j=0;j<10;j++){
			tela_aux[j][k]=putchar(tela[i].grid[j][k]);
		}	
	}
	system("cls");
}

void mostra_tela_aux(char tela_aux[10][10]){
	int k,j,pos,confirma;
	printf("\n\n");
	printf("\t\t\t    0  1  2  3  4  5  6  7  8  9");
	for(k=0;k<10;k++){
		printf("\n");
		printf("\t\t\t %c ",k+65);
		for(j=0;j<10;j++){
			printf(" %c ",tela_aux[k][j]);
		}	
	}
	printf("\n\n");
}

void posiciona_navio(usuario *jogador,int i,mapa *tela){
	int j,barco;
	int confirma=0;
	
	
	barco = escolhe_barco(jogador,i);
	do{
		system("cls");
		confirma = escolhe_coordenadas(jogador,tela,i,barco);
		if(confirma==2){
			system("cls");
			printf("Escolha um espaço");
		}
	}while(confirma!=1);
}

int escolhe_barco(usuario *jogador,int i){
	int j;
	int barco;
	do{
		//mostra para o usuario os navios restantes
		for(j=0;j<10;j++){
			if(jogador[i].navios[j].afundado==0){
				printf("Não posicionado");
			}else{
				printf("Posicionado");
			}
				printf("\tNavio:%15s  ",jogador[i].navios[j].nome);
				printf("Tamanho:%i espaços  ",jogador[i].navios[j].tam);
				printf("Numero:%i\n",j);
				printf("---------------------------------------------\n");
		}
		//escolhe um navio e devolve para a função
		printf("\n\nEscolha um navio:");
		scanf("%i",&barco);
		if(barco<0 || barco>9){
			system("cls");
			printf("numero informado não é valido");
		}
	}while(barco<0 || barco>9 );
	return barco;
}
/*
	ESSA FUNÇÃO ESTA GIGANTE POR QUE EU TIVE MUITOS PROBLEMAS PASSANDO STRUCTS DESSA FUNÇÃO ESPECIFICA PARA OUTRAS, E COMO 
	EU NÃO ACHEI RESPOSTAS RESOUVI POUPAR O COMPILADOR DESSA. MAS EU FIZ O POSSIVEL PARA "PSEUDO FUNCIONALIZAR A FUNÇÃO"
*/
int escolhe_coordenadas(usuario *jogador,mapa *tela,int i,int barco){
	char linha_char;
	int linha,coluna,direcao;
	int j,k,confirma=0;
	char tela_aux[10][10];
	//iniciar o grid auxiliar 
	if(jogador[i].navios[barco].afundado==1){
		for(j=0;j<jogador[i].navios[barco].tam;j++){
			tela[i].grid[jogador[i].navios[barco].coord[j].linha][jogador[i].navios[barco].coord[j].coluna]=putchar('O');			
		}
	}
	inicia_tela_aux(tela_aux,tela,i);
	j=0;
	//recebe e valida as coordenadas da posição do 1° espaço do barco
	// valida_coordenadas();
	do{
		system("cls");
		mostra_tela_aux(tela_aux);
		printf("\n Informe a linha desejada(LETRA): ");
		fflush(stdin);
		scanf("%c",&linha_char);
		printf("\n Informe a coluna desejada(NUMERO): ");
		scanf("%i",&coluna);
		switch(linha_char){
				case 'A':
				case 'a':
					linha=0;
					break;	
				case 'B':
				case 'b':
					linha=1;
					break;
				case 'C':
				case 'c':
					linha=2;
					break;
				case 'D':
				case 'd':
					linha=3;
					break;
				case 'E':
				case 'e':
					linha=4;
					break;
				case 'F':
				case 'f':
					linha=5;
					break;
				case 'G':
				case 'g':
					linha=6;
					break;
				case 'H':
				case 'h':
					linha=7;
					break;
				case 'I':
				case 'i':
					linha=8;
					break;
				case 'J':
				case 'j':
					linha=9;
					break;	
			}
		if(linha<0 || linha>9 || coluna<0 || coluna>9){
			system("cls");
			printf("coordenada submetida invalida! por favor tente novamente");
			confirma=0;
		}else{
			if(tela_aux[linha][coluna]=='X'){
				system("cls");
				printf("coordenada submetida ja ocupada! por favor tente novamente");
				confirma=0;
			}else{
				confirma=1;
			}
		}
	}while(confirma!=1);
	
	//reset da variavel que faz a validação
	confirma=0;
	
	//Mostra as direções possiveis para o jogador(sem validação)
	//mostra_direções();
	tela_aux[linha][coluna]= putchar('X');
	if(tela_aux[linha-1][coluna]=='O'){
		tela_aux[linha-1][coluna]= putchar('1');
	}
	
	if(tela_aux[linha][coluna+1]=='O'){
		tela_aux[linha][coluna+1]= putchar('2');
	}
	
	if(tela_aux[linha+1][coluna]=='O'){
		tela_aux[linha+1][coluna]= putchar('3');
	}
	
	if(tela_aux[linha][coluna-1]=='O'){
		tela_aux[linha][coluna-1]= putchar('4');
	}
	system("cls");
	
	//recebe e valida a direção que o jogador quer colocar seu navio
	//le_valida_direção();
	do{
		confirma=0;
		mostra_tela_aux(tela_aux);
		printf("\n informe a direção que o %s vai ser posicionado",jogador[i].navios[barco].nome);
		scanf("%i",&direcao);
		if(tela_aux[linha-1][coluna]=='1'&& direcao==1){
			confirma=1;
		}else{
			if(tela_aux[linha][coluna+1]=='2'&& direcao==2){
				confirma=1;
			}else{
				if(tela_aux[linha+1][coluna]=='3'&& direcao==3){
					confirma=1;
				}else{
					if(tela_aux[linha][coluna-1]=='4'&& direcao==4){
						confirma=1;
					}else{
						system("cls");
						printf("direção submetida invalida! por favor tente novamente");
						
					}	
				}	
			}
		}	
	}while(confirma!=1);
	
	//reset da variavel que faz a validação
	confirma=0;
	
	//reset da posição inicialmente escolhida (estava dando um warning ter X na posição inicial) que faz a validação
	tela_aux[linha][coluna]= putchar('O');
	
	//coloca 'X' na posição que o navio deve ter
	//posiciona_navio_mar();
	switch(direcao){
		case 1:
			for(j=linha;j>linha-jogador[i].navios[barco].tam;j--){
				if(tela_aux[j][coluna]=='X' || j<0){
					printf("Posição não valida");
					return 2;
				}
				tela_aux[j][coluna] = putchar('X');
				jogador[i].navios[barco].coord[linha-j].linha=j;
				jogador[i].navios[barco].coord[linha-j].coluna=coluna;
			}
			break;
		case 2:
			for(j=coluna;j<coluna+jogador[i].navios[barco].tam;j++){
				if(tela_aux[linha][j]=='X' || j>9){
					printf("Posição não valida");
					return 2;
				}
				tela_aux[linha][j] = putchar('X');
				jogador[i].navios[barco].coord[j-coluna].linha=linha;
				jogador[i].navios[barco].coord[j-coluna].coluna=j;
			}
			break;
		case 3:
			for(j=linha;j<linha+jogador[i].navios[barco].tam;j++){
				if(tela_aux[j][coluna]=='X' || j>9){
					printf("Posição não valida");
					return 2;
				}
				tela_aux[j][coluna] = putchar('X');
				jogador[i].navios[barco].coord[j-linha].linha=j;
				jogador[i].navios[barco].coord[j-linha].coluna=coluna;
			}
			break;
		case 4:
			for(j=coluna;j>coluna-jogador[i].navios[barco].tam;j--){
				if(tela_aux[linha][j]=='X' || j<0){
					printf("Posição não valida");
					return 2;
				}
				tela_aux[linha][j] = putchar('X');
				jogador[i].navios[barco].coord[coluna-j].linha=linha;
				jogador[i].navios[barco].coord[coluna-j].coluna=j;
			}
			break;
	}
	
	//pedindo a confirmação do jogador para caso ele mude de ideia sobre a posição do navio
	//confirma_posição();
	system("cls");
	mostra_tela_aux(tela_aux);
	printf("deseja confirmar essa mudança (1-sim, 0-não)");
	scanf("%i",&confirma);
	if(confirma==1){
		jogador[i].navios[barco].afundado=1;
		for(j=0;j<10;j++){
			for(k=0;k<10;k++){
				if(tela_aux[j][k]!='X' && tela_aux[j][k]!='O'){
					tela_aux[j][k]=putchar('O');
					system("cls");
				}
				tela[i].grid[j][k]=putchar(tela_aux[j][k]);
			}
		}
	}
	return confirma;
}

int jogo(usuario *jogador,mapa *tela,mapa *mar,int turno){
	int linha,coluna,vencedor=3;
	char pausa;
	
	do{
		tiro(linha,coluna,jogador,tela,mar,turno);
		system("cls");
		printf("Turno do jogador: %s \t Seus navios restantes:%i",jogador[turno].nome,jogador[turno].total_navios);
		mostra_mar(jogador,mar,turno);
		vencedor = valida_vencedor(jogador);
		if(vencedor==3){
			printf("\n(|enter- para continuar |p- para pausar)");
			fflush(stdin);
			pausa=getchar();
		}
		//checa se o jogo está em andamento ou alguem ganhou ou o jogo foi parado
		if(vencedor!=3){
			return vencedor;
		}
		
		//passa o turno de um jogador para o outro
		if(turno==0){
			turno=1;
		}else{
			turno=0;
		}
		
		system("cls");
		if(pausa=='p'||pausa=='P'){
			vencedor=2;
		}
	}while(vencedor==3);
	
	
	
	if(vencedor==2){
		salva_jogo(jogador,tela,mar,turno);
	}
}

void mostra_mar(usuario *jogador,mapa *mar,int i){
	int k,j,pos,confirma;
	system("cls");
	printf("Turno do jogador: %s \t Seus navios restantes:%i",jogador[i].nome,jogador[i].total_navios);
	printf("\n\n");
	printf("\t\t\t    0  1  2  3  4  5  6  7  8  9");
	for(k=0;k<10;k++){
		printf("\n");
		printf("\t\t\t %c ",k+65);
		for(j=0;j<10;j++){
			printf(" %c ",mar[i].grid[k][j]);
		}	
	}
	printf("\n\n");
}

void tiro(int linha,int coluna,usuario *jogador,mapa *tela,mapa *mar,int i){
	int j,k,l;
	int confirma;
	char linha_char;
	
	//recebe as coordenadas do tiro que o jogador i fara
	do{
		mostra_mar(jogador,mar,i);
		printf("\n Informe a linha desejada(LETRA): ");
		fflush(stdin);
		scanf("%c",&linha_char);
		printf("\n Informe a coluna desejada(NUMERO): ");
		scanf("%i",&coluna);
		switch(linha_char){
				case 'A':
				case 'a':
					linha=0;
					break;	
				case 'B':
				case 'b':
					linha=1;
					break;
				case 'C':
				case 'c':
					linha=2;
					break;
				case 'D':
				case 'd':
					linha=3;
					break;
				case 'E':
				case 'e':
					linha=4;
					break;
				case 'F':
				case 'f':
					linha=5;
					break;
				case 'G':
				case 'g':
					linha=6;
					break;
				case 'H':
				case 'h':
					linha=7;
					break;
				case 'I':
				case 'i':
					linha=8;
					break;
				case 'J':
				case 'j':
					linha=9;
					break;	
					
				default:
					linha=-1;
			}
		if(linha<0 || linha>9 || coluna<0 || coluna>9){
			system("cls");
			printf("coordenada submetida invalida! por favor tente novamente");
			confirma=0;
		}else{
			if(mar[i].grid[linha][coluna]=='X' || mar[i].grid[linha][coluna]=='O' || mar[i].grid[linha][coluna]=='D'){
				system("cls");
				printf("coordenada submetida ja ocupada! por favor tente novamente");
				confirma=0;
			}else{
				jogador[i].tiros++;
				confirma=1;
			}
		}
	}while(confirma!=1);
	
	//a computação do tiro na tela
	if(i==0){
		mar[i].grid[linha][coluna]=putchar(tela[1].grid[linha][coluna]);
			if(tela[1].grid[linha][coluna]=='X'){
				jogador[i].acertos++;
				for(j=0;j<10;j++){
					for(k=0;k<jogador[i].navios[j].tam;k++){
						if(jogador[i].navios[j].coord[k].linha==linha && jogador[i].navios[j].coord[k].coluna==coluna){
							jogador[i].navios[j].partes_restantes=jogador[i].navios[j].partes_restantes-1;
							if(jogador[i].navios[j].partes_restantes==0){
								jogador[i].total_navios--;
								jogador[i].navios[j].afundado=0;
								for(l=0;l<jogador[i].navios[j].tam;l++){
									mar[i].grid[jogador[i].navios[j].coord[l].linha][jogador[i].navios[j].coord[l].coluna]=putchar('D');
								}
								
							}
							break;
							break;
						}
					}
				}
			}
	}else{
		mar[i].grid[linha][coluna]=putchar(tela[0].grid[linha][coluna]);
		if(tela[0].grid[linha][coluna]=='X'){
			for(j=0;j<10;j++){
				for(k=0;k<jogador[i].navios[j].tam;k++){
					if(jogador[i].navios[j].coord[k].linha==linha && jogador[i].navios[j].coord[k].coluna==coluna){
						jogador[i].navios[j].partes_restantes=jogador[i].navios[j].partes_restantes-1;
						if(jogador[i].navios[j].partes_restantes==0){
							jogador[i].total_navios--;
							jogador[i].navios[j].afundado=0;
							for(l=0;l<jogador[i].navios[j].tam;l++){
								mar[i].grid[jogador[i].navios[j].coord[l].linha][jogador[i].navios[j].coord[l].coluna]=putchar('D');
							}						
						}
						break;
					}
				}
			}
		}
	}
}

int valida_vencedor(usuario *jogador){
	int j;
	for(j=0;j<2;j++){
		if(jogador[j].total_navios==0){
			system("cls");
			printf("\n\n\t\t%s Venceu a partida!",jogador[j].nome);
			fflush(stdin);
			getchar();
			return j;
		}
	}
	return 3;
	
}

void salva_jogo(usuario *jogador,mapa *tela,mapa *mar,int turno){
		int i,j,k;
		FILE *arq;
		
		//abre o arquivo em binario
		arq = fopen("jogo_salvo", "wb");
		assert(arq != NULL);
		//transforma a variavel que lê se tem jogos salvos de false para true
		fprintf(arq,"%i\n",1);
		//salva o restante das variaveis pertinentes à partida
		fprintf(arq,"%i\n",turno);
		for(i=0;i<2;i++){
			fprintf(arq,"%s|%i|%i|%i",jogador[i].nome,jogador[i].acertos,jogador[i].tiros,jogador[i].total_navios);
			for(j=0;j<10;j++){
				fprintf(arq,"|%i",jogador[i].navios[j].afundado);
				fprintf(arq,"|%s",jogador[i].navios[j].nome);
				fprintf(arq,"|%i",jogador[i].navios[j].tam);
				fprintf(arq,"|%i",jogador[i].navios[j].partes_restantes);
				for(k=0;k<5;k++){
					fprintf(arq,"|%i|%i",jogador[i].navios[j].coord[k].linha,jogador[i].navios[j].coord[k].coluna);
				}
				fprintf(arq,"|");
				for(k=0;k<10;k++){
					fprintf(arq,"%c",tela[i].grid[j][k]);
				}
				for(k=0;k<10;k++){
					fprintf(arq,"%c",mar[i].grid[j][k]);
				}
			}
		}
		fclose(arq);
}

void registra_vencedor(usuario *jogador,int vencedor,int perdedor){
	int total_jogadores,jogo_morto=0;
	int i,j;
	int jog_1=0,jog_2=0;
	int pontos_tmp,partidas_tmp,vitorias_tmp;
	char nome_tmp[20];
	float calc_aux;
	FILE *arq;
	rank *jogador_rank, *aux;
	
	//como houve um vencedor do jogo salvo, essa parte transforma em falso a variavel que verifica se há jogos não terminados
	arq = fopen("jogo_salvo","wb");
	assert(arq != NULL);
	fprintf(arq,"0");
	fclose(arq);
	//pontos do vencedor são calculados
	calc_aux=jogador[vencedor].acertos/jogador[vencedor].tiros;
	//abre o arquivo ranking
	system("cls");
	arq = fopen("ranking.txt","r");
	assert(arq != NULL);
	//recebe as variaveis do arquivo para poderem sobrescreve-las sem perder nenhuma informação
	fscanf(arq,"%i\n",&total_jogadores);
	jogador_rank=(rank *)malloc(total_jogadores*sizeof(rank));
	for(i=0;i<total_jogadores;i++){
		fscanf(arq,"%[^|]",&jogador_rank[i].nome);
		fscanf(arq,"|%f",&jogador_rank[i].pontos);
		fscanf(arq,"|%i",&jogador_rank[i].vitorias);
		fscanf(arq,"|%i\n",&jogador_rank[i].partidas);
	}
	//verifica se o nome de algum jogador ja consta no arquivo e se sim adiciona os pontos ao arquivo
	for(i=0;i<total_jogadores;i++){
		if(strcmp(jogador[vencedor].nome,jogador_rank[i].nome)==0){
			jog_1=1;
			jogador_rank[i].pontos=jogador_rank[i].pontos+(200*(1 + calc_aux));
			jogador_rank[i].vitorias++;
			jogador_rank[i].partidas++;
		}
		if(strcmp(jogador[perdedor].nome,jogador_rank[i].nome)==0){
			jog_2=1;
			jogador_rank[i].pontos=jogador_rank[i].pontos+50;
			jogador_rank[i].partidas++;
		}
	}
	//se os "jogs" forem falsos, o codigo adiciona esse nick com os dados dele no jogo ao ranking
	if(jog_1==0){
		total_jogadores++;
		aux=jogador_rank;
		jogador_rank=(rank *)malloc((total_jogadores)*sizeof(rank));
		for(i=0;i<total_jogadores-1;i++){
			strcpy(jogador_rank[i].nome,aux[i].nome);
			jogador_rank[i].pontos=aux[i].pontos;
			jogador_rank[i].partidas=aux[i].partidas;
			jogador_rank[i].vitorias=aux[i].vitorias;
		}
		strcpy(jogador_rank[total_jogadores-1].nome,jogador[vencedor].nome);
		jogador_rank[total_jogadores-1].pontos=200*(1+(jogador[vencedor].acertos/jogador[vencedor].tiros));
		jogador_rank[total_jogadores-1].vitorias=1;
		jogador_rank[total_jogadores-1].partidas=1;
		free(aux);
	}
	if(jog_2==0){
		total_jogadores++;
		aux = jogador_rank;
		jogador_rank = (rank *) malloc((total_jogadores)*sizeof(rank));
		for(i=0;i<total_jogadores-1;i++){
			strcpy(jogador_rank[i].nome,aux[i].nome);
			jogador_rank[i].pontos=aux[i].pontos;
			jogador_rank[i].partidas=aux[i].partidas;
			jogador_rank[i].vitorias=aux[i].vitorias;
		}
		strcpy(jogador_rank[total_jogadores-1].nome,jogador[perdedor].nome);
		jogador_rank[total_jogadores-1].pontos=50;
		jogador_rank[total_jogadores-1].vitorias=0;
		jogador_rank[total_jogadores-1].partidas=1;
		
		free(aux);
	}
	
	fclose(arq);
	
	//bubblesort dos dados dentro do programa
	for(i=0;i<=total_jogadores;i++){
		for(j=i+1;j<total_jogadores;j++){
			if(jogador_rank[i].pontos<jogador_rank[j].pontos){
				//i vai para temporario
				strcpy(nome_tmp,jogador_rank[i].nome);
				pontos_tmp=jogador_rank[i].pontos;
				partidas_tmp=jogador_rank[i].partidas;
				vitorias_tmp=jogador_rank[i].vitorias;
				//j vai para i
				strcpy(jogador_rank[i].nome,jogador_rank[j].nome);
				jogador_rank[i].pontos=jogador_rank[j].pontos;
				jogador_rank[i].partidas=jogador_rank[j].partidas;
				jogador_rank[i].vitorias=jogador_rank[j].vitorias;
				//i(temporario) vai para j
				strcpy(jogador_rank[j].nome,nome_tmp);
				jogador_rank[j].pontos=pontos_tmp;
				jogador_rank[j].partidas=partidas_tmp;
				jogador_rank[j].vitorias=vitorias_tmp;
			}
		}
	}
	
	//cria outro arquivo com o mesmo nome mas agora atualizado para os jogadores adicionais e os jogadores antigos com novos dados
	arq = fopen("ranking.txt","w");
	assert(arq != NULL);
	fprintf(arq,"%i\n",total_jogadores);

	for(i=0;i<total_jogadores;i++){
		fprintf(arq,"%s",jogador_rank[i].nome);
		fprintf(arq,"|%f",jogador_rank[i].pontos);
		fprintf(arq,"|%i",jogador_rank[i].partidas);
		fprintf(arq,"|%i",jogador_rank[i].vitorias);
		fprintf(arq,"\n");
	}
	fclose(arq);
}

void ranking(){
	int total_jogadores,i;
	FILE *arq;
	rank *jogador_rank;
	
	//aqui ele abre o arquivo, lê e joga na tela. Não vejo muito segredo
	arq = fopen("ranking.txt","r");
	assert(arq != NULL);
	fscanf(arq,"%i",&total_jogadores);
	fscanf(arq,"\n");
	jogador_rank=(rank *) malloc(total_jogadores*sizeof(rank));
	for(i=0;i<total_jogadores;i++){
		fscanf(arq,"%[^|]",&jogador_rank[i].nome);
		fscanf(arq,"|%f",&jogador_rank[i].pontos);
		fscanf(arq,"|%i",&jogador_rank[i].partidas);
		fscanf(arq,"|%i",&jogador_rank[i].vitorias);
		fscanf(arq,"\n");
	}
	fclose(arq);
	
	system("cls");
	printf("\tRANKING:\n");
	for(i=0;i<total_jogadores;i++){
		printf("\t%i°-%s:\n",i+1,jogador_rank[i].nome);
		printf("\tPontos|%.0f\n",jogador_rank[i].pontos);
		printf("\tpartidas|%i\n",jogador_rank[i].partidas);
		printf("\tvencidas|%i\n",jogador_rank[i].vitorias);
		printf("\t----------------------------------\n");
	}
	fflush(stdin);
	getchar();
	system("cls");
}
