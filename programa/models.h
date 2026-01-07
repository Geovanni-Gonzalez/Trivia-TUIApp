#ifndef MODELS_H
#define MODELS_H

#include "structures.h"

// --- Game Entities ---

struct Option {
    MyString text;
    
    Option() {}
    Option(const char* t) : text(t) {}
};

struct Question {
    MyString text;
    LinkedList<Option> options;
    int correctIndex; // 1-based index as per file format implies (R: 1)
    bool answered;

    Question() : correctIndex(0), answered(false) {}

    Question(const char* t) : text(t), correctIndex(0), answered(false) {}
    
    void addOption(const char* optText) {
        options.add(Option(optText));
    }
};

struct Category {
    MyString name;
    LinkedList<Question> questions;

    Category() {}
    Category(const char* n) : name(n) {}

    void addQuestion(Question q) {
        questions.add(q);
    }
    
    // Helper to get count of available questions
    int availableQuestions() {
        int count = 0;
        Node<Question>* curr = questions.getHead();
        while(curr) {
            if (!curr->data.answered) count++;
            curr = curr->next;
        }
        return count;
    }
};

struct Lifelines {
    bool splitUsed;   // 50/50
    bool doubleUsed;  // Doble o Nada
    
    Lifelines() : splitUsed(false), doubleUsed(false) {}
};

struct PlayerStats {
    int totalGames;
    int totalPoints;
    int totalCorrect;
    
    PlayerStats() : totalGames(0), totalPoints(0), totalCorrect(0) {}
};

struct Player {
    MyString name;
    int score;
    bool active; 
    Lifelines lifelines;

    Player() : score(0), active(true) {}
    Player(const char* n) : name(n), score(0), active(true) {}
};

#endif // MODELS_H
