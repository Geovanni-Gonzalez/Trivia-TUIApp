#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "structures.h"
#include "models.h"
#include "files.h"
#include "utils.h"
#include <fstream>
#include <stdio.h>

class Game {
private:
    LinkedList<Category> categories;
    LinkedList<Player> players;
    int totalRounds;
    int currentRound;

public:
    Game() : totalRounds(0), currentRound(0) {}

    void start() {
        init_random();
        
        clear_screen();
        print_logo();
        set_color(INFO);
        std::cout << "\n  [+] Inicializando sistemas de trivia..." << std::endl;
        loading_animation();
        
        categories = FileLoader::loadCategories(".");
        
        if (categories.count() == 0) {
            set_color(FAILURE);
            std::cout << "  [!] Error critico: Faltan archivos de datos (*.txt)." << std::endl;
            set_color(NEUTRAL);
            return;
        }

        setupPlayers();

        std::cout << "\n  [?] Cuantas rondas de gloria desean jugar? ";
        totalRounds = get_int(1, 20);

        int totalQs = 0;
        for (int i=0; i<categories.count(); i++) {
             totalQs += categories.get(i)->availableQuestions();
        }
        
        if (totalQs < (totalRounds * players.count())) {
            set_color(FAILURE);
            std::cout << "  [!] Error: Municion insuficiente. (Preguntas: " << totalQs << ")" << std::endl;
            set_color(NEUTRAL);
            pause_screen();
            return;
        }

        currentRound = 1;
        while (currentRound <= totalRounds) {
            playRound();
            currentRound++;
        }

        showResults();
        saveHistory();
    }

private:
    void saveHistory() {
        std::ofstream log("history.txt", std::ios::app);
        if (log.is_open()) {
            log << "--- PARTIDA FINALIZADA ---" << std::endl;
            Node<Player>* curr = players.getHead();
            while (curr) {
                log << curr->data.name << " Score: " << curr->data.score << std::endl;
                curr = curr->next;
            }
            log << "--------------------------" << std::endl;
            log.close();
        }
    }

    void setupPlayers() {
        int numPlayers = 0;
        set_color(HIGHLIGHT);
        std::cout << "\n  [?] Numero de competidores (1-6): ";
        numPlayers = get_int(1, 6);

        std::cin.ignore(1000, '\n');
        for (int i = 0; i < numPlayers; i++) {
            char buffer[100];
            std::cout << "  > Identidad del Jugador " << (i+1) << ": ";
            std::cin.getline(buffer, 100);
            Player p(buffer);
            // Load persistent stats
            PlayerStats stats = ProfileLoader::loadStat(buffer);
            std::cout << "    [Profile] Juegos: " << stats.totalGames 
                      << " | Puntos: " << stats.totalPoints << std::endl;
            players.add(p);
        }
        set_color(NEUTRAL);
    }

    void showResults() {
        clear_screen();
        print_header("RESUMEN FINAL DE LA PARTIDA");
        showScores();
        
        Node<Player>* curr = players.getHead();
        while (curr) {
             PlayerStats stats = ProfileLoader::loadStat(curr->data.name.c_str());
             stats.totalGames++;
             stats.totalPoints += curr->data.score;
             // We'll update stats properly during turns for correct answers
             ProfileLoader::saveStat(curr->data.name.c_str(), stats);
             curr = curr->next;
        }

        int maxScore = -1;
        curr = players.getHead();
        while (curr) {
            if (curr->data.score > maxScore) maxScore = curr->data.score;
            curr = curr->next;
        }

        LinkedList<Player*> winners;
        curr = players.getHead();
        while (curr) {
            if (curr->data.score == maxScore) {
                winners.add(&(curr->data));
            }
            curr = curr->next;
        }

        set_color(SUCCESS);
        std::cout << "\n ************************************************************" << std::endl;
        if (winners.count() > 1) {
            std::cout << "  HAY UN EMPATE! Iniciando ronda de desempate..." << std::endl;
            std::cout << " ************************************************************" << std::endl;
            set_color(NEUTRAL);
            pause_screen();
            handleTieBreaker(maxScore);
        } else {
            std::cout << "  EL GRAN GANADOR ES: " << (*winners.get(0))->name << "!" << std::endl;
            std::cout << " ************************************************************" << std::endl;
        }
        set_color(NEUTRAL);
        pause_screen();
    }

