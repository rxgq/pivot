#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef enum {
    ROCK, PAPER, SCISSORS
} Move;

int rounds, current_round, player_wins, computer_wins = 0;

void game_end() {
    printf("\n\n%s: %d - %d\n",
        (player_wins > computer_wins) ? "Player wins" : 
        (computer_wins > player_wins) ? "Computer wins" : "Draw",
        player_wins, computer_wins);
}

void round_outcome(Move pm, Move cm) {
    printf("\n");
    const char *moves[] = {"ROCK", "PAPER", "SCISSORS"};
    const char *messages[] = {"It's a tie!", "Player wins this round!", "Computer wins this round!"};

    int outcomes[3][3] = {{0, 2, 1}, {1, 0, 2}, {2, 1, 0}};
    int outcome = outcomes[pm][cm];

    printf("\nPlayer chose %s, Computer chose %s. %s\n", moves[pm], moves[cm], messages[outcome]);

    if (outcome == 1) player_wins++;
    if (outcome == 2) computer_wins++;

    printf("Press Enter..");
    getchar();
}

Move player_move() {
    char mov_inp;
    printf("\n\nWhat is your move: (r), (p), (s)?\n");
    
    while (1) {
        scanf(" %c", &mov_inp);
        if (mov_inp == 'r') return ROCK;
        if (mov_inp == 'p') return PAPER;
        if (mov_inp == 's') return SCISSORS;

        printf("\nInvalid move. Please enter (r), (p), or (s):\n");
    }
}

int main() {
    srand(time(NULL));

    printf("How many round should we play?\n");
    scanf("%d", &rounds);
    while (getchar() != '\n'); // clear input buffer

    while(current_round < rounds) {
        round_outcome(player_move(), rand() % 3);
        current_round++;
    }

    game_end();

    return 0;
}