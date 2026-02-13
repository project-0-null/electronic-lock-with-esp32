#include <Arduino.h>
#include <teclado.h>
#include <lcd.h>
#include <adm.h>
#include <pwm.h>

// ======================================================
//funções do menu adm
//=======================================================

bool senha_ja_existe(char* nova, char* ignorar){
    // percorre todas as senhas do array
    for(int i = 0; i < NUM_USUARIOS + 1; i++){
        // ignora a própria senha do usuário que está sendo alterado, pra não barrar quem quer manter a mesma senha
        if(senhas[i] == ignorar){continue;}
        
        if(strcmp(nova, senhas[i]) == 0){return true;} //igual
    }
    return false; //nenhum igual
}


bool ler_nova_senha(char* destino){
    memset(destino, '\0', TAMANHO_SENHA);//limpa o destino antes de ler a nova senha
    int pos=0;

    while(true){
        char tecla = ler_teclado();
        if(tecla =='\0') {continue;} //ignora leituras vazias

        if(tecla=='#') {return false;}//cancela

        if(tecla == '*'){
            if(pos == 4){return true;} //confirma a senha se tiver 4 digitos
            else{continue;}//ignora '*' se a senha ainda nao tiver 4 digitos
        }
        if(pos<4 && tecla >= '0' && tecla <= '9'){//limita a senha a 4 digitos

            destino[pos] = tecla;//
            lcdSetCursor(pos+11,0);
            lcdWrite(tecla);
            pos++;
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
    lcd_print("Nova senha:");
    lcdSetCursor(0,1);
    lcd_print("*-confrima");


    char temp[TAMANHO_SENHA];
    if(ler_nova_senha(temp)){
        if(senha_ja_existe(temp,alvo)){
            LCDclear();
            lcdSetCursor(0,0);
            lcd_print("Senha ja existe");
            delay(1000);
        }else{
            memcpy(alvo,temp,TAMANHO_SENHA);//copia a nova senha pro destino
            LCDclear();
            lcdSetCursor(0,0);
            lcd_print("salvo");
            delay(1000);
        }
    }else{
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Cancelado");
        delay(1000);
    }
    
}

void alterar_senha_adm(){
    LCDclear();
    lcdSetCursor(0,0);
    lcd_print("Nova Senha:");

    char temp[TAMANHO_SENHA];
    if(ler_nova_senha(temp)){

        if(senha_ja_existe(temp, senha_ADM)){
            LCDclear();
            lcdSetCursor(0,0);
            lcd_print("Senha ja existe!");
            delay(2000);
        } else {
            memcpy(senha_ADM, temp, TAMANHO_SENHA);
            LCDclear();
            lcdSetCursor(0,0);
            lcd_print("Salvo!");
            delay(2000);
        }

    } else {
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("Cancelado");
        delay(2000);
    }
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
                    lcdSetCursor(6 + i, 1);//T-1,e-2,m-3,p-4,o-5,:-6
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
            lcdSetCursor(pos+9,0);
            lcdWrite(tecla);
            pos++;
        }

    }
}


void menu_adm(){
    while(ler_teclado() != '\0'); //limpa o buffer de teclas

    while(true){
        atualizaBrilhoLCD();
        LCDclear();
        lcdSetCursor(0,0);
        lcd_print("1-senha 3-voltar");
        lcdSetCursor(0,1);
        lcd_print("2-tranca 4-abrir ");
        char tecla = '\0';//placa de pare
        while(tecla == '\0'){tecla = ler_teclado();atualizaBrilhoLCD();} //espera por uma entrada valida
        
        switch (tecla){

            case '1':
                //aqui tem que ter um menu pra escolher qual senha mudar, e depois um processo pra digitar a nova senha, e depois atualizar o array de senhas
                LCDclear();
                lcdSetCursor(0,0);
                lcd_print("1-alterar 3-adm");
                lcdSetCursor(0,1);
                lcd_print("2-apagar 4-volta");
                tecla = '\0';//placa de pare
                while(tecla == '\0'){tecla = ler_teclado();atualizaBrilhoLCD();} //espera por uma entrada valida
                //tecla = ler_teclado();

                if(tecla == '1'){
                    alterar_senha_usuario();
                }else if(tecla == '2'){
                    apagar_senha_usuario();
                }else if(tecla == '3'){
                    alterar_senha_adm();
                }else if (tecla == '4'){
                    continue;
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
                LCDclear();
                return;
                
            case '4':
                //aqui tem que ter um processo pra abrir a tranca no menu de adm
                digitalWrite(tranca, HIGH);
                LCDclear();
                lcdSetCursor(0,0);
                lcd_print("Bem-vindo");
                lcdSetCursor(0,1);
                lcd_print("administrador");
                delay(tempo_tranca);
                digitalWrite(tranca, LOW);
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