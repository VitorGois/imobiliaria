/*******************************************************************************************************************
		Projeto Imobiliária 	-	Programação Estruturada II	-	Prof. Talita Berbel	-	01/2020

				Alunos:		Raul Ryan Deaque Silva 			RA: 190953
							Otavio Cordeiro de Freitas		RA: 190702
							Vitor Joaquim de Carvalho Gois	RA: 190810
********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

typedef struct endereco{		//Estrutura que contêm os dados de endereço do proprietário
	char logradouro[80];
	char bairro[20];
	char CEP[10];
	char cidade[20];
	char estado[20];
	char fone[15];
	char cel[15];
	char email[30];
}endereco;

typedef struct info_casa{		//Estrutura que contêm as informações sobre as casas cadastradas pelo proprietário
	int num_casa;				//Número da casa/registro do imóvel
	char status_casa;			//Status, se ela está [L]ivre ou [A]lugada
}info_casa;

typedef struct prop{			//Estrutura que contêm os dados do proprietário
	int reg_prop;				//Registro de cadastro do proprietario (gerado automaticamente)
	char nome[80];
	char CPF[15];
	struct endereco end_prop;	//Chama a estrutura endereco, chamando-a de "end_prop"
	int qtde_casa;				//Quantas casas o proprietário possui cadastrada
	struct info_casa casa[5]; 	//Chama a estrutura info_casa, para guardar as informações da casa cadastrada, podendo ter no máximo 5 casas
}prop;

typedef struct adress{			//Estrutura que contêm os dados de endereço do imóvel cadastrado
	char logradouro[80];
	char bairro[20];
	char CEP[10];
	char cidade[20];
}adress;

typedef struct info_loc{		//Informação do imóvel com o registro de quem está alugando (locatário)
	char sigla;
	int reg_loc;
}inf_loc;

union dados{					//Dados do imóvel alugado
	char sigla;
	struct info_loc loc;
};

typedef struct imovel{			//Estrutura que contêm os dados sobre o imóvel cadastrado
	int reg_imov; 				//Registro do imóvel (gerado automaticamente)
	struct adress end_imov;		//Chama a estrutura adress, chamando-a de "end_imov"
	float area;					//Área da casa
	int quartos;				//Quantos quartos há no imóvel
	float valor;				//Valor do aluguel
	union dados status;
}imovel;

typedef struct locatario{		//Estrutura que contêm os dados referente ao locatári{		
	int reg_loc; 					
	char nome[80];
	char CPF[15];
	struct endereco end_loc;		//Chama a estrutura endereço, nomeando-a como end_loc
	int reg_imov; 					
	int dia_venc;
	char inicio[9];
	char termino[9];
}locatario;

void aloca_prop(prop **p,int tam);										// Alocação de espaço na memória para a estrutura prop
void aloca_imov(imovel **p,int tam);									// Alocação de espaço na memória para a estrutura imovel
void aloca_loc(locatario **p,int tam);									// Alocação de espaço na memória para a estrutura locatario
int menu_geral();														// Menu Geral
int menu_adm();															// Menu adminsitração
int menu_prop();														// Menu de opções para o proprietário
int menu_loca();														// Menu de opções para o locatário	
int verifica_qtdReg(int acao);											// Verifica quantos registros foram gravados no arquivo e retorna a quantidade
int verifica_cpfProp(prop *p_p, int Qreg_p, char *n_cpf);				// Verifica se o cpf do proprietário já está registrado
int verifica_cpfLOC(locatario *p_l, int Qreg_l, char *n_cpf);			// Verifica se o cpf do locatário já está registrado
int verifica_regImov(imovel *p_i, int Qreg_i, int r_i);					// Verifica se existe o registro de imóvel informado
int verifica_disp(imovel *p_i, int pos_i);								// Verifica a disponibilidade do imóvel
int verifica_data(locatario *p_l);										// Realiza a verificação da data inforada pelo locatário
int cadastro_prop(prop *p_p, int Qreg_p);								// Realiza o cadastro do proprietário
int cadastro_imov(imovel *p_i, int Qreg_i, prop *p_p, int Qreg_p);		// Realiza o cadastro do proprietário
int cadastro_loc(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i);	// Realiza o cadastro do locatário
void grava_prop(prop *p_p, int pos, char *acao);						// Realiza a gravação dos dados do proprietário no arquivo
void grava_imov(imovel *p_i, int pos, char *acao);						// Realiza a gravação dos dados do imóvel no arquivo
void grava_loc(locatario *p_l, int pos, char *acao);					// Realiza a gravação dos dados do locatário
void mostra_p(prop *p_p, int Qreg_p);									// Mostra todos os dados do proprietário
void mostra_imov(imovel *p_i,int Qreg_i);								// Mostra todos os dados do imóvel
void mostra_loc(locatario *p_l,int Qreg_l);								// Mostra todos os dados do locatário
void consulta_prop(prop *p_p,int Qreg_p);								// Faz a consulta sobre determinado proprietário
void consulta_imov(imovel *p_i, int Qreg_i);							// Faz a consulta sobre determinado imóvel
void consulta_loc(locatario *p_l,int Qreg_l);							// Faz a consulta sobre determinado locatário
void torna_alugado(locatario *p_l, int Qreg_l, imovel *p_i, int pos_i, prop *p_p, int Qreg_p);	// Torna o imóvel [L]ivre para [A]lugado							
void info_aluguel(locatario *p_l, int pos);								// Mostra informação do imóvel já alugado
void editar_prop(prop *p_p, int Qreg_p);								// Faz alteração nos dados do proprietário já cadastrado
void editar_imov(imovel *p_i, int Qreg_i);								// Faz alteração nos dados do imóvel já cadastrado
void editar_loc(locatario *p_l, int Qreg_l);							// Faz alteração nos dados do locatário já cadastrado
void encerra_contrato(locatario *p_l, int Qreg_l, imovel *p_i, int Qreg_i,prop *p_p, int Qreg_p);	// Deleta os dados do locatário
void gerar_rel(locatario *p_l, int Qreg_l, prop *p_p, int Qreg_p,imovel *p_i, int Qreg_i);	// Gera um relatório à respeito de uma locação

main(){
	//definindo o idioma como português e o nome do executavel
	setlocale(LC_ALL, "Portuguese");
	SetConsoleTitle("Imobiliaria Geracao");
	
	//declaração dos ponteiros para estrutura
	prop *p_p=NULL;
	imovel *p_i=NULL;
	locatario *p_l=NULL;
	
	//declaração de variáveis
	int op_p, op_l, op_inicial, op_adm, Qreg_p, Qreg_i, Qreg_l, cont=0, cad, pos, r_i, verif;
	
	//alocando espaço
	aloca_prop(&p_p, 1);
	aloca_imov(&p_i, 1);
	aloca_loc(&p_l, 1);
	
	//verificar quantidade de registros <-1 e 2 indicam a para quem queremos apontar, proprietário ou imóvel->
	Qreg_p=verifica_qtdReg(1);
	Qreg_i=verifica_qtdReg(2);
	Qreg_l=verifica_qtdReg(3);
	
	//Laço com as opções escolhida pelo usuário através da função menu
	do{
		op_inicial=menu_geral();
			
		switch(op_inicial){
			case 1:
				do{
					op_p=menu_prop();	//recebe a opção escolhida pelo usuário
		
					switch(op_p){
						case 1:				//cadastro proprietário
							Sleep(300);		//atraso para próxima ação
							system("CLS");	//limpa a tela
								
							cad=cadastro_prop(p_p, Qreg_p+1);	//retorna um valor para verificar se CPF já está cadastrado
							if(cad!=-1)	//se não existir cadastro irá ser somado +1 no Qreg_p
								Qreg_p++;
							break;
						case 2:				//cadastro imóvel
							Sleep(300);		
							system("CLS");
							
							cad=cadastro_imov(p_i, Qreg_i+1, p_p, Qreg_p);	//retorna um valor para verificar se o imóvel já está cadastrado
							if(cad!=-1)	//se não existir cadastro irá ser somado +1 no Qreg_i
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
					op_l=menu_loca();		//recebe a opção escolhida pelo usuário
					
					switch(op_l){
						case 1:		//mostra imóveis livres
							Sleep(300);
							system("CLS");
							
							consulta_imov(p_i, Qreg_i);
							break;
						case 2:		//consulta por registro
							Sleep(300);
							system("CLS");
							
							printf("\t\t\t==IMOBILIARIA==\n\n");
							
							printf(" Informe o número de registro do imóvel: ");
							scanf("%i", &r_i);		//recebe o registro que o usuário digitar
							fflush(stdin);
							
							Sleep(300);
							system("CLS");
							
							pos=verifica_regImov(p_i, Qreg_i, r_i);		//a função retorna a pos que se encontra a estrutura cujo o registro foi informado pelo usuário
							
							if(pos==-1){
								printf("\n\tDesculpe mas não foi possível encontrar este registro!\n\n");
								sleep(2);
								system("CLS");
							}
							else{
								verif=verifica_disp(p_i, pos);
								
								if(verif!=-1){
									if(verif==1){	//1 se o imóvel estiver Livre
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
									printf("\n\tDesculpe, mas não encontrei nenhum imóvel!\n\n");
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
						case 1:		//exibir proprietários
							mostra_p(p_p, Qreg_p); //mostra os proprietários cadastrados
							break;
						case 2:		//exibir imóveis
							mostra_imov(p_i, Qreg_i); //mostra os imoveis cadastrados
							break;
						case 3:		//exibir locatarios
							mostra_loc(p_l, Qreg_l);
							break;
						case 4:		//consulta proprietário		
							consulta_prop(p_p, Qreg_p);
							break;
						case 5:		//consulta imóvel
							consulta_imov(p_i, Qreg_i); //Consulta especifica
							break;
						case 6:		//consulta locatário
							consulta_loc(p_l, Qreg_l);
							break;
						case 7:		//editar dados do proprietário
							editar_prop(p_p, Qreg_p);
							break;
						case 8:		//editar dados do imóvel
							editar_imov(p_i, Qreg_i);
							break;
						case 9:		//editar dados do locatário
							editar_loc(p_l, Qreg_l);
							break;
						case 10:	//deleta os dados do locatário
							encerra_contrato(p_l, Qreg_l, p_i, Qreg_i, p_p, Qreg_p);
							break;
						case 11:	//gerar relatório
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
		printf("\n\tBem Vindo à Imobiliária Geração!\n\tEm que podemos ajudar?\n\n");
		printf("\t[1] - Sou Propriétario\n\t[2] - Sou Locatário\n\t[3] - Sou Admin\n\t[0] - Desejo sair\n\n-> Sua opção: ");
		scanf("%i", &op);
		
		if(op<0 || op>3){
			printf("\n Desculpe não entendi!\n");
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
		printf("\n\tBem Vindo à administração!\n\tEm que posso ajudar?\n\n");
		
		printf("\t[1] - Exibir Proprietários\n\t[2] - Exibir Imóveis\n\t[3] - Exibir Locatários\n\t[4] - Consulta Proprietário\n\t[5] - Consulta Imóvel");
		printf("\n\t[6] - Consulta Locatário\n\t[7] - Editar Proprietario\n\t[8] - Editar Imovel\n\t[9] - Editar Locatario\n\t[10] - Encerrar Contrato");
		printf("\n\t[11] - Gerar Relatório\n\n\t[0] - Menu Principal\n\n-> Sua opção: ");
		scanf("%i", &op);
		fflush(stdin);
		
		if(op<0 || op>11){
			printf("\n Desculpe não entendi!\n");
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
	
	//Laço para que caso o usuário digite uma opção inválida, possa digitar novamente.
	do{
		printf("\t\t\t==IMOBILIARIA==\n");

		printf("\n\t[1] - Cadastrar Propietario\n\t[2] - Cadastrar Imovel");
		printf("\n\t[0] - Menu inicial\n\n->Opcao: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<0 || op>2){
			printf("\n Desculpe, não entendi!\n\n");	
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>2);
	
	//Retorna a opção desejada pelo usuário à main
	return op;
}//menu

int menu_loca()
{
	int op;
	
	//Laço para que caso o usuário digite uma opção inválida, possa digitar novamente.
	do{
		printf("\t\t\t==IMOBILIARIA==\n\n");

		printf("\tO que deseja fazer?\n\n");
		printf("\t[1] - Ver imóveis\n\t[2] - Alugar imóvel (nº registro)\n\t[0] - Menu inicial\n\n->Sua opção: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<0 || op>2){
			printf("\n Desculpe, não entendi!\n\n");	
			sleep(1);
			system("CLS");
		}
	}while(op<0 || op>2);
	
	//Retorna a opção desejada pelo usuário à main
	return op;
}//menu_loca

int verifica_qtdReg(int acao)
{
	long int cont=0;	//contador para quantidade de registros no arquivo
	FILE *fptr=NULL;	//ponteiro para o arquivo
	
	//Verifica qtd Registro para o proprietário
	if(acao==1){
		if((fptr=fopen("proprietario.bin", "rb"))==NULL){
			return cont;
		}else{
			fseek(fptr, 0, 2);	//função para posicionar o ponteiro no fim do arquivo
			cont=ftell(fptr)/sizeof(prop);	//contador irá receber a quantidade de estruturas que há dentro do arquivo
			fclose(fptr);	//fecha o arquivo
			return cont;	//retorna a quantidade de arquivos encontrados
		}
	}
	
	//Verifica qtd Registro para o imóvel
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
	
	//Verifica qtd Registro para o locatário
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
			fread(p_p, sizeof(prop), 1, fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara o cpf digitado com o que está dentro do arquivo
			if(strcmp(n_cpf, p_p->CPF)==0)
				achou=1;	//se achar, recebe 1
			i++;
		}
		fclose(fptr);	//fecha o arquivo
	}
	if(achou==1){
		return (i-1);	//-1 por conta do i++ no final do while
	}else{
		return (-1);	//caso não for encontrado o cpf
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
		printf("\t\t==CADASTRO DE LOCATÁRIO==\n");
	  	printf("\nNenhum locatário cadastrado! Cadastre um novo!\n");
	  	Sleep(500);
 		system("CLS");
	  	achou=0;
	}
	else{
		while(achou!=1 && i<Qreg_l){
			fseek(fptr, i*sizeof(locatario), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_l, sizeof(locatario), 1, fptr);		//lê do ponteiro da estrutura locatario, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara o cpf digitado com o que está dentro do arquivo
			if(strcmp(n_cpf, p_l->CPF)==0)
				achou=1;	//se achar, recebe 1
			i++;
		}
		fclose(fptr);	//fecha o arquivo
	}
	if(achou==1){
		return (i-1);	//-1 por conta do i++ no final do while
	}else{
		return (-1);	//caso não for encontrado o cpf
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
			fread(p_i, sizeof(imovel), 1, fptr);	//lê a estrutura guardada no arquivo	
			
			if(r_i==p_i->reg_imov)		//se o registro informado for igual ao registro cadastrado, achou aponta 1
				achou=1;
			i++;
		}
		fclose(fptr);
	}
	
	if(achou==1)
		return (i-1);	//-1 por conta do i++ no final de cada while
	else
		return -1;		//caso não encontrar retorna -1 para main
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
	
	printf("\t\t\t==REMOVER LOCATÁRIO==\n\n");
	
	printf("Data atual (dd/mm/aa): ");
	gets(data_atual);
	
	strcpy(data_termino, p_l->termino);		//copia o termino do locatario para a data de termino
	
	//a função strtok separa em tokens(segmentos) de uma string definindo o limite de cada token, neste caso com "/"
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
	char n_cpf[15];		//cpf auxiliar, será usado para verificar a existência do mesmo
	
	printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
	
	printf(" Informe seu CPF: ");	//lê o CPF do proprietário
	gets(n_cpf);
	
	//se a função retornar algo diferente de -1, quer dizer o cpf já está cadastrado.
	if(verifica_cpfProp(p_p, Qreg_p, n_cpf) != -1){
		printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
		printf("\tOps! O CPF digitado já está cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a próxima ação
		system("CLS");	//limpa tela
		
		return(-1);	//retornando -1 para a main
	}
	//caso a função verifica_cpf retornar -1, quer dizer que o cpf não foi cadastrado
	else{
		printf("\t\t\t==CADASTRO DE PROPRIETARIO==\n\n");
		printf("\tEBA! Um novo integrante, conclua seu cadastro conosco abaixo!\n\n");
		
		strcpy(p_p->CPF,n_cpf);		//copia o CPF digitado pelo usuário para a estrutura
		p_p->reg_prop=Qreg_p;		//o reg_prop da estrutura recebe o registro da main (quantidade de registros)
		
		printf(" Seu registro: %i\n",p_p->reg_prop);
		printf(" Nome: ");
		gets(p_p->nome);
		fflush(stdin);
		
		//guarda os dados do endereço do proprietário na estrutura aninhada do proprietário e endereço
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
		
		//incializa a quantidade de casa registrado pelo usuário como 0
		p_p->qtde_casa = 0;
		fflush(stdin);
		
		//função para guardar estes dados informados pelo usuário
		grava_prop(p_p, Qreg_p, "ab");	//ab é para atualizar os dados do arquivo
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
	
	pos=verifica_cpfProp(p_p, Qreg_p, n_cpf);	//verifica se o CPF já foi registrado
	
	//se o retorno da verifica_cpf for -1 não foi encontrado o cpf retornando -1 para a main
	if(pos==-1){
		printf("\t\t\t==CADASTRO DE IMOVEL==\n\n");
		printf("\tDesculpe, mas não encontramos o CPF informado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		sleep(3);
		system("CLS");
		return (-1);	
	}		
	else{
		qtd_c=p_p->qtde_casa;
		
		//Verifica se o foi atingido o limite de casas pertencentes ao proprietario
		if(qtd_c >= 5){
			printf("\t\t\t==CADASTRO DE IMOVEL==\n\n");
			printf("\n\tPera lá apressadinho! Você já atingiu o limite máximo de 5 casas.\n\n");
			sleep(3);
			system("CLS");
			return(-1);
		}
		printf("\t\t\t==CADASTRO DE IMOVEL==\n");
		
		(p_p->casa+qtd_c)->num_casa = Qreg_i;		//aponta para a estrutura aninhada do proprietário com info_casa, recebendo o registro(número) da casa
		(p_p->casa+qtd_c)->status_casa = 'L'; 		//aponta para a estrutura aninhada do proprietário com info_casa, inicializa imóvel como [L]ivre
		p_p->qtde_casa +=1;							//adicionando mais uma casa para o proprietário
		
		grava_prop(p_p,pos,"rb+");	//atualiza a quantidade de casas do proprietário no arquivo
		
	    p_i->reg_imov = Qreg_i;								//registro do imóvel recebe o indice do imovel
	    printf("\n Registro do imóvel: %i\n",p_i->reg_imov);	//mostra o registro do imovel
	    
	    //guarda os dados do imóvel na estrutura aninhada do proprietário e endereço
		//fflush(stdin) limpa o buffer do teclado
		printf("\n\tInformações de Endereço\n");
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
	    printf("\n\tAtributos da residência\n");
	    printf(" Área util(m²): ");
	    scanf("%f",&(p_i->area));
	    fflush(stdin);
	    printf(" Quantidade de quartos: ");
	    scanf("%i",&(p_i->quartos));
	    fflush(stdin);
	    printf(" Valor do aluguel: R$");
	    scanf("%f",&(p_i->valor));
	    fflush(stdin); 	
	    p_i->status.loc.sigla = 'L';		//estrutura aninhada do imóvel com a union dados com a estrutura info_loc para guardar o status do imóvel ->[A]lugado ou [L]ivre<-
	    
	    grava_imov(p_i, Qreg_i, "ab");		//grava os dados do imóvel no arquivo
	    
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
	
	printf("\t\t\t==CADASTRO DE LOCATÁRIO==\n\n");
	
	printf(" Informe o seu CPF: ");	
	gets(n_cpf);
	
	if(verifica_cpfLOC(p_l, Qreg_l, n_cpf) != -1){
		printf("\t\t\t==CADASTRO DE LOCATÁRIO==\n\n");
		printf("\tOps! O CPF digitado já está cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		
		Sleep(700);		
		system("CLS");	
		
		return(-1);	
	}

	else{
		printf("\t\t==CADASTRO DE LOCATÁRIO==\n\n");
		
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
		
		printf(" Data Início (dd/mm/aa): ");
		gets(p_l->inicio);
		
		printf(" Data Término (dd/mm/aa): ");
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
	
	//se tentar abrir o arquivo e não encontrar nada dará erro senão abrirá normal
	if((fptr=fopen("proprietario.bin", acao))==NULL)
		printf("Erro ao abrir o arquivo!\n\n");
	else{
		if(strcmp(acao, "rb+")==0)				//faz a comparação da ação que o usuário quer fazer no arquivo com rb+ (utilizado para ler e fazer uma alteração)
			fseek(fptr, pos*sizeof(prop), 0);	//posiciona o arquivo na posição que está a estruturada analisada na cadastro_prop a partir da origem
		fwrite(p_p, sizeof(prop), 1, fptr);		//escreve no arquivo fptr a estrutura do proprietário que foi gravada na memoria
	}
	fclose(fptr);	//fecha arquivo
}//grava_prop

void grava_imov(imovel *p_i, int pos, char *acao)
{
	FILE *fptr=NULL;	//ponteiro para arquivo
	
	if((fptr=fopen("imovel.bin", acao))==NULL)
		printf("\nErro ao abrir o arquivo!\n\n");
	else{
		if(strcmp(acao, "rb+")==0)				//faz a comparação da ação que o usuário quer fazer no arquivo com rb+ (utilizado para ler e fazer uma alteração)
			fseek(fptr, pos*sizeof(imovel), 0);	//posiciona o arquivo na posição que está a estruturada analisada na cadastro_imov a partir da origem
		fwrite(p_i, sizeof(imovel), 1, fptr);	//grava no arquivo a estrutura do imóvel
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
			fseek(fptr, i*sizeof(prop), 0);		//posiciona o ponteiro no arquivo de acordo com o indice I em relação a origem
			fread(p_p, sizeof(prop), 1, fptr);	//lê a estrutura do arquivo guardando no ponteiro p_p
			
			//mostra todos os dados referente a determinado proprietário
			printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_p->reg_prop,p_p->nome,p_p->CPF,p_p->end_prop.logradouro,p_p->end_prop.bairro,p_p->end_prop.CEP);
	    	printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n\n Quantidade de casas: %i\n",p_p->end_prop.cidade,p_p->end_prop.estado,p_p->end_prop.fone,p_p->end_prop.cel,p_p->end_prop.email,p_p->qtde_casa);
	    	
	    	//mostra os dados da residência a ser alugada
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
	    	fseek(fptr,i*sizeof(imovel),0);		//posiciona o ponteiro no arquivo de acordo com o indice I em relação a origem
	   	 	fread(p_i,sizeof(imovel),1,fptr);	//lê a estrutura do arquivo guardando no ponteiro p_i
	   	 	
	   	 	//mostra todos os dados referente ao imóvel
	    	printf("\n Registro: %i\n\n Logradouro: %s\n Bairro: %s\n CEP: %s\n Cidade: %s\n Area (m²): %.2f",p_i->reg_imov,p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade,p_i->area);
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
	    	fseek(fptr,i*sizeof(locatario),0);		//posiciona o ponteiro no arquivo de acordo com o indice I em relação a origem
	   	 	fread(p_l,sizeof(locatario),1,fptr);	//lê a estrutura do arquivo guardando no ponteiro p_i
	   	 	
	   	 	//se ele encontrar um @ não é mostrado, caso contrário é mostrado
	   	 	if(strcmp(p_l->CPF, "@")!=0){
	   	 	//mostra todos os dados referente ao locatário
	    	printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_l->reg_loc,p_l->nome,p_l->CPF,p_l->end_loc.logradouro,p_l->end_loc.bairro,p_l->end_loc.CEP);
	    	printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n\n",p_l->end_loc.cidade,p_l->end_loc.estado,p_l->end_loc.fone,p_l->end_loc.cel,p_l->end_loc.email);
	    	printf(" Registro do Imovel: %i\n Dia de Vencimento: %i\n Inicio: %s -- Fim: %s\n",p_l->reg_imov,p_l->dia_venc,p_l->inicio,p_l->termino);

			printf("\n=============================================================================\n\n");
			flag++;
			}
	    }
	  	fclose(fptr); 	//fecha arquivo
	}
	
	//se não encontrar nenhum usuário no sistema, ou houver @
	if(flag==0){
		printf("\n\tInfelizmente não temos nenhum cadastro no sistema!\n\n");
	}
	
	system("PAUSE");	//pausa o programa
	system("CLS");	
}//mostra_loc

void consulta_prop(prop *p_p,int Qreg_p)
{
	int op,i,j;
	char cpf[15];
	FILE *fptr=NULL;	//ponteiro para arquivo	
	
	//laço para verificar se o usuário digitou a opção correta
	do{
		printf("\t\t\t==CONSULTA DE PROPRIETARIOS==\n\n");
		
		printf("\t[1]Pesquisa total\n\t[2]Pesquisa por CPF\n\n->Digite a forma de consulta: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<1 || op>2)
			printf("\n\tDesculpe, não entendi!\n\n");
			
		Sleep(700);		//atrasa a próxima ação
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
		gets(cpf);		//lê o CPF a ser consultado
		
		for(i=0;i<Qreg_p;i++)
		{
			fseek(fptr,i*sizeof(prop),0);	//posiciona o arquivo na posição que está a estruturada analisada a partir da origem
			fread(p_p,sizeof(prop),1,fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara se o CPF digitado é o mesmo que esta na estrutura
			if(strcmp(cpf,p_p->CPF)==0){
				//mostra todos os dados do proprietário
				printf("\n Registro: %i\n\n Nome: %s\n CPF: %s\n Logradouro: %s\n Bairro: %s\n CEP: %s",p_p->reg_prop,p_p->nome,p_p->CPF,p_p->end_prop.logradouro,p_p->end_prop.bairro,p_p->end_prop.CEP);
			    printf("\n Cidade: %s\n Estado: %s\n Fone: %s\n Cel: %s\n Email: %s\n Qtde de casa: %i\n",p_p->end_prop.cidade,p_p->end_prop.estado,p_p->end_prop.fone,p_p->end_prop.cel,p_p->end_prop.email,p_p->qtde_casa);
			    
			    //mostra todos os dados da residência a ser alugada
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
	
	//laço para verificar se o usuário informou correntamente a opção desejada
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
	   		fseek(fptr,i*sizeof(imovel),0);		//posiciona o arquivo na posição que está a estruturada analisada a partir da origem
	   	 	fread(p_i,sizeof(imovel),1,fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
	   	 	
	   	 	//verifica se a residência está livre para alugar
	    	if(p_i->status.loc.sigla == 'L')
			{
				//mostra todos os dados referente a residência
				printf(" Registro: %i\t\tStatus: [L]ivre\n",p_i->reg_imov);
				printf("\n\tEndereço:");
				printf("\n\t\tLogradouro: %s\n\t\tBairro: %s\n\t\tCEP: %s\n\t\tCidade: %s\n", p_i->end_imov.logradouro,p_i->end_imov.bairro,p_i->end_imov.CEP,p_i->end_imov.cidade);
				printf("\n\tAtributos:");
	    		printf("\n\t\tArea: %.2f\n\t\tQuartos: %i\n\t\tValor: %.2f\n\n",p_i->area, p_i->quartos,p_i->valor,p_i->status.loc.sigla);
			}
			else
			{
				//mostra todos os dados referente a residência
				printf(" Registro: %i\t\tStatus: [A]lugado\n",p_i->reg_imov);
				printf("\n\tEndereço:");
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
		printf("\t[1] - Area util\n\t[2] - Quantidade de quartos\n\t[3] - Bairro\n\nOpção de busca: ");
		scanf("%i",&opc);
		fflush(stdin);
		
		//verifica a opção desejada de consulta do usuário
		switch(opc)
		{
			case 1:	//por área útil
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Qual a área útil (m²) você está procurando: ");
				scanf("%f",&are_uti);
				fflush(stdin);
				break;
			case 2:	//por quantidade de quartos
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Quantos quartos você está procurando: ");
				scanf("%i",&q_q);
				fflush(stdin);
				break;
			case 3:	//por bairro
				Sleep(700);
				system("CLS");
				
				printf("\t\t\t==CONSULTA DE IMOVEL==\n\n");
				
				printf(" Qual bairro você está procurando: ");
				gets(pesq_bairro);
		}
		
		for(i=0;i<t_imov;i++)
		{
			fseek(fptr,i*sizeof(imovel),0);		//posiciona o arquivo na posição que está a estruturada analisada a partir da origem
			fread(p_i,sizeof(imovel),1,fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			// Verificando se o imovel esta livre e se atende ao que foi consultado
			if((p_i->status.loc.sigla =='L') && (strcmp(pesq_bairro,p_i->end_imov.bairro)==0 || p_i->area == are_uti || p_i->quartos == q_q)){
				//dados do imóvel procurado
				printf("\n Registro: %i\t\tStatus: [L]ivre\n",p_i->reg_imov);
				printf("\n\tEndereço:");
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
		printf("\t\t\t==CONSULTA DE LOCATÁRIOS==\n\n");
		
		printf("\t[1]Pesquisa total\n\t[2]Pesquisa por CPF\n\n->Digite a forma de consulta: ");
		scanf("%i",&op);
		fflush(stdin);
		
		if(op<1 || op>2)
			printf("\n\tDesculpe, não entendi!\n\n");
			
		Sleep(700);		//atrasa a próxima ação
		system("CLS");	//limpa a tela
	}while(op<1 || op>2);
	
	if(op==1){
		mostra_loc(p_l, Qreg_l);
	}
	
	if((fptr=fopen("locatario.bin", "rb"))==NULL){
		printf("\t\t\t==LOCATARIOS CADASTRADOS==\n\n");
		printf("\tNão encontramos nenhum locatário em nosso sistema!\n\n");
		return;
	}
	if(op==2){
		printf("\t\t\t==CONSULTA DE PROPRIETARIOS==\n\n");
		
		printf(" Insira o CPF: ");
		gets(cpf);		//lê o CPF a ser consultado
		
		for(i=0;i<Qreg_l;i++)
		{
			fseek(fptr,i*sizeof(locatario),0);	//posiciona o arquivo na posição que está a estruturada analisada a partir da origem
			fread(p_l,sizeof(locatario),1,fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			//compara se o CPF digitado é o mesmo que esta na estrutura
			if(strcmp(cpf,p_l->CPF)==0){
				flag=1;
				
				//mostra todos os dados referente a determinado locatário
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
			printf("\t\tNão foi possível encontrar o CPF no sistema!");
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
			p_i->status.loc.sigla='A';				//O imovel alugado será renomado para [A]lugado
			p_i->status.loc.reg_loc=p_l->reg_loc;	//Será passado o registro do locatário para o imóvel
			
			
			if((fptr3=fopen("proprietario.bin", "rb"))==NULL)
				printf("\nErro ao abrir o arquivo!\n\n");
			else{

				while(achou!=1 && i<Qreg_p){
					fseek(fptr3, i*sizeof(prop), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
					fread(p_p, sizeof(prop), 1, fptr3);		//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
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
		printf("\tNão encontramos nenhum locatário em nosso sistema!\n\n");
	}
	else
	{
		printf("\t\t\t==IMOBILIÁRIA==\n\n");
		printf("\tDesculpe, mas infelizmente este imóvel já esta sendo alugado!\n");
		printf("\tInformações referente ao aluguel\n\n");
		
		fseek(fptr, pos*sizeof(locatario), 0);		//posiciona o ponteiro no arquivo de acordo com o pos em relação a origem
		fread(p_l, sizeof(locatario), 1, fptr);		//lê a estrutura do arquivo guardando no ponteiro p_l
			
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
	char n_cpf[15];		//cpf auxiliar, será usado para verificar a existência do mesmo
	int op,pos;
	printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
	
	printf(" Informe o CPF: ");	//lê o CPF do proprietário
	gets(n_cpf);
	pos=verifica_cpfProp(p_p, Qreg_p, n_cpf);
	
	//se a função retornar algo diferente de -1, quer dizer o cpf já está cadastrado.
	if(pos == -1){
		printf("\t\t\t==EDITAR DE PROPRIETARIO==\n\n");
		printf("\tOps! O CPF digitado nao está cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a próxima ação
		system("CLS");	//limpa tela
		return;
	}
	else{
		do{
			printf("\t\t\t==EDITAR PROPRIETARIO==\n\n");
			printf("\tOque deseja alterar neste proprietario?\n\n\t[1] - Nome\n\t[2] - Logradouro\n\t[3] - Bairro\n\t[4] - CEP");
			printf("\n\t[5] - Cidade\n\t[6] - Fone\n\t[7] - Estado\n\t[8] - Cel\n\t[9] - Email\n\t[0] - Sair\n\n->Sua opção: ");
		
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<0 || op>9){
				printf("\n Desculpe não entendi!\n");
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
			case 3:		//consulta proprietário		
				printf(" Novo Bairro: ");
				gets(p_p->end_prop.bairro);
				break;
			case 4:		//consulta imóvel
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
				
		grava_prop(p_p, pos,"rb+");	//rb+ é para ler e atualizar os dados do arquivo
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
		printf("\tDesculpe, mas não encontramos o numero de registro do imovel informado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		sleep(3);
		system("CLS");
		return;	
	}		
		else{
		do{
			printf("\t\t\t==EDITAR IMOVEL==\n\n");
			printf("\tOque deseja alterar neste imovel?\n\n\t[1] - Logradouro\n\t[2] - Bairro\n\t[3] - CEP");
			printf("\n\t[4] - Cidade\n\t[5] - Area\n\t[6] - Quartos\n\t[7] - Valor\n\t[0] - Sair\n\n->Sua opção: ");
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<1 || op>8){
				printf("\n Desculpe não entendi!\n");
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
	char n_cpf[15];		//cpf auxiliar, será usado para verificar a existência do mesmo
	int op,pos;
	printf("\t\t\t==EDITAR LOCATÁRIO==\n\n");
	
	printf(" Informe o CPF: ");	//lê o CPF do proprietário
	gets(n_cpf);
	pos=verifica_cpfLOC(p_l, Qreg_l, n_cpf);
	
	system("CLS");
	
	//se a função retornar algo diferente de -1, quer dizer o cpf já está cadastrado.
	if(pos == -1){
		printf("\t\t\t==EDITAR DE LOCATÁRIO==\n\n");
		printf("\tOps! O CPF digitado nao está cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a próxima ação
		system("CLS");	//limpa tela
		return;
	}
	else{
		do{
			printf("\t\t\t==EDITAR LOCATÁRIO==\n\n");
			printf("\t Oque deseja alterar neste proprietario?\n\n\t[1] - Nome\n\t[2] - Logradouro\n\t[3] - Bairro\n\t[4] - CEP");
			printf("\n\t[5] - Cidade\n\t[6] - Fone\n\t[7] - Estado\n\t[8] - Cel\n\t[9] - Email\n\t[0] - Sair\n\n->Sua opção: ");
		
			scanf("%i", &op);
			fflush(stdin);
			
			if(op<0 || op>9){
				printf("\n Desculpe não entendi!\n");
				sleep(1);
				system("CLS");
			}
		}while(op<0 || op>9);
		system("cls");
		printf("\t\t\t==EDITAR LOCATÁRIO==\n\n");
		switch (op){
			case 1:
				printf(" Novo Nome: ");
				gets(p_l->nome);
				break;
			case 2:	
				printf(" Novo Logradouro: ");
				gets(p_l->end_loc.logradouro);
				break;	
			case 3:		//consulta proprietário		
				printf(" Novo Bairro: ");
				gets(p_l->end_loc.bairro);
				break;
			case 4:		//consulta imóvel
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
				
		grava_loc(p_l, pos,"rb+");	//rb+ é para ler e atualizar os dados do arquivo
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
	
	pos_l=verifica_cpfLOC(p_l, Qreg_l, n_cpf);	//irá ir para função de verificar o CPF e retorna a posição encontrada
	
	//caso não encontrar nenhum cpf
	if(pos_l == -1){
		printf("\n\tOps! O CPF digitado nao está cadastrado em nosso sistema!\n\t\tIrei te redirecionar para a página anterior.\n\n");
		
		sleep(3);		//tempo de espera para executar a próxima ação
		system("CLS");	//limpa tela
		return;
	}
	else{
		mostra_loc(p_l, pos_l+1);	//+1 por conta do for
		pag=verifica_data(p_l);		//recebe da verifica_data se a data está anterior ao término do contrato ou não	
		r_i = p_l->reg_imov;
		pos_i = verifica_regImov(p_i,Qreg_i,r_i);	//vai buscar a posição do imóvel
		
		//se estiver cancelando antes do término
		if(pag==1){
			system("CLS");
			printf("\t\t\t==ENCERRAR CONTRATO==\n\n");
			printf("\n\tDevido ao cancelamento ocorrer antes da data de término de contrato.\n\tDeverá ser pago uma multa: R$");
			printf("%.2f\n\n", p_i->valor);
		}
		
		//se estiver cancelando depois do término
		else{
			system("CLS");
			printf("\t\t\t==ENCERRA CONTRATO==\n\n");
			printf("\n\tEstá livre de multa!\n\n");
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
			fread(p_p, sizeof(prop), 1, fptr);	//lê do ponteiro da estrutura proprietário, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
			for(j=0;j<p_p->qtde_casa;j++)
			{	
				//verifica se o num da casa é igual ao reg do imovel
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
		
		//copia o @ para o CPF do locatário
		strcpy(p_l->CPF,"@");
		
		//grava os arquivos
		grava_loc(p_l,pos_l,"rb+");
		grava_prop(p_p,i-1,"rb+");
		grava_imov(p_i,pos_i,"rb+");				
	}else{
		printf("\nREGISTRO NAO ENCONTRADO");		//caso não for encontrado o cpf
	}
	
	
}//deleta_loc

void gerar_rel(locatario *p_l, int Qreg_l, prop *p_p, int Qreg_p,imovel *p_i, int Qreg_i)
{
	
//	entrar com o dia e mostrar nº de registro do imóvel com vencimento para o dia
//	escolhido, bem como, valor, registro, nome e CPF do locatário, registro, nome e CPF
//	do proprietário.
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
	  	printf("\nNenhum locatário cadastrado! Cadastre um novo!\n");
	  	Sleep(500);
 		system("CLS");
	  	achou=0;
	}
	else{
		for(i=0;i<Qreg_l;i++){
			fseek(fptr_l, i*sizeof(locatario), 0);		//posiciona  o ponteiro no inicio de cadas estrutura que existir a partir da origem do arquivo
			fread(p_l, sizeof(locatario), 1, fptr_l);		//lê do ponteiro da estrutura locatario, um arquivo do tamanho da estrutura dentro do arquivo fptr
			
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

