#include <xinu.h>

#define TEMPO_CICLO_AQUECIMENTO 60  // Tempo padrão de aquecimento em segundos
#define TEMPO_MAXIMO_AQUECIMENTO 600 // Tempo máximo de aquecimento (10 minutos)

// Semáforos e variáveis globais
sid32 semaforoPorta, semaforoLuz, semaforoCiclo;
bool emExecucao = FALSE;
bool portaAberta = FALSE;
int tempoAquecimento;
bool luzInternaLigada = FALSE;
int tempoRestante;

// Função para o relógio cortesia
void relogioCortesia(int tempo) {
    int i;
    for (i = tempo; i > 0; i--) {
        kprintf("Restam %d segundos...\n", i);
        sleepms(1000);  // Aguarda 1 segundo
    }
}

// Função para simular o aquecimento com ciclos de potência
void aquecer(int tempo) {
    int i;
    tempoRestante = tempo;

    // Chama o relógio cortesia para a contagem regressiva
    relogioCortesia(tempo);

    for (i = tempo; i > 0; i--) {
        if (portaAberta) {
            kprintf("\nCiclo cancelado: porta aberta!\n");
            return;
        }

        // Simula o ciclo de aquecimento com potência variável
        if (i % 10 == 0) {
            kprintf("Ajustando potencia...\n");
        }

        kprintf("Tempo restante: %d segundos\n", i);
        sleepms(1000);  // Simula 1 segundo de aquecimento
        tempoRestante--;
    }

    // Após o aquecimento, emite um bip
    kprintf("\nTempo de aquecimento concluido!\n");
    bip();
}

// Função para simular o bip
void bip() {
    kprintf("Bip! Bip! Bip!\n");
}

// Função para cancelar o ciclo de aquecimento (emergência)
void cancelar() {
    kprintf("\nCiclo de aquecimento cancelado.\n");
    portaAberta = TRUE;
}

// Função de controle da porta
void controlePorta() {
    // Simula o controle da porta do micro-ondas
    kprintf("Porta fechada.\n");
    portaAberta = FALSE;
}

// Função de controle de luz interna
void controleLuz() {
    // Simula o controle da luz interna durante o aquecimento
    kprintf("Luz interna ligada.\n");
    luzInternaLigada = TRUE;
}

// Função para selecionar o programa de aquecimento
int selecionarPrograma() {
    int escolha;
    kprintf("\nSelecione o programa:\n");
    kprintf("1 - Carnes\n2 - Peixe\n3 - Frango\n4 - Lasanha\n5 - Pipoca\n");
    kprintf("Escolha: ");
    scanf("%d", &escolha);
    return escolha;
}

// Função para programação de ação futura
void programarAquecimentoFuturo(int tempoFuturo) {
    kprintf("\nAquecimento programado para %d segundos no futuro.\n", tempoFuturo);
    sleepms(tempoFuturo * 1000);  // Espera o tempo programado
    kprintf("Iniciando aquecimento programado...\n");
    aquecer(tempoAquecimento);  // Inicia o aquecimento após o tempo programado
}

// Função para simular a respiração ou refrigeração do micro-ondas
void resfriamento() {
    kprintf("\nIniciando ciclo de resfriamento...\n");
    sleepms(5000);  // Simula um tempo de resfriamento (5 segundos)
    kprintf("Resfriamento concluido.\n");
}

// Função para traçar o prato durante o aquecimento
void tracaoPrato() {
    kprintf("\nPrato girando...\n");
    sleepms(1000);  // Simula 1 segundo de rotação do prato
}

// Função principal
int main() {
    int programa;
    int tempoFuturo, escolha, tempoResfriamento;
    
    semaforoPorta = semcreate(1);  // Semáforo para controle da porta
    semaforoLuz = semcreate(1);    // Semáforo para controle da luz
    semaforoCiclo = semcreate(1);  // Semáforo para controle do ciclo de aquecimento

    // Seleciona o programa
    programa = selecionarPrograma();

    // Define o tempo de aquecimento baseado no programa
    switch (programa) {
        case 1: tempoAquecimento = 180; break;  // Carnes
        case 2: tempoAquecimento = 120; break;  // Peixe
        case 3: tempoAquecimento = 150; break;  // Frango
        case 4: tempoAquecimento = 90; break;   // Lasanha
        case 5: tempoAquecimento = 60; break;   // Pipoca
        default:
            kprintf("Programa invalido!\n");
            return 0;
    }

    // Seleciona a programação futura
    kprintf("Deseja programar o aquecimento para um tempo futuro? (1 para sim, 0 para não): ");
    scanf("%d", &escolha);
    if (escolha == 1) {
        kprintf("Informe o tempo futuro em segundos: ");
        scanf("%d", &tempoFuturo);
        programarAquecimentoFuturo(tempoFuturo);
    } else {
        // Controle da porta e luz interna
        controlePorta();
        controleLuz();

        // Inicia o aquecimento
        aquecer(tempoAquecimento);

        // Resfriamento após o aquecimento
        resfriamento();

        // Tração do prato
        tracaoPrato();

        // Finaliza com o bip
        kprintf("Ciclo concluido. Bom apetite!\n");
    }

    return 0;
}
