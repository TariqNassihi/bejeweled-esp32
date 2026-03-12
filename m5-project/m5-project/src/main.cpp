#include "board.c"
#include "main.h"

// Functie om de positie van de cursor te verplaatsen op basis van versnellingsgegevens
void move_position(float acc_x, float acc_y)
{
  // Oude cursor wissen op basis van de huidige rotatiestatus
  if (cursor.rotate)
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size, game.cell_size * 2, WHITE);
  }
  else
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size * 2, game.cell_size, WHITE);
  }

  int moveX = 0;
  int moveY = 0;

  // Bepaal de bewegingsrichting op basis van de versnellingsgegevens
  if (abs(acc_x) > abs(acc_y))
  {
    // Horizontale beweging
    if (acc_x > MIN_TILT)
    {
      moveX = -game.cell_size;
    }
    else if (acc_x < -MIN_TILT)
    {
      moveX = game.cell_size;
    }
  }
  else
  {
    // Verticale beweging
    if (acc_y > MIN_TILT)
    {
      moveY = game.cell_size;
    }
    else if (acc_y < -MIN_TILT)
    {
      moveY = -game.cell_size;
    }
  }

  // Verplaats de cursorpositie
  cursor.x += moveX;
  cursor.y += moveY;

  // Houd de cursor binnen het scherm
  cursor.x = constrain(cursor.x, 0, M5.Lcd.width() - (cursor.rotate ? game.cell_size : game.cell_size * 2));
  cursor.y = constrain(cursor.y, 0, M5.Lcd.height() - (cursor.rotate ? game.cell_size * 2 : game.cell_size));

  // Houd de cursor binnen het speelbord
  cursor.x = constrain(cursor.x, BOARD_OFFSET_X, BOARD_OFFSET_X + (game.size_column - (cursor.rotate ? 1 : 2)) * game.cell_size);
  cursor.y = constrain(cursor.y, BOARD_OFFSET_Y, BOARD_OFFSET_Y + (game.size_row - (cursor.rotate ? 2 : 1)) * game.cell_size);

  // Teken de cursor op de nieuwe positie
  if (cursor.rotate)
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size, game.cell_size * 2, BLACK);
  }
  else
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size * 2, game.cell_size, BLACK);
  }
}

// Functie om de positie van de cursor bij te werken op basis van de versnellingsgegevens
void updateCursorPosition()
{
  float acc_x = 0, acc_y = 0, acc_z = 0;

  // Versnellingsgegevens ophalen
  M5.IMU.getAccelData(&acc_x, &acc_y, &acc_z);

  // Positie van de cursor bijwerken
  move_position(acc_x, acc_y);

  // Zorg ervoor dat de cursor binnen het scherm blijft
  cursor.x = constrain(cursor.x, 0, M5.Lcd.width() - game.cell_size);
  cursor.y = constrain(cursor.y, 0, M5.Lcd.height() - game.cell_size);
}

// Functie om de kleur op basis van de boardwaarde te bepalen
int find_color(int x, int y)
{
  int color;
  switch (board[x][y])
  {
  case 0:
  case 10:
    color = RED;
    break;

  case 1:
  case 11:
    color = ORANGE;
    break;

  case 2:
  case 12:
    color = GREEN;
    break;

  case 3:
  case 13:
    color = BLUE;
    break;

  case 4:
  case 14:
    color = YELLOW;
    break;

  case 5:
  case 15:
    color = CYAN;
    break;

  case 6:
  case 16:
    color = MAGENTA;
    break;

  default:
    color = WHITE;
    break;
  }

  return color;
}

