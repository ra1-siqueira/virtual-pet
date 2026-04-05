// coin system isn't properly implemented yet

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define UP_ARROW 65
#define DOWN_ARROW 66
//#define LEFT_ARROW 75
//#define RIGHT_ARROW 77
#define ENTER 10

#define RED "\e[91m"
#define BLUE "\e[34m"
#define YELLOW "\e[1;33m"
#define YELLOW_BG "\033[43m"
//#define ESP_YEL "\e[4;33m"
#define RESET "\e[0m"

typedef struct {
	char name[20];
	int energy;
	int food;
	int fun;
} Pet;

Pet pet;
int coins;

void inputBufferOFF() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void inputBufferON() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void State1() {
	printf(YELLOW "\n        $$$$$$$$$$$      ::::STATUS::::\n" RESET);
	printf(YELLOW "      $   ;     ;   $    Name: "RESET"%s\n" , pet.name);
	printf(YELLOW "      $      ^      $    Energy: "RESET"%d\n" , pet.energy);
	printf(YELLOW "      $             $    Food: "RESET"%d\n" , pet.food);
	printf(YELLOW "        $$$$$$$$$$$      Fun: "RESET"%d\n" , pet.fun);
}

void State2() {
	printf(YELLOW "\n        $$$$$$$$$$$      ::::STATUS::::\n" RESET);
	printf(YELLOW "      $   o     o   $    Name: "RESET"%s\n" , pet.name);
	printf(YELLOW "      $      .      $    Energy: "RESET"%d\n" , pet.energy);
	printf(YELLOW "      $             $    Food: "RESET"%d\n" , pet.food);
	printf(YELLOW "        $$$$$$$$$$$      Fun: "RESET"%d\n" , pet.fun);
}

void State3() {
	printf(YELLOW "\n        $$$$$$$$$$$      ::::STATUS::::\n" RESET);
	printf(YELLOW "      $   O     O   $    Name: "RESET"%s\n" , pet.name);
	printf(YELLOW "      $      U      $    Energy: "RESET"%d\n" , pet.energy);
	printf(YELLOW "      $             $    Food: "RESET"%d\n" , pet.food);
	printf(YELLOW "        $$$$$$$$$$$      Fun: "RESET"%d\n" , pet.fun);
}

void State4() {
	printf(YELLOW "\n        $$$$$$$$$$$      ::::STATUS::::\n" RESET);
	printf(YELLOW "      $   ^     ^   $    Name: "RESET"%s\n" , pet.name);
	printf(YELLOW "      $  ``  w  ´´  $    Energy: "RESET"%d\n" , pet.energy);
	printf(YELLOW "      $             $    Food: "RESET"%d\n" , pet.food);
	printf(YELLOW "        $$$$$$$$$$$      Fun: "RESET"%d\n" , pet.fun);	
}

void showStatus() {
	if (pet.energy == 0 &&
		pet.food == 0 &&
		pet.fun == 0) State1();

	else {
		if (pet.energy == 100 &&
			pet.food == 100 &&
			pet.fun == 100) State4();

		else {
			if (pet.energy >= 60 &&
				pet.food >= 60 &&
				pet.fun >= 60) State3();

			else State2();
		}
	}
}

void show_menuWELCOME(int selected) {
	system("clear");

	printf(YELLOW "\n\n                 W E L C O M E  T O\n" RESET);
	printf(BLUE "          ~^~^~ V I R T U A L  P E T ~^~^~\n\n" RESET);

	//printf("\n   Do you want to create a new pet or load an existing one?\n\n");

	if (selected == 0)
		printf("                " YELLOW_BG "   CREATE NEW PET    " RESET "\n");
	else
		printf("              " YELLOW "     Create new pet    " RESET "\n");
			
	if (selected == 1)
		printf("                " YELLOW_BG "  LOAD EXISTING PET  " RESET "\n");
	else
		printf("              " YELLOW "    Load existing pet " RESET "\n");
		
	if (selected == 2)
		printf("                " YELLOW_BG "    SEE PET LIST     " RESET "\n");
	else
		printf("              " YELLOW "      See pet list    " RESET "\n");
}

