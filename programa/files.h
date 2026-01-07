#ifndef FILES_H
#define FILES_H

#include <windows.h>
#include <fstream>
#include <iostream>
#include "models.h"
#include "structures.h"

// Helper to remove newline char
void clean_line(char* line) {
    int len = my_strlen(line);
    if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
}

class FileLoader {
public:
    // Load all categories from a directory path
    static LinkedList<Category> loadCategories(const char* dirPath) {
        LinkedList<Category> categories;
        
        MyString searchPath(dirPath);
        searchPath = searchPath + "\\*.txt";

        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE) {
            std::cout << "No se encontraron archivos en: " << dirPath << std::endl;
            return categories;
        }

        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // It's a file
                MyString fileName = findData.cFileName;
                
                // Category name is filename without extension?
                // Or just use filename as category name
                // Let's strip ".txt" for cleaner name
                char nameBuf[256];
                my_strcpy(nameBuf, fileName.c_str());
                int len = my_strlen(nameBuf);
                if (len > 4) nameBuf[len-4] = '\0'; // Remove .txt
                
                Category cat(nameBuf);

                // Construct full path
                MyString fullPath(dirPath);
                fullPath = fullPath + "\\" + fileName;

                loadQuestionsFromFile(fullPath.c_str(), cat);
                
                // Only add if it has questions?
                if (cat.questions.count() > 0) {
                     categories.add(cat);
                     std::cout << "Cargada categoria: " << nameBuf << " (" << cat.questions.count() << " preguntas)" << std::endl;
                }
            }
        } while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
        return categories;
    }

private:
    static void loadQuestionsFromFile(const char* filepath, Category& cat) {
        std::ifstream file(filepath);
        if (!file.is_open()) return;

        char buffer[1024];
        Question* currentQ = NULL;

        while (file.getline(buffer, 1024)) {
            // Check first char logic
            // P: Question
            // O: Option
            // R: Answer
            // Line format might be "P: Text" or just "P Text" or "P:Text"
            // We assume "X: ..." format based on description.
            // Actually description says: "P: indica una pregunta"
            
            if (my_strlen(buffer) < 3) continue;

            char type = buffer[0];
            char* content = buffer + 2; // Skip "X:" or "X "
            // Handle if space or colon
            if (buffer[1] == ':') content = buffer + 2;
            else if (buffer[1] == ' ') content = buffer + 2;
            else content = buffer + 1; // Maybe just "PText"? assuming colon.

            // Skip leading spaces
            while (*content == ' ') content++;

            if (type == 'P') {
                // Save previous question if exists
                if (currentQ != NULL) {
                    cat.addQuestion(*currentQ);
                    delete currentQ; 
                }
                currentQ = new Question(content);
            } else if (type == 'O') {
                if (currentQ) {
                    currentQ->addOption(content);
                }
            } else if (type == 'R') {
                 if (currentQ) {
                     // Parse int manually or use atoi
                     currentQ->correctIndex = atoi(content);
                 }
            }
        }
        
        // Add last question
        if (currentQ != NULL) {
             cat.addQuestion(*currentQ);
             delete currentQ;
        }

        file.close();
    }
};

class ProfileLoader {
public:
    static void saveStat(const char* name, PlayerStats stats) {
        // Simple update or append
        // For C++98, we'll just append to a list in memory and rewrite the file
        // Or simpler: just append and the last entry wins? No, let's do it right.
        std::ifstream fin("profiles.txt");
        LinkedList<MyString> lines;
        char buffer[256];
        bool found = false;
        
        while(fin.getline(buffer, 256)) {
            if (my_strstr(buffer, name) == buffer) { // Starts with name
                char updated[256];
                sprintf(updated, "%s:%d,%d,%d", name, stats.totalGames, stats.totalPoints, stats.totalCorrect);
                lines.add(MyString(updated));
                found = true;
            } else if (my_strlen(buffer) > 0) {
                lines.add(MyString(buffer));
            }
        }
        fin.close();

        if (!found) {
            char updated[256];
            sprintf(updated, "%s:%d,%d,%d", name, stats.totalGames, stats.totalPoints, stats.totalCorrect);
            lines.add(MyString(updated));
        }

        std::ofstream fout("profiles.txt");
        Node<MyString>* curr = lines.getHead();
        while(curr) {
            fout << curr->data.c_str() << std::endl;
            curr = curr->next;
        }
        fout.close();
    }

    static PlayerStats loadStat(const char* name) {
        PlayerStats stats;
        std::ifstream fin("profiles.txt");
        char buffer[256];
        while(fin.getline(buffer, 256)) {
            if (my_strstr(buffer, name) == buffer) {
                char* p = my_strchr(buffer, ':');
                if (p) {
                    sscanf(p+1, "%d,%d,%d", &stats.totalGames, &stats.totalPoints, &stats.totalCorrect);
                }
                break;
            }
        }
        fin.close();
        return stats;
    }
};

#endif // FILES_H
