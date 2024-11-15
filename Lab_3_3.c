#include <stdio.h> // Standard input/output library
#include <stdlib.h> // Standard library
#include <string.h> // String manipulation functions
#include <time.h> // Time functions

#define MAX_TEAMS 5       // Maximum number of teams that can be enrolled
#define MAX_PLAYERS 5    // Maximum number of players in a team

// Structure to store individual player information
struct Player {
    char name[25];         // Player's full name
    int kit_number;        // Unique kit number for the player (1-99)
    char dob[50];          // Date of birth (in DD/MM/YYYY format)
    char position[50];     // Player's position in the team (e.g., Forward, Goalkeeper)
};

// Structure to store team information, including players
struct Team {
    char team_name[20];            // Team's name
    struct Player players[MAX_PLAYERS]; // Array of players in the team
    int num_players;               // Number of players currently in the team
};

// Global variables
struct Team teams[MAX_TEAMS];      // Array to hold all enrolled teams
int enrolled_teams_count = 0;      // Track the number of enrolled teams

// Function prototypes
void display_menu(); // Display the main menu
void enroll_team(); // Enroll a new team
void add_player(); // Add a player to a team
void search_and_update_player(); // Search for a player and update details
void display_team_statistics(); // Display team statistics
int calculate_age(const char* dob); // Calculate age from date of birth
void handle_invalid_input(); // Handle invalid user input
int select_team(); // Select a team from the list
int validate_kit_number(int team_index, int kit_number); // Validate kit number
int validate_player_name(int team_index, char* name, int player_index_to_skip); // Validate player name
void update_player_info(int team_index, int player_index); // Update player details

/**
 * Main function initializes the program and displays the main menu in a loop.
 * Allows user to select options to manage teams and players.
 */
int main() {
    int user_choice;

    // Initialize each team to have zero players initially
    for (int i = 0; i < MAX_TEAMS; i++) {
        teams[i].num_players = 0;
    }

    // Main menu loop: allows user to choose an action repeatedly until they exit
    while (1) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &user_choice);
        getchar(); // Consume the newline character left by scanf

        // Execute the appropriate function based on the user's choice
        switch (user_choice) {
            case 1:
                enroll_team();
                break;
            case 2:
                add_player();
                break;
            case 3:
                search_and_update_player();
                break;
            case 4:
                display_team_statistics();
                break;
            case 5:
                printf("Thank you for using the League Team Application.\nExiting...\n");
                return 0;
            default:
                handle_invalid_input();
        }
    }
}

/**
 * Function to display the main menu of the application.
 * Lists the available options for the user to choose from.
 */
void display_menu() {
    printf("\n--- League Team Application ---\n");
    printf("1. Enroll a New Team\n");
    printf("2. Add a Player to a Team\n");
    printf("3. Search and Update Player Details\n");
    printf("4. Display Team Statistics\n");
    printf("5. Exit Application\n");
}

/**
 * Enrolls a new team by requesting a unique team name.
 * Ensures that the maximum number of teams isn't exceeded and prevents duplicate team names.
 */
void enroll_team() {
    if (enrolled_teams_count >= MAX_TEAMS) { // Check if max teams limit is reached
        printf("The maximum number of teams has already been enrolled.\n");
        return; // Exit the function if the limit is reached
    }
    char team_name[20]; 
    printf("Enter the name of the team: ");
    if (fgets(team_name, sizeof(team_name), stdin) == NULL) { // Read team name
        printf("Error reading team name.\n");
        return; // Exit the function if an error occurs
    }
    team_name[strcspn(team_name, "\n")] = '\0'; // Remove newline character
    if (strlen(team_name) == 0) { // Check if team name is empty
        printf("Team name cannot be empty.\n");
        return; // Exit the function if the team name is empty
    }
    // Check if a team with this name already exists
    for (int i = 0; i < enrolled_teams_count; i++) {
        if (strcasecmp(teams[i].team_name, team_name) == 0) {
            printf("A team with this name already exists.\n");
            return;
        }
    }
    // Add the new team if no duplicates are found
    strcpy(teams[enrolled_teams_count].team_name, team_name); // Copy team name to the team structure
    teams[enrolled_teams_count].num_players = 0; // Initialize the number of players to zero
    enrolled_teams_count++; // Increment the count of enrolled teams
    printf("Team %s has been enrolled successfully.\n", team_name);
}

/**
 * Adds a player to a team selected by the user.
 * Collects player information (name, kit number, DOB, position) and ensures it is unique within the team.
 */
