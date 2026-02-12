#include <Arduino.h>
#include <teclado.h>
#include <lcd.h>
#include <adm.h>

// ====================================================== 
// variaveis gloabis 
// ======================================================

extern char* senhas[];

// ======================================================
//funções do menu adm
//=======================================================


bool ler_nova_senha(char* destino){
    memmset(destino, '\0', TAMANHO_SENHA);//limpa o destino antes de ler a nova senha
    int pos=0;

    while(true){
        char tecla = ler_teclado();
        if(tecla =='\0') {continue;} //ignora leituras vazias

        if(tecla=='#') {return false;}

        if(tecla == '*'){
            if(pos == 4){
                continue;//ignora '*' se a senha ja tiver 4 digitos
            }

            if(pos<4){//limita a senha a 4 digitos
                destino[pos] = tecla;//
                lcdSetCursor(pos,1);
                lcdWrite(tecla);
                pos++;
            }
        }
    }
}

void alterar_senha_usuario(){
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Usuario (1-3):");
    lcdSetCursor(0,1);
    lcd_print("#-cancelar");

    char* alvo =nullptr;//ponteiro pra senha alvo

    while(alvo == nullptr){
        char op = ler_teclado();
        if(op=='\0'){continue;}
        if(op=='#'){return;}//cancela

        if(op=='1'){alvo=senha_1;}else if(op=='2'){alvo=senha_2;}else if(op=='3'){alvo = senha_3;}
    }
    
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Nova_senha:");
    lcdSetCursor(0,1);
    lcd_print("*-confrima");


    if(ler_nova_senha(alvo)){
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Salvo!");
    }else{
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Cancelado");
    }
    delay(2000);
    
}

void alterar_senha_adm(){
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Nova Senha ADM:");

    if(ler_nova_senha(senha_ADM)){
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Salvo!");
    }else{
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Cancelado");
    }
    delay(2000);
}

void apagar_senha_usuario(){
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Apagar(1/2/3)");
    lcdSetCursor(0,1);
    lcd_print("#-cancela");

    char* alvo = nullptr;
    char op;

    while (alvo==nullptr){
        op = ler_teclado();
        if(op=='\0'){continue;}
        if(op=='#'){return;}//cancela

        if(op=='1'){alvo=senha_1;}else if(op=='2'){alvo=senha_2;}else if(op=='3'){alvo = senha_3;}
        }

    memset(alvo, '\0', TAMANHO_SENHA);
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Senha apagada");
    lcdSetCursor(0,1);
    char msg[20];
    sprintf(msg,"user:%c", op);
    lcd_print(msg);
    delay(2000);
}

void ajustar_tempo_tranca(){
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Tempo(s):");
    lcdSetCursor(0,1);
    lcd_print("*-ok #-voltar");

    int pos=0;
    int tempo_s=0;

    while(true){
        char tecla = ler_teclado();
        if(tecla == '\0'){continue;}
        if(tecla == '#'){return;}

        if(tecla=='*'){
            if(pos > 0){
                tempo_tranca = (unsigned long)tempo_s *1000;//converte pra ms
                LCDclear();
                lcdSetCursor(0,0);
                lcd_print("Salvo!");
                lcdSetCursor(0,1);
                lcd_print("Tempo:");
                int tempo = tempo_s;
                int digitos = (tempo>=100) ? 3 : (tempo >= 10) ? 2 : 1 ;//se tempo >=100 tem 3 digitos(100,230,546), se tempo >=10 tem 2 digitos(10,25,30), se nao ele tem um digito sṍ(1,4,6)
                for (int i = digitos -1; i>=0;i--){
                    lcdSetCursor(6 + i, i);//T-1,e-2,m-3,p-4,o-5,:-6
                    lcdWrite('0'+(tempo%10));//pega o ultimo digito do tempo e escreve no lcd
                    tempo /=10;//remove o ultimo digito do tempo
                }
                lcdSetCursor(6 + digitos,1);
                lcd_print("s");
                delay(2000);
                return;
            }
            continue;//ignora '*' se nao tiver digitado nada
        }
        if(tecla >= '0' && tecla <= '9' && pos < 3){//limita a entrada a numeros e a 3 digitos (999s = 16min, mais que o suficiente pra um tempo de tranca)
            tempo_s = tempo_s *10 + (tecla - '0');//adiciona o novo digito ao tempo
            lcdSetCursor(pos,1);
            lcdWrite(tecla);
            pos++;
        }

    }
}


void menu_adm(){
    while(ler_teclado() != '\0'); //limpa o buffer de teclas

    while(true){
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("1-senhas  3-voltar");
        lcdSetCursor(0,1);
        lcd_print("2-tranca ");
        char tecla = '\0';//placa de pare
        while(tecla == '\0'){tecla = ler_teclado();} //espera por uma entrada valida
    

        if(tecla == '3') {return;}
        
        switch (tecla){

            case '1':
                //aqui tem que ter um menu pra escolher qual senha mudar, e depois um processo pra digitar a nova senha, e depois atualizar o array de senhas
                LCDclear();
                lcdSetCursor(0,0);
                lcd_print("1-alterar 3-adm");
                lcdSetCursor(0,1);
                lcd_print("2-apagar");
                tecla = '\0';//placa de pare
                while(tecla == '\0'){tecla = ler_teclado();} //espera por uma entrada valida
                //tecla = ler_teclado();

                if(tecla == '1'){
                    alterar_senha_usuario();
                }else if(tecla == '2'){
                    apagar_senha_usuario();
                }else if(tecla == '3'){
                    alterar_senha_adm();
                }else{
                    return;
                }
                break;
            
            case '2':
                //aqui tem que ter um processo pra digitar o novo tempo de tranca,
                ajustar_tempo_tranca();
                break;

            case '3':
                //aqui tem que ter um processo pra sair do menu de adm e voltar pro menu normal
                break;

            default:
                LCDclear();
                lcdSetCursor(0,0);
                lcd_print("Opcao invalida");
                delay(2000);
                break;
        }

    }
}