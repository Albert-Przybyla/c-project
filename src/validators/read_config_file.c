#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_config_file(const char *file_path, int *a, int *b, int *c, int *price_a, int *price_b, int *price_c)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        printf("ERR: can't open file %s\n", file_path);
        return 0;
    }

    char line[256];
    int line_count = 0; // Zmienna do śledzenia, którą linię przetwarzamy (ilości czy ceny)

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0; // Usunięcie nowej linii

        if (line_count == 0) // Pierwsza linia (ilości produktów)
        {
            char *token = strtok(line, ",");
            while (token)
            {
                int quantity = 0;
                char type = '\0';

                // Sprawdzamy "x" (ilość produktu)
                char *x_pos = strchr(token, 'x');
                if (x_pos)
                {
                    *x_pos = '\0';
                    quantity = atoi(token);
                    type = *(x_pos + 1);
                }

                // Przypisanie ilości do odpowiedniego surowca
                if (type == 'A')
                {
                    *a = quantity;
                }
                else if (type == 'B')
                {
                    *b = quantity;
                }
                else if (type == 'C')
                {
                    *c = quantity;
                }
                token = strtok(NULL, ",");
            }
        }
        else if (line_count == 1) // Druga linia (ceny produktów)
        {
            char *token = strtok(line, ",");
            while (token)
            {
                int price = 0;
                char type = '\0';

                // Sprawdzamy końcówkę (typ produktu)
                if (sscanf(token, "%d%c", &price, &type) == 2)
                {
                    // Przypisanie ceny do odpowiedniego surowca
                    if (type == 'A')
                    {
                        *price_a = price;
                    }
                    else if (type == 'B')
                    {
                        *price_b = price;
                    }
                    else if (type == 'C')
                    {
                        *price_c = price;
                    }
                }
                token = strtok(NULL, ",");
            }
        }
        line_count++;
    }

    fclose(file);
    return 1;
}