void add_player() {
    if (enrolled_teams_count == 0) { // Ensure there are teams to add players to
        printf("No teams have been enrolled yet. Please enroll a team first.\n");
        return; // Exit the function if no teams are enrolled
    }

    int team_choice = select_team(); // Let user choose a team
    if (team_choice == -1) return;   // Exit if invalid selection

    if (teams[team_choice].num_players >= MAX_PLAYERS) { // Check max players
        printf("This team already has the maximum number of players (%d).\n", MAX_PLAYERS);
        return; // Exit if max players are reached
    }

    struct Player new_player; // Temporary player structure to hold input details
    int valid_input = 0; // Flag to check if input is valid

    while (!valid_input) { // Loop until valid input is provided
        printf("Enter player name (max 25 characters): ");
        if (fgets(new_player.name, sizeof(new_player.name), stdin) == NULL) { // Read player name
            printf("Error reading player name.\n");
            return; 
        }
        new_player.name[strcspn(new_player.name, "\n")] = '\0'; // Remove newline character

        // Check for duplicate player name
        if (validate_player_name(team_choice, new_player.name, -1)) {
            printf("A player with name %s already exists in this team.\n", new_player.name);
            printf("The following players are already enrolled in the team\n");
            //Print all names enrolled in all teams
            for (int i = 0; i < enrolled_teams_count; i++) {
                for (int j = 0; j < teams[i].num_players; j++) {
                    printf("Team %s\n", teams[i].team_name);
                    printf("Player %d: %s\n", j + 1, teams[i].players[j].name);
                }
            }
            continue; // Re-prompt for valid player name
        }

        printf("Enter kit number (1-99): ");
        scanf("%d", &new_player.kit_number);
        getchar();  // Clear newline left by scanf

        // Validate kit number and check for duplicates
        if (!validate_kit_number(team_choice, new_player.kit_number)) { // Check kit number
            printf("Enter a unique kit number between 1 and 99.\n");
            printf("The following kit numbers are already enrolled in the team\n");
            // print all kit numbers from all teams
            for (int i = 0; i < enrolled_teams_count; i++) {
                for (int j = 0; j < teams[i].num_players; j++) {
                    printf("Team %s\n", teams[i].team_name);
                    printf("Player %d: %d\n", j + 1, teams[i].players[j].kit_number);
                }
            }
            continue; // Re-prompt for valid kit number
        }

        printf("Enter date of birth (DD/MM/YYYY): ");
        if (fgets(new_player.dob, sizeof(new_player.dob), stdin) == NULL) { // Read DOB
            printf("Error reading date of birth.\n");
            return;
        }
        new_player.dob[strcspn(new_player.dob, "\n")] = '\0'; // Remove newline character

        printf("Enter player position (e.g., Forward): ");
        if (fgets(new_player.position, sizeof(new_player.position), stdin) == NULL) { // Read position
            printf("Error reading position.\n");
            return;
        }
        new_player.position[strcspn(new_player.position, "\n")] = '\0'; // Remove newline character

        // If all inputs are valid, break out of the loop and add player
        valid_input = 1;
    }

    // Add player to the selected team
    // Adds the new_player to the selected team's players array at the position indicated by num_players,
    // then increments the num_players count for that team.
    teams[team_choice].players[teams[team_choice].num_players++] = new_player;
    printf("Player %s has been successfully added to team %s.\n", new_player.name, teams[team_choice].team_name);
}

/**
 * Lets the user select a team by displaying a list of available teams.
 * return The index of the selected team or -1 if the choice is invalid.
 */
int select_team() {
    printf("Select a team:\n");
    for (int i = 0; i < enrolled_teams_count; i++) {
        printf("%d. %s\n", i + 1, teams[i].team_name); // Display teams with 1-based indexing
    }

    int team_choice;
    scanf("%d", &team_choice);
    getchar();

    if (team_choice < 1 || team_choice > enrolled_teams_count) { // Validate choice
        handle_invalid_input();
        return -1;
    }
    return team_choice - 1; // Convert to zero-based index
}

/**
 * Validates that a kit number is unique within the team and within the valid range (1-99).
 * parameters:-
 * team_index The index of the team
 * kit_number The kit number to validate
 * return 1 if the kit number is valid and unique, 0 otherwise.
 */
