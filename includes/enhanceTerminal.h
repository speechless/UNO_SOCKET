/**
 * @file enhanceTerminal.h
 */

#ifndef ENHANCE_TERMINAL
#define ENHANCE_TERMINAL

enum
{
	BLACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	BOLD = 1,
	FAINT,
	ITALIC,
	UNDERLINE,
	BLINK,
	INV = 7,
	HIDDEN,
	STRIKETHROUGH,
};

/* ------------------------------------------------------------------------ */
/*          GESTION DU MODE D'AFFICHAGE SUR LE TERMINAL                     */
/* ------------------------------------------------------------------------ */
/**
 * Efface le terminal
 */
#define clearScreen()  	{ printf("\033[H\033[2J");fflush(stdout); }

 /**
  * Applique un style de texte
  */
#define setTerm(param)  { printf("\033[1m\033[%dm", param); fflush(stdout); }

  /**
   * Réinitialise le style de texte
   */
#define resetTerm()		setTerm(0)

#endif