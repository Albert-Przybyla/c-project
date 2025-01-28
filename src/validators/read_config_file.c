#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_config_file(const char *file_path, const char *key, int *a, int *b, int *c)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        printf("ERR: can't open file %s\n", file_path);
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, ",");
        while (token)
        {
            int quantity = 0;
            char type = '\0';

            char *x_pos = strchr(token, 'x');
            if (x_pos)
            {
                *x_pos = '\0';
                quantity = atoi(token);
                type = *(x_pos + 1);
            }

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

    fclose(file);
    return 1;
}