int validate_kit_number(int team_index, int kit_number) {
    if (kit_number < 1 || kit_number > 99) { // Check valid range
        printf("Invalid kit number. Please enter a number between 1 and 99.\n");
        return 0;
    }

    // Check for duplicate kit number across all teams
    for (int i = 0; i < enrolled_teams_count; i++) {
        for (int j = 0; j < teams[i].num_players; j++) {
            if (teams[i].players[j].kit_number == kit_number) {
                printf("A player with kit number %d already exists in team %s.\n", kit_number, teams[i].team_name);
                return 0;
            }
        }
    }
    return 1; // Kit number is valid
}


/**
 * Validates that the player name is unique within the team.
 * team_index The index of the team
 * name The name of the player to be validated
 * return 1 if the name is a duplicate, 0 if unique.
 */
int validate_player_name(int team_index, char* name, int player_index_to_skip) {
    for (int i = 0; i < enrolled_teams_count; i++) {
        for (int j = 0; j < teams[i].num_players; j++) {
            // Skip checking the current player being updated (if applicable)
            if (i == team_index && j == player_index_to_skip) continue;

            if (strcasecmp(teams[i].players[j].name, name) == 0) {
                printf("A player with the name %s already exists in team %s.\n", name, teams[i].team_name);
                return 1; // Duplicate name found
            }
        }
    }
    return 0; // Name is unique
}

/**
 * Searches for a player in a team based on either kit number or name, then updates details if found.
 */
void search_and_update_player() {
    if (enrolled_teams_count == 0) {
        printf("No teams have been enrolled yet.\n");
        return;
    }

    // Ask user for search option (kit number or name)
    int search_option;
    printf("Search for a player by:\n1. Kit Number\n2. Name\n");
    scanf("%d", &search_option);
    getchar();

    int found = 0; // Track if player is found
    if (search_option == 1) {
        int kit_number;
        printf("Enter the kit number: ");
        scanf("%d", &kit_number);
        getchar();

        for (int i = 0; i < enrolled_teams_count && !found; i++) { // Loop through all teams
            for (int j = 0; j < teams[i].num_players; j++) { // Loop through players in each team
                if (teams[i].players[j].kit_number == kit_number) { // Check for matching kit number
                    // Display player details and ask for update
                    printf("Player found in team %s:\n", teams[i].team_name);
                    printf("Player Name: %s\n", teams[i].players[j].name);
                    printf("Kit Number: %d\n", teams[i].players[j].kit_number);
                    printf("DOB: %s\n", teams[i].players[j].dob);
                    printf("Position: %s\n", teams[i].players[j].position);
                    printf("Do you want to update player details? (1 for Yes, 0 for No): ");
                    int update_choice;
                    scanf("%d", &update_choice);
                    getchar();
                    if (update_choice == 1) {
                        update_player_info(i, j);
                    }
                    found = 1;
                    break;
                }
            }
        }
    } else if (search_option == 2) {
        char player_name[25];
        printf("Enter the player's name: ");
        if (fgets(player_name, sizeof(player_name), stdin) == NULL) { // Read player name
            printf("Error reading player name.\n");
            return;
        }
        player_name[strcspn(player_name, "\n")] = '\0'; // Remove newline character

        for (int i = 0; i < enrolled_teams_count && !found; i++) { // Loop through all teams
            for (int j = 0; j < teams[i].num_players; j++) { // Loop through players in each team
                if (strcasecmp(teams[i].players[j].name, player_name) == 0) { // Check for matching name
                    // Display player details and ask for update
                    printf("Player found in team %s:\n", teams[i].team_name);
                    printf("Player Name: %s\n", teams[i].players[j].name);
                    printf("Kit Number: %d\n", teams[i].players[j].kit_number);
                    printf("DOB: %s\n", teams[i].players[j].dob);
                    printf("Position: %s\n", teams[i].players[j].position);
                    printf("Do you want to update player details? (1 for Yes, 0 for No): ");
                    int update_choice; // Ask for update choice
                    scanf("%d", &update_choice);
                    getchar(); // Clear newline left by scanf
                    if (update_choice == 1) {
                        update_player_info(i, j);
                    }
                    found = 1;
                    break;
                }
            }
        }
    } else {
        handle_invalid_input(); 
        return;
    }

    if (!found) printf("Player not found.\n"); // Notify if player is not found
}

/**
 * Displays all teams' statistics, including player details.
 * If a team has no players, it notifies the user.
 */