void show_menuPET(int selected) {
	system("clear");

	showStatus();

	printf(YELLOW " ____________________________________________________\n\n" RESET);

	if (selected == 0) printf(YELLOW "^.^ FEED" RESET "\n");
	else printf("    feed\n");

	if (selected == 1) printf(YELLOW "^.^ PLAY" RESET "\n");
	else printf("    play\n");

	if (selected == 2) printf(YELLOW "^.^ SLEEP" RESET "\n");
	else printf("    sleep\n");

	if (selected == 3) printf(YELLOW "^.^ POTIONS" RESET "\n");
	else printf("    potions\n");

	if (selected == 4) printf(YELLOW "^.^ RESTART" RESET "\n");
	else printf("    restart\n");

	if (selected == 5) printf(YELLOW "^.^ EXIT" RESET "\n");
	else printf("    exit\n");

	printf("\n                                  You have: " YELLOW "%d coins\n" RESET, coins);
	printf("(You get +10 coins every time you complete an action)\n");

	printf(YELLOW " ____________________________________________________\n\n" RESET);
} 

void createPet() {
	system("clear");

	inputBufferON();
	
	FILE *f = fopen("pets.bin", "ab");
	if (f == NULL) printf(RED "ERRO.\n" RESET);

	printf("\n\n    " YELLOW_BG "      +~+ CREATE YOUR PET! +~+      " RESET "\n");
	printf(YELLOW "\n    Create a name for your pet: " RESET);
    scanf(" %[^\n]", pet.name);

	pet.energy = 0;
	pet.food = 0;
	pet.fun = 0;

	fwrite(&pet, sizeof(Pet), 1, f);

	fclose(f);

	printf("\n\n    " BLUE "Your pet was created sucessfully!\n" RESET);
	getchar();

	getchar();
    
	inputBufferOFF();

}

long findPos(FILE *f, char *name) {
	Pet pet_temp;
	long pos;

	rewind(f);

	while(1) {
		pos = ftell(f);

		if (fread(&pet_temp, sizeof(Pet), 1, f) != 1) break;		
		if (strcmp(pet_temp.name, name) == 0) return pos;
	}

	return -1L;
}

int loadData(Pet *p) {
	FILE *f = fopen("pets.bin", "rb");

	int found;
	long pos = findPos(f, p->name);

	if (pos == -1L) found = 0;
	else {
		found = 1;
		fseek(f, pos, SEEK_SET);
		fread(p, sizeof(Pet), 1, f);
	}

	fclose(f);
	
	return found;
}

int updateData(Pet *updated_p) {
	FILE *f = fopen("pets.bin", "rb+");

	long pos = findPos(f, updated_p->name);

	fseek(f, pos, SEEK_SET);
	fwrite(updated_p, sizeof(Pet), 1, f);

	fclose(f);

}

int loadPet() {
	system("clear");

	inputBufferON();

	char name_search[20];
	int found;

	printf("\n    " YELLOW_BG "      +~+ FIND YOUR PET! +~+      " RESET "\n");
	printf(YELLOW "\n   Enter the name of your pet: " RESET);
	scanf(" %[^\n]", name_search);
	strcpy(pet.name, name_search);

	found = loadData(&pet);

	if (found == 0) {
		system("clear");

		printf(RED "\n   PET NOT FOUND :(\n" RESET);
		printf("\n   Sorry, but we couldn't find '%s'.\n   What about creating a new pet?\n", name_search);
		printf(BLUE "\n   >> press ENTER to go back to the menu...\n" RESET);

		getchar();
	}
	else {
		printf(BLUE "\n   YOUR PET WAS FOUND\n" RESET);
		printf("   %s was waiting for you!\n", name_search);
		printf(BLUE "\n   >> press ENTER to continue...\n" RESET);

		getchar();
	}

	getchar();
    
	inputBufferOFF();

	return found;
}

void listPets() {
	FILE *f = fopen("pets.bin", "rb");
	if (f == NULL) printf(RED "ERRO.\n" RESET);

	system("clear");

	int count = 0;

	printf(YELLOW_BG " Name                    Energy          Food            Fun \n" RESET);

	while (fread(&pet, sizeof(Pet), 1, f) == 1) {
		printf(" %-12s %15d %14d %15d \n", pet.name, pet.energy, pet.food, pet.fun);
		count++;
	}
	printf("\n");

	if (count == 0) {
		printf(BLUE "\n   NOTHING TO SEE HERE\n" RESET);
		printf("\n   No registered pets yet.\n   Try creating some first.\n");
	}

	printf(BLUE "\n   >> press ENTER to continue...\n" RESET);

	getchar();

	fclose(f);
}