// Functie om het speelbord te tekenen op het M5Stick-scherm
void drawBoardOnStick()
{
  for (int i = 0; i < game.size_row; i++)
  {
    for (int j = 0; j < game.size_column; j++)
    {
      int k = board[i][j];
      if (k > 9 && k != 19) // kijk als het een speciale diamant is, want ze worden op een ander manier getekent (driehoeken)
      {
        M5.Lcd.fillRect(BOARD_OFFSET_X + j * game.cell_size, BOARD_OFFSET_Y + i * game.cell_size, game.cell_size, game.cell_size, WHITE);

        M5.Lcd.fillTriangle(
            BOARD_OFFSET_X + j * game.cell_size, BOARD_OFFSET_Y + (i + 1) * game.cell_size,
            BOARD_OFFSET_X + (j + 1) * game.cell_size, BOARD_OFFSET_Y + (i + 1) * game.cell_size,
            BOARD_OFFSET_X + j * game.cell_size + game.cell_size / 2, BOARD_OFFSET_Y + i * game.cell_size,
            find_color(i, j));

        M5.Lcd.drawRect(BOARD_OFFSET_X + j * game.cell_size, BOARD_OFFSET_Y + i * game.cell_size, game.cell_size, game.cell_size, WHITE);
      }
      else
      {
        M5.Lcd.fillRect(BOARD_OFFSET_X + j * game.cell_size, BOARD_OFFSET_Y + i * game.cell_size, game.cell_size, game.cell_size, find_color(i, j));
        M5.Lcd.drawRect(BOARD_OFFSET_X + j * game.cell_size, BOARD_OFFSET_Y + i * game.cell_size, game.cell_size, game.cell_size, WHITE);
      }
    }
  }
}

// Functie om tekst (score en resterende zetten ) op het scherm weer te geven
void draw_text()
{
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 5);
  M5.Lcd.print("Moves left:");

  M5.Lcd.setCursor(0, 15);
  M5.Lcd.print("Score = ");
}

// Functie om de cursor op het scherm te tekenen
void drawCursor()
{
  if (cursor.rotate)
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size * 2, game.cell_size, WHITE);
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size, game.cell_size * 2, BLACK);
  }
  else
  {
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size, game.cell_size * 2, WHITE);
    M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size * 2, game.cell_size, BLACK);
  }
}

// Functie om de cursor te roteren wanneer de A-knop wordt ingedrukt
void rotate_cursor()
{
  if (M5.BtnA.wasPressed())
  {
    // Controleer of de cursor zich in de laatste kolom bevindt
    if (cursor.x >= BOARD_OFFSET_X + (game.size_column - 2) * game.cell_size && cursor.rotate)
    {
      // Verplaats de cursor naar links om te voorkomen dat deze buiten het bord gaat
      M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size, game.cell_size * 2, WHITE);
      cursor.x -= game.cell_size;
    }

    if (cursor.y >= BOARD_OFFSET_Y + (game.size_row - 2) * game.cell_size && !cursor.rotate)
    {
      // Verplaats de cursor omhoog om te voorkomen dat deze buiten het bord gaat
      M5.Lcd.drawRect(cursor.x, cursor.y, game.cell_size * 2, game.cell_size, WHITE);
      cursor.y -= game.cell_size;
    }

    // rotatiestatus veranderen
    cursor.rotate = !cursor.rotate;

    // Teken de cursor op de nieuwe positie
    drawCursor();
  }
}

// Functie om kleuren te wisselen op basis van de cursorpositie
void swap_color()
{
  // Huidige positie van de cursor bepalen
  int x1 = (cursor.y - BOARD_OFFSET_Y) / game.cell_size;
  int y1 = cursor.x / game.cell_size;

  int x2, y2;

  // Bepaal de tweede positie op basis van de rotatiestatus van de cursor
  if (!cursor.rotate)
  {
    x2 = x1;
    y2 = y1 + 1;
  }
  else
  {
    x2 = x1 + 1;
    y2 = y1;
  }

  if (game.always_swap) // kijk als je eerst moet controleren op een patroon voor een swap
  {
    // Verminder het aantal resterende zetten en wissel de kleuren
    game.moves_left -= 1;
    swap_numbers(x1, y1, x2, y2);
  }
  else
  {
    check_pattern_after_swap(x1, y1, x2, y2);
  }
}

// Functie om de score op het scherm bij te werken
void update_score()
{
  M5.Lcd.setCursor(47, 15);
  M5.Lcd.fillRect(47, 15, 20, 10, TFT_BLACK);
  M5.Lcd.println(game.score);
}

// Functie om het aantal resterende zetten op het scherm bij te werken
void update_moves()
{
  M5.Lcd.fillRect(66, 5, 20, 10, TFT_BLACK);
  M5.Lcd.setCursor(66, 5);
  if (game.moves_left < 10)
  {
    M5.Lcd.print("0");
  }

  M5.Lcd.println(game.moves_left);
}

