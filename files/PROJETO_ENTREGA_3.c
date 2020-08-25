/*******************************************************************************************************************
		Projeto Imobili�ria 	-	Programa��o Estruturada II	-	Prof. Talita Berbel	-	01/2020

				Alunos:		Raul Ryan Deaque Silva 			RA: 190953
							Otavio Cordeiro de Freitas		RA: 190702
							Vitor Joaquim de Carvalho Gois	RA: 190810
********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

typedef struct endereco{		//Estrutura que cont�m os dados de endere�o do propriet�rio
	char logradouro[80];
	char bairro[20];
	char CEP[10];
	char cidade[20];
	char estado[20];
	char fone[15];
	char cel[15];
	char email[30];
}endereco;

typedef struct info_casa{		//Estrutura que cont�m as informa��es sobre as casas cadastradas pelo propriet�rio
	int num_casa;				//N�mero da casa/registro do im�vel
	char status_casa;			//Status, se ela est� [L]ivre ou [A]lugada
}info_casa;

typedef struct prop{			//Estrutura que cont�m os dados do propriet�rio
	int reg_prop;				//Registro de cadastro do proprietario (gerado automaticamente)
	char nome[80];
	char CPF[15];
	struct endereco end_prop;	//Chama a estrutura endereco, chamando-a de "end_prop"
	int qtde_casa;				//Quantas casas o propriet�rio possui cadastrada
	struct info_casa casa[5]; 	//Chama a estrutura info_casa, para guardar as informa��es da casa cadastrada, podendo ter no m�ximo 5 casas
}prop;

typedef struct adress{			//Estrutura que cont�m os dados de endere�o do im�vel cadastrado
	char logradouro[80];
	char bairro[20];
	char CEP[10];
	char cidade[20];
}adress;

typedef struct info_loc{		//Informa��o do im�vel com o registro de quem est� alugando (locat�rio)
	char sigla;
	int reg_loc;
}inf_loc;

union dados{					//Dados do im�vel alugado
	char sigla;
	struct info_loc loc;
};

typedef struct imovel{			//Estrutura que cont�m os dados sobre o im�vel cadastrado
	int reg_imov; 				//Registro do im�vel (gerado automaticamente)
	struct adress end_imov;		//Chama a estrutura adress, chamando-a de "end_imov"
	float area;					//�rea da casa
	int quartos;				//Quantos quartos h� no im�vel
	float valor;				//Valor do aluguel
	union dados status;
}imovel;

typedef struct locatario{		//Estrutura que cont�m os dados referente ao locat�ri{		
	int reg_loc; 					
	char nome[80];
	char CPF[15];
	struct endereco end_loc;		//Chama a estrutura endere�o, nomeando-a como end_loc
	int reg_imov; 					
	int dia_venc;
	char inicio[9];
	char termino[9];
}locatario;

void aloca_prop(prop **p,int tam);										// Aloca��o de espa�o na mem�ria para a estrutura prop
void aloca_imov(imovel **p,int tam);									// Aloca��o de espa�o na mem�ria para a estrutura imovel
void aloca_loc(locatario **p,int tam);									// Aloca��o de espa�o na mem�ria para a estrutura locatario
int menu_geral();														// Menu Geral
int menu_adm();															// Menu adminsitra��o
int menu_prop();														// Menu de op��es para o propriet�rio
int menu_loca();														// Menu de op��es para o locat�rio	
int verifica_qtdReg(int acao);											// Verifica quantos registros foram gravados no arquivo e retorna a quantidade
int verifica_cpfProp(prop *p_p, int Qreg_p, char *n_cpf);				// Verifica se o cpf do propriet�rio j� est� registrado
int verifica_cpfLOC(locatario *p_l, int Qreg_l, char *n_cpf);			// Verifica se o cpf do locat�rio j� est� registrado
int verifica_regImov(imovel *p_i, int Qreg_i, int r_i);					// Verifica se existe o registro de im�vel informado
int verifica_disp(imovel *p_i, int pos_i);								// Verifica a disponibilidade do im�vel
int verifica_data(locatario *p_l);										// Realiza a verifica��o da data inforada pelo locat�rio
int cadastro_prop(prop *p_p, int Qreg_p);								// Realiza o cadastro do propriet�rio
int cadastro_imov(imovel *p_i, int Qreg_i, prop *p_p, int Qreg_p);		// Realiza o cadastro do propriet�rio
int cadastro_loc(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i);	// Realiza o cadastro do locat�rio
void grava_prop(prop *p_p, int pos, char *acao);						// Realiza a grava��o dos dados do propriet�rio no arquivo
void grava_imov(imovel *p_i, int pos, char *acao);						// Realiza a grava��o dos dados do im�vel no arquivo
void grava_loc(locatario *p_l, int pos, char *acao);					// Realiza a grava��o dos dados do locat�rio
void mostra_p(prop *p_p, int Qreg_p);									// Mostra todos os dados do propriet�rio
void mostra_imov(imovel *p_i,int Qreg_i);								// Mostra todos os dados do im�vel
void mostra_loc(locatario *p_l,int Qreg_l);								// Mostra todos os dados do locat�rio
void consulta_prop(prop *p_p,int Qreg_p);								// Faz a consulta sobre determinado propriet�rio
void consulta_imov(imovel *p_i, int Qreg_i);							// Faz a consulta sobre determinado im�vel
void consulta_loc(locatario *p_l,int Qreg_l);							// Faz a consulta sobre determinado locat�rio
void torna_alugado(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i, prop *p_p, int Qreg_p);	// Torna o im�vel [L]ivre para [A]lugado							
void info_aluguel(locatario *p_l, int pos);								// Mostra informa��o do im�vel j� alugado
void editar_prop(prop *p_p, int Qreg_p);								// Faz altera��o nos dados do propriet�rio j� cadastrado
void editar_imov(imovel *p_i, int Qreg_i);								// Faz altera��o nos dados do im�vel j� cadastrado
void editar_loc(locatario *p_l, int Qreg_l);							// Faz altera��o nos dados do locat�rio j� cadastrado
void encerra_contrato(locatario *p_l, int Qreg_l, imovel *p_i, int Qreg_i,prop *p_p, int Qreg_p);	// Deleta os dados do locat�rio
void gerar_rel(locatario *p_l, int Qreg_l, prop *p_p, int Qreg_p,imovel *p_i, int Qreg_i);	// Gera um relat�rio � respeito de uma loca��o

main(){
	//definindo o idioma como portugu�s e o nome do executavel
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("Imobiliaria Geracao");
	
	//declara��o dos ponteiros para estrutura
	prop *p_p=NULL;
	imovel *p_i=NULL;
	locatario *p_l=NULL;
	
	//declara��o de vari�veis
	int op_p, op_l, op_inicial, op_adm, Qreg_p, Qreg_i, Qreg_l, cont=0, cad, pos, r_i, verif;
	
	//alocando espa�o
	aloca_prop(&p_p, 1);
	aloca_imov(&p_i, 1);
	aloca_loc(&p_l, 1);
	
	//verificar quantidade de registros <-1 e 2 indicam a para quem queremos apontar, propriet�rio ou im�vel->
	Qreg_p=verifica_qtdReg(1);
	Qreg_i=verifica_qtdReg(2);
	Qreg_l=verifica_qtdReg(3);
	
	//La�o com as op��es escolhida pelo usu�rio atrav�s da fun��o menu
	do{
		op_inicial=menu_geral();
			
		switch(op_inicial){
			case 1:
				do{
					op_p=menu_prop();	//recebe a op��o escolhida pelo usu�rio
		
					switch(op_p){
						case 1:				//cadastro propriet�rio
							Sleep(300);		//atraso para pr�xima a��o
							system("CLS");	//limpa a tela
								
							cad=cadastro_prop(p_p, Qreg_p+1);	//retorna um valor para verificar se CPF j� est� cadastrado
							if(cad!=-1)	//se n�o existir cadastro ir� ser somado +1 no Qreg_p
								Qreg_p++;
							break;
						case 2:				//cadastro im�vel
							Sleep(300);		
							system("CLS");
							
							cad=cadastro_imov(p_i, Qreg_i+1, p_p, Qreg_p);	//retorna um valor para verificar se o im�vel j� est� cadastrado
							if(cad!=-1)	//se n�o existir cadastro ir� ser somado +1 no Qreg_i
								Qreg_i++;
							break;
						default:	//retorna ao menu inicial
							printf("\nRetornando");
							Sleep(350);
							printf(".");
							Sleep(350);
							printf(".");
							Sleep(350);
							printf(".");
							Sleep(150);
							system("CLS");
							break;	
					}
				}while(op_p!=0);
				break;
			case 2:
				do{
					op_l=menu_loca();		//recebe a op��o escolhida pelo usu�rio
					
					switch(op_l){
						case 1:		//mostra im�veis livres
							Sleep(300);
							system("CLS");
							
							consulta_imov(p_i, Qreg_i);
							break;
						case 2:		//consulta por registro
							Sleep(300);
							system("CLS");
							
							printf("\t\t\t==IMOBILIARIA==\n\n");
							
							printf(" Informe o n�mero de registro do im�vel: ");
							scanf("%i", &r_i);		//recebe o registro que o usu�rio digitar
							fflush(stdin);
							
							Sleep(300);
							system("CLS");
							
							pos=verifica_regImov(p_i, Qreg_i, r_i);		//a fun��o retorna a pos que se encontra a estrutura cujo o registro foi informado pelo usu�rio
							
							if(pos==-1){
								printf("\n\tDesculpe mas n�o foi poss�vel encontrar este registro!\n\n");
								sleep(2);
								system("CLS");
							}
							else{
								verif=verifica_disp(p_i, pos);
								
								if(verif!=-1){
									if(verif==1){	//1 se o im�vel estiver Livre
										cad=cadastro_loc(p_l, Qreg_l+1, p_i, pos);
									
										if(cad!=-1){
											torna_alugado(p_l, Qreg_l+1, p_i, pos, p_p, Qreg_p);
											Qreg_l++;
											
										}
									}
									else{
										info_aluguel(p_l, pos);
									}
								}
								else{
									printf("\n\tDesculpe, mas n�o encontrei nenhum im�vel!\n\n");
								}
							}			
							
							break;
						default:	//retorna ao menu inicial
							printf("\n Retornando");
							Sleep(200);
							printf(".");
							Sleep(200);
							printf(".");
							Sleep(200);
							printf(".");
							Sleep(150);
							system("CLS");
					}	
				}while(op_l!=0);
				break;
			case 3:
				do{
					op_adm=menu_adm();
					
					switch(op_adm){
						case 1:		//exibir propriet�rios
							mostra_p(p_p, Qreg_p); //mostra os propriet�rios cadastrados
							break;
						case 2:		//exibir im�veis
							mostra_imov(p_i, Qreg_i); //mostra os imoveis cadastrados
							break;
						case 3:		//exibir locatarios
							mostra_loc(p_l, Qreg_l);
							break;
						case 4:		//consulta propriet�rio		
							consulta_prop(p_p, Qreg_p);
							break;
						case 5:		//consulta im�vel
							consulta_imov(p_i, Qreg_i); //Consulta especifica
							break;
						case 6:		//consulta locat�rio
							consulta_loc(p_l, Qreg_l);
							break;
						case 7:		//editar dados do propriet�rio
							editar_prop(p_p, Qreg_p);
							break;
						case 8:		//editar dados do im�vel
							editar_imov(p_i, Qreg_i);
							break;
						case 9:		//editar dados do locat�rio
							editar_loc(p_l, Qreg_l);
							break;
						case 10:	//deleta os dados do locat�rio
							encerra_contrato(p_l, Qreg_l, p_i, Qreg_i, p_p, Qreg_p);
							break;
						case 11:	//gerar relat�rio
							gerar_rel(p_l, Qreg_l, p_p, Qreg_p, p_i, Qreg_i);
							break;
						default:	//retorna ao menu inicial
							printf("\n Retornando");
							Sleep(200);
							printf(".");
							Sleep(200);
							printf(".");
							Sleep(200);
							printf(".");
							Sleep(150);
							system("CLS");
				
					}
				}while(op_adm!=0);
				break;			
			default:
				printf("\n\t\tOBRIGADO POR USAR NOSSO SISTEMA\n");
				printf("\t\t\tVORsystems%c\n\n", 169);
				break;
		}
	}while(op_inicial!=0);
}//main

void aloca_prop(prop **p,int tam)
{
	if((*p=(prop*)realloc(*p,tam*sizeof(prop)))==NULL){
		exit(1);
	}
}//aloca proprietarios

void aloca_imov(imovel **p,int tam)
{
    if((*p=(imovel*)realloc(*p,tam*sizeof(imovel)))==NULL){
		exit(1);
	}
}//aloca imoveis

void aloca_loc(locatario **p,int tam)
{
	if((*p=(locatario*)realloc(*p,tam*sizeof(locatario)))==NULL){
		exit(1);
	}
}//aloca locatario

int menu_geral()
{
	int op;
	
	do{
		printf("\t\t\t==IMOBILIARIA==\n");
		printf("\n\tBem Vindo � Imobili�ria Gera��o!\n\tEm que podemos ajudar?\n\n");
		printf("\t[1] - Sou Propri�tario\n\t[2] - Sou Locat�rio\n\t[3] - Sou Admin\n\t[0] - Desejo sair\n\n-> Sua op��o: ");
		scanf("%i", &op);
		
		if(op<0 || op>3){
			printf("\n Desculpe n�o entendi!\n");
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>3);
	
	Sleep(500);
	system("CLS");
	
	return op;
}//menu_geral

int menu_adm()
{
	int op;
	
	do{
		printf("\t\t\t==IMOBILIARIA==\n");
		printf("\n\tBem Vindo � administra��o!\n\tEm que posso ajudar?\n\n");
		
		printf("\t[1] - Exibir Propriet�rios\n\t[2] - Exibir Im�veis\n\t[3] - Exibir Locat�rios\n\t[4] - Consulta Propriet�rio\n\t[5] - Consulta Im�vel");
		printf("\n\t[6] - Consulta Locat�rio\n\t[7] - Editar Proprietario\n\t[8] - Editar Imovel\n\t[9] - Editar Locatario\n\t[10] - Encerrar Contrato");
		printf("\n\t[11] - Gerar Relat�rio\n\n\t[0] - Menu Principal\n\n-> Sua op��o: ");
		scanf("%i", &op);
		fflush(stdin);
		
		if(op<0 || op>11){
			printf("\n Desculpe n�o entendi!\n");
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>11);
	
	Sleep(500);
	system("CLS");
	
	return op;
}//menu_adm

int menu_prop()
{
	int op;
	
	//La�o para que caso o usu�rio digite uma op��o inv�lida, possa digitar novamente.
	do{
		printf("\t\t\t==IMOBILIARIA==\n");

		printf("\n\t[1] - Cadastrar Propietario\n\t[2] - Cadastrar Imovel");
		printf("\n\t[0] - Menu inicial\n\n->Opcao: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<0 || op>2){
			printf("\n Desculpe, n�o entendi!\n\n");	
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>2);
	
	//Retorna a op��o desejada pelo usu�rio � main
	return op;
}//menu

int menu_loca()
{
	int op;
	
	//La�o para que caso o usu�rio digite uma op��o inv�lida, possa digitar novamente.
	do{
		printf("\t\t\t==IMOBILIARIA==\n\n");

		printf("\tO que deseja fazer?\n\n");
		printf("\t[1] - Ver im�veis\n\t[2] - Alugar im�vel (n� registro)\n\t[0] - Menu inicial\n\n->Sua op��o: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<0 || op>2){
			printf("\n Desculpe, n�o entendi!\n\n");	
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>2);
	
	//Retorna a op��o desejada pelo usu�rio � main
	return op;
}//menu_loca

int verifica_qtdReg(int acao)
{
	long int cont=0;	//contador para quantidade de registros no arquivo
	FILE *fptr=NULL;	//ponteiro para o arquivo
	
	//Verifica qtd Registro para o propriet�rio
	if(acao==1){
		if((fptr=fopen("proprietario.bin", "rb"))==NULL){
			return cont;
		}else{
			fseek(fptr, 0, 2);	//fun��o para posicionar o ponteiro no fim do arquivo
			cont=ftell(fptr)/sizeof(prop);	//contador ir� receber a quantidade de estruturas que h� dentro do arquivo
			fclose(fptr);	//fecha o arquivo
			return cont;	//retorna a quantidade de arquivos encontrados
		}
	}
	
	//Verifica qtd Registro para o im�vel
	else if(acao==2){
		if((fptr=fopen("imovel.bin", "rb"))==NULL){
			return cont;
		}else{
			fseek(fptr, 0, 2);
			cont=ftell(fptr)/sizeof(imovel);
			fclose(fptr);
			return cont;
		}
	}
	
	//Verifica qtd Registro para o locat�rio
	else{
		if((fptr=fopen("locatario.bin", "rb"))==NULL){
			return cont;
		}else{
			fseek(fptr, 0, 2);
			cont=ftell(fptr)/sizeof(locatario);
			fclose(fptr);
			return cont;
		}
	}
}//verifica_qtdReg

int verifica_cpfProp(prop *p_p, int Qreg_p, char *n_cpf)
{
	int i=0, achou;
	FILE *fptr=NULL;	//ponteiro para o arquivo
	
	system("CLS");	//limpa a tela
	
	//verifica se existe o arquivo
	if((fptr=fopen("proprietario.bin", "rb"))==NULL){
		printf("\t\t==CADASTRO DE PROPRIETARIO==\n");
	  	printf("\nNenhum proprietario cadastrado! Cadastre um novo!\n");
	  	Sleep(200);
 		system("CLS");
	  	achou=0;
	}
	else{
		while(achou!=1 && i<Qreg_p){
			fseek(fptr, i*sizeof(prop), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_p, sizeof(prop), 1, fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara o cpf digitado com o que est� dentro do arquivo
			if(strcmp(n_cpf, p_p->CPF)==0)
				achou=1;	//se achar, recebe 1
			i++;
		}
		fclose(fptr);	//fecha o arquivo
	}
	if(achou==1){
		return (i-1);	//-1 por conta do i++ no final do while
	}else{
		return (-1);	//caso n�o for encontrado o cpf
	}
	
	system("PAUSE");
}//verifica_cpfProp

int verifica_cpfLOC(locatario *p_l, int Qreg_l, char *n_cpf)
{
	int i=0, achou;
	FILE *fptr=NULL;	//ponteiro para o arquivo
	
	system("CLS");	//limpa a tela
	
	//verifica se existe o arquivo
	if((fptr=fopen("locatario.bin", "rb"))==NULL){
		printf("\t\t==CADASTRO DE LOCAT�RIO==\n");
	  	printf("\nNenhum locat�rio cadastrado! Cadastre um novo!\n");
	  	Sleep(500);
 		system("CLS");
	  	achou=0;
	}
	else{
		while(achou!=1 && i<Qreg_l){
			fseek(fptr, i*sizeof(locatario), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_l, sizeof(locatario), 1, fptr);		//l� do ponteiro da estrutura locatario, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara o cpf digitado com o que est� dentro do arquivo
			if(strcmp(n_cpf, p_l->CPF)==0)
				achou=1;	//se achar, recebe 1
			i++;
		}
		fclose(fptr);	//fecha o arquivo
	}
	if(achou==1){
		return (i-1);	//-1 por conta do i++ no final do while
	}else{
		return (-1);	//caso n�o for encontrado o cpf
	}
}//verifica_cpf

int verifica_regImov(imovel *p_i, int Qreg_i, int r_i)
{
	int i=0, achou;
	FILE *fptr=NULL;
	
	if((fptr=fopen("imovel.bin", "rb"))==NULL){
		printf("\nErro ao abrir o arquivo!\n\n");
		achou=0;
	}
	else{
		while(achou!=1 && i<Qreg_i){
			fseek(fptr, i*sizeof(imovel), 0);		//posiciona o ponteiro no inicio de cada estrutura a partir da origem
			fread(p_i, sizeof(imovel), 1, fptr);	//l� a estrutura guardada no arquivo	
			
			if(r_i==p_i->reg_imov)		//se o registro informado for igual ao registro cadastrado, achou aponta 1
				achou=1;
			i++;
		}
		fclose(fptr);
	}
	
	if(achou==1)
		return (i-1);	//-1 por conta do i++ no final de cada while
	else
		return -1;		//caso n�o encontrar retorna -1 para main
}//verifica_regImov

int verifica_disp(imovel *p_i, int pos_i)
{
	FILE *fptr=NULL;
	
	if((fptr=fopen("imovel.bin", "rb"))==NULL){
		printf("\nErro ao abrir o arquivo!\n\n");
		return -1;
	}
	else{
		fseek(fptr, pos_i*sizeof(imovel), 0);
		fread(p_i, sizeof(imovel), 1, fptr);
		
		if((p_i->status.loc.sigla)=='L'){
			return 1;
		}
		else{
			return 0;
		}
	}
}//verifica_disponivel

int verifica_data(locatario *p_l)
{
	char *dt,data_termino[9],data_atual[9];
	int i,termino[3],atual[3],verif=0;
	
	printf("\t\t\t==REMOVER LOCAT�RIO==\n\n");
	
	printf("Data atual (dd/mm/aa): ");
	gets(data_atual);
	
	strcpy(data_termino, p_l->termino);		//copia o termino do locatario para a data de termino
	
	//a fun��o strtok separa em tokens(segmentos) de uma string definindo o limite de cada token, neste caso com "/"
    dt = strtok(data_termino,"/");		
	for(i=0;i<3;i++){	
		termino[i] = atoi(dt);	
		dt = strtok(NULL,"/");	
	}
	
	dt = strtok(data_atual,"/");
	
	for(i=0;i<3;i++){	
		atual[i] = atoi(dt);
		dt = strtok(NULL,"/");	
	}
	
	//VETOR[2] -> ANO
	//VETOR[1] -> MES
	//VETOR[0] -> DIA
	if(atual[2]==termino[2]){
		if(atual[1]==termino[1]){ 
			if(atual[0]<termino[0]){ 
				verif=1;
			}
		}else if(atual[1] < termino[1]){
			verif=1;
		}
	}else if(atual[2]<termino[2]){ //Se o ano atual for maior que o ano de termino
		verif = 1;
	}
	
	return(verif);
	
}//verifica_data

int cadastro_prop(prop *p_p, int Qreg_p)
{
	char n_cpf[15];		//cpf auxiliar, ser� usado para verificar a exist�ncia do mesmo
	
	printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
	
	printf(" Informe seu CPF: ");	//l� o CPF do propriet�rio
	gets(n_cpf);
	
	//se a fun��o retornar algo diferente de -1, quer dizer o cpf j� est� cadastrado.
	if(verifica_cpfProp(p_p, Qreg_p, n_cpf) != -1){
		printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
		printf("\tOps! O CPF digitado j� est� cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a pr�xima a��o
		system("CLS");	//limpa tela
		
		return(-1);	//retornando -1 para a main
	}
	//caso a fun��o verifica_cpf retornar -1, quer dizer que o cpf n�o foi cadastrado
	else{
		printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
		printf("\tEBA! Um novo integrante, conclua seu cadastro conosco abaixo!\n\n");
		
		strcpy(p_p->CPF,n_cpf);		//copia o CPF digitado pelo usu�rio para a estrutura
		p_p->reg_prop=Qreg_p;		//o reg_prop da estrutura recebe o registro da main (quantidade de registros)
		
		printf(" Seu registro: %i\n",p_p->reg_prop);
		printf(" Nome: ");
		gets(p_p->nome);
		fflush(stdin);
		
		//guarda os dados do endere�o do propriet�rio na estrutura aninhada do propriet�rio e endere�o
		//fflush(stdin) limpa o buffer do teclado
		printf(" Logradouro: ");
		gets(p_p->end_prop.logradouro);
		fflush(stdin);
		printf(" Bairro: ");
		gets(p_p->end_prop.bairro);
		fflush(stdin);
		printf(" CEP: ");
		gets(p_p->end_prop.CEP);
		fflush(stdin);
		printf(" Cidade: ");
		gets(p_p->end_prop.cidade);
		fflush(stdin);
		printf(" Estado: ");
		gets(p_p->end_prop.estado);
		fflush(stdin);
		printf(" Fone: ");
		gets(p_p->end_prop.fone);
		fflush(stdin);
		printf(" Cel: ");
		gets(p_p->end_prop.cel);
		fflush(stdin);
		printf(" Email: ");
		gets(p_p->end_prop.email);
		fflush(stdin);
		
		//incializa a quantidade de casa registrado pelo usu�rio como 0
		p_p->qtde_casa = 0;
		fflush(stdin);
		
		//fun��o para guardar estes dados informados pelo usu�rio
		grava_prop(p_p, Qreg_p, "ab");	//ab � para atualizar os dados do arquivo
	}
	
	printf("\n\tCadastro realizado com sucesso!\n\n");
	system("PAUSE");	//pausa a tela
	system("CLS");		//limpa a tela
	return(1);			//retorna 1 para a main se foi cadastrado um novo cpf
}//cadastro_prop

int cadastro_imov(imovel *p_i, int Qreg_i, prop *p_p, int Qreg_p)
{
	char n_cpf[15];
	int i, qtd_c, pos;
	
	printf("\t\t\t==CADASTRO DE IMOVEL==\n\n");
	
	printf("Informe o CPF do proprietario: ");
    gets(n_cpf);
	
	pos=verifica_cpfProp(p_p, Qreg_p, n_cpf);	//verifica se o CPF j� foi registrado
	
	//se o retorno da verifica_cpf for -1 n�o foi encontrado o cpf retornando -1 para a main
	if(pos==-1){
		printf("\t\t\t==CADASTRO DE IMOVEL==\n\n");
		printf("\tDesculpe, mas n�o encontramos o CPF informado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		sleep(3);
		system("CLS");
		return (-1);	
	}		
	else{
		qtd_c=p_p->qtde_casa;
		
		//Verifica se o foi atingido o limite de casas pertencentes ao proprietario
		if(qtd_c >= 5){
			printf("\t\t\t==CADASTRO DE IMOVEL==\n\n");
			printf("\n\tPera l� apressadinho! Voc� j� atingiu o limite m�ximo de 5 casas.\n\n");
			sleep(3);
			system("CLS");
			return(-1);
		}
		printf("\t\t\t==CADASTRO DE IMOVEL==\n");
		
		(p_p->casa+qtd_c)->num_casa = Qreg_i;		//aponta para a estrutura aninhada do propriet�rio com info_casa, recebendo o registro(n�mero) da casa
		(p_p->casa+qtd_c)->status_casa = 'L'; 		//aponta para a estrutura aninhada do propriet�rio com info_casa, inicializa im�vel como [L]ivre
		p_p->qtde_casa +=1;							//adicionando mais uma casa para o propriet�rio
		
		grava_prop(p_p,pos,"rb+");	//atualiza a quantidade de casas do propriet�rio no arquivo
		
	    p_i->reg_imov = Qreg_i;								//registro do im�vel recebe o indice do imovel
	    printf("\n Registro do im�vel: %i\n",p_i->reg_imov);	//mostra o registro do imovel
	    
	    //guarda os dados do im�vel na estrutura aninhada do propriet�rio e endere�o
		//fflush(stdin) limpa o buffer do teclado
		printf("\n\tInforma��es de Endere�o\n");
	    printf(" Logradouro: ");
	    gets(p_i->end_imov.logradouro);
	    fflush(stdin);
	    printf(" Bairro: ");
	    gets(p_i->end_imov.bairro);
	    fflush(stdin);
	    printf(" CEP: ");
	    gets(p_i->end_imov.CEP);
	    fflush(stdin);
	    printf(" Cidade: ");
	    gets(p_i->end_imov.cidade);
	    fflush(stdin);
	    printf("\n\tAtributos da resid�ncia\n");
	    printf(" �rea util(m�): ");
	    scanf("%f",&(p_i->area));
	    fflush(stdin);
	    printf(" Quantidade de quartos: ");
	    scanf("%i",&(p_i->quartos));
	    fflush(stdin);
	    printf(" Valor do aluguel: R$");
	    scanf("%f",&(p_i->valor));
	    fflush(stdin); 	
	    p_i->status.loc.sigla = 'L';		//estrutura aninhada do im�vel com a union dados com a estrutura info_loc para guardar o status do im�vel ->[A]lugado ou [L]ivre<-
	    
	    grava_imov(p_i, Qreg_i, "ab");		//grava os dados do im�vel no arquivo
	    
	    printf("\n\tCadastro realizado com sucesso!\n\n");
	    
	    sleep(2);
	    system("CLS");
	}
}//cadastro_imov

int cadastro_loc(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i)
{
	FILE *fptr=NULL;
	char n_cpf[15];	
	int verif;	
	
	printf("\t\t\t==CADASTRO DE LOCAT�RIO==\n\n");
	
	printf(" Informe o seu CPF: ");	
	gets(n_cpf);
	
	if(verifica_cpfLOC(p_l, Qreg_l, n_cpf) != -1){
		printf("\t\t\t==CADASTRO DE LOCAT�RIO==\n\n");
		printf("\tOps! O CPF digitado j� est� cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		
		Sleep(700);		
		system("CLS");	
		
		return(-1);	
	}

	else{
		printf("\t\t==CADASTRO DE LOCAT�RIO==\n\n");
		
		strcpy(p_l->CPF,n_cpf);		
		p_l->reg_loc=Qreg_l;		
		
		printf(" Registro: %i\n\n",p_l->reg_loc);
		printf(" Nome: ");
		gets(p_l->nome);
		fflush(stdin);
		
		printf(" Logradouro: ");
		gets(p_l->end_loc.logradouro);
		fflush(stdin);
		printf(" Bairro: ");
		gets(p_l->end_loc.bairro);
		fflush(stdin);
		printf(" CEP: ");
		gets(p_l->end_loc.CEP);
		fflush(stdin);
		printf(" Cidade: ");
		gets(p_l->end_loc.cidade);
		fflush(stdin);
		printf(" Estado: ");
		gets(p_l->end_loc.estado);
		fflush(stdin);
		printf(" Fone: ");
		gets(p_l->end_loc.fone);
		fflush(stdin);
		printf(" Cel: ");
		gets(p_l->end_loc.cel);
		fflush(stdin);
		printf(" Email: ");
		gets(p_l->end_loc.email);
		fflush(stdin);
		
		if((fptr=fopen("imovel.bin", "rb"))==NULL){
			printf("\n\tErro ao abrir o arquivo!\n\n");
		}
		else{
			fseek(fptr, pos_i*sizeof(imovel), 0);
			fread(p_i, sizeof(imovel), 1, fptr);
		}
		
		p_l->reg_imov=p_i->reg_imov;
		
		printf("\n Dia Vencimento: ");
		scanf("%i", &p_l->dia_venc);
		fflush(stdin);
		
		printf(" Data In�cio (dd/mm/aa): ");
		gets(p_l->inicio);
		
		printf(" Data T�rmino (dd/mm/aa): ");
		gets(p_l->termino);	
		
		grava_loc(p_l, Qreg_l, "ab");
	}
	
	printf("\n\tCadastro realizado com sucesso!\n\n");
	system("PAUSE");	
	system("CLS");		
	return(1);		
}//cadastro_loc

void grava_prop(prop *p_p, int pos, char *acao)
{
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	//se tentar abrir o arquivo e n�o encontrar nada dar� erro sen�o abrir� normal
	if((fptr=fopen("proprietario.bin", acao))==NULL)
		printf("Erro ao abrir o arquivo!\n\n");
	else{
		if(strcmp(acao, "rb+")==0)				//faz a compara��o da a��o que o usu�rio quer fazer no arquivo com rb+ (utilizado para ler e fazer uma altera��o)
			fseek(fptr, pos*sizeof(prop), 0);	//posiciona o arquivo na posi��o que est� a estruturada analisada na cadastro_prop a partir da origem
		fwrite(p_p, sizeof(prop), 1, fptr);		//escreve no arquivo fptr a estrutura do propriet�rio que foi gravada na memoria
	}
	fclose(fptr);	//fecha arquivo
}//grava_prop

void grava_imov(imovel *p_i, int pos, char *acao)
{
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	if((fptr=fopen("imovel.bin", acao))==NULL)
		printf("\nErro ao abrir o arquivo!\n\n");
	else{
		if(strcmp(acao, "rb+")==0)				//faz a compara��o da a��o que o usu�rio quer fazer no arquivo com rb+ (utilizado para ler e fazer uma altera��o)
			fseek(fptr, pos*sizeof(imovel), 0);	//posiciona o arquivo na posi��o que est� a estruturada analisada na cadastro_imov a partir da origem
		fwrite(p_i, sizeof(imovel), 1, fptr);	//grava no arquivo a estrutura do im�vel
	}
	fclose(fptr);	//fecha o arquivo
}//grava_imov

void grava_loc(locatario *p_l, int pos, char *acao)
{
	FILE *fptr=NULL;	
	
	if((fptr=fopen("locatario.bin", acao))==NULL)
		printf("Erro ao abrir o arquivo!\n\n");
	else{
		if(strcmp(acao, "rb+")==0)				
			fseek(fptr, pos*sizeof(locatario), 0);	
		fwrite(p_l, sizeof(locatario), 1, fptr);		
	}
	
	fclose(fptr);	//fecha arquivo
}//grava_loc

void mostra_p(prop *p_p, int Qreg_p)
{
	int i, j;
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	system("CLS");	//limpa tela
	
	if((fptr=fopen("proprietario.bin", "rb"))==NULL){
		printf("\nErro ao abrir o arquivo!\n\n");
	}
	else
	{
		printf("\t\t\t==PROPRIETARIOS CADASTRADOS==\n");
		
		for(i=0; i<Qreg_p; i++)	//i indica qual estrutura estamos acessando
		{
			fseek(fptr, i*sizeof(prop), 0);		//posiciona o ponteiro no arquivo de acordo com o indice I em rela��o a origem
			fread(p_p, sizeof(prop), 1, fptr);	//l� a estrutura do arquivo guardando no ponteiro p_p
			
			//mostra todos os dados referente a determinado propriet�rio
			printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_p->reg_prop,p_p->nome,p_p->CPF,p_p->end_prop.logradouro,p_p->end_prop.bairro,p_p->end_prop.CEP);
	    	printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n\n Quantidade de casas: %i\n",p_p->end_prop.cidade,p_p->end_prop.estado,p_p->end_prop.fone,p_p->end_prop.cel,p_p->end_prop.email,p_p->qtde_casa);
	    	
	    	//mostra os dados da resid�ncia a ser alugada
	    	for(j=0;j<p_p->qtde_casa;j++)
			{
	    		printf("\n\tCasa %i:\n\t\tNum Casa: %i\n\t\tStatus da Casa: %c\n",j+1,(p_p->casa+j)->num_casa,(p_p->casa+j)->status_casa);
			}
			printf("\n=============================================================================\n\n");
		}
		fclose(fptr);	//fecha arquivo;
	}
	
	system("PAUSE");	//pausa o programa
	system("CLS");		//limpa a tela
}//mostra_prop

void mostra_imov(imovel *p_i,int Qreg_i)
{
	int i;
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	system("cls");	//limpa a tela
	
	if((fptr=fopen("imovel.bin","rb"))==NULL)
	{
	  printf("\nErro ao abrir o arquivo!\n\n");
	}
	else
	{
	  	printf("\t\t\t==IMOVEIS CADASTRADOS==\n");
	  	
	  	for(i=0;i<Qreg_i;i++)	//i indica qual estrutura estamos acessando
	    {
	    	fseek(fptr,i*sizeof(imovel),0);		//posiciona o ponteiro no arquivo de acordo com o indice I em rela��o a origem
	   	 	fread(p_i,sizeof(imovel),1,fptr);	//l� a estrutura do arquivo guardando no ponteiro p_i
	   	 	
	   	 	//mostra todos os dados referente ao im�vel
	    	printf("\n Registro: %i\n\n Logradouro: %s\n Bairro: %s\n CEP: %s\n Cidade: %s\n Area (m�): %.2f",p_i->reg_imov,p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade,p_i->area);
	   		printf("\n Quartos: %i\n Valor: R$%.2f\n Sigla: %c\n\n",p_i->quartos,p_i->valor,p_i->status.loc.sigla);
	    	printf("=============================================================================\n\n");
	    }
	  	fclose(fptr); 	//fecha arquivo
	}
	
	system("PAUSE");	//pausa o programa
	system("CLS");	
}//mostra_imov

void mostra_loc(locatario *p_l,int Qreg_l)
{
	int i, flag=0;
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	system("cls");	//limpa a tela
	
	if((fptr=fopen("locatario.bin","rb"))==NULL)
	{
	  printf("\nErro ao abrir o arquivo!\n\n");
	}
	else
	{
	  	printf("\t\t\t==LOCATARIOS CADASTRADOS==\n");
	  	
	  	for(i=0;i<Qreg_l;i++)	//i indica qual estrutura estamos acessando
	    {
	    	fseek(fptr,i*sizeof(locatario),0);		//posiciona o ponteiro no arquivo de acordo com o indice I em rela��o a origem
	   	 	fread(p_l,sizeof(locatario),1,fptr);	//l� a estrutura do arquivo guardando no ponteiro p_i
	   	 	
	   	 	//se ele encontrar um @ n�o � mostrado, caso contr�rio � mostrado
	   	 	if(strcmp(p_l->CPF, "@")!=0){
	   	 	//mostra todos os dados referente ao locat�rio
	    	printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_l->reg_loc,p_l->nome,p_l->CPF,p_l->end_loc.logradouro,p_l->end_loc.bairro,p_l->end_loc.CEP);
	    	printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n\n",p_l->end_loc.cidade,p_l->end_loc.estado,p_l->end_loc.fone,p_l->end_loc.cel,p_l->end_loc.email);
	    	printf(" Registro do Imovel: %i\n Dia de Vencimento: %i\n Inicio: %s -- Fim: %s\n",p_l->reg_imov,p_l->dia_venc,p_l->inicio,p_l->termino);

			printf("\n=============================================================================\n\n");
			flag++;
			}
	    }
	  	fclose(fptr); 	//fecha arquivo
	}
	
	//se n�o encontrar nenhum usu�rio no sistema, ou houver @
	if(flag==0){
		printf("\n\tInfelizmente n�o temos nenhum cadastro no sistema!\n\n");
	}
	
	system("PAUSE");	//pausa o programa
	system("CLS");	
}//mostra_loc

void consulta_prop(prop *p_p,int Qreg_p)
{
	int op,i,j;
	char cpf[15];
	FILE *fptr=NULL;	//ponteiro para arquivo	
	
	//la�o para verificar se o usu�rio digitou a op��o correta
	do{
		printf("\t\t\t==CONSULTA DE PROPRIETARIOS==\n\n");
		
		printf("\t[1]Pesquisa total\n\t[2]Pesquisa por CPF\n\n->Digite a forma de consulta: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<1 || op>2)
			printf("\n\tDesculpe, n�o entendi!\n\n");
			
		Sleep(700);		//atrasa a pr�xima a��o
		system("CLS");	//limpa a tela
	}while(op<1 || op>2);
	
	if(op==1){
		mostra_p(p_p,Qreg_p);
	}
	
	//Verificando se existem registros no arquivo para ser consultaodos (Vendo se nao ta vazio o arquivo)
	if((fptr=fopen("proprietario.bin","rb"))==NULL){
		printf("\nErro ao abrir o arquivo!\n\n");
		return; //Saindo da funcao ja que o arquivo esta vazio
	}
	
	if(op==2){
		printf("\t\t\t==CONSULTA DE PROPRIETARIOS==\n\n");
		
		printf(" Insira o CPF: ");
		gets(cpf);		//l� o CPF a ser consultado
		
		for(i=0;i<Qreg_p;i++)
		{
			fseek(fptr,i*sizeof(prop),0);	//posiciona o arquivo na posi��o que est� a estruturada analisada a partir da origem
			fread(p_p,sizeof(prop),1,fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara se o CPF digitado � o mesmo que esta na estrutura
			if(strcmp(cpf,p_p->CPF)==0){
				//mostra todos os dados do propriet�rio
				printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_p->reg_prop,p_p->nome,p_p->CPF,p_p->end_prop.logradouro,p_p->end_prop.bairro,p_p->end_prop.CEP);
			    printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n Qtde de casa: %i\n",p_p->end_prop.cidade,p_p->end_prop.estado,p_p->end_prop.fone,p_p->end_prop.cel,p_p->end_prop.email,p_p->qtde_casa);
			    
			    //mostra todos os dados da resid�ncia a ser alugada
				for(j=0;j<p_p->qtde_casa;j++){
			    		printf("\n\tCasa %i:\n\t\tNum Casa: %i\n\t\tStatus da Casa: %c\n",j+1,(p_p->casa+j)->num_casa,(p_p->casa+j)->status_casa);
				}
			}
		}
		
		fclose(fptr);	//fecha o arquivo
		
		printf("\n");
		
		system("pause");
		system("cls");	
	}
}//consulta_prop

void consulta_imov(imovel *p_i, int t_imov)
{
	int op, opc, q_q,i;
	char pesq_bairro[20];
	float are_uti;
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	//la�o para verificar se o usu�rio informou correntamente a op��o desejada
	do{
		printf("\t\t==CONSULTA DE IMOVEL==\n\n");
		
		printf("\t[1] - Pesquisa total\n\t[2] - Pesquisa parcial(Area util, Quantidade de quartos ou Bairro)\n\nForma de consulta: ");
		scanf("%i",&op);
		fflush(stdin);
	}while(op<1 || op>2);
	
	//Verificando se existem registros no arquivo para ser consultaodos (Vendo se nao ta vazio o arquivo)
	if((fptr=fopen("imovel.bin","rb"))==NULL){
		printf("\nErro ao abrir o arquivo!\n\n");
		return; //Saindo da funcao ja que o arquivo esta vazio
	}	
	
	if(op == 1){
		Sleep(700);
		system("CLS");
		
		printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
		
	 	for(i=0;i<t_imov;i++)
		{
	   		fseek(fptr,i*sizeof(imovel),0);		//posiciona o arquivo na posi��o que est� a estruturada analisada a partir da origem
	   	 	fread(p_i,sizeof(imovel),1,fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
	   	 	
	   	 	//verifica se a resid�ncia est� livre para alugar
	    	if(p_i->status.loc.sigla == 'L')
			{
				//mostra todos os dados referente a resid�ncia
				printf(" Registro: %i\t\tStatus: [L]ivre\n",p_i->reg_imov);
				printf("\n\tEndere�o:");
				printf("\n\t\tLogradouro: %s\n\t\tBairro: %s\n\t\tCEP: %s\n\t\tCidade: %s\n", p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade);
				printf("\n\tAtributos:");
	    		printf("\n\t\tArea: %.2f\n\t\tQuartos: %i\n\t\tValor: %.2f\n\n",p_i->area, p_i->quartos,p_i->valor,p_i->status.loc.sigla);
			}
			else
			{
				//mostra todos os dados referente a resid�ncia
				printf(" Registro: %i\t\tStatus: [A]lugado\n",p_i->reg_imov);
				printf("\n\tEndere�o:");
				printf("\n\t\tLogradouro: %s\n\t\tBairro: %s\n\t\tCEP: %s\n\t\tCidade: %s\n", p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade);
				printf("\n\tAtributos:");
	    		printf("\n\t\tArea: %.2f\n\t\tQuartos: %i\n\t\tValor: %.2f\n\n",p_i->area, p_i->quartos,p_i->valor,p_i->status.loc.sigla);
			}
			printf("=============================================================================\n\n");
	    }
	    
		fclose(fptr);		//fecha o arquivo
	  	system("pause");
	  	system("cls");
	}
	else{
		Sleep(700);
		system("CLS");
		
		printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
		printf("\t[1] - Area util\n\t[2] - Quantidade de quartos\n\t[3] - Bairro\n\nOp��o de busca: ");
		scanf("%i",&opc);
		fflush(stdin);
		
		//verifica a op��o desejada de consulta do usu�rio
		switch(opc)
		{
			case 1:	//por �rea �til
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Qual a �rea �til (m�) voc� est� procurando: ");
				scanf("%f",&are_uti);
				fflush(stdin);
				break;
			case 2:	//por quantidade de quartos
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Quantos quartos voc� est� procurando: ");
				scanf("%i",&q_q);
				fflush(stdin);
				break;
			case 3:	//por bairro
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Qual bairro voc� est� procurando: ");
				gets(pesq_bairro);
		}
		
		for(i=0;i<t_imov;i++)
		{
			fseek(fptr,i*sizeof(imovel),0);		//posiciona o arquivo na posi��o que est� a estruturada analisada a partir da origem
			fread(p_i,sizeof(imovel),1,fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			// Verificando se o imovel esta livre e se atende ao que foi consultado
			if((p_i->status.loc.sigla =='L') && (strcmp(pesq_bairro,p_i->end_imov.bairro)==0 || p_i->area == are_uti || p_i->quartos == q_q)){
				//dados do im�vel procurado
				printf("\n Registro: %i\t\tStatus: [L]ivre\n",p_i->reg_imov);
				printf("\n\tEndere�o:");
				printf("\n\t\tLogradouro: %s\n\t\tBairro: %s\n\t\tCEP: %s\n\t\tCidade: %s\n", p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade);
				printf("\n\tAtributos:");
	    		printf("\n\t\tArea: %.2f\n\t\tQuartos: %i\n\t\tValor: %.2f\n\n",p_i->area, p_i->quartos,p_i->valor,p_i->status.loc.sigla);
			}
			printf("=============================================================================\n\n");
		}
		
		fclose(fptr);	//fecha o arquivo
		
		system("pause");
		system("cls");
	}
}//consulta_imov

void consulta_loc(locatario *p_l,int Qreg_l)
{
	int i, j, op, flag;
	char cpf[15];
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	do{
		printf("\t\t\t==CONSULTA DE LOCAT�RIOS==\n\n");
		
		printf("\t[1]Pesquisa total\n\t[2]Pesquisa por CPF\n\n->Digite a forma de consulta: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<1 || op>2)
			printf("\n\tDesculpe, n�o entendi!\n\n");
			
		Sleep(700);		//atrasa a pr�xima a��o
		system("CLS");	//limpa a tela
	}while(op<1 || op>2);
	
	if(op==1){
		mostra_loc(p_l, Qreg_l);
	}
	
	if((fptr=fopen("locatario.bin", "rb"))==NULL){
		printf("\t\t\t==LOCATARIOS CADASTRADOS==\n\n");
		printf("\tN�o encontramos nenhum locat�rio em nosso sistema!\n\n");
		return;
	}
	if(op==2){
		printf("\t\t\t==CONSULTA DE PROPRIETARIOS==\n\n");
		
		printf(" Insira o CPF: ");
		gets(cpf);		//l� o CPF a ser consultado
		
		for(i=0;i<Qreg_l;i++)
		{
			fseek(fptr,i*sizeof(locatario),0);	//posiciona o arquivo na posi��o que est� a estruturada analisada a partir da origem
			fread(p_l,sizeof(locatario),1,fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara se o CPF digitado � o mesmo que esta na estrutura
			if(strcmp(cpf,p_l->CPF)==0){
				flag=1;
				
				//mostra todos os dados referente a determinado locat�rio
				printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_l->reg_loc,p_l->nome,p_l->CPF,p_l->end_loc.logradouro,p_l->end_loc.bairro,p_l->end_loc.CEP);
	    		printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n\n",p_l->end_loc.cidade,p_l->end_loc.estado,p_l->end_loc.fone,p_l->end_loc.cel,p_l->end_loc.email);
	    		printf(" Registro do Imovel: %i\n Dia de Vencimento: %i\n Inicio: %s -- Fim: %s\n",p_l->reg_imov,p_l->dia_venc,p_l->inicio,p_l->termino);
			}
		}
		
		fclose(fptr);	//fecha o arquivo
		
		printf("\n");
		
		if(flag==1){
			system("pause");
			system("cls");
		}
		else{
			printf("\t\tN�o foi poss�vel encontrar o CPF no sistema!");
			sleep(2);
			system("CLS");
		}
	}
}//consulta_loc

void torna_alugado(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i, prop *p_p, int Qreg_p)
{
	FILE *fptr=NULL;		//ponteiro para arquivo do locatario
	FILE *fptr2=NULL;		//ponteiro para arquivo do imovel
	FILE *fptr3=NULL;		//ponteiro para arquivo do proprietario
	
	int i=0, j, achou=0;
	
	if((fptr=fopen("locatario.bin", "rb"))==NULL)
		printf("\nErro ao abrir o arquivo!\n\n");
	else{
		fseek(fptr, Qreg_l*sizeof(locatario), 0);
		fread(p_l, Qreg_l*sizeof(locatario), 1, fptr);
		
		if((fptr2=fopen("imovel.bin", "rb"))==NULL)
			printf("\nErro ao abrir o arquivo!\n\n");
		else{
			fseek(fptr2, pos_i*sizeof(imovel), 0);
			fread(p_i, pos_i*sizeof(imovel), 1, fptr2);
			
			p_i->status.sigla='A';
			p_i->status.loc.sigla='A';				//O imovel alugado ser� renomado para [A]lugado
			p_i->status.loc.reg_loc=p_l->reg_loc;	//Ser� passado o registro do locat�rio para o im�vel
			
			
			if((fptr3=fopen("proprietario.bin", "rb"))==NULL)
				printf("\nErro ao abrir o arquivo!\n\n");
			else{

				while(achou!=1 && i<Qreg_p){
					fseek(fptr3, i*sizeof(prop), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
					fread(p_p, sizeof(prop), 1, fptr3);		//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
					//compara o registro do imovel com o da casa do proprietario
					for(j=0; j<(p_p->qtde_casa); j++){
						
						if(p_i->reg_imov==(p_p->casa+j)->num_casa){
							(p_p->casa+j)->status_casa='A';
							achou=1;//se achar, recebe 1
						}
					}	
					i++;
				}
				fclose(fptr3);
			}
			fclose(fptr2);			
		}
		fclose(fptr);
		grava_imov(p_i, pos_i, "rb+");
		if(achou == 1){
			grava_prop(p_p,i-1,"rb+");	
		}
	}
}//torna_alugado

void info_aluguel(locatario *p_l, int pos)
{
	int i, j;
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	if((fptr=fopen("locatario.bin", "rb"))==NULL){
		printf("\t\t\t==LOCATARIOS CADASTRADO==\n\n");
		printf("\tN�o encontramos nenhum locat�rio em nosso sistema!\n\n");
	}
	else
	{
		printf("\t\t\t==IMOBILI�RIA==\n\n");
		printf("\tDesculpe, mas infelizmente este im�vel j� esta sendo alugado!\n");
		printf("\tInforma��es referente ao aluguel\n\n");
		
		fseek(fptr, pos*sizeof(locatario), 0);		//posiciona o ponteiro no arquivo de acordo com o pos em rela��o a origem
		fread(p_l, sizeof(locatario), 1, fptr);		//l� a estrutura do arquivo guardando no ponteiro p_l
			
		//mostra todos os dados referente ao aluguel
	    printf(" Registro do Imovel: %i\n\tDia de Vencimento: %i\n\tInicio: %s -- Fim: %s\n",p_l->reg_imov,p_l->dia_venc,p_l->inicio,p_l->termino);

		printf("\n=============================================================================\n\n");
		
		fclose(fptr);	//fecha arquivo;
	}
	
	system("PAUSE");	//pausa o programa
	system("CLS");		//limpa a tela
}//info_aluguel

void editar_prop(prop *p_p, int Qreg_p)
{
	char n_cpf[15];		//cpf auxiliar, ser� usado para verificar a exist�ncia do mesmo
	int op,pos;
	printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
	
	printf(" Informe o CPF: ");	//l� o CPF do propriet�rio
	gets(n_cpf);
	pos=verifica_cpfProp(p_p, Qreg_p, n_cpf);
	
	//se a fun��o retornar algo diferente de -1, quer dizer o cpf j� est� cadastrado.
	if(pos == -1){
		printf("\t\t\t==EDITAR DE PROPRIETARIO==\n\n");
		printf("\tOps! O CPF digitado nao est� cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a pr�xima a��o
		system("CLS");	//limpa tela
		return;
	}
	else{
		do{
			printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
			printf("\tOque deseja alterar neste proprietario?\n\n\t[1] - Nome\n\t[2] - Logradouro\n\t[3] - Bairro\n\t[4] - CEP");
			printf("\n\t[5] - Cidade\n\t[6] - Fone\n\t[7] - Estado\n\t[8] - Cel\n\t[9] - Email\n\t[0] - Sair\n\n->Sua op��o: ");
		
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<0 || op>9){
				printf("\n Desculpe n�o entendi!\n");
				sleep(1);
				system("CLS");
			}
		}while(op<0 || op>9);
		system("cls");
		printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
		switch (op){
			case 1:
				printf(" Novo Nome: ");
				gets(p_p->nome);
				break;
			case 2:	
				printf(" Novo Logradouro: ");
				gets(p_p->end_prop.logradouro);
				break;	
			case 3:		//consulta propriet�rio		
				printf(" Novo Bairro: ");
				gets(p_p->end_prop.bairro);
				break;
			case 4:		//consulta im�vel
				printf(" Novo CEP: ");
				gets(p_p->end_prop.CEP);//Consulta especifica
				break;
			case 5:
				printf(" Novo Cidade: ");
				gets(p_p->end_prop.cidade);
				break;
			case 6:
				printf(" Novo Fone: ");
				gets(p_p->end_prop.fone);
				break;
			case 7:
				printf("Novo Estado: ");
				gets(p_p->end_prop.estado);
				break;
			case 8:
				printf(" Novo Cel: ");
				gets(p_p->end_prop.cel);
				break;
			case 9:
				printf(" Novo Email: ");
				gets(p_p->end_prop.email);
				break;
			default:	//retorna ao menu inicial
				printf("\n Retornando");
				Sleep(200);
				printf(".");
				Sleep(200);
				printf(".");
				Sleep(200);
				printf(".");
				Sleep(150);
				system("CLS");
		}
				
		grava_prop(p_p, pos,"rb+");	//rb+ � para ler e atualizar os dados do arquivo
	}
	
	printf("\n\tAlteracao realizada com sucesso!\n\n");
	system("PAUSE");	//pausa a tela
	system("CLS");		//limpa a tela
}//editar_prop

void editar_imov(imovel *p_i, int Qreg_i)
{
	int i, nreg, pos,op;
	
	printf("\t\t\t==EDITAR IMOVEL==\n\n");
	
	printf("\tInforme o numero de registro do imovel: ");
    scanf("%i",&nreg);
    fflush(stdin);
	system("cls");
	pos=verifica_regImov(p_i, Qreg_i, nreg);	
	
	if(pos==-1){
		printf("\t\t\t==EDITAR IMOVEL==\n\n");
		printf("\tDesculpe, mas n�o encontramos o numero de registro do imovel informado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		sleep(3);
		system("CLS");
		return;	
	}		
		else{
		do{
			printf("\t\t\t==EDITAR IMOVEL==\n\n");
			printf("\tOque deseja alterar neste imovel?\n\n\t[1] - Logradouro\n\t[2] - Bairro\n\t[3] - CEP");
			printf("\n\t[4] - Cidade\n\t[5] - Area\n\t[6] - Quartos\n\t[7] - Valor\n\t[0] - Sair\n\n->Sua op��o: ");
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<1 || op>8){
				printf("\n Desculpe n�o entendi!\n");
				sleep(1);
				system("CLS");
			}else{
				system("cls");
				printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
				switch (op){
					case 1:	
						printf(" Novo Logradouro: ");
						gets(p_i->end_imov.logradouro);
						break;	
					case 2:				
						printf(" Novo Bairro: ");
						gets(p_i->end_imov.bairro);
						break;
					case 3:		
						printf(" Novo CEP: ");
						gets(p_i->end_imov.CEP);
						break;
					case 4:
						printf(" Nova Cidade: ");
						gets(p_i->end_imov.cidade);
						break;
					case 5:
						printf(" Nova Area: ");
						scanf("%f",&(p_i->area));
						fflush(stdin);
						break;
					case 6:
						printf(" Nova quantidade de Quartos: ");
						scanf("%i",&(p_i->quartos));
						fflush(stdin);
						break;
					case 7:
						printf(" Novo Valor: ");
						scanf("%f",&(p_i->valor));
						fflush(stdin);
						break;
					default:	//retorna ao menu inicial
						printf("\n\tAlteracao realizada com sucesso!\n\n");
						system("pause");
						printf("\t\nRetornando");
						Sleep(200);
						printf(".");
						Sleep(200);
						printf(".");
						Sleep(200);
						printf(".");
						Sleep(150);
						system("CLS");
				}
				system("CLS");	
			}
		}while(op<0 || op>7);	
	}
	
	grava_imov(p_i, pos,"rb+");
}//editar_imov

void editar_loc(locatario *p_l, int Qreg_l)
{
	char n_cpf[15];		//cpf auxiliar, ser� usado para verificar a exist�ncia do mesmo
	int op,pos;
	printf("\t\t\t==EDITAR LOCAT�RIO==\n\n");
	
	printf(" Informe o CPF: ");	//l� o CPF do propriet�rio
	gets(n_cpf);
	pos=verifica_cpfLOC(p_l, Qreg_l, n_cpf);
	
	system("CLS");
	
	//se a fun��o retornar algo diferente de -1, quer dizer o cpf j� est� cadastrado.
	if(pos == -1){
		printf("\t\t\t==EDITAR DE LOCAT�RIO==\n\n");
		printf("\tOps! O CPF digitado nao est� cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a pr�xima a��o
		system("CLS");	//limpa tela
		return;
	}
	else{
		do{
			printf("\t\t\t==EDITAR LOCAT�RIO==\n\n");
			printf("\t Oque deseja alterar neste proprietario?\n\n\t[1] - Nome\n\t[2] - Logradouro\n\t[3] - Bairro\n\t[4] - CEP");
			printf("\n\t[5] - Cidade\n\t[6] - Fone\n\t[7] - Estado\n\t[8] - Cel\n\t[9] - Email\n\t[0] - Sair\n\n->Sua op��o: ");
		
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<0 || op>9){
				printf("\n Desculpe n�o entendi!\n");
				sleep(1);
				system("CLS");
			}
		}while(op<0 || op>9);
		system("cls");
		printf("\t\t\t==EDITAR LOCAT�RIO==\n\n");
		switch (op){
			case 1:
				printf(" Novo Nome: ");
				gets(p_l->nome);
				break;
			case 2:	
				printf(" Novo Logradouro: ");
				gets(p_l->end_loc.logradouro);
				break;	
			case 3:		//consulta propriet�rio		
				printf(" Novo Bairro: ");
				gets(p_l->end_loc.bairro);
				break;
			case 4:		//consulta im�vel
				printf(" Novo CEP: ");
				gets(p_l->end_loc.CEP);//Consulta especifica
				break;
			case 5:
				printf(" Novo Cidade: ");
				gets(p_l->end_loc.cidade);
				break;
			case 6:
				printf(" Novo Fone: ");
				gets(p_l->end_loc.fone);
				break;
			case 7:
				printf("Novo Estado: ");
				gets(p_l->end_loc.estado);
				break;
			case 8:
				printf(" Novo Cel: ");
				gets(p_l->end_loc.cel);
				break;
			case 9:
				printf(" Novo Email: ");
				gets(p_l->end_loc.email);
				break;
			default:	//retorna ao menu inicial
				printf("\n Retornando");
				Sleep(200);
				printf(".");
				Sleep(200);
				printf(".");
				Sleep(200);
				printf(".");
				Sleep(150);
				system("CLS");
		}
				
		grava_loc(p_l, pos,"rb+");	//rb+ � para ler e atualizar os dados do arquivo
	}
	
	printf("\n\tAlteracao realizada com sucesso!\n\n");
	system("PAUSE");	//pausa a tela
	system("CLS");		//limpa a tela
}//editar_loc

void encerra_contrato(locatario *p_l, int Qreg_l, imovel *p_i, int Qreg_i, prop *p_p, int Qreg_p)
{
	//transforma o cpf em @
	
	char n_cpf[15], data[11];
	int pos_l, pag,r_i,pos_i,i=0, achou=0,j,pos_casa;
	FILE *fptr=NULL;
	
	printf("\t\t\t==ENCERRAR CONTRATO==\n\n");
	
	printf(" Informe o CPF: ");
	gets(n_cpf);
	
	pos_l=verifica_cpfLOC(p_l, Qreg_l, n_cpf);	//ir� ir para fun��o de verificar o CPF e retorna a posi��o encontrada
	
	//caso n�o encontrar nenhum cpf
	if(pos_l == -1){
		printf("\n\tOps! O CPF digitado nao est� cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a p�gina anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a pr�xima a��o
		system("CLS");	//limpa tela
		return;
	}
	else{
		mostra_loc(p_l, pos_l+1);	//+1 por conta do for
		pag=verifica_data(p_l);		//recebe da verifica_data se a data est� anterior ao t�rmino do contrato ou n�o	
		r_i = p_l->reg_imov;
		pos_i = verifica_regImov(p_i,Qreg_i,r_i);	//vai buscar a posi��o do im�vel
		
		//se estiver cancelando antes do t�rmino
		if(pag==1){
			system("CLS");
			printf("\t\t\t==ENCERRAR CONTRATO==\n\n");
			printf("\n\tDevido ao cancelamento ocorrer antes da data de t�rmino de contrato.\n\tDever� ser pago uma multa: R$");
			printf("%.2f\n\n", p_i->valor);
		}
		
		//se estiver cancelando depois do t�rmino
		else{
			system("CLS");
			printf("\t\t\t==ENCERRA CONTRATO==\n\n");
			printf("\n\tEst� livre de multa!\n\n");
		}	
	}
	system("PAUSE");
	system("CLS");
		
	//verifica se existe o arquivo
	if((fptr=fopen("proprietario.bin", "rb"))==NULL){
		printf("\t\t==ENCERRA CONTRATO==\n");
	  	printf("\nNenhum proprietario cadastrado! Cadastre um novo!\n");
	  	Sleep(200);
 		system("CLS");
	  	achou=0;
	}
	else{
		while(achou!=1 && i<Qreg_p){
			fseek(fptr, i*sizeof(prop), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_p, sizeof(prop), 1, fptr);	//l� do ponteiro da estrutura propriet�rio, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			for(j=0;j<p_p->qtde_casa;j++)
			{	
				//verifica se o num da casa � igual ao reg do imovel
				if((p_p->casa+j)->num_casa == p_i->reg_imov){
					achou=1;
					pos_casa=j;
					j=p_p->qtde_casa+1;
				}
			}
			i++;
		}
		fclose(fptr);	//fecha o arquivo
	}
	if(achou==1){
		//muda de alugado para livre
		(p_p->casa+pos_casa)->status_casa = 'L';
		p_i->status.sigla='L';
		p_i->status.loc.sigla='L';	
		p_i->status.loc.reg_loc=-1;
		
		//copia o @ para o CPF do locat�rio
		strcpy(p_l->CPF,"@");
		
		//grava os arquivos
		grava_loc(p_l,pos_l,"rb+");
		grava_prop(p_p,i-1,"rb+");
		grava_imov(p_i,pos_i,"rb+");				
	}else{
		printf("\nREGISTRO NAO ENCONTRADO");		//caso n�o for encontrado o cpf
	}
	
	
}//deleta_loc

void gerar_rel(locatario *p_l, int Qreg_l, prop *p_p, int Qreg_p,imovel *p_i, int Qreg_i)
{
	
//	entrar com o dia e mostrar n� de registro do im�vel com vencimento para o dia
//	escolhido, bem como, valor, registro, nome e CPF do locat�rio, registro, nome e CPF
//	do propriet�rio.
	int dia, i=0,j,k, achou;
	FILE *fptr_l=NULL,*fptr_p=NULL, *fptr_i=NULL;
	printf("\t\t\t==RELATORIO==\n");
	printf("\n\tInsira o dia de vencimento: ");
	scanf("%i",&dia);
	fflush(stdin);
	system("cls");
	printf("\t\t\t==RELATORIO==\n\n\tDIA DE VENCIMENTO: %i\n",dia);
	printf("\n=======================================================================\n\n");
	//verifica se existe o arquivo
	if((fptr_l=fopen("locatario.bin", "rb"))==NULL){
	  	printf("\nNenhum locat�rio cadastrado! Cadastre um novo!\n");
	  	Sleep(500);
 		system("CLS");
	  	achou=0;
	}
	else{
		for(i=0;i<Qreg_l;i++){
			fseek(fptr_l, i*sizeof(locatario), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_l, sizeof(locatario), 1, fptr_l);		//l� do ponteiro da estrutura locatario, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara o dia digitado com o do vencimento
			if(p_l->dia_venc == dia){
				if(strcmp(p_l->CPF, "@")!=0){
					if((fptr_i=fopen("imovel.bin", "rb"))==NULL){
					  	printf("\nNenhum imovel cadastrado! Cadastre um novo!\n");
					  	Sleep(500);
				 		system("CLS");
					}else{
						printf("\t--IMOVEL--");
						for(j=0;j<Qreg_i;j++){
							fseek(fptr_i, j*sizeof(imovel), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
							fread(p_i, sizeof(imovel), 1, fptr_i);
							if(p_l->reg_imov == p_i->reg_imov){
								printf("\n\t\tRegistro do Imovel: %i\n\t\tValor: %.2f\n\n",p_i->reg_imov,p_i->valor);
								j=Qreg_i+1;
							}
						}
						printf("\t--LOCATARIO--");
						printf("\n\t\tRegistro do Locatario: %i\n\t\tNome: %s\n\t\tCPF: %s\n\n",p_l->reg_loc,p_l->nome,p_l->CPF);
						fclose(fptr_i);	
					}
					if((fptr_p=fopen("proprietario.bin", "rb"))==NULL){
						  	printf("\nNenhum proprietario cadastrado! Cadastre um novo!\n");
						  	Sleep(500);
					 		system("CLS");
					}else{
						for(j=0;j<Qreg_p;j++){
							fseek(fptr_p, j*sizeof(prop), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
							fread(p_p, sizeof(prop), 1, fptr_p);
							for(k=0;k<p_p->qtde_casa;k++){
								if((p_p->casa+k)->num_casa == p_i->reg_imov){
									printf("\t--PROPRIETARIO--");
									printf("\n\t\tRegistro do Proprietario: %i\n\t\tNome: %s\n\t\tCPF: %s\n\n",p_p->reg_prop,p_p->nome,p_p->CPF);
									k=p_p->qtde_casa+1;
								}
							}
						}
						fclose(fptr_p);	
					}
					printf("\n=======================================================================\n\n");	
				}
			}
		}
		fclose(fptr_l);	//fecha o arquivo
	}
	
	system("PAUSE");
	system("cls");	   	 	
}

