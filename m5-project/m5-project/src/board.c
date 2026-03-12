
#include "board.h"

int **board;

void allocate_board() // dynamische geheugenallocatie voor het bord
{
    board = (int **)malloc(game.size_row * sizeof(int *));
    for (int i = 0; i < game.size_row; i++)
    {
        board[i] = (int *)malloc(game.size_column * sizeof(int));
    }
}

void free_board() // geef het geheugen terug vrij
{
    for (int i = 0; i < game.size_row; i++)
    {
        free(board[i]);
    }
    free(board);
}
void place_diamonds(void) // plaats de diamanten in het bordt (cijfers)
{

    srand(time(NULL));
    for (int i = 0; i < game.size_row; i++)
    {
        for (int j = 0; j < game.size_column; j++)
        {
            // Genereer een willekeurig getal tussen 0 en 6 voor die een kleur representeert
            int color = rand() % game.number_color;
            board[i][j] = color;
        }
    }
}

void print_board(void) // print het bord
{
    for (int i = 0; i < game.size_row; i++)
    {
        for (int j = 0; j < game.size_column; j++)
        {
            printf("%i ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void remove_and_replace_patterns() // verwijder alle patronen van het bord bij de start
{
    int changed = 1;
    while (changed == 1) // doorgaan totdat er niets is veranderd op het bord
    {
        changed = 0;
        for (int i = 0; i < game.size_row; i++)
        {
            for (int j = 0; j < game.size_column; j++)
            {
                // Check horizontaal
                if (j <= game.size_column - 3 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2])
                {
                    // zet een ander kleur op het positie
                    board[i][j] = rand() % game.number_color;

                    changed = 1;
                }
                // Check verticaal
                else if (i <= game.size_row - 3 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j])
                {
                    board[i][j] = rand() % game.number_color;

                    changed = 1;
                }
            }
        }
    }
}

void check_special_diamond(int x, int y) // wordt aangeroepen met de positie van een speciale diamant
{

    if (board[x][y] != 19 && ((x <= game.size_row - 3 && board[x][y] == board[x + 1][y] + 10 && board[x][y] == board[x + 2][y] + 10) ||
                              (x >= 2 && board[x][y] == board[x - 1][y] + 10 && board[x][y] == board[x - 2][y] + 10) ||
                              (x > 0 && x != game.size_row - 1 && board[x][y] == board[x - 1][y] + 10 && board[x][y] == board[x + 1][y] + 10))) // zoek een patroon verticaal
    {

        delete_vertical(y);
    }
    else if (board[x][y] != 19 && ((y <= game.size_column - 3 && board[x][y] == board[x][y + 1] + 10 && board[x][y] == board[x][y + 2] + 10) ||
                                   (y >= 2 && board[x][y] == board[x][y - 1] + 10 && board[x][y] == board[x][y - 2] + 10) ||
                                   (y > 0 && x != game.size_column - 1 && board[x][y] == board[x][y - 1] + 10 && board[x][y] == board[x][y + 1] + 10))) // zoek een patroon horizontaal
    {
        delete_horizontal(x);
    }
}

void delete_vertical(int y) // verwijder heel het kolom
{
    for (int i = 0; i < game.size_row; i++)
    {
        board[i][y] = 9;
    }
}

void delete_horizontal(int x) // verwijder heel het rij
{
    for (int i = 0; i <= game.size_column - 1; i++)
    {

        board[x][i] = 9;
    }
}

void check_pattern_board() // zoek naar patronen in het bord
{
    for (int i = 0; i < game.size_row; i++)
    {
        for (int j = 0; j < game.size_column; j++)
        {

            if (board[i][j] > 9)
            {
                check_special_diamond(i, j); // moet de hele rij/kolom verwijderen als er een speciaal diamant is
            }
            // check horizontal
            else if (j <= game.size_column - 3 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2]) // patroon van lengte 3 gevonden
            {
                int temp = board[i][j]; // verwijder deze patroon
                board[i][j] = 9;
                board[i][j + 1] = 9;
                board[i][j + 2] = 9;

                for (int k = 3; k < game.size_row && j + k < game.size_column; k++) // kijk als het patroon groter dan 3 is
                {
                    if (board[i][j + k] == temp) // patroon groter dan 4 gevonden
                    {
                        board[i][j + k] = 9;
                        board[i][j] = temp + 10;
                    }
                    else
                    {
                        break; // Verlaat de binnenste lus want hier eindigt het patroon
                    }
                }
            }
            // Check vertical en zelfde als erboven

            else if (i <= game.size_row - 3 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j])
            {
                int temp = board[i][j];
                board[i][j] = 9;
                board[i + 1][j] = 9;
                board[i + 2][j] = 9;

                for (int k = 3; k < game.size_column && i + k < game.size_row; k++)
                {
                    if (board[i + k][j] == temp)
                    {
                        board[i + k][j] = temp + 10;
                        board[i + k - 1][j] = 9;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
}

// Speciale diamanten hebben een waarde die 10 groter is dan die van normale diamanten;
// daarom moet ik eerst 10 aftrekken bij het onderzoeken van een patroon.

int give_real_value(int x, int y)
{

    if (board[x][y] > 9)
    {
        return board[x][y] - 10;
    }
    else
    {
        return board[x][y];
    }
}

void check_horizontal(int x, int y, int offsetX1, int offsetX2, int *pattern_found) // zoek horizontaal een patroon
{
    if (y + offsetX2 < game.size_column && y + offsetX1 >= 0 &&
        give_real_value(x, y) == give_real_value(x, y + offsetX1) &&
        give_real_value(x, y) == give_real_value(x, y + offsetX2))
    {
        *pattern_found = 1;
    }
}

void check_vertical(int x, int y, int offsetY1, int offsetY2, int *pattern_found) // zoek verticaal een patroon
{
    {
        if (x + offsetY2 < game.size_row && x + offsetY1 >= 0 &&
            give_real_value(x, y) == give_real_value(x + offsetY1, y) &&
            give_real_value(x, y) == give_real_value(x + offsetY2, y))
        {
            *pattern_found = 1;
        }
    }
}

void check_pattern_after_swap(int position1_x, int position1_y, int position2_x, int position2_y)
{
    // Voer de swap uit
    swap_numbers(position1_x, position1_y, position2_x, position2_y);

    // Controleer op een patroon na de swap
    int pattern_found = 0;

    // Controleer horizontaal naar rechts
    check_horizontal(position1_x, position1_y, 1, 2, &pattern_found);
    check_horizontal(position2_x, position2_y, 1, 2, &pattern_found);

    // Controleer horizontaal naar links
    check_horizontal(position1_x, position1_y, -1, -2, &pattern_found);
    check_horizontal(position2_x, position2_y, -1, -2, &pattern_found);

    // Controleer horizontaal links en rechts
    check_horizontal(position1_x, position1_y, -1, 1, &pattern_found);
    check_horizontal(position2_x, position2_y, -1, 1, &pattern_found);

    // Controleer verticaal naar boven
    check_vertical(position1_x, position1_y, -1, -2, &pattern_found);
    check_vertical(position2_x, position2_y, -1, -2, &pattern_found);

    // Controleer verticaal naar beneden
    check_vertical(position1_x, position1_y, 1, 2, &pattern_found);
    check_vertical(position2_x, position2_y, 1, 2, &pattern_found);

    // Controleer verticaal boven en beneden
    check_vertical(position1_x, position1_y, -1, 1, &pattern_found);
    check_vertical(position2_x, position2_y, -1, 1, &pattern_found);

    // als er geen patroon wordt gevonden, annuleer dan de swap
    if (!pattern_found)
    {
        swap_numbers(position1_x, position1_y, position2_x, position2_y);
    }
    else
    {
        game.moves_left -= 1; // er is een patroon na een swap, dus aantal moves decrementen
    }
}

void shift_down() // kleuren van het bord doen 'vallen' op lege posities (cijfer 9 = lege posities)
{
    for (int i = 0; i < game.size_row; i++)
    {
        for (int j = 0; j < game.size_column; j++)
        {
            if ((i == 0 && board[i][j] == 9) || board[i][j] == 19) // Als er een lege positie op de eerste rij is, moet er een willekeurige kleur worden toegewezen aan deze positie. en wint de speler een punt

            {

                board[i][j] = rand() % game.number_color;
                game.score += 1;
            }

            if (board[i][j] == 9)
            {
                // Wissel het getal erboven met het huidige getal, waardoor de kleur naar beneden valt.

                swap_numbers(i, j, i - 1, j);
            }
        }
    }
}

void swap_numbers(int position1_x, int position1_y, int position2_x, int position2_y) // swap 2 posities in het bord
{
    // Controleer of de posities binnen het bereik van het bord vallen
    if (position1_x >= 0 && position1_x < game.size_row && position1_y >= 0 && position1_y < game.size_column &&
        position2_x >= 0 && position2_x < game.size_row && position2_y >= 0 && position2_y < game.size_column)
    {
        // Wissel de waarden op de opgegeven posities
        int temp = board[position1_x][position1_y];
        board[position1_x][position1_y] = board[position2_x][position2_y];
        board[position2_x][position2_y] = temp;
    }
}
