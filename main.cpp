#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

using namespace std;

struct Banco{
    char numeroC[16];
    char numeroAgen[16];
    char nomeTitu[100];
    char CPF[13];
    float saldo;
};

struct Extrato{
    char numeroC[16];
    char numeroAgen[16];
    char hist[100];
    float valor;
    char horario[10];
    char dat[10];

};

struct tm * timeinfo;

void nome(char nome[]);
void data(char buffer[]);
void hora(char buffer[]);
void mostrar(int i, Banco B[]);
bool EhCpf(char cpf[]);
void menu ( int &opcao);
void excluir(Banco B[], int pos, int &idx, int idxe, Extrato E[]);



int main()
{
    setlocale(LC_ALL,"");
    ofstream out;
    ifstream inf;

    Banco B[205];
    Extrato E[2000];

    int cont=0; // serve para contar os erros de dados invalidos
    int opcao;
    int idx=0, idxe=0;
    char agen[16] , conta[16];

    ///Abrir o arquivo em modo binario para leitura;
    inf.open("contas.dat", ios::binary);

    if(!inf){
        cout<<"Erro ao abrir o arquivo\n";
    }
    else{

        inf.read((char*)&idx , sizeof(idx));

        inf.read((char *)B , sizeof(B));

        inf.read((char *)&idxe, sizeof(idxe));

        inf.read((char *)E, sizeof(E));

        inf.close();
    }



    do{
        Menu: // Vai para o inicio caso exceder o limite de tentativas
        system("cls");
        menu(opcao);
        switch(opcao)
        {
        case 1: ///Cadastrando----------------------------------------------
            system("cls");
            if(idx==200){
                cout<<"Servidor cheio, estamos a resolver isso.\n";
                system("pause");
                break;
            }
            else{
                cout<<"Cadastrando sua conta\n\n\n";
                cont=0;//zera sempre que estiver antes de um atalho goto
                contaD: //goto, caso conta for existente
                cout<<"Numero da conta: ";
                cin>>B[idx].numeroC;
                for(int i=0;i<idx;i++){
                    if(strcmp(B[idx].numeroC,B[i].numeroC)==0){
                        cont++;
                        if(cont==3){
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout<<"Numero da conta já existente!"
                            <<" Digite novamente.\n\n";
                        goto contaD;

                    }
                }
                cout<<"Numero da agência: ";
                cin>>B[idx].numeroAgen;
                cout<<"Nome do titular: ";
                nome(B[idx].nomeTitu);///------------------------
                printf("\033[31m");
                cout<<"\nAtenção: o CPF deve ser digitado sem pontos, obedecendo o seguinte modelo \"123456789-10\", sem as aspas";
                cout<< " (CPF Fictício).\n";
                cout<< "OBS: e obrigatório o uso do hífen na décima posição pra separar dos dois últimos dígitos.\n\n";
                printf("\033[37m");
                cont=0;
                Ncpf:
                cout<<"CPF: ";
                cin>>B[idx].CPF;
                //Verifica se há CPF iguais
                for(int i=0;i<idx;i++){
                    if(strcmp(B[idx].CPF , B[i].CPF)==0){
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        printf("\033[31m");
                        cout<<"Este CPF já está cadastrado, digite novamente!\n";
                        printf("\033[37m");
                        goto Ncpf;
                    }
                }
                //Verifica se o CPF é válido
                if(EhCpf(B[idx].CPF)==false){//Se for falso dirá que o cpf não é valido e terá que digitar novamente
                    cont++;
                    if(cont==3){//Verifica se tentou três vezes
                        cout << "\nLimites de tentativas excedido! \n";
                        system("Pause");
                        goto Menu; // Vai para o menu
                    }

                    cout << "CPF inválido! Digite novamente... \n";
                    goto Ncpf;
                }

                cont=0;
                saldo:
                cout<<"Seu saldo inicial: ";
                cin>>B[idx].saldo;

                if(B[idx].saldo<=0){
                    cout<<"Valor inválido, digite novamente!\n";
                    cont++;
                    if(cont==3){//Verifica se tentou três vezes
                        cout << "\nLimites de tentativas excedido! \n";
                        system("Pause");
                        goto Menu; // Vai para o menu
                    }
                    goto saldo;
                }
                printf("\033[32m");
                cout<<"CADASTRADO COM SUCESSO!\n";
                printf("\033[37m");

                ///Extrato

                strcpy(E[idxe].numeroAgen,B[idx].numeroAgen);
                strcpy(E[idxe].numeroC,B[idx].numeroC);
                strcpy(E[idxe].hist,"Saldo Inicial");
                E[idxe].valor=B[idx].saldo;
                hora(E[idxe].horario);
                data(E[idxe].dat);
                idxe++;

                ///Extrato

                idx++;
                system("pause");

            }



            break;


        case 2: ///Alterando Dados-------------------------------------------
            bool t;
            t=false; //Inicia falso
            cont=0;
            inicio:
            system("cls");
            cout<<"ALTERAR DADOS\n\n\n";
            cout<<"Numero da conta: ";
            cin>>conta;
            cout<<"Numero da agencia: ";
            cin>>agen;
            for(int i=0;i<idx;i++){
                if(strcmp(conta , B[i].numeroC)==0 && strcmp(agen, B[i].numeroAgen)==0){
                    cout<<"DIGITE OS NOVOS DADOS\n";
                    cout<<"Novo nome do Titular: ";
                    nome(B[i].nomeTitu);
                    cont=0;
                    Ncpf2:
                    cout<<"\nNovo CPF: ";
                    cin>>B[i].CPF;

                    for(int j=0;j<idx;j++){
                        if(strcmp(B[j].CPF , B[i].CPF)==0 && i!=j ){ //i!=j por que pode utilizar o mesmo cpf de novo
                            cont++;
                            if(cont==3){//Verifica se tentou três vezes
                                cout << "\nLimites de tentativas excedido! \n";
                                system("Pause");
                                goto Menu; // Vai para o menu
                            }
                            cout<<"Este CPF já está cadastrado , digite novamente!\n";
                            goto Ncpf;
                        }
                    }

                    if(EhCpf(B[i].CPF)==false){
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout << "CPF invalido! Digite novamente... \n";
                        goto Ncpf2;
                    }
                    printf("\033[32m");
                    cout<<"Dados atualizados com SUCESSO!\n\n";
                    printf("\033[37m");
                    t=true; // Como os dados foram encontrado, se torna verdadeiro
                    break;

                }
            }
            if(t==false){ //Se os dados não foram encontrados
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"OPS... Os dados digitados não existem, "
                    <<"digite os novamente\n\n";
                system("pause");
                goto inicio;
            }

            system("pause");
            break;

        case 3:///Exclusão da Conta------------------------------------------------Testarr!!!!!!!!!!!
            cont=0;
            inic: // Goto
            system("cls");
            bool t2;
            t2=false;
            int pos; /// Posição da conta corrente no vetor
            cout<<"EXCLUSAO DE DADOS\n\n\n";
            cout<<"Informe numero da conta e da agencia para fazer a exclusao dos seus dados\n";
            cout<<"Numero da conta: ";
            cin>>conta;
            cout<<"Numero da agencia: ";
            cin>>agen;
            for(int i=0;i<idx;i++){
                if(strcmp(conta , B[i].numeroC)==0 && strcmp(agen, B[i].numeroAgen)==0){
                    pos=i;//pos, recebe a posiçao
                    t2=true; //Posição encontrada
                    break;
                }
            }

            if(t2){//Se for encontrada a posição, entra nesta condição
                excluir(B, pos, idx, idxe, E); //Procedimento que exclui as contas substituindo
                printf("\033[32m");
                cout<<"Seus dados foram totalmentes excluídos do nosso banco de dados\n\n";
                printf("\033[37m");
            }
            else{
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"OPS... Esses dados digitados não existem,"
                    <<"digite os novamente\n\n";
                system("pause");
                goto inic;
            }

            system("pause");
            break;


        case 4: ///Consultar Saldo-------------------------------------------------
            cont=0;
            case4: // goto, caso dados sejam invalidos
            system("cls");
            bool t3;
            t3=false; // Se for encontrado a conta, tornará true
            cout<<"CONSULTAR SALDO\n\n\n";
            cout<<"Para consultar o seu saldo informe estes dados\n";
            cout<<"Numero da conta: ";
            cin>>conta;
            cout<<"Numero da agencia: ";
            cin>>agen;

            for(int i=0;i<idx;i++){
                if(strcmp(conta , B[i].numeroC)==0 && strcmp(agen , B[i].numeroAgen)==0){
                        cout<<"\n\n\n";
                        mostrar(i, B);
                            t3=true; //Dados encontrados
                            break;
                }
            }

            if(t3==false){ //Dados não encontrados
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"OPS... Os dados digitados não existem,"
                    <<" tente novamente...\n\n";
                system("pause");
                goto case4; //volta ao inicio
            }

            system("pause");
            break;

        case 5:///Fazer Deposito--------------------------------------
            cont=0;
            case5: //goto, caso dados sejam invalidos
            int novoS;
            bool t4;
            t4=false;
            system("cls");
            cout<<"FAZER DEPÓSITO\n\n";
            cout<<"Para fazer um depósito informe os dados\n";
            cout<<"Numero da conta: ";
            cin>>conta;
            cout<<"Numero da agencia: ";
            cin>>agen;
            for(int i=0;i<idx;i++){
                if(strcmp(conta , B[i].numeroC)==0 && strcmp(agen , B[i].numeroAgen)==0){
                    cout<< "\n\n\n";
                    mostrar(i, B);
                    cont=0;
                    novo:// goto, caso valor seja negativo
                    cout<<"Digite o valor do depósito: ";
                    cin>>novoS;
                    if(novoS<=0){
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout<<"Valor inválido, digite novamente.\n";
                        goto novo;
                    }

                    B[i].saldo+=novoS;
                    t4=true;
                    printf("\033[32m");
                    cout<<"Deposito realizado com sucesso! \n\n";
                    printf("\033[37m");
                    break;
                }
            }
            if(t4==false){//conta não encontrada
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"OPS... OS dados digitados não existem,"
                    <<" tente novamente...\n\n";
                system("pause");
                goto case5; //Volta ao inicio
            }

            ///Extrato

                strcpy(E[idxe].numeroAgen,agen);
                strcpy(E[idxe].numeroC,conta);
                strcpy(E[idxe].hist,"Deposito ");
                E[idxe].valor=novoS;
                hora(E[idxe].horario);
                data(E[idxe].dat);
                idxe++;

            ///Extrato

            system("pause");
            break;

        case 6:///Realizar Saque-----------------------------------
            cont=0;
            case6: //goto, caso dados sejam invalidos
            int saque;
            bool t5;
            t5=false;
            system("cls");
            cout<<"REALIZAR SAQUE\n\n";
            cout<<"Para realizar o saque informe esses dados\n";
            cout<<"Numero da conta: ";
            cin>>conta;
            cout<<"Numero da agencia: ";
            cin>>agen;
            for(int i=0;i<idx;i++){
                if(strcmp(conta , B[i].numeroC)==0 && strcmp(agen , B[i].numeroAgen)==0){
                    cout<< "\n\n\n";
                    mostrar(i, B);
                    cont=0;
                    saq:// goto, caso valor seja negativo
                    cout<<"Digite o valor do saque: ";
                    cin>>saque;
                    if(saque <= 0 || saque > B[i].saldo ){
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout<<"\nValor inválido,"
                            <<" digite novamente.\n";
                        goto saq; //volta digitar o saque
                    }

                    B[i].saldo-=saque;
                    t5=true;
                    printf("\033[32m");
                    cout<<"Saque realizado com sucesso!\n\n";
                    printf("\033[37m");
                    break;
                }
            }
            if(t5==false){//conta não encontrada
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"OPS... Os dados digitados não existem,"
                    <<" tente novamente...\n\n";
                system("pause");
                goto case6; //Volta ao inicio
            }

            ///Extrato

                strcpy(E[idxe].numeroAgen,agen);
                strcpy(E[idxe].numeroC,conta);
                strcpy(E[idxe].hist,"Saque ");
                E[idxe].valor=saque*-1;
                hora(E[idxe].horario);
                data(E[idxe].dat);
                idxe++;

            ///Extrato

            system("pause");
            break;

        case 7:///Transferir Valores-----------------------------------------------
            cont = 0;
            case7://goto, caso os dados sejam invalidos
            system("cls");
            bool t6, t7; //Caso sejam encontrados os dados das contas tornam-se true
            t6=false; // Se permanecer false ate o final, os dados são invalidos
            t7=false;
            int p1, p2; // Posição da conta 1, e conta 2, respectivamente, no vetor.
            char conta1[16], agen1[16], conta2[16], agen2[16]; //Var. pra receber os dados das duas contas,
            //num e agencia

            cout<<"REALIZAR TRANSFERENCIA\n";
            cout<<"Para realizar a transferencia informe os dados "
                <<"da conta ORIGEM e DESTINO\n\n";
            cout << "Conta ORIGEM\n";
            cout<<"Numero da conta: ";
            cin>>conta1;
            cout<<"Numero da agencia: ";
            cin>>agen1;
            cout << "\n\n";

            cout << "Conta DESTINO\n";
            cout<<"Numero da conta: ";
            cin>>conta2;
            cout<<"Numero da agencia: ";
            cin>>agen2;

            int en;
            en=0;//Incrementa, e se ficar igual a 2 da break, pois achou as duas contas
            for(int i=0;i<idx;i++){
                if(strcmp(conta1 , B[i].numeroC)==0 && strcmp(agen1 , B[i].numeroAgen)==0){
                    p1=i;
                    t6=true;
                    en++;
                }
                if(strcmp(conta2 , B[i].numeroC)==0 && strcmp(agen2 , B[i].numeroAgen)==0){
                    p2=i;
                    t7=true;
                    en++;
                }
                if(en==2) break; //se encontrar os dois, para
            }

            if(t6 == false || t7 == false){
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout << "\nDados invalidos, tente novamente...\n";
                system("pause");
                goto case7; //Volta ao inicio
            }
            else{
                printf("\033[32m");
                cout << "\nDados encontrados com sucesso!\n";
                printf("\033[37m");
                system("pause");
                system("cls");
                cout<< "\nConta Origem\n";
                mostrar(p1, B);

                cout<< "Conta Destino\n";
                mostrar(p2, B);
            }
            int transf; //Recebera o valor da transferencia
            cont=0;
            trf:// goto, caso transferencia seja invalida
            cout << "Digite o valor da transferencia: ";
            cin >> transf;
            if(transf <= 0 || transf > B[p1].saldo){//condições da transf.
                cont++;
                if(cont==3){//Verifica se tentou três vezes
                    cout << "\nLimites de tentativas excedido! \n";
                    system("Pause");
                    goto Menu; // Vai para o menu
                }
                cout<<"\nValor inválido,"
                    <<" digite novamente.\n";
                goto trf;// Volta a digitar o valor
            }

            B[p1].saldo-=transf; // Decrementa a conta origem
            B[p2].saldo+=transf; // Incrementa a conta destino

            ///Extrato

                strcpy(E[idxe].numeroAgen,agen1);
                strcpy(E[idxe].numeroC,conta1);
                strcpy(E[idxe].hist,"Transferencia Enviada");
                //strcat(E[idxe].hist,conta2);
                //strcat(E[idxe].hist," Agencia ");
                //strcat(E[idxe].hist,agen2);
                E[idxe].valor=transf*-1;
                hora(E[idxe].horario);
                data(E[idxe].dat);
                idxe++;

           //----------------------------------

                strcpy(E[idxe].numeroAgen,agen2);
                strcpy(E[idxe].numeroC,conta2);
                strcpy(E[idxe].hist,"Transferencia Recebida");
                //strcat(E[idxe].hist,conta1);
                //strcat(E[idxe].hist," Agencia ");
                //strcat(E[idxe].hist,agen1);
                E[idxe].valor=transf;
                hora(E[idxe].horario);
                data(E[idxe].dat);
                idxe++;

            ///Extrato

            printf("\033[32m");
            cout << "Transferencia realizada com sucesso!\n";
            printf("\033[37m");

            system("pause");
            break;

        case 8:///Mostrar Contas---------------------------------------
            system("cls");
            if(idx==0){
                cout<<"NAO HA CONTAS CADASTRADAS!\n\n";
            }
            else{
                int ip; //Posição da conta
                for(int i=0;i<idx;i++){
                    ip=i;
                    cout<< "Conta "<< ++ip <<"\n\n";
                    mostrar(i, B);

                }

            }
        system("pause");
        break;

        case 9:/// Emitir o extrato em tela e arquivo
            system("cls");
            int opc; //opção
            int p; //posição da conta
            bool t8; // true se encontrar conta


            cout << "EMITIR EXTRATO DA CONTA\n\n\n";
            cout << "Escolha uma opcao\n\n";
            cout << "1 - Emitir em Tela.\n2 - Emitir em Arquivo.\n\nDigite: ";
            cin >> opc;

            switch(opc){
                case 1: ///Extrato em tela***********************
                    cont = 0;
                    case1: // goto caso conta tiver invalida
                    t8=false;
                    system("cls");
                    cout << "EMITIR EXTRATO EM TELA\n\n";
                    cout << "Digite estes dados\n";
                    cout << "Numero da conta: ";
                    cin >> conta;
                    cout << "Numero da agencia: ";
                    cin >> agen;
                    int ac; // recebe posição da conta no vetor de contas

                    for(int i=0;i<idxe;i++){///Verificando se a conta existe
                        if(strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                            p=i;
                            //t8=true;
                            for(int j=0;j<idx;j++){ //encontrando posição da conta no vetor contas
                                if(strcmp(conta , B[j].numeroC)==0 && strcmp(agen , B[j].numeroAgen)==0){
                                ac=j;
                                t8=true;
                                printf("\033[32m");
                                cout << "\n\nDados encontrados com SUCESSO!\n";
                                printf("\033[37m");
                                system("pause");
                                system("cls");
                                break;

                                }
                            }


                            break;// finaliza for de verificação
                        }
                    }



                    char dt[10], hr[10];
                    data(dt);
                    hora(hr);


                   if(t8==false){///Se não for encontrada entrará nesta
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout << "\n\nDados invalidos! Tente novamente...\n";
                        system("pause");
                        goto case1; //volta ao inicio
                    }
                    else{ /// Se a conta foi encontrada, entrará nesta condição
                        cout << "\t\t\t\t\tEXTRATO EM TELA\n\n";
                        cout << "Nome: " << B[ac].nomeTitu << "\t\t Agência\\Conta: " << B[ac].numeroAgen << "\\" << B[ac].numeroC;
                        cout << "\nData: " << dt << "\t\t\t Hora: " << hr;
                        cout << "\n----------------------------------------------------------------------------------------------\n\n";

                        cout.precision(2); //Precisão de duas casas depois da virgula
                        cout.setf(ios::fixed);
                        for(int i=p;i<idxe;i++){
                            if(strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0 && strcmp(E[i].hist,"Total")!=0){
                                cout << "Horario: "<< E[i].horario;
                                cout << "\nData: " << E[i].dat;
                                cout << "\nHistorico: " << E[i].hist;
                                cout << "\nValor: ";
                                if(E[i].valor>0)
                                    cout << "\033[32m";
                                else
                                    cout << "\033[31m";
                                cout << E[i].valor;
                                cout << "\033[37m";
                                cout << "\n-------------------------------------\n\n";
                            }
                        }

                        cout << "\nSaldo atual:\033[32m R$ " << B[ac].saldo << "\033[37m\n\n";

                    }


                    break;//finaliza case 1 de extrato em tela


                case 2: ///Extrato em aquivo*******************
                    cont=0;
                    case2: //goto se não encontrar conta
                    t8=false;
                    system("cls");
                    cout << "EMITIR EXTRATO EM ARQUIVO\n\n";
                    cout << "Digite estes dados\n";
                    cout << "Numero da conta: ";
                    cin >> conta;
                    cout << "Numero da agencia: ";
                    cin >> agen;
                    int ac2; // recebe posição da conta no vetor de contas

                    for(int i=0;i<idxe;i++){///Verificando se a conta existe
                        if(strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                            p=i;
                            //t8=true;
                            for(int j=0;j<idx;j++){ //encontrando posição da conta no vetor contas
                                if(strcmp(conta , B[j].numeroC)==0 && strcmp(agen , B[j].numeroAgen)==0){
                                    ac2=j;
                                    t8=true;
                                    printf("\033[32m");
                                    cout << "\n\n Dados encontrados com SUCESSO!\n";
                                    printf("\033[37m");
                                    system("pause");
                                    system("cls");
                                    break;

                                }
                            }

                            break;// finaliza for de verificação
                        }
                    }

                    if(t8==false){///Se não for encontrada entrará nesta
                        cont++;
                        if(cont==3){//Verifica se tentou três vezes
                            cout << "\nLimites de tentativas excedido! \n";
                            system("Pause");
                            goto Menu; // Vai para o menu
                        }
                        cout << "\n\nDados inválidos! Tente novamente...\n";
                        system("pause");
                        goto case2; //volta ao inicio
                    }

                        ///Extrato
                    strcpy(E[idxe].numeroAgen,agen);
                    strcpy(E[idxe].numeroC,conta);
                    strcpy(E[idxe].hist,"Total");
                    E[idxe].valor=B[ac2].saldo;
                    hora(E[idxe].horario);
                    data(E[idxe].dat);
                    idxe++;
                        ///Extrato

                    system("cls");
                    out.open ("HardDiskBank.html");
                    if(! out)
                        cout<<"ERRO ao abrir/criar arquivo\n";
                    else{
                        char DT[10],HR[10];
                        hora(HR);
                        data(DT);
                        out<<"<html></br></br></br><font face='arial'><style>"
                           <<".tabelão {border-style: solid;border-width:1px;width:46%;align:left;background:white;}.tab{background:#bababa;}.tab1{background: #efefef;}.borda{text-shadow: 2px 2px 2px black;}"
                           <<".columns {position: relative;padding-left:0.625em;padding-right: 0.625em;float: left;}</style><head><title>AraBanco - Extrato</title></head><body background='ak.png'><center><font class='borda' size='20'color='yellow'><b> HardDiskBank </b></font></center></br><center><div class='tabelão'>"
                           <<"<body class='r' ><h1><center> Extrato Bancário</h1><center> <b> Nome: </b>" << B[ac2].nomeTitu << "<b> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; Agência/Conta: </b>" <<B[ac2].numeroAgen<<'/'<< B[ac2].numeroC << "</center>"
                           <<"<center> <b> Data: </b> " << DT  <<" <b> &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; Horário: </b> "<< HR <<" </center><br/>"
                           <<"<center> <b> Histórico </b><br/><br/><center> <TABLE BORDER RULES=rows><col width='100'><col width='120'><col width='180'><col width='80'><col width='120'>"
                           <<"<th class='Tab'><b><center> Data </b></th><th th class='Tab'><b><center> Horário </b></th><th class='Tab' align='left'>"
                           <<"<b> Lançamento </b></th><th class='tab' align='right'><b> Valor(R$) <b></th><th class='tab' align='right'><b> Saldo(R$) <b></th>";
                        if(t8){
                            int y=0;
                            for(int i=p;i<idxe;i++){
                                if( strcmp(E[i].hist,"Saldo Inicial")== 0 || strcmp(E[i].hist,"Total")==0){
                                    if(y%2==1 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr class='tab1'><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td><b>"<< E[i].hist /* Historico */ <<"</b></td><td align='right'>"<< "" /* Valor */ << "</td><td align='right'><b>"<< E[i].valor << "</b></td></tr>";
                                    }
                                    else
                                    if(y%2==0 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td><b>"<< E[i].hist /* Historico */ <<"</b></td><td align='right'>"<< "" /* Valor */ << "</td><td align='right'><b>"<< E[i].valor <<"</b></td></tr>";
                                    }
                                }
                                else
                                if(E[i].valor > 0){
                                    if(y%2==1 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr class='tab1'><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td>"<< E[i].hist /* Historico */ <<"</td><td align='right'><font color='green'><b>"<< E[i].valor /* Valor */ << "</b></font></td><td align='right'><b>"<< "" << "</b></td></tr>";
                                    }
                                    else
                                    if(y%2==0 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td>"<< E[i].hist /* Historico */ <<"</td><td align='right'><font color='green'><b>"<< E[i].valor /* Valor */ << "</b></font></td><td align='right'><b>"<< "" <<"</b></td></tr>";
                                    }
                                }
                                else
                                if(E[i].valor < 0){
                                    if(y%2==1 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr class='tab1'><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td>"<< E[i].hist /* Historico */ <<"</td><td align='right'><font color='red'><b>"<< E[i].valor /* Valor */ << "</b></font></td><td align='right'><b>"<< "" << "</b></td></tr>";
                                    }
                                    else
                                    if(y%2==0 && strcmp(conta , E[i].numeroC)==0 && strcmp(agen , E[i].numeroAgen)==0){
                                        y++;
                                        out<<"<tr><td><center>"<<  E[i].dat /* Data */ <<"</center></td><td><center>"<< E[i].horario /* Hora */ <<"</center></td><td>"<< E[i].hist /* Historico */ <<"</td><td align='right'><font color='red'><b>"<< E[i].valor /* Valor */ << "</b></font></td><td align='right'><b>"<< "" <<"</b></td></tr>";
                                    }
                                }
                            }
                            out << "</table><br /></div></body></br><div align=center><button type=button value=Print this page onclick=window.print() id=btn>Imprimir</button>&nbsp;<button type=button value=Close this page onclick=window.close() id=btn>Voltar</button></div></font> <br /></html>";



                        }
                        out.close();

                    }

                    system("HardDiskBank.html");
                    break;


                default:
                    system("cls");
                    printf("\033[31m");
                    cout << "Opção inválida!\n";
                    printf("\033[37m");
                    break;

            }

                system("Pause");
                break;//finaliza case 9



        case 10:///Finalizando e Salvando ------------------------------------------
            ///tentar abrir o arquivo em modo binario;
            out.open("contas.dat", ios::binary);

            ///verificar se o arquivo foi nao aberto
            if(! out)
                cout<<"ERRO ao abrir/criar arquivo\n";
            else{
                ///gravar a variavel idx;
                out.write((char *)&idx  , sizeof(idx));

                ///Gravar a var B;
                out.write((char *)B , sizeof(B));

                out.write((char *)&idxe, sizeof(idxe));

                out.write((char *)E, sizeof(E));

                printf("\033[32m");
                cout<<"\nDados salvos!\n";
                printf("\033[37m");
                out.close();
                }

                cout << "Até a próxima.\n\n";
                system("pause");
                break;

                default:
                system("cls");
                printf("\033[31m");
                cout << "Opção inválida!\n";
                printf("\033[37m");
                system("pause");
                break;
        }




    }while(opcao!=10);

    return 0;

}///Fim da int main


///Funçoes e Procedimentos
void menu ( int &opcao)//Menu de opçoes
{
    cout<<"\n********************************************\n"
        <<"*"          <<"\t\tHardDiskBank\t" <<"\t   *\t"
        <<"\n********************************************\n";
    cout<<"Menu de opções\n\n\n";
    cout<<" 1 - Cadastrar conta corrente\n";
    cout<<" 2 - Alterar dados de uma conta corrente\n";
    cout<<" 3 - Excluir dados de uma conta\n";
    cout<<" 4 - Consultar o seu saldo\n";
    cout<<" 5 - Fazer depósito\n";
    cout<<" 6 - Realizar saque\n";
    cout<<" 7 - Transferência de valores\n";
    cout<<" 8 - Mostrar todas as contas e saldos\n";
    cout<<" 9 - Emitir extrato da conta\n";
    cout<<" 10 - Sair\n";
    cout<<"ESCOLHA UMA OPÇÃO: ";
    cin>>opcao;
}

void mostrar(int i, Banco B[]){
    cout<<"Numero da Conta: "  <<B[i].numeroC<<"\n\n"
        <<"Numero da Agencia: "<<B[i].numeroAgen<<"\n\n"
        <<"Nome do Titular: "  <<B[i].nomeTitu<<"\n\n"
        <<"CPF: "  <<B[i].CPF  <<"\n\n"
        <<"Saldo: "<<B[i].saldo<<"\n\n"
        <<"-------------------------------------------\n";

}




bool EhCpf(char cpf[]){//Verificar se o cpf é válido
    int s1=2, s2=3; //Numeros a multiplicar cada posição do vetor cpf
    int soma1=0, soma2=0; //armazena as somas
    int x1, x2;//numeros encontrados, comparando com os 2 ultimos digitos
    //do cpf

    for(int i=8;i>=0;i--,s1++,s2++){
        soma1+=s1*(cpf[i]-'0'); // converte multiplica e incrementa
        soma2+=s2*(cpf[i]-'0'); // converte multiplica e incrementa
    }
    if(soma1%11>=2) //descobre o penultimo digito do cpf
        x1=11-(soma1%11);
    else
        x1=0;

    soma2+=x1*2;// depois de multiplica 2 pela penultima posic. e incrementa

    if(soma2%11>=2) //descobre o ultimo digito
        x2=11-(soma2%11);
    else
        x2=0;

    int n1, n2;

    n1=cpf[10]-'0'; // converte pra int
    n2=cpf[11]-'0'; // converte pra int

    if(n1==x1 && n2==x2 && cpf[9]=='-') //condição pra ser valido
        return true;
    else
        return false;


}/// Fizaliza


void excluir(Banco B[], int pos, int &idx, int idxe, Extrato E[]){ //Procedimento pra excluir contas
    for(int i=0;i<idxe;i++){
        if(strcmp(B[pos].numeroC, E[i].numeroC)==0){
            strcpy(E[i].numeroC,"-1");
        }
    }
    for(int i=pos;i<idx-1;i++){
        B[i]=B[i+1];
    }


    idx--;
}

void data(char buffer[]){
    time_t rawtime;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buffer,80,"%d/%m/%y",timeinfo);
}
void hora(char buffer[]){
    time_t rawtime;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buffer,80,"%H:%M %p",timeinfo);
}

void nome(char nome[]){
    bool t=false;
    bool b=false;
    int c;
    int i;
    i=0;
    do {
    c=getch();
    if((c>=65 && c<=90) ||(c>=97 && c<=122 ) || c==32 || c==8 && i>0){
        cout << (char)c;
        nome[i]=c;//recebe caractere com base na tabela ASCII
        i++;
        if(c==8){
            i=i-2; //momento em que apaga, (pra não superlotar o vetor)
            printf(" \b"); //apagar carac. da tela
        }


        if(c==32){ // Não deixar o vetor receber space enquanto não houver caracteres antes
            for(int j=0;j<i;j++){
                if(nome[j]!=32){ //existe caracteres diferentes de space
                    b=true;
                }
            }
            if(b==false){ //não existe, volta pra posição anterior
                i--;
            }
        }

    }
    if(c==13 && i>0){//pra não dar enter sem digitar nada
        for(int j=0;j<i;j++){
            if((int)nome[j]!=32){
                t=true;
            }
        }
    }

    } while (t != true);
    nome[i]='\0';

}