void display_team_statistics() {
    if (enrolled_teams_count == 0) { // Check if teams are enrolled
        printf("No teams have been enrolled yet.\n");
        return;
    }
    for (int i = 0; i < enrolled_teams_count; i++) { // Loop through all teams
        printf("\nTeam: %s\n", teams[i].team_name);
        printf("Number of players: %d\n", teams[i].num_players); // Display number of players

        if (teams[i].num_players == 0) { // Check if team has players
            printf("No players in this team.\n");
            continue;
        }
        int total_age = 0; // Initialize total age for calculating average

        for (int j = 0; j < teams[i].num_players; j++) { // Loop through players in the team
            int age = calculate_age(teams[i].players[j].dob); // Calculate age
            total_age += age; // Add to total age for average calculation
            printf("  Player %d: Name: %s, Kit Number: %d, DOB: %s, Position: %s, Age: %d\n",
                   j + 1, teams[i].players[j].name,
                   teams[i].players[j].kit_number,
                   teams[i].players[j].dob,
                   teams[i].players[j].position,
                   age); // Display player details
        }
        double average_age = (double)total_age / teams[i].num_players; // Calculate average age
        printf("Average age of players in this team: %.2f years\n", average_age); // Display average age
    }
}

/*
* Function to calculate age from date of birth (DOB).
* parameters:-
* dob The date of birth in the format "DD MM YYYY"
* return The age calculated based on the current date.
*/
int calculate_age(const char* dob) {
    // Variables to store date components
    int day, month, year;
    int current_day, current_month, current_year;
    
    // Parse the DOB (format: DD MM YYYY)
    sscanf(dob, "%d %d %d", &day, &month, &year);
    
    // Get current date
    time_t t = time(NULL); // Get current time
    struct tm tm = *localtime(&t); //Get current time
    current_day = tm.tm_mday; // Day of the month
    current_month = tm.tm_mon + 1; // Months are 0-based
    current_year = tm.tm_year + 1900; // Years since 1900

    // Calculate age
    int age = current_year - year;
    if (current_month < month || (current_month == month && current_day < day)) {
        age--; // Adjust if birthday hasn't occurred yet this year
    }
    return age;
}

/**
 * Updates a player's details based on user's choice (name, kit number, DOB, or position).
 * Ensures any new kit number is unique within the team.
 */
void update_player_info(int team_index, int player_index) { 
    int choice;
    printf("1. Update Name\n2. Update Kit Number\n3. Update DOB\n4. Update Position\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            printf("Enter new name: ");
            char new_name[25];
            if (fgets(new_name, sizeof(new_name), stdin) == NULL) {
                printf("Error reading name.\n");
                return;
            }
            new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
            
            // Check for duplicate name before updating
            if (validate_player_name(team_index, new_name, player_index)) {
                printf("A player with the name %s already exists in this team.\n", new_name);
                return; // Exit without updating
            }

            // If no duplicate, update the name
            strcpy(teams[team_index].players[player_index].name, new_name);
            printf("Player name updated successfully.\n");
            break;
            
        case 2:
            printf("Enter new kit number: ");
            int new_kit;
            scanf("%d", &new_kit);
            getchar();
            if (!validate_kit_number(team_index, new_kit)) {
                printf("Invalid or duplicate kit number.\n");
                return; // Exit if invalid or duplicate kit number
            }
            teams[team_index].players[player_index].kit_number = new_kit; // Update kit number
            printf("Kit number updated successfully.\n");
            break;
        case 3:
            printf("Enter new DOB (DD/MM/YYYY): ");
            if (fgets(teams[team_index].players[player_index].dob, sizeof(teams[team_index].players[player_index].dob), stdin) == NULL) { // Read new DOB
                printf("Error reading DOB.\n");
                return;
            }
            teams[team_index].players[player_index].dob[strcspn(teams[team_index].players[player_index].dob, "\n")] = '\0'; // Remove newline character
            printf("DOB updated successfully.\n");
            break;
        case 4:
            printf("Enter new position: ");
            if (fgets(teams[team_index].players[player_index].position, sizeof(teams[team_index].players[player_index].position), stdin) == NULL) { // Read new position
                printf("Error reading position.\n");
                return;
            }
            teams[team_index].players[player_index].position[strcspn(teams[team_index].players[player_index].position, "\n")] = '\0'; // Remove newline character
            printf("Position updated successfully.\n");
            break;
        default:
            handle_invalid_input();
            break;
    }
}

/**
 * Function to display a message for invalid inputs.
 * Provides feedback for incorrect choices or data entries.h
 */
void handle_invalid_input() {
    printf("Invalid input. Please try again.\n");
}