void feedPet() {
	FILE *f = fopen("pets.bin", "rb+");

	if (pet.food == 100) {
		printf(YELLOW "\n%s is full (^._.^)\n" RESET, pet.name);
		printf("\nTry doing something else.\n");
	}
	else {
		pet.food += 20;
		coins += 10;

		updateData(&pet);

		printf(YELLOW "\nYou fed %s! (˶ˆ꒳ˆ˵)\n" RESET, pet.name);
	}
	fclose(f);
}

void playWPet() {
	FILE *f = fopen("pets.bin", "rb+");

	if (pet.fun == 100 || pet.energy < 10 || pet.food < 10) {
		printf(YELLOW "\n%s can't play right now (^._.^)\n" RESET, pet.name);

		if (pet.fun == 100) printf("\nTry doing something else.\n");
			
		if (pet.fun != 100)
			printf("\nTip: %s can't play when hungry or tired.\n", pet.name);
	}
	else {
		pet.fun += 20;
		pet.energy -= 20;
		coins += 10;

		updateData(&pet);

		printf(YELLOW "\n%s enjoyed playing with you! (ﾉ´ヮ`)ﾉ*: ･ﾟ\n" RESET, pet.name);
	}
	fclose(f);
}

void sleepPet() {
	FILE *f = fopen("pets.bin", "rb+");

	if (pet.energy == 100) {
		printf(YELLOW "\n%s doesn't want to sleep! (^._.^)\n" RESET, pet.name);
		printf("\nTry doing something else.\n");
	}
	else {
		pet.energy += 20;
		coins += 10;

		updateData(&pet);

		printf(YELLOW "\n%s had a good nap! ʕ-ᴥ-ʔ\n" RESET, pet.name);
	}
	fclose(f);
}

void potionShop() {

	printf(YELLOW "\nSorry, the Potion Shop is temporarily not working... Try again later." RESET "\n");

	/*system("clear");

	int option;

	printf("\n  ------------POTION SHOP-----------\n");
	printf("  --> 1 - Max Potion (100 coins) <--\n");
	printf("  --> 2 - Min Potion (200 coins) <--\n");

	printf("\n\n  Which potion do you want to buy? -> ");
	scanf("%d", &option);
	//printf("\n\n         (>>>Wanna go back instead? Press 0)\n");

	getchar();
	
	switch (option) {
		case 1:
			if(coins < 100)
				printf("\nSorry, you don't have enough coins...\n");
			else {
				coins -= 100;
				pet.energy = 100;
				pet.food = 100;
				pet.fun = 100;
			} break;
		case 2:
			if(coins < 200)
				printf("\nSorry, you don't have enough coins...\n");
			else {
				coins -= 200;
				pet.energy = 0;
				pet.food = 0;
				pet.fun = 0;
			} break;
		//case 0: return;
	}*/
}

void exitGame() {
	printf(YELLOW "\n%s will be waiting for you! See ya! ( ´ ∀ `)ノ～ ♡\n\n" RESET, pet.name);

	inputBufferON();
}

void BROWSE(int *selected, int *menu_size) {
    getchar();

    switch(getchar()) {
        case UP_ARROW:
            if (*selected > 0) (*selected)--; break;
        case DOWN_ARROW:
            if (*selected < *menu_size) (*selected)++; break;
    }
}

int RUN(char key, int selected) {
	int menu_size;
	int found = 1, repeat = 1;

	while (repeat == 1) {	
		show_menuWELCOME(selected);

		key = getchar();
		menu_size = 2;

		if (key == '\033') BROWSE(&selected, &menu_size);
		else if (key == ENTER) {
			switch(selected) {
				case 0:
					createPet();
					repeat = 0;
					break;

				case 1:
					found = loadPet();
					if (found == 1) repeat = 0;
					break;

				case 2:
					listPets();
					break;
			}
		}
	}

	while (1) {
		show_menuPET(selected);

		key = getchar();
		menu_size = 5;

		if (key == '\033') BROWSE(&selected, &menu_size);
		else if (key == ENTER) {
			switch(selected) {
				case 0: feedPet();    break;
				case 1: playWPet();   break;
				case 2: sleepPet();   break;
				case 3: potionShop(); break;
				case 4:               return 1;
				case 5: exitGame();   return 0;
			}

			printf(BLUE "\n>> press ENTER...");

			getchar();  	
		}
	}
}

int main() {
	int selected = 0, loop;
	char key;
	coins = 0;

	inputBufferOFF();

	do loop = RUN(key, selected);	
	while (loop == 1);

	inputBufferON();

    return 0;
}
