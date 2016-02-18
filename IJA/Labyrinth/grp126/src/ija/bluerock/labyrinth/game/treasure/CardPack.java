package ija.bluerock.labyrinth.game.treasure;

import java.io.Serializable;
import java.util.Random;

/**
 * Balíček pokladov (úlohových kartičiek).
 *
 * @author  <a href="mailto:xkocou08@stud.fit.vutbr.cz">Martin Kocour (xkocou08)</a>
 */
public class CardPack implements Serializable {

    protected int maxSize;
    protected int initSize;
    protected short top = 0; //WARNING! try something better
    protected TreasureCard[] cards;

    /**
     * Vytvorí nový balíček.
     *
     * @param maxSize   Definuje maximálny počet pokladov v balíčku.
     * @param initSize  Definuje počiatočný počet pokladov v balíčku.
     */
    public CardPack(int maxSize, int initSize) {
        if(maxSize >= initSize){
            Treasure.createSet();
            this.maxSize = maxSize;
            this.initSize = initSize;
            cards = new TreasureCard[maxSize];
            for (int i = 0; i < initSize; i++){
                cards[i] = new TreasureCard(Treasure.getTreasure(i));
            }
        }
    }

    /**
     *
     * @return Vracia najvrchnejší poklad z balíčka.
     */
    public TreasureCard popCard() {
        return this.cards[(top++)%this.maxSize];
    }

    public void pushCard(TreasureCard card) {
        this.cards[(top--)%this.maxSize] = card;
    }

    /**
     *
     * @return Vracia aktuálny počet kariet v balíčku.
     */
    public int size() {
        return (this.initSize - this.top);
    }

    /**
     * Náhodne zamieša balíček.
     * @see Random
     */
    public void shuffle() {
        Random rnd = new Random();
        int index1 = rnd.nextInt(this.initSize-1);
        int index2 = rnd.nextInt(this.initSize-1);
        TreasureCard tmp;
        for (int i = rnd.nextInt(15)+10; i >= 0; i--){
            tmp = cards[index1];
            cards[index1] = cards[index2];
            cards[index2] = tmp;
            index1 = rnd.nextInt(this.initSize-1);
            index2 = rnd.nextInt(this.initSize-1);
        }
    }
}