// Functie om het huidige niveau op het scherm weer te geven
void draw_level()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(20, 40);
  M5.Lcd.print("Niveau:");
  M5.Lcd.println(game.level);
}

// Functie om een nieuw niveau te starten
void begin_level()
{

  draw_level(); // tekent het niveau-nummer op het scherm
  delay(2000);

  // Bord initialiseren en diamanten plaatsen
  allocate_board();
  place_diamonds();
  remove_and_replace_patterns();

  // Scherm vullen met zwart en het speelbord tekenen
  M5.Lcd.fillScreen(BLACK);
  drawBoardOnStick();
  draw_text();

  // Cursorpositie opnieuw instellen
  cursor.x = 0;
  cursor.y = 50;
}

// Functie voor het starten van een nieuw level , Verhoog de grootte van het bord en andere kenmerken als de huidige grootte niet 9 is

void new_level()
{
  if (game.size_row != 9)
  {
    game.size_row += 1;
    game.size_column += 1;
    game.cell_size -= 1;
    game.number_color += 1;
    game.score = 0;
  }

  game.level += 1;
  game.moves_left_init += 5;
  game.score_needed += +20;
}

// Functie voor het tonen van het game-over scherm
void game_over()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.print("Game over =(");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.print("Press A to");
  M5.Lcd.setCursor(0, 75);
  M5.Lcd.print("restart");
}

// Functie voor het herstarten van het spel na een game-over
void restart()
{
  game.should_continue = false;

  // Wacht op de knop A om opnieuw te starten
  while (!game.should_continue)
  {
    M5.update();
    if (M5.BtnA.wasPressed())
    {
      game.score = 0;
      game.moves_left_init += 5;
      game.should_continue = true;
    }
  }
}

// Functie voor het kiezen van de spelmodus (always_swap of swap when pattern)
void look_mode()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.print("Choose mode");
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.print("Press A:");
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.print("always swap");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.print("Press B:");
  M5.Lcd.setCursor(0, 70);
  M5.Lcd.print("swap when");
  M5.Lcd.setCursor(0, 80);
  M5.Lcd.print("pattern");

  // Wacht op de knoppen A of B om de spelmodus in te stellen
  while (true)
  {
    M5.update();
    if (M5.BtnA.wasPressed())
    {
      game.always_swap = true;
      break;
    }
    else if (M5.BtnB.wasPressed())
    {
      break;
    }
  }
}

// Functie voor het tekenen van het hoofdmenu als de speler op de A en B knopt heeft gedrukt
void draw_menu()
{
  M5.Lcd.fillScreen(TFT_BLACK);

  int startY = 35;

  // Teken de menu-opties
  M5.Lcd.setCursor(0, startY);
  M5.Lcd.print("1. Back");

  M5.Lcd.setCursor(0, startY + 30);
  M5.Lcd.print("2. Save");

  M5.Lcd.setCursor(0, startY + 60);
  M5.Lcd.print("3. Load");

  M5.Lcd.setCursor(0, startY + 90);
  M5.Lcd.print("4. Reset");

  // Markeer de eerste instructie en wacht op input
  highlightInstruction(1, TFT_RED);
  wait_input_menu(1);
}

// Functie voor het markeren van instructies in het hoofdmenu
void highlightInstruction(int currentInstruction, int color)
{
  int startY = 35;
  int lineHeight = 30;

  for (int i = 1; i <= 4; ++i)
  {
    M5.Lcd.setCursor(0, startY + (i - 1) * lineHeight);

    if (i == currentInstruction)
    {
      M5.Lcd.setTextColor(color);
    }

    // Tekst voor elk menu-item
    switch (i)
    {
    case 1:
      M5.Lcd.print("1. Back");
      break;
    case 2:
      M5.Lcd.print("2. Save");
      break;
    case 3:
      M5.Lcd.print("3. Load");
      break;
    case 4:
      M5.Lcd.print("4. Reset");
      break;
    }

    M5.Lcd.setTextColor(TFT_WHITE);
  }
}