    void playRound() {
        char roundTitle[50] = "FASE DE JUEGO - RONDA ";
        char num[10];
        sprintf(num, "%d", currentRound);
        my_strcat(roundTitle, num);

        Node<Player>* currP = players.getHead();
        while (currP != NULL) {
            Player* p = &(currP->data);
            if (p->active) {
                takeTurn(p, roundTitle);
            }
            currP = currP->next;
        }
        
        showScores();
        pause_screen();
    }

    void takeTurn(Player* p, const char* roundTitle) {
        // Choose Category with Menu
        int catCount = categories.count();
        const char** catOptions = new const char*[catCount];
        Node<Category>* currCat = categories.getHead();
        for(int i=0; i<catCount; i++) {
            catOptions[i] = currCat->data.name.c_str();
            currCat = currCat->next;
        }

        MyString menuTitle = MyString("TURNO DE: ") + p->name + MyString(" | ") + MyString(roundTitle);
        int catChoice = menu_selector(catOptions, catCount, menuTitle.c_str());
        delete[] catOptions;

        Category* selectedCat = categories.get(catChoice);
        if (selectedCat->availableQuestions() == 0) {
            set_color(FAILURE);
            std::cout << "  [!] ERROR: Categoria agotada. Pierde turno." << std::endl;
            set_color(NEUTRAL);
            pause_screen();
            return;
        }

        // --- LIFELINE MENU ---
        bool useDouble = false;
        bool useFifty = false;

        const char* lifeOptions[] = { "Responder normalmente", "Usar 50/50 (1 restante)", "Usar Doble o Nada (1 restante)", "Pasar Turno" };
        if (p->lifelines.splitUsed) lifeOptions[1] = "[USADO] 50/50";
        if (p->lifelines.doubleUsed) lifeOptions[2] = "[USADO] Doble o Nada";

        int lifeChoice = menu_selector(lifeOptions, 4, "GESTION DE COMODINES");
        if (lifeChoice == 1 && !p->lifelines.splitUsed) { useFifty = true; p->lifelines.splitUsed = true; }
        if (lifeChoice == 2 && !p->lifelines.doubleUsed) { useDouble = true; p->lifelines.doubleUsed = true; }
        if (lifeChoice == 3) return;

        // Get Question
        LinkedList<Question*> available;
        Node<Question>* qNode = selectedCat->questions.getHead();
        while (qNode) {
            if (!qNode->data.answered) available.add(&(qNode->data));
            qNode = qNode->next;
        }
        Question* q = *available.get(random_int(0, available.count() - 1));

        // Display Question with Timer
        clear_screen();
        print_header("TRANSMISION DE PREGUNTA");
        set_color(HIGHLIGHT);
        std::cout << "  ┌────────────────────────────────────────────────────────" << std::endl;
        std::cout << "  │ " << q->text << std::endl;
        std::cout << "  └────────────────────────────────────────────────────────" << std::endl;
        set_color(NEUTRAL);

        int optCount = q->options.count();
        bool* hidden = new bool[optCount];
        for(int i=0; i<optCount; i++) hidden[i] = false;

        if (useFifty) {
            int hiddenCount = 0;
            for(int i=0; i<optCount && hiddenCount < 2; i++) {
                if ((i+1) != q->correctIndex) {
                    hidden[i] = true;
                    hiddenCount++;
                }
            }
        }

        // Display options
        for(int i=0; i<optCount; i++) {
            std::cout << "      " << (i+1) << ". ";
            if (hidden[i]) std::cout << "[ BLOQUEADO ]" << std::endl;
            else std::cout << q->options.get(i)->text << std::endl;
        }

        // Timer Start
        DWORD startTime = GetTickCount();
        int timeLimit = 15;
        
        std::cout << "\n  TIEMPO LIMITE: " << timeLimit << " SEGUNDOS!" << std::endl;
        set_color(INFO);
        
        // Custom input loop for timer
        int answer = -1;
        while (true) {
            if (check_timer(startTime, timeLimit)) {
                answer = -2; // Timeout
                break;
            }
            if (_kbhit()) {
                answer = get_int(1, optCount);
                // If 50/50 was used, ensure the selected option is not hidden
                if (useFifty && hidden[answer-1]) {
                    set_color(FAILURE);
                    std::cout << "  [!] Opcion bloqueada. Intente de nuevo." << std::endl;
                    set_color(INFO);
                    // Reset input buffer and continue loop
                    while (_kbhit()) _getch(); 
                    continue;
                }
                break;
            }
            Sleep(100);
        }
        delete[] hidden;

        if (answer == -2) {
            set_color(FAILURE);
            std::cout << "\n  >>> TIEMPO AGOTADO! <<<\n" << std::endl;
            play_sound(false);
        } else if (answer == q->correctIndex) {
            int pts = currentRound * (useDouble ? 2 : 1);
            set_color(SUCCESS);
            std::cout << "\n  >>> EXCELENTE! +" << pts << " PUNTOS <<<\n" << std::endl;
            play_sound(true);
            p->score += pts;
            q->answered = true;
            // Update correct count in profile
            PlayerStats stats = ProfileLoader::loadStat(p->name.c_str());
            stats.totalCorrect++;
            ProfileLoader::saveStat(p->name.c_str(), stats);
        } else {
            set_color(FAILURE);
            std::cout << "\n  >>> ERROR! La respuesta era " << q->correctIndex << " <<<\n" << std::endl;
            play_sound(false);
            if (useDouble) {
                std::cout << "  [!] El Doble o Nada ha fallado. Pierde " << currentRound << " pts." << std::endl;
                p->score -= currentRound;
            }
        }
        set_color(NEUTRAL);
        pause_screen();
    }

