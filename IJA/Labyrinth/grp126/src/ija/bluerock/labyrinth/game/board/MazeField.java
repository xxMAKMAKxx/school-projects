package ija.bluerock.labyrinth.game.board;


import java.io.Serializable;

/**
 * Hracie políčko, obsahuje kartu s popisom labyrintu.
 *
 * @author Martin Kocour
 **/
public class MazeField implements Serializable {
    final private int row;
    final private int col;
    private MazeCard card;

    /**
     * Vytovrí hracie políčko.
     * @param r R-súradnica políčka.
     * @param c C-súradnica políčka.
     */
    public MazeField(int r, int c) {
        row = r;
        col = c;
        card = null;
    }

    /**
     *
     * @return Vracia, na ktorom riadku sa nachádza políčko.
     */
    public int row() {
        return this.row;
    }

    /**
     *
     * @return Vracia, v ktorom stĺpci sa nachádza políčko.
     */
    public int col() {
        return this.col;
    }

    /**
     *
     * @return Vracia hrací kameň na políčku.
     */
    public MazeCard getCard() {
        return this.card;
    }

    /**
     * Vloží hrací kameň na políčko.
     * @param c Kameň.
     */
    public void putCard(MazeCard c) {
        this.card = c;
    }


    /**
     * Vytlačí typ kameňa na políčku.
     */
    public void printField() {
        card.printCard();
    }
}
