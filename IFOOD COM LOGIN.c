#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_PEDIDOS 10
#define MAX_USUARIO 50

typedef struct {
    char login[MAX_USUARIO];
    char senha[20];
    char endereco[100];
} Usuario;

Usuario usuarioCadastrado;

typedef struct {
    char nome[50];
    float preco;
} Item;

typedef struct {
    char nome[50];
    Item cardapio[3];
} Restaurante;

typedef struct {
    char usuario[MAX_USUARIO];
    char restaurante[50];
    char item[50];
    int tempoEntrega;
} Pedido;

Restaurante restaurantes[3] = {
    {"Pizza Prime", {{"Pizza Margherita", 25.0}, {"Pizza Calabresa", 28.0}, {"Refrigerante", 6.0}}},
    {"Burguer Top", {{"X-Burguer", 20.0}, {"Batata Frita", 12.0}, {"Suco", 7.0}}},
    {"Sorvetes Ice", {{"Flocos", 10.0}, {"Napolitano", 15.0}, {"Chocolate", 8.00}}}
};

Pedido historico[MAX_PEDIDOS];
int totalPedidos = 0;

int login(char *usuario) {
    char loginDigitado[MAX_USUARIO];
    char senhaDigitada[20];

    printf("==== LOGIN ====\n");
    printf("Login: ");
    scanf(" %[^\n]", loginDigitado);

    printf("Senha: ");
    scanf(" %[^\n]", senhaDigitada);

    if (strcmp(loginDigitado, usuarioCadastrado.login) == 0 &&
        strcmp(senhaDigitada, usuarioCadastrado.senha) == 0) {
        
        strcpy(usuario, usuarioCadastrado.login); // Salva o nome do usuário para usar depois
        printf("Login bem-sucedido! Bem-vindo(a), %s!\n\n", usuario);
        sleep(2);
        system("clear");
        return 1;
    } else {
        printf("Login ou senha incorretos.\n");
        return 0;
    }
}

void cadastroUsuario() {
    printf("==== CADASTRO ====\n");
    printf("Digite seu nome de usuário: ");
    scanf(" %[^\n]", usuarioCadastrado.login);

    printf("Digite sua senha: ");
    scanf(" %[^\n]", usuarioCadastrado.senha);

    printf("Digite seu endereço: ");
    scanf(" %[^\n]", usuarioCadastrado.endereco);

    printf("\nCadastro realizado com sucesso!\n\n");
    sleep(2);
    system("clear");
}

 
void mostrarRestaurantes() {
    system ("clear");
    printf("==== RESTAURANTES ====\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, restaurantes[i].nome);
        
    }
}

void mostrarCardapio(int indexRestaurante) {
    system ("clear");
    printf("==== CARDÁPIO - %s ====\n", restaurantes[indexRestaurante].nome);
    for (int i = 0; i < 3; i++) {
        printf("%d. %s - R$ %.2f\n", i + 1, restaurantes[indexRestaurante].cardapio[i].nome, restaurantes[indexRestaurante].cardapio[i].preco);
        
    }
}

void mostrarCupons() {
    system ("clear");
    printf("\n==== MEUS CUPONS ====\n");
    printf("1. DESCONTO10 - Aplica 10%% de desconto no valor total do pedido.\n");
    printf("2. PROMO5 - Aplica 5%% de desconto no valor total do pedido.\n");
    printf("Use o código ao final do pedido para receber o desconto.\n\n");
    
}

int tempoEntregaAleatorio() {
    return (rand() % 45) + 15; 
}

void fazerPedido(char *usuario) {
    int escolhaRest, escolhaItem;
    char continuar;
    Pedido pedidosTemp[3];
    int qtdPedidosTemp = 0;
    float totalPedido = 0.0;

    mostrarRestaurantes();
    printf("Escolha o restaurante (1-3): ");
    scanf("%d", &escolhaRest);

    if (escolhaRest < 1 || escolhaRest > 3) {
        printf("Restaurante inválido.\n");
        return;
    }

    int tempoEntregaUnico = tempoEntregaAleatorio();

    do {
        mostrarCardapio(escolhaRest - 1);
        printf("Escolha o item do cardápio (1-3): ");
        scanf("%d", &escolhaItem);

        if (escolhaItem < 1 || escolhaItem > 3) {
            printf("Item inválido.\n");
            return;
        }

        Pedido novo;
        strcpy(novo.usuario, usuario);
        strcpy(novo.restaurante, restaurantes[escolhaRest - 1].nome);
        strcpy(novo.item, restaurantes[escolhaRest - 1].cardapio[escolhaItem - 1].nome);
        novo.tempoEntrega = tempoEntregaUnico;

        pedidosTemp[qtdPedidosTemp++] = novo;
        totalPedido += restaurantes[escolhaRest - 1].cardapio[escolhaItem - 1].preco;

        if (totalPedidos + qtdPedidosTemp > MAX_PEDIDOS) {
            printf("Limite máximo de pedidos atingido!\n");
            break;
        }

        printf("Deseja pedir mais algo deste restaurante? (s/n): ");
        scanf(" %c", &continuar);
        system("clear");

    } while ((continuar == 's' || continuar == 'S') && qtdPedidosTemp < 3);

    for (int i = 0; i < qtdPedidosTemp; i++) {
        historico[totalPedidos++] = pedidosTemp[i];
    }

    if (qtdPedidosTemp > 0) {
        printf("Resumo do seu pedido no restaurante %s:\n", restaurantes[escolhaRest - 1].nome);
        for (int i = 0; i < qtdPedidosTemp; i++) {
            printf("- %s\n", pedidosTemp[i].item);
        }

        printf("Tempo estimado de entrega: %d minutos\n", tempoEntregaUnico);
        printf("Total antes do desconto: R$ %.2f\n", totalPedido);

        char cupom[20];
        float desconto = 0.0;
        printf("Digite um cupom de desconto (ou pressione ENTER para pular): ");
        getchar(); // limpar buffer
        fgets(cupom, sizeof(cupom), stdin);
        cupom[strcspn(cupom, "\n")] = '\0'; 

        if (strcmp(cupom, "DESCONTO10") == 0) {
            desconto = 0.10;
            printf("Cupom válido! Desconto de 10%% aplicado.\n");
        } else if (strcmp(cupom, "PROMO5") == 0) {
            desconto = 0.05;
            printf("Cupom válido! Desconto de 5%% aplicado.\n");
        } else if (strlen(cupom) > 0) {
            printf("Cupom inválido. Nenhum desconto aplicado.\n");
        }

        float totalFinal = totalPedido * (1 - desconto);
        printf("Total final: R$ %.2f\n\n", totalFinal);
    } else {
        printf("Nenhum item foi pedido.\n");
    }
}


void mostrarHistorico(char *usuario) {
    printf("\n==== HISTÓRICO DE PEDIDOS - %s ====\n", usuario);
    for (int i = 0; i < totalPedidos; i++) {
        if (strcmp(historico[i].usuario, usuario) == 0) {
            printf("%d. %s - %s (%d min)\n", i + 1, historico[i].restaurante, historico[i].item, historico[i].tempoEntrega);
        }
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    char usuario[MAX_USUARIO];
    int opcao;

    cadastroUsuario(); 

    while (!login(usuario)) {
        printf("Tente novamente.\n\n");
    }

    do {
        printf("==== MENU ====\n");
        printf("1. Fazer pedido\n");
        printf("2. Ver histórico\n");
        printf("3. Meus cupons\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                fazerPedido(usuario);
                break;
            case 2:
                mostrarHistorico(usuario);
                break;
            case 3:
                mostrarCupons();
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 4);

    return 0;
}