    void showScores() {
        std::cout << "\n------------------------------------------------------------" << std::endl;
        std::cout << "                - TABLA DE PUNTUACIONES - " << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
        Node<Player>* curr = players.getHead();
        while (curr) {
            std::cout << "  > " << curr->data.name << ": " << curr->data.score << " pts" << std::endl;
            curr = curr->next;
        }
        std::cout << "------------------------------------------------------------" << std::endl;
    }



    void handleTieBreaker(int tieScore) {
        // Set non-tied players to inactive
        Node<Player>* curr = players.getHead();
        while (curr) {
            if (curr->data.score < tieScore) {
                curr->data.active = false;
            }
            curr = curr->next;
        }

        // Sudden death loops
        bool tie = true;
        while (tie) {
            // Check if questions available
            int totalAvail = 0;
            for(int i=0; i<categories.count(); i++) 
                totalAvail += categories.get(i)->availableQuestions();
            
            if (totalAvail == 0) {
                 std::cout << "Se acabaron las preguntas. Empate definitivo." << std::endl;
                 return;
            }

            currentRound++; // Keep increasing value? Or fixed? "Puntos por pregunta correcta: 1 x número de ronda" implies it continues increasing.
            playRound();

            // Check if tie broken
            int maxS = -1;
             Node<Player>* pNode = players.getHead();
            while (pNode) {
                if (pNode->data.active && pNode->data.score > maxS) maxS = pNode->data.score;
                pNode = pNode->next;
            }
            
            int countMax = 0;
            pNode = players.getHead();
            while (pNode) {
                 if (pNode->data.active && pNode->data.score == maxS) countMax++;
                 pNode = pNode->next;
            }

            if (countMax == 1) {
                tie = false;
                // Find winner name
                pNode = players.getHead();
                while(pNode) {
                    if (pNode->data.active && pNode->data.score == maxS) {
                         std::cout << "\nDESEMPATE TERMINADO. GANADOR: " << pNode->data.name << std::endl;
                    }
                    pNode = pNode->next;
                }
            } else {
                 std::cout << "El empate persiste. Otra ronda." << std::endl;
            }
        }
    }
};

#endif // GAME_H
