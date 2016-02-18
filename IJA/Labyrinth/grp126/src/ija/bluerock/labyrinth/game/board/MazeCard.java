package ija.bluerock.labyrinth.game.board;

import ija.bluerock.labyrinth.game.treasure.TreasureCard;

import java.io.Serializable;

/**
 * Hracia karta, udáva popis cesty v labyrinte na určitom policku.
 *
 * @author Martin Kocour
 **/
public class MazeCard implements Serializable {

    private CANGO exit1;
    private CANGO exit2;
    private CANGO exit3;
    private String type;
    private TreasureCard treasureCard;

    /**
     * Vytvorí novú hraciu kartu s dvoma východmi.
     *
     * @param exit1 definuje 1. východ z  karty
     * @param exit2 definuje 2. východ z  karty
     * @param type typ karty (C, L)
     */
    public MazeCard(CANGO exit1, CANGO exit2, String type) {
        this.exit1 = exit1;
        this.exit2 = exit2;
        this.exit3 = null;
        this.type  = type;
        treasureCard = null;
    }

    /**
     * Vytvorí novú hraciu kartu s troma východmi.
     *
     * @param exit1 definuje 1. východ z  karty
     * @param exit2 definuje 2. východ z  karty
     * @param exit3 definuje 3. východ z  karty
     * @param type typ karty (F)
     */
    public MazeCard(CANGO exit1, CANGO exit2, CANGO exit3, String type) {
        this.exit1 = exit1;
        this.exit2 = exit2;
        this.exit3 = exit3;
        this.type  = type;
    }

    /**
     * Vytlačí typ karty na stdout.
     */
    public void printCard() {
        System.out.print(type);
    }

    public enum CANGO {
        LEFT, UP, RIGHT, DOWN;

        private static CANGO[] values = values();

        /**
         * Metoda vracia dalsi typ.
         * @return next CANGO direction
         **/
        private CANGO next() {
            return values[(this.ordinal() + 1) % values.length];
        }

        public static CANGO complementDirection(CANGO dir){
            switch (dir) {
                case LEFT:
                    return RIGHT;
                case RIGHT:
                    return LEFT;
                case UP:
                    return DOWN;
                default:
                    return UP;
            }
        }
    }

    /**
     * Vyrobí nový kameň.
     * Statická metóda typu "factory".
     * @param type Typ kameňa (C, L, F)
     * @return Vráti nový kame??.
     * @throws IllegalArgumentException V prípade nesprávneho argumentu.
     * @see IllegalArgumentException
     */
    public static MazeCard create(String type) throws IllegalArgumentException {
        switch (type) {
            case "C":
                return new MazeCard(CANGO.LEFT, CANGO.UP, type);
            case "L":
                return new MazeCard(CANGO.LEFT, CANGO.RIGHT, type);
            case "F":
                return new MazeCard(CANGO.LEFT, CANGO.UP, CANGO.RIGHT, type);
            default:
                throw new IllegalArgumentException();
        }
    }

    /**
     * Zistí či je možné daný hrací kame?? opustiť v smere dir.
     * @param dir Udáva smer východu z karty.
     * @return Vracia True ak sa dá kartu v požadovanom smere opustiť inak False.
     */
    public boolean canGo(MazeCard.CANGO dir) {
        if(this.exit3 != null)
            return this.exit1.equals(dir) || this.exit2.equals(dir) || this.exit3.equals(dir);
        else
            return this.exit1.equals(dir) || this.exit2.equals(dir);
    }

    /**
     * Otočí kartu o 90° vpravo.
     */
    public void turnRight() {
        this.exit1 = this.exit1.next();
        this.exit2 = this.exit2.next();
        if(this.exit3 != null) this.exit3 = this.exit3.next();
    }

    /**
     * Otočí kartu na požadovanú pozíciu.
     * @param ex1 Konečný 1. východ z karty.
     * @param ex2 Konečný 2. východ z karty.
     * @see MazeCard#turnToPosition(CANGO, CANGO, CANGO)
     */
    public void turnToPosition(CANGO ex1, CANGO ex2) {
        while (this.exit1 != ex1 || this.exit2 != ex2) {
            this.turnRight();
        }
    }

    /**
     * Otočí kartu na požadovanú pozíciu.
     * @param ex1 Konečný 1. východ z karty.
     * @param ex2 Konečný 2. východ z karty.
     * @param ex3 Konečný 3. východ z karty.
     * @see MazeCard#turnToPosition(CANGO, CANGO)
     */
    public void turnToPosition(CANGO ex1, CANGO ex2, CANGO ex3) {
        while (this.exit1 != ex1 || this.exit2 != ex2 || this.exit3 != ex3) {
            this.turnRight();
        }
    }

    /**
     *
     * @return Vracia poklad na kameni.
     */
    public TreasureCard getTresaureCard() {
        return this.treasureCard;
    }

    /**
     * Umiestni poklad na kame??.
     * @param card Poklad.
     */
    public void putTresaureCard(TreasureCard card) {
        this.treasureCard = card;
    }

    /**
     *
     * @return Vracia typ kame??a.
     */
    public String getType() {
        return this.type;
    }
}
