package ija.bluerock.labyrinth.game.player;

import static ija.bluerock.labyrinth.game.board.MazeCard.CANGO;
import ija.bluerock.labyrinth.game.board.MazeField;
import ija.bluerock.labyrinth.game.treasure.CardPack;
import ija.bluerock.labyrinth.game.treasure.TreasureCard;

import java.io.Serializable;

/**
 * Hráč. Každý hráč má za úlohu nájsť svoj poklad, ktorý mu je automaticky pridelený.
 * Hráč vyhráva ak získa určitý počet pokladov.
 *
 * @author Martin Kocour
 */
public class Player implements Serializable {
    private int id; /* identifikacne cislo hraca */
    private MazeField position; /* aktualna pozicia na hracej ploche */
    private TreasureCard treasure; /* hladany poklad, karta */
    private CardPack pack; /* balíček kariet */
    private int score; /* suma ziskanych kariet */

    /**
     * Vytovrí nového hráča.
     *
     * @param id Identifikačné číslo hráča (1-4).
     * @param start Počiatočná pozícia hráča na hracej ploche.
     * @param firstTreasure Prvý poklad (úlohová kartička), ktorý má hráč nájsť.
     * @param pack Odkaz na balíček kariet, zdiela sa medzi hráčmi.
     */
    public Player(int id, MazeField start, TreasureCard firstTreasure, CardPack pack) {
        this.id = id;
        this.treasure = firstTreasure;
        this.score = 0;
        this.pack = pack;
        move(start);
    }

    /**
     * Pohne hráčom na nové políčko.
     *
     * @param newPosition Políčko, kam sa má hráč pohnúť.
     */
    public void move(MazeField newPosition) {
        position = newPosition;
        if(position != null) {
            TreasureCard card = position.getCard().getTresaureCard();
            if (card != null && card.equals(treasure)) {
                //nasiel som poklad
                position.getCard().putTresaureCard(null); //vymazem poklad
                score++;
                treasure = null;
            }
        }
    }

    /**
     * Metoda určuje či as hráč môže pohnúť v danom smere.
     * @param dir Smer pohybu.
     * @return Vracia True ak sa hráč môže daným smerom pohnúť inak False.
     */
    public boolean canGo(CANGO dir) {
        return position.getCard().canGo(dir);
    }

    /**
     *
     * @return Vracia aktuálnu pozíciu hráča.
     */
    public MazeField getPosition(){
        return position;
    }

    /**
     *
     * @return Vracia ID hráča.
     */
    public int getID() {
        return id;
    }

    /**
     *
     * @return Vracia poklad, ktorý má hráč práve nájsť.
     */
    public TreasureCard getTreasureCard() {
        return treasure;
    }

    /**
     *
     * @return Vracia skóre hráča.
     */
    public int getScore() {
        return score;
    }

    /**
     * Nastaví skóre na hráča.
     * @param score Nové skóre hráča.
     */
    public void putScore(int score) {
        this.score = score;
    }

    /**
     * Nastaví nový poklad hráčovi.
     * @param card Nový poklad.
     */
    public void putTreasureCard(TreasureCard card) {
        this.treasure = card;
    }
}
