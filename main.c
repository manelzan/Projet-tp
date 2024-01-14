#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;

void insertAtEnd(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void bubbleSort() {
    int swapped;
    Node* ptr1;
    Node* lptr = NULL;

    if (head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data > ptr1->next->data) {
                int temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void deleteNode(int key) {
    Node* temp = head;
    Node* prev = NULL;

    if (temp != NULL && temp->data == key) {
        head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;
    free(temp);
}

Node* searchNode(int key) {
    Node* temp = head;

    while (temp != NULL) {
        if (temp->data == key)
            return temp;
        temp = temp->next;
    }

    return NULL;
}

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Liste Chaînée avec Raylib");

    int numElements = 0;
    int inputValue = 0;
    bool sortButtonClicked = false;
    bool inputActive = false;
    bool searchButtonClicked = false;
    int searchValue = 0;
    Node* searchResult = NULL;
    bool addButtonClicked = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!inputActive && !addButtonClicked) {
            int key = GetKeyPressed();
            if (key >= 48 && key <= 57) {
                numElements = numElements * 10 + (key - 48);
            }

            if (IsKeyPressed(KEY_ENTER) && numElements != 0) {
                inputActive = true;
            }
        }

        if (inputActive && !sortButtonClicked && !addButtonClicked) {
            int key = GetKeyPressed();
            if (key >= 48 && key <= 57) {
                inputValue = inputValue * 10 + (key - 48);
            }

            if (IsKeyPressed(KEY_ENTER) && inputValue != 0) {
                insertAtEnd(inputValue);
                inputValue = 0;
                numElements--;

                if (numElements == 0) {
                    sortButtonClicked = true;
                }
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){50, 100, 100, 40}) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            bubbleSort();
            sortButtonClicked = true;
        }

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){200, 100, 100, 40}) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Supprimer le premier élément
            if (head != NULL) {
                Node* temp = head;
                head = head->next;
                free(temp);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 100, 100, 40}) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Recherche d'un élément
            searchButtonClicked = true;
            searchResult = NULL;
            searchValue = 0;
        }

        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){500, 100, 100, 40}) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            // Ajouter un élément
            addButtonClicked = true;
            inputValue = 0;
        }

        if (addButtonClicked) {
            int key = GetKeyPressed();
            if (key >= 48 && key <= 57) {
                inputValue = inputValue * 10 + (key - 48);
            }

            if (IsKeyPressed(KEY_ENTER) && inputValue != 0) {
                insertAtEnd(inputValue);
                inputValue = 0;
                addButtonClicked = false;
            }
        }

        if (searchButtonClicked) {
            int key = GetKeyPressed();
            if (key >= 48 && key <= 57) {
                searchValue = searchValue * 10 + (key - 48);
            }

            if (IsKeyPressed(KEY_ENTER) && searchValue != 0) {
                searchResult = searchNode(searchValue);
                searchButtonClicked = false;
            }
        }

        // Dessiner...
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dessiner la liste chaînée
        Node* temp = head;
        int xPos = 50;

        while (temp != NULL) {
            // Dessiner la cellule
            DrawRectangle(xPos, 200, 40, 40, BLUE);

            // Dessiner la flèche sauf pour le dernier élément
            if (temp->next != NULL) {
                DrawLine(xPos + 40, 220, xPos + 40 + 40, 220, BLACK);
            }

            // Dessiner le texte au centre de la cellule
            DrawText(TextFormat("%d", temp->data), xPos + 10, 210, 20, WHITE);

            temp = temp->next;
            xPos += 80; // Ajuster la distance entre les rectangles
        }

        // Dessiner les boutons
        DrawRectangleRec((Rectangle){50, 100, 100, 40}, sortButtonClicked ? RED : GREEN);
        DrawText("Trier", 70, 110, 20, WHITE);

        DrawRectangleRec((Rectangle){200, 100, 100, 40}, RED);
        DrawText("Supprimer", 210, 110, 20, WHITE);

        DrawRectangleRec((Rectangle){350, 100, 100, 40}, searchButtonClicked ? RED : BLUE);
        DrawText("Rechercher", 360, 110, 20, WHITE);

        DrawRectangleRec((Rectangle){500, 100, 100, 40}, addButtonClicked ? RED : PURPLE);
        DrawText("Ajouter", 520, 110, 20, WHITE);

        // Dessiner le résultat de la recherche
        if (searchResult != NULL) {
            DrawText("Element trouvé !", 650, 110, 20, GREEN);
        } else if (!searchButtonClicked && searchValue != 0) {
            DrawText("Element non trouvé.", 650, 110, 20, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
