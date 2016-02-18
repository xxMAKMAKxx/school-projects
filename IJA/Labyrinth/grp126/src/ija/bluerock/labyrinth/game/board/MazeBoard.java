package ija.bluerock.labyrinth.game.board;

import ija.bluerock.labyrinth.game.treasure.CardPack;
import ija.bluerock.labyrinth.game.treasure.TreasureCard;

import java.io.Serializable;
import java.util.Random;

import static java.lang.Math.abs;

/**
 * Hracia plocha, skladá sa z hracich políčok.
 *
 * @author Martin Kocour
 **/
public class MazeBoard implements Serializable {
    private int n; /* stupen matice board, udava pocet riadkov*stlpcov*/
    private MazeField[][] fields; /* matica jednotlivych policok na doske board */
    private MazeCard freeCard; /* volna karta */
    private int countC = 0; /* pocet "C" kariet */
    private int countL = 0; /* pocet "L" kariet */
    private int countF = 0; /* pocet "F" kariet */

    private MazeBoard(int n) {
        this.n = n;
        fields = new MazeField[n][n];
        freeCard = null;

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                fields[r][c] = new MazeField(r+1, c+1);
            }
        }
    }

    /**
     * Vytvorí novú hraciu plochu z políčok MazeField bez hracích kameňov.
     *
     * @param n             Udáva veľkosť štvorcovej matice.
     *                      Musí byť nepárne číslo v intervale (5, 11).
     * @return MazeBoard    Vracia nainicializovanú hraciu plochu.
     **/
    public static MazeBoard createMazeBoard(int n) throws IllegalArgumentException {
        if(n % 2 != 0 && n >= 5 && n <= 11)
            return new MazeBoard(n);
        else
            throw new IllegalArgumentException("N should be odd and in <5, 11>");
    }

    /**
     * Vytvorí novú hru.
     * Uloží na hraciu plochu kamene a úlohové kartičky(poklady).
     *
     * Na políčka nauklada nahodne nove karty v pomere 1:1:1
     * a vytvorí voľný hrací kameň.
     * @param numberOfTresaures Pocet pokladu ve hre.
     */
    public void newGame(int numberOfTresaures) {
        createCards();
        setTresaures(numberOfTresaures);

        int MIN = Math.min(countC, countF);
        MIN = Math.min(MIN, countL);
        if (MIN == countC) {
            freeCard = MazeCard.create("C");
            countC++;
        }
        else if (MIN == countF) {
            freeCard = MazeCard.create("F");
            countF++;
        }
        else {
            freeCard = MazeCard.create("L");
            countL++;
        }
    }

    /**
     * Metodá zistí čí políčko je v rohu hracej plochy.
     * @param r R-súradnica políčka.
     * @param c C-súradnica políčka.
     * @return Vracia True, ak políčko je v roku inak False.
     */
    public boolean isCorner(int r, int c) {
        return (r == 0 && c == 0) || (r == 0 && c == n - 1) || (r == n - 1 && c == 0) || (r == n - 1 && c == n - 1);
    }

    /**
     * Metóda slúži na lepší prístup k políčkam hracej plochy.
     *
     * @param r R-súradnica od 1 po veľkosť hracej plochy.
     * @param c C-súradnica od 1 po veľkosť hracej plochy.
     * @return Políčko na hracej ploche.
     * @see MazeField
     */
    public MazeField get(int r, int c) {
        if(r > 0 && r <= n && c > 0 && c <= n)
            return fields[r-1][c-1];
        else
            return null;
    }

    /**
     * @return Vracia volný kameň.
     */
    public MazeCard getFreeCard() {
        return freeCard;
    }

    /**
     * Posúva karty na policka podľa prametru mf.
     *
     * Stará volná karta sa vloží na políčko mf.
     * Vysunutá karta sa stáva novou voľnou kartou.
     *
     * @param mf    Udáva smer posunu.
     **/
    public void shift(MazeField mf) {
        int c = mf.col();
        int r = mf.row();
        MazeCard oldFreeCard = freeCard;
        if(c > 0 && c <= n && r > 0 && r <= n){
            if(c % 2 == 0) {
                if(r == 1){
                    freeCard = get(n,c).getCard();
                    /* idem zdola hore v stlpci,
                     * vkladam predchadzajucu kartu na aktualne policko
                     */
                    for(int row = n; row > 1; row--) {
                        get(row, c).putCard(get(row - 1, c).getCard());
                    }
                    get(r,c).putCard(oldFreeCard);
                }
                else if(r == n){
                    freeCard = get(1,c).getCard();
                    /* idem zhora dole v stlpci,
                     * vkladam nasledujucu kartu na aktualne policko
                     */
                    for(int row = 1; row < n; row++) {
                        get(row, c).putCard(get(row + 1, c).getCard());
                    }
                    get(r,c).putCard(oldFreeCard);
                }
            }
            else if(r % 2 == 0){
                if(c == 1){
                    freeCard = get(r,n).getCard();
                    /* idem zprava dolava v riadku,
                     * vkladam predchadzajucu kartu na aktualne policko
                     */
                    for(int col = n; col > 1; col--) {
                        get(r, col).putCard(get(r, col - 1).getCard());
                    }
                    get(r,c).putCard(oldFreeCard);
                }
                else if(c == n){
                    freeCard = get(r,1).getCard();
                    /* idem zlava doprava v riadku,
                     * vkladam nasledujucu kartu na aktualne policku
                     */
                    for(int col = 1; col < n; col++) {
                        get(r, col).putCard(get(r, col + 1).getCard());
                    }
                    get(r,c).putCard(oldFreeCard);
                }
            }
        }
    }

    /**
     * Vytvara pseudonahodne cisla a podla nich potom vracia MazeCard
     *
     * Dava pozor aby boli nahodne karty priblizne v pomere 1:1:1
     *
     * @return random MazeCard
     **/
    private MazeCard getRandomCard(Random rnd) {
        MazeCard randomCard;
        int MAX = (n * n) / 3;
        int number = rnd.nextInt() % 3;
        number = abs(number);
        switch (number) {
            case 0:
                if (countC < (MAX - 1)) {
                    countC++;
                    randomCard = MazeCard.create("C");
                    break;
                }
            case 1:
                if (countL < MAX) {
                    countL++;
                    randomCard = MazeCard.create("L");
                    break;
                }
            case 2:
                if (countF < MAX) {
                    countF++;
                    randomCard = MazeCard.create("F");
                    break;
                }
            default:
                if (countC < (MAX - 1)) {
                    countC++;
                    randomCard = MazeCard.create("C");
                    break;
                }
                else if (countL < MAX) {
                    countL++;
                    randomCard = MazeCard.create("L");
                    break;
                }
                else if (countF < MAX) {
                    countF++;
                    randomCard = MazeCard.create("F");
                    break;
                }
                else {
                    number = rnd.nextInt() % 3;
                    number = abs(number);
                    switch (number) {
                        case 0:
                            countC++;
                            randomCard = MazeCard.create("C");
                            break;
                        case 1:
                            countL++;
                            randomCard = MazeCard.create("L");
                            break;
                        default:
                            countF++;
                            randomCard = MazeCard.create("F");
                            break;
                    }
                }
        }
        return randomCard;
    }

    private void createCards() {
        MazeCard card;
        Random rnd = new Random();
        countC = 4;
        /* vztah medzi n a f je nasledujuci:
                n = 5 => 5
                n = 7 => 5 + 7
                n = 9 => 5 + 7 + 9
                n = 11=> 5 + 7 + 9 + 11
          */
        countF = 5 + (n-5 <= 0? 0 : 1)*7 + (n-7 <= 0? 0 : 1)*9 + (n-11 <= 0? 0 : 1)*11;
        countL = 0;
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if(isCorner(r, c)) {
                    card = MazeCard.create("C");
                    if(r == 0 && c == 0)
                        card.turnToPosition(MazeCard.CANGO.RIGHT, MazeCard.CANGO.DOWN);
                    else if(r == 0 && c == n-1)
                        card.turnToPosition(MazeCard.CANGO.DOWN, MazeCard.CANGO.LEFT);
                    else if(r == n-1 && c == 0)
                        card.turnToPosition(MazeCard.CANGO.UP, MazeCard.CANGO.RIGHT);
                    else
                        card.turnToPosition(MazeCard.CANGO.LEFT, MazeCard.CANGO.UP);
                }
                else if(r % 2 == 0 && c % 2 == 0){
                    card = MazeCard.create("F");
                    if(r == 0 && c >= 2 && c <= n-3)
                        card.turnToPosition(MazeCard.CANGO.RIGHT, MazeCard.CANGO.DOWN, MazeCard.CANGO.LEFT);
                    else if (r == n-1 && c >= 2 && c <= n-3)
                        card.turnToPosition(MazeCard.CANGO.LEFT, MazeCard.CANGO.UP, MazeCard.CANGO.RIGHT);
                    else if (c == 0  && r >= 2 && r <= n-3)
                        card.turnToPosition(MazeCard.CANGO.UP, MazeCard.CANGO.RIGHT, MazeCard.CANGO.DOWN);
                    else if (c == n-1 && r >= 2 && r <= n-3)
                        card.turnToPosition(MazeCard.CANGO.DOWN, MazeCard.CANGO.LEFT, MazeCard.CANGO.UP);
                }
                else {
                    card = getRandomCard(rnd);
                    int rotate = abs(rnd.nextInt() % 4);
                    while (rotate > 0) {
                        card.turnRight();
                        rotate--;
                    }
                }
                fields[r][c].putCard(card);
            }
        }
    }


    private void setTresaures(int amount) {
        Random rnd = new Random();
        int count = 0;
        CardPack pack = new CardPack(amount, amount);
        while (count != amount) {
            int row = abs(rnd.nextInt() % n);
            int col = abs(rnd.nextInt() % n);
            if (fields[row][col].getCard().getTresaureCard() == null) {
                TreasureCard card = pack.popCard();
                fields[row][col].getCard().putTresaureCard(card);
                count++;
            }
        }
    }

    /**
    * Vytlačí na stdout hracie pole.
    */
    public void printBoard() {
        System.out.println("C: "+ countC +" F: " + countF +" L: "+ countL );
        System.out.println("--------------------");
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                fields[r][c].printField();
                System.out.print("  ");
            }
            System.out.println("");
        }
        System.out.println("Free card: "+freeCard.getType());
    }
}
