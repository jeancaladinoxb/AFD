#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TRANS 100
#define MAX_LINE 200

typedef struct {
    char estado[50];
    char simbolo[50];
    char siguiente[50];
} Transicion;

// Función para quitar espacios y saltos de línea (segura)
void trim(char *str) {
    char *start = str;
    char *end;

    // Quitar espacios al inicio
    while(*start == ' ' || *start == '\t') start++;

    // Mover la cadena hacia el inicio si hacía falta
    if (start != str) memmove(str, start, strlen(start) + 1);

    // Quitar espacios/nuevas líneas al final
    end = str + strlen(str) - 1;
    while(end >= str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }
}

int main() {
    FILE *conf = fopen("Conf.txt", "r");
    FILE *cad = fopen("Cadenas.txt", "r");

    if (!conf || !cad) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    Transicion transiciones[MAX_TRANS];
    char inicial[50] = "";
    char finales[200] = "";
    int numTrans = 0;

    char linea[MAX_LINE];
    while (fgets(linea, sizeof(linea), conf)) {
        linea[strcspn(linea, "\n")] = 0; // quitar salto de línea

        if (linea[0] == '#' || strlen(linea) < 2) continue;

        if (strncmp(linea, "Inicial=", 8) == 0) {
            sscanf(linea, "Inicial=%49s", inicial);
            trim(inicial);
        } else if (strncmp(linea, "Finales=", 8) == 0) {
            sscanf(linea, "Finales=%199s", finales);
            trim(finales);
        } else if (strchr(linea, ',')) {
            // Es una transición
            char e1[50], simbolo[50], e2[50];
            int n = sscanf(linea, "%49[^,],%49[^,],%49s", e1, simbolo, e2);
            if (n == 3) {
                trim(e1); trim(simbolo); trim(e2);
                strncpy(transiciones[numTrans].estado, e1, 49);
                strncpy(transiciones[numTrans].simbolo, simbolo, 49);
                strncpy(transiciones[numTrans].siguiente, e2, 49);
                transiciones[numTrans].estado[49] = '\0';
                transiciones[numTrans].simbolo[49] = '\0';
                transiciones[numTrans].siguiente[49] = '\0';
                numTrans++;
            }
        }
    }
    fclose(conf);

    // Procesar cadenas
    while (fgets(linea, sizeof(linea), cad)) {
        linea[strcspn(linea, "\n")] = 0; // quitar salto de línea
        if (strlen(linea) < 1) continue;

        char estado[50];
        strncpy(estado, inicial, 49);
        estado[49] = '\0';

        printf("Cadena: %s\n", linea);
        printf("Recorrido: %s", estado);

        int aceptada = 1;
        char *token = strtok(linea, " ");
        while (token) {
            trim(token);  // limpiar token
            int encontrado = 0;

            for (int i = 0; i < numTrans; i++) {
                if (strcmp(transiciones[i].estado, estado) == 0 &&
                    strcmp(transiciones[i].simbolo, token) == 0) {
                    strncpy(estado, transiciones[i].siguiente, 49);
                    estado[49] = '\0';
                    printf(" -> %s", estado);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("\nTransición no encontrada: (%s, %s)\n", estado, token);
                aceptada = 0;
                break;
            }
            token = strtok(NULL, " ");
        }

        // Verificar si el último estado es final
        if (aceptada && strstr(finales, estado)) {
            printf("\nACEPTADA\n\n");
        } else {
            printf("\nRECHAZADA\n\n");
        }
    }
    fclose(cad);
    return 0;
}