// Wacht op invoer in het hoofdmenu en voert de geselecteerde actie uit
void wait_input_menu(int curr)
{
  while (true)
  {
    M5.update();

    if (M5.BtnA.wasPressed()) // Markeer de volgende instructie bij het indrukken van knop A
    {

      highlightInstruction(curr, TFT_WHITE);
      highlightInstruction(++curr % 5, TFT_RED);
    }

    if (M5.BtnB.wasPressed()) // Voer de geselecteerde actie uit bij het indrukken van knop B
    {

      M5.Lcd.fillScreen(BLACK);
      draw_text();
      switch (curr % 5)
      {
      case 1:
        // Actie voor menu-optie 1
        break;
      case 2:
        // Actie voor menu-optie 2 (Save)
        save_game();
        break;
      case 3:
        // Actie voor menu-optie 3 (Load)
        free_board();
        load_game();

        break;
      case 4:
        // Actie voor menu-optie 4 (reset)
        game.score = 0;
        game.moves_left = game.moves_left_init;
        place_diamonds();
        remove_and_replace_patterns();

        break;
      }

      M5.update();
      break;
    }
  }
}

// Functie voor het opslaan van het spel in EEPROM
void save_game()
{
  int address = 0;

  // Sla de game state op in EEPROM
  EEPROM.put(address, game);
  address += sizeof(game);

  // Sla de bordgegevens op in EEPROM
  for (int i = 0; i < game.size_row; i++)
  {
    for (int j = 0; j < game.size_column; j++)
    {
      EEPROM.write(address, board[i][j]);
      address++;
    }
  }
  EEPROM.commit(); // Forceer EEPROM om gegevens te schrijven
}

// Functie voor het laden van het spel uit EEPROM
void load_game()
{
  int address = 0;

  // Laad de game state uit EEPROM
  EEPROM.get(address, game);
  address += sizeof(game);

  // Wijs geheugen toe voor het bord en teken de cursor
  allocate_board();
  drawCursor();

  // Laad de bordgegevens uit EEPROM
  for (int i = 0; i < game.size_row; i++)
  {
    for (int j = 0; j < game.size_column; j++)
    {
      board[i][j] = EEPROM.read(address);
      address++;
    }
  }
}

// Initialisatie van de cursor op positie (0, 50)
Cursor cursor = {0, 50};

// Initialisatie van het spel (game_state) met startwaarden
game_state game = {12, 10, 0, 0, 1, false, 7, 6, 5, true, 35};

void setup()
{
  M5.begin();
  M5.IMU.Init();
  Serial.begin(115200);
  Serial.flush();
  EEPROM.begin(512);
  look_mode();
}

void loop()
{
  M5.update();

  // Blijf doorgaan zolang het spel moet doorgaan
  while (game.should_continue)
  {
    // Herstel de zetten en score voor het begin van elk level
    game.moves_left = game.moves_left_init;
    game.score = 0;
    begin_level();
    drawCursor();

    // Ga door zolang het vereiste scoreniveau niet is bereikt en er zetten over zijn
    while (game.score < game.score_needed && game.moves_left != 0)
    {
      M5.update();

      // Controleer of zowel knop B als knop A zijn ingedrukt om het menu te openen
      if (M5.BtnB.wasPressed() && M5.BtnA.wasPressed())
      {
        draw_menu();
      }

      // Controleer of knop B is ingedrukt om de kleur te wisselen
      if (M5.BtnB.wasPressed())
      {
        swap_color();
        M5.update();
      }

      // Controleer bordpatroon en voer bordupdates uit
      check_pattern_board();
      shift_down();
      drawBoardOnStick();

      // Voer cursorrotatie en cursorpositie-updates uit
      rotate_cursor();
      updateCursorPosition();

      // Voer score- en zettenupdates uit
      update_score();
      update_moves();

      // Wacht een korte periode voordat de volgende iteratie begint
      delay(800);
    }
    free_board();

    // Controleer of het vereiste scoreniveau is bereikt voor het volgende level
    if (game.score >= game.score_needed)
    {
      new_level();
    }
    else
    {
      // Toon het game-over scherm en wacht op herstart
      game_over();
      restart();
    }
  }
}
