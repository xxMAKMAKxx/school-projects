package ija.bluerock.labyrinth.game.treasure;

import java.io.Serializable;

/**
 * Poklad. Každý poklad má svoj vlastný identifikačný kód.
 *
 * @author  <a href="mailto:xkocou08@stud.fit.vutbr.cz">Martin Kocour (xkocou08)</a>
 */
public class Treasure implements Serializable {

    private final static Treasure[] treasures = new Treasure[24];
    private int code;

    private Treasure(int code) {
        this.code = code;
    }

    /**
     * Vytvorí množinu 24 pokladov.
     */
    public static void createSet() {
        for (short i = 0; i < treasures.length; i++)
            treasures[i] = new Treasure(i);
    }

    /**
     * Vráti poklad s daným kódom.
     * @param code Kód pokladu.
     * @return  Vracia null v prípade, že kód je mimo rozsah inak vráti poklad.
     */
    public static Treasure getTreasure(int code) {
        if((code > treasures.length - 1) || (code < 0))
            return null;
        else
            return treasures[code];
    }

    /**
     *
     * @return Vráti kód pokladu.
     */
    public int getCode() {
        return code;
    }
}

