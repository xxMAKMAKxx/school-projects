package ija.bluerock.labyrinth.game.treasure;

import java.io.Serializable;

/**
 * Kartička s pokladom.
 *
 * @see Treasure
 * @author  <a href="mailto:xkocou08@stud.fit.vutbr.cz">Martin Kocour (xkocou08)</a>
 */
public class TreasureCard implements Serializable {

    private Treasure tr;

    /**
     * Vytvorí novú kartičku s pokladom.
     *
     * @param tr Poklad.
     * @see Treasure
     */
    public TreasureCard(Treasure tr) {
        this.tr = tr;
    }

    @Override
    public boolean equals(Object o){
        if(o instanceof TreasureCard) {
            TreasureCard c = (TreasureCard) o;
            return this.hashCode() == c.hashCode();
        }
        else
            return false;
    }

    @Override
    public int hashCode() {
        return tr.getCode();
    }

    /**
     *
     * @return Vráti poklad na kartičke.
     */
    public Treasure getTreasure() {
        return tr;
    }
}
